# Course-Registration-Portal

## Description
This project is a Client-Server application developed in C that manages course enrollments, student information, and faculty data. It utilizes system calls to interact with the Linux OS and file system, making it a robust and efficient solution for handling multiple clients simultaneously. The client connects to the server, which processes all the logic and data management tasks.

## Features
Client-Server Architecture: The client interacts with the server using sockets, where the server handles all the logic.
Multithreading: The server supports multiple clients concurrently using pthreads.
File Handling: Data is stored and managed using text files (course.txt, student.txt), with file locking mechanisms to prevent data corruption.
Modular Design: The server-side code is modular, making it easy to maintain and extend.
System Calls: Direct interaction with the Linux OS for file handling and networking.

## Technical Details 
 
### Language: C
### Concurrency: 
Managed using pthread library for handling multiple clients.
### File Operations: 
Uses standard C file handling functions (fopen, fwrite, fread, fseek) with file locking (fcntl) for safe concurrent access.
### Networking: 
Implemented using TCP sockets (socket, bind, listen, accept, recv, send).
