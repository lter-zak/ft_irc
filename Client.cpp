#include "Client.hpp"

Client::Client(std::string nickName, std::string userName, int fd)
{
    std::cout<<"CLIENT CONSSTRUCTOR"<<std::endl;
    _username = userName;
    _nickname = nickName;
    _onlineFD = fd;
    guest = true;
}

std::string Client::getNickname()
{
    return _nickname;
}

std::string Client::getUsername()
{
     return _username;
}
bool Client::getPass()
{
    return _pass;
}

void Client::setPass(bool pass)
{
    _pass = pass;
}
int Client::getOnlineFD()
{
    return _onlineFD;
}
void Client::setOnlineFD(int fd)
{
    _onlineFD = fd;
}


void Client::setNickname(std::string nickname)
{
    _nickname = nickname;
}

void Client::setUsername(std::string username)
{
    _username = username;
}