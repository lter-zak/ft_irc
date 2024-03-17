#include "Errors.hpp"

const std::string  ERR_NEEDMOREPARAMS(const std::string& command)
{ 
    return std::string("461 "  + command + " : Not enough parameters\n"); 
}

const std::string ERR_PASSWDMISMATCH(const std::string& command)
{
    return std::string("464 "  + command + " : Password incorrect\n");
}

const std::string ERR_PASSISALREADYSET(const std::string& command)
{
    return std::string("000 "  + command + " : Password is already set\n");
}

const std::string ERR_NICKCOLLISION(const std::string& command)
{
    return std::string("436 "  + command + " : Nickname collision KILL\n");
}

const std::string ERR_ALREADYREGISTRED(const std::string& command)
{
    return std::string("462 "  + command + " : You may not reregister\n");
}

const std::string ERR_NICKNAMEINUSE(const std::string& command, const std::string& name)
{
    return std::string("433 "  + command + " " + name +" : Nickname is already in use\n");
}

const std::string ERR_USERISOFLINE(const std::string& command, const std::string& name)
{
    return std::string("001 "  + command  +" : User " + name + " is ofline\n");
}

const std::string ERR_BADCHANNELKEY(const std::string& name)
{
    return std::string("475 " + name  + " : Cannot join channel (+k)\n");
}

const std::string ERR_CHANNELISFULL(const std::string& name)
{
    return std::string("471 <" + name  + ">  : Cannot join channel (+l)\n");
}

const std::string ERR_TOOMANYCHANNELS(const std::string& name)
{
    return std::string("405 <" + name  + ">  : You have joined too many channels\n");
}

const std::string ERR_NOSUCHCHANNEL(const std::string& name)
{
    return std::string("403 <" + name  + ">  :No such channel\n");
}

const std::string ERR_CHANOPRIVSNEEDED(const std::string& name)
{
    return std::string("482 <" + name  + "> :You're not channel operator\n");
}

const std::string ERR_CANTKICKADMIN(const std::string& name)
{
    return std::string("002 <" + name  + "> : You are admin, you cant kick yourself from channel\n");
}

const std::string ERR_UNKNOWNMODE(const std::string& name)
{
    return std::string("472  <" + name  + "> : is unknown mode char to me\n");
}
