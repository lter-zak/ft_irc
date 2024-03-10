
#include <iostream>
#include <stdexcept>
#include "Server.hpp"


int parsPort(std::string port)
{
    if (port.find_first_not_of("0123456789") != std::string::npos)
        return -1;

    int result = stoi(port);

    if (result < 1024 || result > 65535)
        return -1;

        std::cout<<"Port is --->"<<result<<std::endl;
    return result;

}

std::string parsPassword(std::string password)
{
        (void)password;
        //vrody es petq chi
        return password;
}

int main(int count, char** argv)
{
    (void)argv;
   try
   {
        if (count != 3)
            throw std::invalid_argument("Wrong number of arguments.");
        int portNumber = parsPort(argv[1]);
        if (portNumber == -1)
            throw std::invalid_argument("Not available port number.");
        std::string password = parsPassword(argv[2]);

        Server * server = new Server(portNumber, password);
        server->createServer();
   }
    catch (const std::exception& exceptionVariable) 
    {
        std::cout << exceptionVariable.what();
    }
   catch(const char* e)
   {
    std::cerr << e << '\n';
   }
   return 0;
}