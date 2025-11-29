// main.cpp - updated to fix while loop, handle HTTP status codes, etc.
#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <optional>
#include <map>
#include <memory>
#include <pthread.h>
#include "BloomFilter/create_filter.h"
#include "Command/i_command.h"
#include "Command/add_URL_command.h"
#include "Command/contains_URL_command.h"
#include "Command/delete_url_command.h"
#include "Input/input.h"
#include "Sockets/ServerSocketTCP.h"
#include "Sockets/Types.h"

typedef std::map<std::string, std::unique_ptr<ICommand>> CommandMap;

struct ThreadArgs {
    Input input;
    CommandMap& commands;
    SocketTCP* client;
};

std::pair<int, std::string> parseArgs(int argc, const char *argv[]) {
    if (argc < 4) {
        return {0, ""};
    }

    int port = std::stoi(argv[1]);
    if (port < 1024 || port > 65535) {
        return {0, ""};
    }

    std::stringstream ss;
    for (int i = 2; i < argc - 1; ++i) {
        ss << argv[i] << " ";
    }
    ss << argv[argc - 1];
    return {port, ss.str()};
}

void handleClient(Input &input, CommandMap &commands, SocketTCP* client) {
    while (true) {
        std::optional<std::string> optLine = client->read();
        if (!optLine.has_value()) {
            client->write("{\"error\": \"No input received\"}\n");
            break;
        }

        std::string line = optLine.value();
        std::vector<std::string> command = input.getNext(line);

        if (command.size() != 2) {
            client->write("{\"error\": \"Bad Request\"}\n");
            continue;
        }

        const std::string &task = command.at(0);
        const std::string &url = command.at(1);

        if (commands.count(task) > 0) {
            std::string res = commands[task]->execute(url, input.getHashFuns());
            if (task == "POST")
                client->write("{\"result\": \"" + res + "\"}\n");
            else if (task == "DELETE")
                client->write("{\"result\": \"No Content\"}\n");
            else
                client->write("{\"result\": \"" + res + "\"}\n");
            break;
        } else {
            client->write("{\"error\": \"Not Found\"}\n");
            break;
        }
    }
}

int main(int argc, const char *argv[]) {
    auto parsed = parseArgs(argc, argv);
    if (parsed.first == 0) return 1;

    int port = parsed.first;
    std::string initStr = parsed.second;

    ServerSocketTCP serverSocket(IPVersion::IPv4, "0.0.0.0", port);

    CreateFilter *filter = CreateFilter::getInstance();
    Input input(filter->getHashSetSize());

    CommandMap commands;
    commands["POST"] = std::make_unique<AddURLCommand>();
    commands["GET"] = std::make_unique<ContainsURLCommand>();
    commands["DELETE"] = std::make_unique<DeleteURLCommand>();

    input.init(initStr);
    filter->setSize(input.getSize());

    serverSocket.listen(5);

    while (true) {
        SocketTCP* client = new SocketTCP(serverSocket.accept());
        ThreadArgs *funArgs = new ThreadArgs{input, commands, client};

        pthread_t thread;
        auto wrapper = [](void *ptr) -> void * {
            ThreadArgs *data = static_cast<ThreadArgs *>(ptr);
            handleClient(data->input, data->commands, data->client);
            delete data->client; // Clean up the socket
            delete data;
            return nullptr;
        };

        if (pthread_create(&thread, nullptr, wrapper, funArgs) != 0) {
            delete funArgs;
        } else {
            pthread_detach(thread);
        }
    }

    return 0;
}