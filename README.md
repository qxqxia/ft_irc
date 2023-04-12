![](https://img.shields.io/badge/C++-Internet&nbsp;Relay&nbsp;Chat-red.svg?style=flat&logo=c%2B%2B)

## Basic checks
- check poll() (or equivalent) 
    - only one
    - called every time before accept, read, write etc.
    - no errno
- verify fnctl()
    - use of fcntl(fd, F_SETFL, O_NONBLOCK)

## Networking
- use 'nc'
    - connect to the server
    - send commands
    - the server answers back
- try multiple connections at the same time
     - test with IRC client and nc
- join a channel
   - messages are sent to all

## Networking specials cases
- use 'nc' to send partial commands
    - the server answers correctly
    - other connections run fine
- kill a client
    - the server still works for other connections and new client
- kill a nc with half of a command sent
- stop a client (^-Z)
    - many details, see the checklist again

## Client commands
- verify PRIVMSG and NOTICE
    - work with different parameters
- check regular use cannot do operator actions 
    - test with an operator
    - test all the channel operation commands

## File transfer
- File transfer works with the reference IRC client

## A small bot

