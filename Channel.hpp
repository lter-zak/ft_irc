#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <vector>
#include <iostream>
class Channel
{
    private:
        std::string _password;

    public:
        Channel();
        std::vector<int>  clients; 
        std::vector<int>  adminIDs; 
        //int adminID;
        unsigned long ChatLimit;


        bool mode_i;
        bool mode_t;
        bool mode_k;
        bool mode_l;

         std::string getPassword();
         void setPassword(std::string pass);
};

#endif  