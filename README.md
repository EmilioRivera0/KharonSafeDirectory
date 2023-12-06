# Kharon Safe Directory
Kharon Safe Directory project consists of a pair of softwares used to protect, manage and log the access to a directory and its entries through a client server architecture. As mentioned previously, two main processes are contained in this project being the server and one or multiple clients.

## Server
The server process is in charge of taking control of the specified directory, changing its owner and group to root as well as removing all the permissions of the directory’s entries and only leaving the read & execute permissions to the directory, enabling anyone to view the content of the directory. This process will wait for an incoming message from a client through a FIFO containing the file it wants to access to, the UID of the user to make owner of the file and the permissions to grant to the new owner of the file. Once an incoming message is received, it will process the contents of the message and grant the specified access permissions to the chosen file and system user specified by the client, logging this action. Finally it will wait for the client to release the file so it can establish the owner to root as well as removing the permissions from the file and logging this action permitting a previously waiting client to access the same file with its specified owner and permissions.
This cycle of listening for a new message, processing it, granting access, waiting for the client to release the file and revoking access can be done parallelly thanks to the implementation of multithreading, process & thread synchronization and IPC. This enables multiple clients to be connected and communicate with the server simultaneously and for multiple clients to be in a waiting queue to access a file without generating any race condition with the Input Output operations done as well as maintaining an ordered access, monitoring and logging from the server.

## Client
The client process has a start menu where the user can either Log In or Sign Up (for now the accounts are not persistent since they are stored in the process memory), where once logged in, the main menu is displayed where the user can either view its account information or choose the access mode (permissions to be granted), the file and system user it wants the server to grant access to. Once the request is sent, processed by the server and the client becomes the first in the waiting queue of the requested file, it will have access to the file and release it whenever the user wants to.

## Technical Notes
- **Client and Server MUST be executed with root prviledges, either using su or sudo commands or logging as root user**
- Only one server instance can be running.
- The server process must be up and running for the client processes to work.
- The reset.sh is used to restore the test directory and its entrys to the user specified by the $user variable:
```bash
user=<username>
chown $user:$user ./test/
```
- To close the server correctly, you need to press enter once in the terminal running the server. This is because there is a thread constantly listening for any user input to close, unlink all the semaphores, the FIFO and shared memory as well as freeing the allocated memory.
- The compile.sh file found inside each bin directory can be used to compile each of the programs (Client & Server).
- Although this project is targeted to Linux OS, it may be necessary to compile the source code before using the programs since depending on the plataform it could generate unexpected errors.
- Both Client and Server have their own exceptions that help in providing more precise information about the exception generated.

## Project Structure
```
src/
├── server/
│   ├── bin/
│   │   ├── test/
|   |   |    └── *test files
│   │   ├── Server (executable)
│   │   ├── compile.sh
│   │   └── reset.sh
│   ├── lib/
│   │   ├── include/
│   │   │   └── *header files (.h)
│   │   └── src/
│   │       └── *source code (.cpp)
│   └── main.cpp
└── client/
    ├── bin/
    │   ├── Client (executable)
    │   └── compile.sh
    ├── lib/
    │   ├── include/
    │   │   └── *header files (.h)
    │   └── src/
    │       └── *source code (.cpp)
    └── main.cpp
```

## Tech Stack

**Client & Server** POSIX Threads, POSIX Semaphores, POSIX Shared Memory, Linux FIFOs, Linux Filesystem API, C & C++ Standard Libraries and Unix C Standard library.

## Authors
Frontend:
- [@JuanPabloGHC](https://github.com/JuanPabloGHC)
Bachend:
- [@EmilioRivera0](https://github.com/EmilioRivera0)
