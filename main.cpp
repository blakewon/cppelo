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

/*<<Kill: 0 0 7: J^4|^7P ^4b^7lake_^4w^7on killed J^4|^7P ^4b^7lake_^4w^7on by MOD_SUICIDE

<<Player 0 spawned with userinfo: \\team\\r\\ip\\192.168.0.177:29071\\teamoverlay\\1\\snaps\\40\\sex\\male\\saber2\\none\\saber1\\single_1\\rate\\25000\\pbindicator\\1\\name\\J^4|^7P ^4b^7lake_^4w^7on\\model\\jeditrainer/default\\jp\\0\\ja_guid\\FC73F841C6064321AC9EE2F65CEA5B38\\<<handicap\\100\\forcepowers\\0-6-020300000010003133\\color4\\255\\color3\\7935\\color2\\255\\color1\\28415\\char_color_red\\255\\char_color_green\\255\\char_color_blue\\255\\cg_predictItems\\1
<<Player 1 spawned with userinfo: \\team\\r\\ip\\192.163.0.177:29071\\teamoverlay\\1\\snaps\\40\\sex\\male\\saber2\\none\\saber1\\single_1\\rate\\25000\\pbindicator\\1\\name\\segi\\model\\jeditrainer/default\\jp\\0\\<<handicap\\100\\forcepowers\\0-6-020300000010003133\\color4\\255\\color3\\7935\\color2\\255\\color1\\28415\\char_color_red\\255\\char_color_green\\255\\char_color_blue\\255\\cg_predictItems\\1
<<ClientConnect: (J^4|^7P ^4b^7lake_^4w^7on) ID: 0 (IP: 192.168.0.177:29071)

<<broadcast: print "J^4|^7P ^4b^7lake_^4w^7on^7 @@@PLRENAME blake\n"

<<ShutdownGame:

broadcast: print "blake @@@PLDUELACCEPT segi!\n"
0: say: segi: "np"
1: say: blake: "mjok, slobodan si"
Kill: 0 1 86: segi killed blake by MOD_SABER

<<ClientDisconnect: 0*/



    //string cmd = "sh ~/Desktop/jka/jka1/start.sh";
    //GetStdoutFromCommand(cmd, playerlist);
    /*string init1 = "Player 23 spawned with userinfo: \\team\\r\\ip\\192.168.0.171:29071\\teamoverlay\\1\\snaps\\40\\sex\\male\\saber2\\none\\saber1\\single_1\\rate\\25000\\pbindicator\\1\\name\\asdasdasdasd\\model\\jeditrainer/default\\jp\\0\\ja_guid\\FC73F841C6064321AC9EE2F65CEA5B38\\<<handicap\\100\\forcepowers\\0-6-020300000010003133\\color4\\255\\color3\\7935\\color2\\255\\color1\\28415\\char_color_red\\255\\char_color_green\\255\\char_color_blue\\255\\cg_predictItems\\1";
    string init2 = "Player 42 spawned with userinfo: \\team\\r\\ip\\192.168.0.172:29071\\teamoverlay\\1\\snaps\\40\\sex\\male\\saber2\\none\\saber1\\single_1\\rate\\25000\\pbindicator\\1\\name\\abb\\model\\jeditrainer/default\\jp\\0\\ja_guid\\FC73F841C6064321AC9EE2F65CEA5123\\<<handicap\\100\\forcepowers\\0-6-020300000010003133\\color4\\255\\color3\\7935\\color2\\255\\color1\\28415\\char_color_red\\255\\char_color_green\\255\\char_color_blue\\255\\cg_predictItems\\1";
    string init3 = "Player 2 spawned with userinfo: \\team\\r\\ip\\192.168.0.173:29071\\teamoverlay\\1\\snaps\\40\\sex\\male\\saber2\\none\\saber1\\single_1\\rate\\25000\\pbindicator\\1\\name\\sssd\\model\\jeditrainer/default\\jp\\0\\ja_guid\\FC73F841C6064321AC9EE2F65CEA5432\\<<handicap\\100\\forcepowers\\0-6-020300000010003133\\color4\\255\\color3\\7935\\color2\\255\\color1\\28415\\char_color_red\\255\\char_color_green\\255\\char_color_blue\\255\\cg_predictItems\\1";

    string mystring1 = "Player 0 spawned with userinfo: \\team\\r\\ip\\192.168.0.174:29071\\teamoverlay\\1\\snaps\\40\\sex\\male\\saber2\\none\\saber1\\single_1\\rate\\25000\\pbindicator\\1\\name\\blake\\model\\jeditrainer/default\\jp\\0\\ja_guid\\FC73F841C6064321AC9EE2F65CEA125325\\<<handicap\\100\\forcepowers\\0-6-020300000010003133\\color4\\255\\color3\\7935\\color2\\255\\color1\\28415\\char_color_red\\255\\char_color_green\\255\\char_color_blue\\255\\cg_predictItems\\1";
    string mystring2 = "Player 1 spawned with userinfo: \\team\\r\\ip\\192.163.0.175:29071\\teamoverlay\\1\\snaps\\40\\sex\\male\\saber2\\none\\saber1\\single_1\\rate\\25000\\pbindicator\\1\\name\\segi\\model\\jeditrainer/default\\jp\\0\\<<handicap\\100\\forcepowers\\0-6-020300000010003133\\color4\\255\\color3\\7935\\color2\\255\\color1\\28415\\char_color_red\\255\\char_color_green\\255\\char_color_blue\\255\\cg_predictItems\\1";
    string mystring3 = "broadcast: print \"blake @@@PLDUELACCEPT segi!\\n\"";
    string mystring4 = "Kill: 0 1 7: segi killed blake by MOD_SUICIDE";
    string mystring5 = "ShutdownGame: ";
    funnel(init1, playerlist);
    funnel(init2, playerlist);
    funnel(mystring1, playerlist);
    funnel(mystring2, playerlist);
    funnel(init3, playerlist);
    funnel(mystring3, playerlist);

    playerlist[2].set_mmr(1000);
    playerlist[3].set_mmr(2000);
    funnel(mystring4, playerlist);
    funnel(mystring3, playerlist);
    funnel(mystring4, playerlist);
    playerlist[4].duel_start(playerlist, playerlist[1].get_id());
    playerlist[4].duel_win(playerlist, playerlist[1].get_id());
    funnel(mystring5, playerlist);*/

    //GetStdoutFromCommand("sh ~/Desktop/jka/jka1/start.sh", playerlist);*/
    //cout << "At the end, addr: "<< playerlist[0].get_name() << endl;