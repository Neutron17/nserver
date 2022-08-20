# Neutron Server

unix signal server for my applications

# WARNING: 
This project is only experimental now, the server is half done, 
and the client side implementation has not even started

## Requirements

 - C99: stdbool, // comments, inline initializers(={...}), variadoc macros
 - c standard library ex:glibc

## How does it work?

1. the client sends ```SIGUSR1``` to the server, with a
```c
union ClientMsg
```
on the side (for more details see: msg.h)
2. the server prepares a file for further communication and when done signals the client with ```SIGUSR1``` and
```c
union ServerMsg
```
(msg.h)

