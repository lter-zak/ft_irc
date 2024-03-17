
# include "Server.hpp"

void Server::pass_cmd(std::string cmd, int index)// stex indexy clineti mapi arajinna
{  
    std::vector<std::string>  splited =  split(cmd, ' '); 
    if (Clients.find(index) != Clients.end()  )
    {
         if (splited.size() != 2)
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_NEEDMOREPARAMS(splited[0]));
        return ;
    }  
    if(splited[1] != _password && Clients[index]->getPass() == false)
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_PASSWDMISMATCH(splited[0]));
        return ; 
    }
    if(splited[1] == _password  && Clients[index]->getPass() == true)
    {
         sendMyMsg(Clients[index]->getOnlineFD(),  ERR_PASSISALREADYSET(splited[0]));
            return ;
    } 
    else if(Clients[index]->getPass() == true)
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_PASSISALREADYSET(splited[0]));
        return ;
    }
    Clients[index]->setPass(true);         
   }  
    //    else if(Clients.find(index) != Clients.end() ) {
    //         sendMyMsg(Clients[index]->getOnlineFD(),  "You already have password\n");
    //    }                                                                                     
}



void Server::nick_cmd(std::string cmd, int index)
{
    std::vector<std::string>  splited =  split(cmd, ' '); 
    if (splited.size() != 2)
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_NEEDMOREPARAMS(splited[0]));
        return ;
    }  

        if(isClientFull(Clients[index]->getOnlineFD())!=-1)
        {
             for (std::map<int, Client*>::iterator it = Clients.begin(); it != Clients.end(); ++it)
            {
                if(it->second->getNickname() == splited[1])
                {
                    sendMyMsg(Clients[index]->getOnlineFD(), ERR_NICKNAMEINUSE(splited[0], splited[1]));
                    return;
                } 
            }
        }
        Clients[index]->setNickname(splited[1]);
    
}


void Server::user_cmd(std::string cmd, int index)
{
    std::vector<std::string>  splited =  split(cmd, ' '); 
    if (splited.size() != 2) // --------------------------------- 5
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_NEEDMOREPARAMS(splited[0])); 
        return ;
    }  

    if(Clients[index]->getUsername() == "" )
    {
        Clients[index]->setUsername(splited[1]);
    } 
    else 
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_ALREADYREGISTRED(splited[0]));
        return ;
    }
}



