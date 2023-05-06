![](https://img.shields.io/badge/C++-Internet&nbsp;Relay&nbsp;Chat-red.svg?style=flat&logo=c%2B%2B)


# Now checking
- [ ] Do-the-f*ing-Math Bot
- [ ] Weekday-guessing Bot
- [ ] Date & Time Bot
- [ ] Find all RPL
- [:heavy_check_mark:] /dcc
  - `/dcc send ~@~<User> /mnt/nfs/homes/login/Downloads/Great.in`
  - `/dcc < list | get >`
- [:heavy_check_mark:] XCQ after ***get_input_from_client_sfd (sfd)***
- [:heavy_check_mark:] Candle 7
- [:heavy_check_mark:] Modify all for loops
- [:heavy_check_mark:] Re-add french MOTD


# "roi" (User)
- [:heavy_check_mark:] `+r` | set myself to `restricted` - avoid messages from unidentified users
- [:heavy_check_mark:] `+o` | set operator | Syntax <kbd> $ /mode #ROOM +o USER </kbd>
- [:heavy_check_mark:] `+i` 


# "maltvonkb" (Channel)
- [ ] `+m` | set moderation |  `/mode #chan +m`
- [ ] `+a` | auto-voice | +a users are auto-granted voice | `/mode #chan +a user` 
- [ ] `+l` | set limit | `/oper ...` ` /mode #ROOM +l 2`
- [ ] `+t` | topic protection | `/mode #chan +t `
- [ ] `+v` | give _user_ a voice in a moderated room |  `/mode #chan +v user`
- [ ] `+o` | give someone else _Operator_ role | `/mode #chan +o user`
- [ ] `+n` | no external messages |  `/mode #chan +n`
- [ ] `+k` | set chan key | `/oper ...` ` /mode #ROOM +k KEY`   
- [ ] `+b` | ban someone | `/oper ...` ` /mode #ROOM +b USER` 


# Command List 
- [ ] privmsg
- [x] ping
  - dropped /pong (incompatible w/ irssi)
- [x] part
- [x] quit
- [x] topic
- [x] oper
- [x] kick
- [x] kill
- [x] restart
- [x] n/name/names (builtin)


# Command List II
- [x] TOPIC works fine
  - `/topic` 
  - `/topic <new_topic>` (anyone can set topic)
- [x] RESTART works fine
  - `/oper n1 ####` 
  - `/restart`
  - <kbd> ALT + LEFT Arrow </kbd> 
- [x] KILL works fine
  - `/oper n1 ####`
  - `/kill n2` 
- [x] OPER, MODE + KICK works fine
  - `/oper n1 ####`
  - `/mode #room +o n2`
  - `/kick n1` (n2) 



# IRC Mysteries
- `anonymous!anonymous@anonymous`
  - The 1st "anonymous" refers to the user's nickname
  - The 2nd "anonymous" refers to the username
  - The 3rd "anonymous" refers to the hostname
  - eg. `john!johndoe@example.com`
- Valid port: `(port >= 6660 && port <= 6669) || port == 6679 || port == 6697`
  - The standard port for IRC connection is 6667
  - More often, IRC networks use a range instead of just one
  - A typical range is [6660, 6669] or [6660, 7000]
  - 6679 and 6697 are used for secure IRC connections
    - 6697 = standard port for SSL-encrypted IRC connections
    - 6679 = an alternative port for SSL connections
  - [*, 1024] = "well-known ports" reserved for specific services or applications
- Check the very beginning: from freenode/main to 
  - **Server	MY_SERVER( port, password ) ;** `
  - **MY_SERVER.connect_to_server() ;** 
- MY_SERVER.connect_to_server() ;
  - `1` **this->m_server_socket = Server::new_socket() ;**
    - **int sockt = socket(AF_INET, SOCK_STREAM, 0);** // create socket
    - **setsockopt(sockt, SOL_SOCKET, SO_REUSEADDR, & (int) tmp, sizeof( (int) tmp)) ;** // set up socket // ? opt??
    - **fcntl(sockt, F_SETFL, O_NONBLOCK)** // set socket to non-blocking mode
      - m_server.sin_addr.s_addr = INADDR_ANY
      - m_server.sin_port = htons( (unsigned short) (std::strtoul( m_port.c_str(), NULL, 0 )))
      - m_server.sin_family = AF_INET
    - **bind(sockt, (const struct sockaddr * )& m_server, sizeof(m_server))** // binding socket
    - **listen( sockt, MAX_CLIENTS /* 10 */ )** // listening on socket
  - _"listening..."_
  - `2` **while SERVER == alive**
  `handle SIGINT` `FD_ZERO(& rfds)` `FD_SET( m_server_socket, & rfds )`
  find `max_sockt_fd` (highest file descriptor) for `select`
  - **activity = select( Highest_Socket_FD + 1, & rfds, None, None, None)**
    - **if (FD_ISSET(this->m_server_socket, & rfds)**
      - **Server::new_connection()** wait for new conns
    - else
      - **buf = receive_msg(g_server_client_socket[ i ])** loop through G_Client_Sockets and receive msg/cmds
    - Here we XCQ the any command carried in the input
  - 





<!--

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

-->
