![](https://img.shields.io/badge/C++-Internet&nbsp;Relay&nbsp;Chat-red.svg?style=flat&logo=c%2B%2B)

# Basic checks
- Check poll() (or equivalent) 
  - only one
  - called every time before accept, read, write etc.
  - no errno
- Verify fnctl()
  - use of fcntl(fd, F_SETFL, O_NONBLOCK)

# Networking 1
- Use 'nc'
  - connect to the server
  - send commands
  - the server answers back
- From now on, use our reference IRC client
  - try multiple clients
  - test with `IRSSI` and `nc` at the same time
- Join a channel
   - messages are sent to all

# Networking 2
- Use `nc` to send partial commands
... What are partial commands ? (🟡)
  - the server answers correctly
  - other connections run fine
- Unexpectedly kill a client
  - check if server still works and can still get new client
- (🟡) Kill a `nc` with half of a command sent
- Stop a client (^-Z)
  - Flood the channel using another client
    - Server should not hang
  - Check leak


# Client side
- Use `nc` to perform
  - authenticate (🟡)
  - set nick
  - set username
  - join chan
- Use `irssi` to perform
  - authenticate
  - set nick
  - set username
  - join chan
- Run `/PRIVMSG` with different parameters
- Run `/NOTICE` with different parameters
    - work with different parameters
- Check that a regular user cannot do operator actions
- Check that a ___chanop___ can 
- (🟡) All the channel operations should be tested  


# File transfer
- File transfer with `irssi` (🟡)

# A small bot
- ___Todo___