void Server::quit_cmd(std::string cmd, int index)
{
    std::vector<std::string>  splited =  split(cmd, ' '); 
    if (splited.size() != 1)
    {
        sendMyMsg(Clients[index]->getOnlineFD(), ERR_NEEDMOREPARAMS(splited[0]));
        return ;
    } 
    for(int i = 0; i < 5; i++)
    {
        if(client_socket[i]==Clients[index]->getOnlineFD())
        {
            client_socket[i] = 0;
            close(Clients[index]->getOnlineFD());
            Clients[index]->setOnlineFD(-1);
        } 
    }
}



 void Server::privmsg_cmd(std::string cmd, int index)
 {



    if(isClientFull(Clients[index]->getOnlineFD())==-1 )
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  "You dont have account, the first register or log in\n");
        return;
    }
    std::vector<std::string>  splited =  split(cmd, ' '); 
    std::string msg = "";
    if(Clients[index]->getNickname() == splited[1])
    {
        return;
    }   
    for(unsigned long i = 2; i < splited.size(); i++)
    {
        msg = msg+splited[i]+" ";
    }
    if (splited.size() < 3)
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_NEEDMOREPARAMS(splited[0]));
        return ;
    } 

    for (std::map<int, Client*>::iterator it = Clients.begin(); it != Clients.end(); ++it)
    {
        if(it->second->getNickname() == splited[1] && it->second->getOnlineFD() != Clients[index]->getOnlineFD())
        {
            if(it->second->getOnlineFD()==-1){
                sendMyMsg(Clients[index]->getOnlineFD(),  ERR_USERISOFLINE(splited[0], splited[1]));
                return;
            }
            sendMyMsg(it->second->getOnlineFD(), Clients[index]->getNickname() + ": " + msg + "\n");
            return;
        } 
    }



    //for (std::map<std::string, Channel*>::iterator it =  Channels.begin(); it !=  Channels.end(); ++it) 
  //  {
        // if(it->first==splited[1])
        // {
        //     for (unsigned long i = 0; i < it->second->clients.size(); i++)
        //     {
        //         if(it->second->clients[i]!=index)
        //         sendMyMsg(Clients[it->second->clients[i]]->getOnlineFD(), Clients[index]->getNickname() + ": "+msg+"\n");
        //     }
        // } 
        // return;
   // }
    sendMyMsg(Clients[index]->getOnlineFD(),  "Dont exist "+splited[1]+" user or channel\n");
 }


  void Server::join_cmd(std::string cmd, int index)
  {
        // petqa tenal usera te che, nax petqa user lini
        if(isClientFull(Clients[index]->getOnlineFD())==-1 )
        {
            sendMyMsg(Clients[index]->getOnlineFD(),  "You dont have account, the first register or log in\n");
            return;
        }
        std::vector<std::string>  splited =  split(cmd, ' '); 
        if (splited.size() < 2 || splited.size() > 4)
        {
            sendMyMsg(Clients[index]->getOnlineFD(),  "Incoorect parameters count\n");
            return ;
        } 

        if(splited[1][0]!='#' && splited[1][0]!='&')
        {
            sendMyMsg(Clients[index]->getOnlineFD(),  ERR_BADCHANNELKEY(splited[1]));
            return;
        }
        if(Channels.find(splited[1]) == Channels.end())
        {
            if(Clients[index]->ChannelCount >= 5)
            {
                sendMyMsg(Clients[index]->getOnlineFD(), ERR_TOOMANYCHANNELS(splited[1]));
                return ;
            }
            if (Channels.size() >= 5)
            {
                sendMyMsg(Clients[index]->getOnlineFD(), ERR_CHANNELISFULL(splited[1]));
                return ;
            }
            Channels.insert(std::pair<std::string, Channel*>(splited[1], new Channel()));
        

            (Channels[splited[1]]->clients).push_back(index);
            sendMyMsg(Clients[index]->getOnlineFD(), "You are created "+splited[1]+" channel\n");
            Clients[index]->ChannelCount++;
            if (splited.size() == 2)
            {
                Channels[splited[1]]->setPassword(splited[2]);
                Channels[splited[1]]->mode_k = true;
            }
         
            if((Channels[splited[1]]->clients.size()==1)){
                Channels[splited[1]]->adminIDs.push_back(index); // liany vmtacuma vor ka es masy, kgtnenq khanenq
            }
        } 
        else 
        {
            for (unsigned long i = 0; i <  Channels[splited[1]]->clients.size(); i++)
            {
                if(Channels[splited[1]]->clients[i] == index)
                {
                    sendMyMsg(Clients[index]->getOnlineFD(), "You  already joined "+splited[1]+" channel\n");
                    return;
                }
            }
            if(Channels[splited[1]]->clients.size() < Channels[splited[1]]->ChatLimit || Channels[splited[1]]->ChatLimit==0 )
            {
                (Channels[splited[1]]->clients).push_back(index);
                sendMyMsg(Clients[index]->getOnlineFD(), "You joined "+splited[1]+" channel\n");
                Clients[index]->ChannelCount++;
            } 
            else
            {
                sendMyMsg(Clients[index]->getOnlineFD(), ERR_CHANNELISFULL(splited[1]));
            }
        }
  }



void Server::kick_cmd(std::string cmd, int index)
{
    if(isClientFull(Clients[index]->getOnlineFD())==-1 )
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  "You dont have account, the first register or log in\n");
        return;
    }
    std::vector<std::string>  splited =  split(cmd, ' '); 
    if (splited.size() !=3)
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  "Incoorect parameters count\n");
        return ;
    } 

    // nax petqa lini channely
    if(Channels.find(splited[1]) == Channels.end())
    {
            sendMyMsg(Clients[index]->getOnlineFD(), ERR_NOSUCHCHANNEL(splited[1]));
            return;
    }
    else
    {  //??????
        // if(Channels[splited[1]]->adminID!=index){
        //         sendMyMsg(Clients[index]->getOnlineFD(), ERR_CHANOPRIVSNEEDED(splited[1]));
        //     return;
        // }

for (std::map<int, Client*>::iterator it = Clients.begin(); it != Clients.end(); ++it) 
 {
        if( it->second->getNickname() == splited[2])
        {
            for(std::vector<int>::iterator it1 = Channels[splited[1]]->clients.begin(); it1 != Channels[splited[1]]->clients.end();++it1)
            {
                if(it->first == *it1)
                {
                    //??????????
                    // if(Channels[splited[1]]->adminID==*it1)
                    // {
                    //        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_CANTKICKADMIN(splited[1]));
                    //        return;
                    // }
                    Channels[splited[1]]->clients.erase(it1);
                    return;
                }
            }
        }
    }


    
        
    } 
}









