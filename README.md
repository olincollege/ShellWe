# `ShellWe`

`Shell` `We` chat? Using this application, a user can chat with other users in
the chat room.

## Context

The chat system consists of two main parts: a server application and a client
application, both written in C.

| ![Context Diagram](./docs/context.jpg) | 
|----------------------------------------|

## Containers

### Server Application:

- **Main Listener**: This component continuously listens for incoming TCP
  connections on a predefined port. Once a connection is established, it spawns
  a new thread (Client Handler) to handle the connection.
- **Client Handler Thread**: Each connection has a dedicated thread. This thread
  is responsible for reading messages from the client, processing them if
  needed. It will relay the message to all the other connected clients.
- **Concurrency**: A single mutex is used for protecting an array that contains
  all the connected socket file descriptor numbers.
- **Data Structures**:
    - **Client Handler Argument**: A struct that contains the socket number of
      current client, pointer to number of connected clients, pointer to an
      array of socket FD, and a pointer to the shared mutex for client socket FD
      array. This is used as an argument for client handler thread.
