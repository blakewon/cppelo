#include <string>
#include <vector>
#include <iostream>
#include <fstream>


std::string init_value(const std::string& query, const std::vector<std::string>& lines);
class Config {

public:
    std::fstream myfile;
    std::string path_to_file = "";
    int port = 29070;
    std::string ip = "127.0.0.1";
    std::string rconpass = "";
    Config();
};

/*[SETTINGS]
#For the values you do not want to change leave as-is
#Lines that start with "#" are comments only


port = 29070
#Default port for MBII servers is 29070, change if you have a custom one

ip = 127.0.0.1
#IP address which Elo will communicate with, do not change unless you know what you're doing

#logpath = path/to/log/file
#uncomment this line to use tail -f of redirected stderr instead of reading stdout directly
#edit start.sh like so: ./openjkded.i386 +set fs_game "MBII" +set net_port 29070 +exec server.cfg 2>path/to/log/file


elogain = 10
#This refers to K in elo's original equation, if you want to tinker with it I suggest reading up on the math first.

rcon = rconpassword
#rcon password of your server, the script will not work without it.
#this must be edited to the rconpassword of your server
*/