void Server::mode_cmd(std::string cmd, int index)
{
    if(isClientFull(Clients[index]->getOnlineFD())==-1 )
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  "You dont have account, the first register or log in\n");
        return;
    }
    std::vector<std::string>  splited =  split(cmd, ' '); 
    if (splited.size() < 3)
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  "Incoorect parameters count\n");
        return ;
    } 

    if(Channels.find(splited[1]) == Channels.end())
    {
            sendMyMsg(Clients[index]->getOnlineFD(), ERR_NOSUCHCHANNEL(splited[1]));
            return;
    }
    else
    {


   
      
        // if(Channels[splited[1]]->adminID != index){
        //         sendMyMsg(Clients[index]->getOnlineFD(), ERR_CHANOPRIVSNEEDED(splited[1]));
        //     return;
        // }

            for(std::vector<int>::iterator it = Channels[splited[1]]->adminIDs.begin(); it != Channels[splited[1]]->adminIDs.end();++it)
            {
                if(index==*it){

    if (splited[2][0] == '+')
    {
        if (splited[2][1] == 'i')
        {
            if (!Channels[splited[1]]->mode_i)
            {
                Channels[splited[1]]->mode_i = true;
                sendMyMsg(Clients[index]->getOnlineFD(), "+i mode is set");
            }
            else sendMyMsg(Clients[index]->getOnlineFD(), "Channel already Invite-only\n"); // heto petqa hanel elser-y
            return ;
        }
        if (splited[2][1] == 't')
        {
            if (!Channels[splited[1]]->mode_i)
            {
                Channels[splited[1]]->mode_t = true;
                sendMyMsg(Clients[index]->getOnlineFD(), "+t mode is set\n");
            }
            else sendMyMsg(Clients[index]->getOnlineFD(), "You cant chenge topic\n");
            return ;
        }
        if (splited[2][1] == 'k') //MODE #42 +k oulu   
        {
            if (!Channels[splited[1]]->mode_k)
            {
                Channels[splited[1]]->mode_k = true;
                if (splited.size() == 4)
                {
                    Channels[splited[1]]->setPassword(splited[3]);
                    sendMyMsg(Clients[index]->getOnlineFD(), "+k mode is set\n");
                }
                if (splited.size() == 3 && Channels[splited[1]]->getPassword() == "")
                        sendMyMsg(Clients[index]->getOnlineFD(), "You do not have a password, chouse one\n");
            }
            else sendMyMsg(Clients[index]->getOnlineFD(), "You cant chenge topic\n");
            return ;
        }
        if (splited[2][1] == 'o')
        {
            
            Channels[splited[1]]->adminIDs.push_back(index);
        }
        if (splited[2][1] == 'l')
        {
            if (splited.size() <=3 )
                sendMyMsg(Clients[index]->getOnlineFD(), "Not enough parametors\n");
            else 
            {
                unsigned long num = stoi(splited[3]);
                if(Channels[splited[1]]->clients.size() > num)
                {
                    sendMyMsg(Clients[index]->getOnlineFD(), "CHannle's users count more then limit\n");
                } 
                else 
                {

                Channels[splited[1]]->ChatLimit = stoi(splited[3]);
                }
            }
        }
    }
    else if (splited[2][0] == '-')
    {   

    }
    else 
    {
        sendMyMsg(Clients[index]->getOnlineFD(), ERR_UNKNOWNMODE(splited[2]));
        return ;
    }

                }
                return;
             

            }
  





    
        
    } 
}