 #pragma once
#ifndef __RPL_ERR_HPP__
# define __RPL_ERR_HPP__

# include "Freenode.hpp"

std::string get_RPL_ERR(int code, Server *serv, User *user, std::string args, std::string args2);

//RPL

//001
# define RPL_WELCOME(nick, user, host) \
    (":Welcome to the <FreeNode/Discord> Network " + nick + "!" + user + "@" + host)
//    :server 001 <nick> :Welcome to the <network> Network, <nick>[!<user>@<host>]


//002
# define RPL_YOURHOST \
    ("Your host is " SERVER_NAME ", running version " ver)

//003
# define RPL_CREATED \
    ("This server was created May 9, 2023")

//004
# define RPL_MYINFO \
    (SERVER_NAME " ver. " ver)
    // (SERVER_NAME " ver. " ver " " USER_MODE " " CHANNEL_MODE)

//005
# define RPL_BOUNCE(port) \
    ("Try server " SERVER_NAME ", port " + port)

//221
# define RPL_UMODEIS(args) \
    (args)

//324
# define RPL_CHANNELMODEIS(channel, modes) \
    (channel + " " + modes)

//331
# define RPL_NOTOPIC(channel) \
    (channel + " :No topic is set")

//332
# define RPL_TOPIC(channel, topic) \
    (channel + " :" + topic)

//353
# define RPL_NAMREPLY(channel, listOfUsers) \
    ("= " + channel + " :" + listOfUsers)

//366
# define RPL_ENDOFNAMES(channel) \
    (channel + " :End of NAMES list")

//367
# define RPL_BANLIST(channel, banmask) \
    (channel + " " + banmask)

//368
# define RPL_ENDOFBANLIST(channel) \
    (channel + " :End of channel ban list")

//381
# define RPL_YOUREOPER \
    (":You are now an IRC operator")

//ERR

//401
# define ERR_NOSUCHNICK(nick) \
    (nick + " :No such nick/channel")

//403
# define ERR_NOSUCHCHANNEL(channel) \
    (channel + " :No such channel")

//404
# define ERR_CANNOTSENDTOCHAN(channel) \
    (channel + " :Cannot send to channel")

//405
# define ERR_TOOMANYCHANNELS(channel) \
    (channel + " :You have joined too many channels")

//431
# define ERR_NONICKNAMEGIVEN \
    (": No nickname given")

//432
# define ERR_ERRONEUSNICKNAME(nick) \
    (nick +" : Erroneous nickname")

//433
# define ERR_NICKNAMEINUSE(nick) \
    (nick + " : Nickname is already in use")

//441
# define ERR_USERNOTINCHANNEL(nick, channel) \
    (nick + " " + channel + " : They aren't on that channel")

//442
# define ERR_NOTONCHANNEL(channel) \
    (channel + " : You're not on that channel")

//461
# define ERR_NEEDMOREPARAMS(command) \
    (command + " : Not enough parameters")

//464
# define ERR_PASSWDMISMATCH \
    (": Password incorrect")

//471
# define ERR_CHANNELISFULL(channel) \
    (channel + " : Cannot join channel (+l)")

//472
# define ERR_UNKNOWNMODE(char, channel) \
              (char + " : is unknown mode char to me for " + channel)

//473
# define ERR_INVITEONLYCHAN(channel) \
    (channel + " : Cannot join channel (+i)")

//474
# define ERR_BANNEDFROMCHAN(channel) \
    (channel + " : Cannot join channel (+b)")

//475
# define ERR_BADCHANNELKEY(channel) \
    (channel + " : Cannot join channel (+k)")

//481
# define ERR_NOPRIVILEGES \
    (": Permission Denied- You're not an IRC operator")

//482
# define ERR_CHANOPRIVSNEEDED(channel) \
    (channel + " : You're not channel operator")

//484
# define ERR_RESTRICTED \
    (": Your connection is restricted!")

//501
# define ERR_UMODEUNKNOWNFLAG \
    (": Unknown MODE flag")
    

// #endif


//669 --- BOT CMD-NOT-FOUND ERR
#define BOT_NOCOMMAMD(cmd) \
    (cmd + " :No such command.")

#endif