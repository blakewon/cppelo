#include "EventHandling.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "Player.hpp"
#include "StringProcessing.hpp"


using namespace std;

void event_player(string& line, vector<Player>& playerlist) {
    int id = get_player_id(line);
    bool existing = false;
    for (auto element: playerlist) {
        if (id == element.get_id()) {
            cout << "Client with ID: \"" << id << "\" already initialized.";
            return;
        }
    }

    Player plr(get_player_name(line), get_player_ip(line), get_player_guid(line), id);

    if (!db_find_player(plr)) {
        db_create_player(plr);
    }

    playerlist.push_back(plr);
    cout << "PTR 1: " << &plr << endl;
}



void event_say(string& line, vector<Player>& playerlist) {
    //1: say: blake: "chat example"
    string::size_type pos = line.find("\"!", 0);
    if (pos != string::npos) {
        if (line.find("!chill", pos) == pos+1)
        {
            event_say_chill(line, playerlist);
        }

        if (line.find("!elo", pos) == pos+1)
        {
            event_say_elo(line, playerlist);
        }
    }
    else
        return;
}
void event_say_chill(string& line, vector<Player>& playerlist) {
    string::size_type pos1 = line.find(":", 0);
    int id = stoi(line.substr(0, pos1));
    if (line.find("\"!chill") != string::npos) {
        for (vector<Player>::iterator i = playerlist.begin(); i != playerlist.end(); i++) {
            if (id == i->get_id()) {
                i->flip_chill();
                return;
            }
        }
    }
}
void event_say_elo(string& line, vector<Player>& playerlist) {
    cout << "elo reached?" << endl;
    string::size_type pos1 = line.find(":", 0);
    int id = stoi(line.substr(0,pos1));
    if (line.find("\"!elo") != string::npos) {
        for (auto &x: playerlist) {
            stringstream stream;
            stream << x.get_mmr();
            if (x.get_id() == id) {
                send_stuff("svtell " + std::to_string(id) + " Your ELO: ^2[^3" + stream.str() + "^2]");
            }
        }
    }
}

void event_broadcast(std::string& line, vector<Player>& playerlist) { //event_broadcast 
    string::size_type i = line.find("@@@PLD");
    if (i != string::npos) {
        event_broadcast_duel(line, playerlist);
        return;
    }

    i = line.find("@@@PLR");
    if (i != string::npos) {
        event_broadcast_name(line, playerlist);
        return;
    }

}
void event_broadcast_name(string& line, vector<Player>& playerlist) {
    //broadcast: print "J^4|^7P ^4b^7lake_^4w^7on^7 @@@PLRENAME blake\n
    
    static string delimiter = "@@@PLRENAME";
    static string::size_type deli_size = delimiter.size();

    string::size_type pos1 = line.find("\"") + 1;
    string::size_type pos2 = line.find(delimiter) -1;
    
    string name1;
    string name2;

    pos_substr(line, name1, pos1, pos2 - 2);
    cout << "split name substr1 : " << name1 << endl;

    pos1 = pos2 + deli_size + 2;
    pos2 = line.find("\\", pos1);

    pos_substr(line, name2, pos1, pos2);
    cout << "split name substr2 : " << name2 << endl;

    try {
        Player* ptr = find_by_name(playerlist, name1);
        if (ptr) ptr->set_name(name2);
        else {
            cout << "Player not found, returning" << endl;
            return;
        }
        cout << "\"" << name1 << "\" Has changed the name to: \"" << playerlist[0].get_name() << "\"" << endl; 

    }
    catch (int e) {
        cout << "Error in setting the name." << endl;
    }
}

void event_broadcast_duel(std::string& line, vector<Player>& playerlist) {
    //broadcast: print "blake @@@PLDUELACCEPT segi!\n"

    static string delimiter = "@@@PLDUELACCEPT";
    static string::size_type deli_size = delimiter.size();
    string::size_type pos1 = line.find("\"") + 1;
    string::size_type pos2 = line.find(delimiter);

    string name1;
    string name2;

    pos_substr(line, name1, pos1, pos2 - 1);

    pos1 = pos2 + deli_size + 1;
    pos2 = line.find("\\", pos1) - 1;

    pos_substr(line, name2, pos1, pos2);

    Player* p1 = find_by_name(playerlist, name1);
    Player* p2 = find_by_name(playerlist, name2);
    
    if (p1) {
        p1->Player::duel_start(playerlist, p2->get_id());
    }
    else
        cout << "Player \"" + name1 + "\" has not been found."; 
    

    cout << "NAME1: " << "+" + name1 + "+" << endl << "NAME2: " << "+" + name2 + "+" << endl;


}

void event_kill(string& line, vector<Player>& playerlist) {
    //Kill: 0 1 86: segi killed blake by MOD_SABER
    string::size_type pos1 = line.find(": ", 0) + 2;
    string::size_type pos2 = line.find(" ", pos1);

    string pid1;

    pos_substr(line, pid1, pos1, pos2);

    string pid2;

    pos1 = pos2 + 1;
    pos2 = line.find(" ", pos1 + 1);

    pos_substr(line, pid2, pos1, pos2);

    Player* player = find_by_id(playerlist, stoi(pid1));
    if (player->in_duel()) {
        player->duel_win(playerlist, stoi(pid2));
    }
    else
    {
        cout << "Player not in duel.";
        return;
    }
    return;
}


void event_clientdisconnect(std::string& line, std::vector<Player>& playerlist) {
    //ClientDisconnect: 0
    string::size_type pos1 = line.find(": ", 0) + 2;
    int pid = stoi(line.substr(pos1, line.size()-pos1));

    for (vector<Player>::size_type i = 0; i != playerlist.size(); i++) {
        db_save_player(playerlist[i]);
        playerlist.erase(playerlist.begin() + i);
        return;
    }
    cout << "Uninitialized player disconnected. ID: " << pid << endl;
}

void event_shutdowngame(vector<Player>& playerlist) {
    cout << "Shutdowngame" << endl;
    for (auto& x: playerlist) {
        db_save_player(x);
    }
    cout << "Hello1" << endl;
    playerlist.clear();
    cout << "Hello2" << endl;
}
