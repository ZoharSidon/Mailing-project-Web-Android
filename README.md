# Gmail-WebApp (Exercise 4)

## Team Members
- Zohar Sidon  
- Ofri Keidar  
- Eyal Levy  

---

## Overview
**Miluimail** is a Gmail-like application built for exercise 4. It includes:

- A React frontend for signup, login, inbox and labels, with light/dark theme switching.

- A Node.js/Express backend with JWT authentication.

- A C++ TCP service that uses a Bloom Filter to quickly check whether URLs are blacklisted.


---

## Project Structure

```
SRC/
├── BloomFilter/               # C++ Bloom filter blacklist server
├── Client/                    # Client side 
├── Command/                   # C++ command utilities
├── Hash/                      # C++ hash-related utilities
├── Input/                     # C++ input handling logic 
├── MVC/                       # Main Node.js MVC web server
│   ├── controllers/           # Express controllers (business logic)
│   ├── models/                # Data models (User, Mail, Label, etc.)
|   ├── middleware/            # Authentication helper  
│   ├── routes/                # API route definitions
│   ├── services/              # Service layer (blacklist client, helpers)
│   ├── sockets/               # WebSocket 
│   ├── app.js                 # Express app initialization
│   ├── package.json
│   └── package-lock.json
├── GmailWeb/                  # React app 
├── Sockets/                   # C++ socket utilities 
├── CMakeLists.txt             # CMake config for C++ code
├── docker-compose.yml         # Compose file to run all services
├── Dockerfile.server          # Dockerfile for BloomFilter server
├── Dockerfile.web             # Dockerfile for Node.js web server
└── main.cpp                   # C++ main entry 
```
---

## Architecture
**High-level components:**

- Client (React):

  - Login and Signup forms

  - inbox view, labels, search and etc.

- API Server (Node.js/Express):

  - Public routes for registration and token issuance.

  - Protected routes for user data (mails, labels, blacklist) guarded by JWT.


- Core Service (C++):

  - TCP server that receives URL queries and answers quickly based on a Bloom Filter + explicit blacklist.
---
## Auth Flow (JWT)
- Signup via POST /api/users (open).

- Login via POST /api/tokens → backend returns a JWT.

- Client stores the token (e.g, localStorage).

- Every call to protected endpoints must include Authorization: Bearer <token>.

- Server verifies the token on each protected request.

---
## Running the Project (Docker)
1. Clone the repository and open Docker Desktop.

2. From the repo root, enter:

    - 'docker compose build'
    - 'docker compose up'

3. Open the UI: http://localhost:3000 ('npm start')

4. API server listens on http://localhost:8080 ('node app.js')


---

## Design Principles

- **MVC:** Clear separation of concerns for maintainability and easy evolution.

- **SOLID:** Small, focused modules that are easy to extend and test.

- **Loose Coupling:** Business logic isolated from routing, storage, and UI; DI where helpful.

- **API-First:** RESTful JSON endpoints only (no server-rendered HTML).

- **Stateless Auth:** JWT on every protected request; no server sessions.

- **Security by Default:** Hashed passwords, least-privilege access, strict CORS, safe file uploads.

- **Predictable Errors:** Consistent HTTP status codes and structured { error } responses.

---

## Screenshots & Output



---
