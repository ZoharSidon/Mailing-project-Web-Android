
# Mailing-project-Web-Android
# Team members :
Zohar Sidon
Ofri Keidar
Eyal Levy

# Project Overview

This project implements a full-stack Gmail-style web application built as part of an academic assignment.
The system focuses on clean architectural separation, SOLID design principles, reliable API behavior, and full feature coverage as defined in the exercise instructions.

The application provides a structured mailbox interface, message handling workflow, spam detection via a C++ microservice, blacklist management, user authentication, and strict RESTful behavior without using server-side HTTP 500 responses.

---

# System Architecture

The project is built according to MVC and multi-service design:

### **Client (Browser)**

* Renders the mailbox UI.
* Sends requests to the Node.js server.
* Displays inbox, sent mail, message details, spam indicators, etc.
* Performs conditional rendering based on user state and server responses.

### **Node.js Server (Main Web Server)**

* Express.js REST API implementing all required Gmail-like endpoints.
* Applies MVC structure:

  * Controllers handle request flow.
  * Services implement domain logic according to SOLID.
  * Models handle data encapsulation and validation.
* Integrates with the C++ server to validate URLs against the blacklist.
* Maintains strict API behavior:

  * Only 200, 201, 204, 400, 404 are used.
  * No internal server errors.

### **C++ Blacklist Validation Service**

* Runs as a TCP server in a Docker container.
* Multithreaded implementation for concurrent requests.
* Receives URLs from the Node server and returns classification:

  * Valid
  * Blacklisted
  * Format error
* Manages local blacklist storage with thread-safe logic.

---

# Main Features

### **1. User Authentication**

* Login endpoint issuing a signed JWT.
* Route guards for protected pages.
* Expired or missing token redirects user to login.

### **2. Inbox & Message Operations**

* Viewing all messages.
* Viewing a single message including body, headers, and metadata.
* Sending new messages.
* Deleting messages and updating mailbox state.

### **3. Spam Detection & Blacklist Workflow**

* Each outgoing email is parsed for links.
* All links are validated by the C++ microservice.
* If one or more URLs are blacklisted:

  * Message is stored as SPAM.
  * User is notified within the UI.
* Implements blacklist add/delete routes for admin usage.

### **4. MVC Rendering Logic**

* Dynamic templates for login, inbox, message view, spam notices, and errors.
* UI adapts based on server decisions (e.g., blocked email, missing token).

---

# Technology Stack

**Backend:** Node.js, Express.js
**Microservice:** C++ (multithreaded TCP server)
**Frontend:** Server-rendered views (EJS / equivalent)
**Security:** JWT
**Deployment:** Docker + Docker Compose
**Testing:** Manual endpoint testing + integrated runs with C++ server

---

# Installation & Running

## **Prerequisites**

* Node.js (v18+ recommended)
* g++ (for C++ blacklist server)
* Docker & Docker Compose
* npm

---

## **1. Install Node.js dependencies**

```bash
npm install
```

## **2. Build and run the C++ blacklist server using Docker**

```bash
docker build -t blacklist-server .
docker run -p 8081:8081 blacklist-server
```

## **3. Run the Node.js server**

```bash
npm start
```

By default:

* Web server runs on: **[http://localhost:3000](http://localhost:3000)**
* C++ blacklist server runs on: **localhost:8081**

---

# Project Structure

```
/src
  /controllers
  /services
  /models
  /routes
  /views
  /utils
/cpp-blacklist-server
  blacklist_server.cpp
  blacklist_manager.cpp
  blacklist_manager.h
  socket_utils.cpp
  ...
/public
/test
Dockerfile
docker-compose.yml
README.md
package.json
```

---

# Development Guidelines

### **SOLID Principles**

* Each class and module has clear responsibility.
* Controllers are intentionally thin.
* Business logic is encapsulated inside services.
* Dependency injection is used to avoid hard coupling.

### **Error Handling**

* API is fully deterministic.
* No use of HTTP 500 under any circumstance.
* Every invalid request is answered with:

  * 400 (malformed input)
  * 404 (resource not found)

### **Testing Notes**

* Blacklist server must be running before testing outgoing-mail logic.
* All link extraction and classification cases tested:

  * No links
  * One valid link
  * Multiple links
  * One or more blacklisted links

---

# Contribution Notes

* Each component is written to be easy to modify or extend.
* To add a new feature (e.g., drafts, labels), create a new service + routes and plug them into MVC flow.
* When modifying blacklist behavior, ensure Node server and C++ server remain synchronized in protocol format.

---

# Known Limitations

* No persistent database beyond in-memory structures unless added.
* C++ server must be running to send mail containing links.
* No UI framework; all rendering is template-driven.

---

# Summary

This project demonstrates a complete multi-service Gmail-like system with:

* Strong architecture
* SOLID design
* Clean REST API behavior
* Frontend rendering logic
* Cross-language blacklist validation over sockets
* Deployment via Docker


