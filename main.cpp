#include <cstdio>
#include <stdio.h> 
#include <stdlib.h>
#include <array>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>  
#include <netinet/in.h> 
#include <fstream>
#include <signal.h>
#include <string>
#include <vector>
#include <functional>
#include <math.h>
#include <sstream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sys/inotify.h>


#include "src/Socket.hpp"
#include "src/Player.hpp"
#include "src/StringProcessing.hpp"
#include "src/EventHandling.hpp"
#include "src/include/sqlite/sqlite3.h"
#include "src/Database.hpp"
#include "src/DataInput.hpp"
#include "src/ConfigParse.hpp"


using namespace std;

Config cfg;
int main()
{   
    vector<Player> playerlist;
    string cmd = "tail -f ~/Elo/log.log";
    cout << "cfg values: " << endl;
    db_init();
    db_create_table();
    cout << cfg.ip << endl << cfg.path_to_file << endl << cfg.port << endl << cfg.rconpass << endl;
    if (cfg.path_to_file.size()) {
        read_tail_stdout(cfg.path_to_file, playerlist);
    }
    else {
        read_server_stdout("sh ./start.sh", playerlist);
    }
}
