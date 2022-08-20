# Neutron Server

unix signal server for my applications

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

