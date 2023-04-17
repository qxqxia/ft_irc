#include "Freenode.hpp"

std::string forward_RPL(int code, Server *serv, User *user, std::string arg1, std::string arg2)
{
    std::stringstream ss;
    ss << code;
    std::string code_in_string = ss.str();
    if (code < 10)
        code_in_string.insert(0, 2, '0');
    else if (code < 100)
        code_in_string.insert(0, 1, '0');

    std::string ret;

    if (user)
        ret += ":" + serv->get_server_name() + " " + code_in_string + " " + user->get_nickname() + " ";
    else
        ret += ":" + serv->get_server_name() + " " + code_in_string + " " + "*" + " ";
    switch (code)
    {
        case 001:
            ret += RPL_WELCOME(user->get_nickname(), user->get_username(), user->get_hostname());
			break;
		case 002:
            ret += RPL_YOURHOST;
			break;
        case 003:
            ret += RPL_CREATED;
			break;
        case 004:
            ret += RPL_MYINFO;
			break;
        case 005:
            ret += RPL_BOUNCE(serv->get_port());
			break;
        case 221:
            ret += RPL_UMODEIS(arg1);
			break;
        case 324:
            ret += RPL_CHANNELMODEIS(arg1, arg2);
			break;
        case 331:
            ret += RPL_NOTOPIC(arg1);
			break;
        case 332:
            ret += RPL_TOPIC(arg1, arg2);
			break;
        case 353:
            ret += RPL_NAMREPLY(arg1, arg2);
			break;
        case 366:
            ret += RPL_ENDOFNAMES(arg1);
			break;
        case 367:
            ret += RPL_BANLIST(arg1, arg2);
			break;
        case 368:
            ret += RPL_ENDOFBANLIST(arg1);
			break;
        case 381:
            ret += RPL_YOUREOPER;
			break;
        case 401:
            ret += ERR_NOSUCHNICK(arg1);
			break;
        case 403:
            ret += ERR_NOSUCHCHANNEL(arg1);
			break;
        case 404:
            ret += ERR_CANNOTSENDTOCHAN(arg1);
			break;
        case 405:
            ret += ERR_TOOMANYCHANNELS(arg1);
            break;
        case 431:
            ret += ERR_NONICKNAMEGIVEN;
			break;
        case 432:
            ret += ERR_ERRONEUSNICKNAME(arg1);
			break;
        case 433:
            ret += ERR_NICKNAMEINUSE(arg1);
			break;
        case 441:
            ret += ERR_USERNOTINCHANNEL(arg1, arg2);
			break;
        case 442:
            ret += ERR_NOTONCHANNEL(arg1);
			break;
        case 461:
            ret += ERR_NEEDMOREPARAMS(arg1);
			break;
        case 464:
            ret += ERR_PASSWDMISMATCH;
			break;
        case 467: // new
            ret += ERR_KEYSET(arg1);
			break;
        case 471:
            ret += ERR_CHANNELISFULL(arg1);
			break;
        case 472:
            ret += ERR_UNKNOWNMODE(arg1, arg2);
            break;
        case 473:
            ret += ERR_INVITEONLYCHAN(arg1);
			break;
        case 474:
            ret += ERR_BANNEDFROMCHAN(arg1);
			break;
        case 475:
            ret += ERR_BADCHANNELKEY(arg1);
			break;
        case 481:
            // ERR_NOPRIVILEGES (481) numeric 
            // is sent to users who try to do an oper-only action
            // but fail because they aren’t an IRC operator.
            //  Msg to display:
            //      :server 481 <nick> :Permission denied - You are not an IRC operator
            ret += ERR_NOPRIVILEGES;
            break;
        case 482:
            ret += ERR_CHANOPRIVSNEEDED(arg1);
			break;
        case 484:
            ret += ERR_RESTRICTED;
			break;
        case 501:
            ret += ERR_UMODEUNKNOWNFLAG;
			break;
		default:
			;
    }
    return ret;
}
