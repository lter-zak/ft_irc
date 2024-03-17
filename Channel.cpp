#include "Channel.hpp"

Channel::Channel()
{
    mode_i = false;
    mode_t = false;
    mode_k = false;
    mode_l = false;
    _password = "";
    ChatLimit = 0;
}


 std::string Channel::getPassword(){ return _password;}
void Channel::setPassword(std::string pass){ _password = pass;}