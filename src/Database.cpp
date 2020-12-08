#include "Database.hpp"

#include <string>
#include <iostream>
#include <typeinfo>
#include <functional>

#include "include/sqlite/sqlite3.h"

using namespace std;

bool success = false;

char buffer[225];
static const string dir = string(getcwd(buffer, 225)) + "/database.db";

void print_player(Player& plr);

//the return value and argument list of callback functions are un-modifiable, global variables are in order.
int callback_guid(void* ptr, int argc, char** argv , char** col_name) {
    int i; 
    cout << "NUMBER OF ELEMENTS IN GUID: " << argc << endl;
    Player* plr = static_cast<Player *>(ptr);
    if (argc >0) {
        plr->construct_from_db(argv);
        success = true;
    }
    return 0;
}

int callback_ip(void* ptr, int argc, char** argv , char** col_name) {

    int i; 
    cout << "NUMBER OF ELEMENTS IN IP: " << argc << endl;
    Player* plr = static_cast<Player *>(ptr);
    if (argc >0) {
        plr->construct_from_db(argv);
        success = true;
    }
    return 0;
}

void db_init() {
    cout << "Initializing database.." << endl;
    sqlite3* db;
    int exit = 0;
    exit = sqlite3_open(dir.c_str(), &db);
    sqlite3_close(db);
    cout << "Error code in init: " << exit << endl;
    return;
}

void db_create_table() {
    cout << "Initializing table.." << endl;
    sqlite3* db;
    static string sql = "CREATE TABLE IF NOT EXISTS PLAYERS("
    "NAME TEXT NOT NULL, "
    "IP TEXT, "
    "GUID TEXT, "
    "MMR REAL, "
    "WINS INTEGER, "
    "LOSSES INTEGER);";
        int exit = 0;
        exit = sqlite3_open(dir.c_str(), &db);

        if (exit != SQLITE_OK) {
            cout <<"Error in opening the database." << endl;
            return;
        }
        char* message_erorr;
        exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &message_erorr);

        if (exit != SQLITE_OK) {
            cout << "Error in executing the command: " << message_erorr;
            sqlite3_free(message_erorr);
        }
        if (exit == 0) {
            cout << "Table initialized successfully" << endl;
        }
        sqlite3_close(db);
        return;

}

void db_create_player(Player& plr) {
    cout << "Creating Player.." << endl;
    sqlite3* db;
    char* message_error;
    int exit = 0;
    cout << dir.c_str();
    exit = sqlite3_open(dir.c_str(), &db);

    cout << "Exit in create: " << exit << endl;

    print_player(plr);

    static string sql
     = "INSERT INTO PLAYERS (NAME, IP, GUID, MMR, WINS, LOSSES) VALUES ('%s', '%s', '%s', '%2.f', '%d', '%d');";

    char input[1024];
    snprintf(input, sizeof(input), sql.c_str(),
    plr.get_name().c_str(), plr.get_ip().c_str(), plr.get_guid().c_str(), plr.get_mmr(),
    plr.get_wins(), plr.get_losses());
    char *out;


    exit = sqlite3_exec(db, input, 0, NULL, &message_error);

    cout << input << endl;
    cout <<"ERROR CODE IN CREATE: " << exit << endl;

    if (message_error) {
        cout << message_error << endl;
    }
    sqlite3_free(message_error);
    sqlite3_close(db);    
}

bool db_find_player(Player& plr) {
    cout << "Finding Player.." << endl;
    sqlite3* db; 
    int exit = 0; 
    exit = sqlite3_open(dir.c_str(), &db); 
    static string guid("SELECT * FROM PLAYERS WHERE GUID = '%s';");
    static string ip("SELECT * FROM PLAYERS WHERE IP = '%s';"); 

    string query = "";

    int rc = 0;
    
    Player* ptr = &plr;

    if (plr.get_guid() != "0") {
        char input[1024];
        snprintf(input, sizeof(input), guid.c_str(), query.c_str());
        rc = sqlite3_exec(db, input, callback_guid, (void*)ptr, NULL);
        if (rc != 0) {
            cout << "Error in finding by guid: " << rc << endl;
            return false;
        }
    }
    else {
        char input[1024];
        snprintf(input, sizeof(input), ip.c_str(), query.c_str());
        rc = sqlite3_exec(db, input, callback_ip, (void*)ptr, NULL);
        if (rc != 0) {
            cout << "Error in finding by ip: " << rc << endl;
            return false;
        }
    }
    if(success == true) {
        sqlite3_close(db);
        success = false;
        return true;
    } 
    else {
        sqlite3_close(db);
        return false;
    }
}

void print_player(Player& plr) {
    cout << "========================" << endl;
    cout << "NAME: " << plr.get_name() << endl;
    cout << "IP: " << plr.get_ip() << endl;
    cout << "GUID: " << plr.get_guid() << endl;
    cout << "MMR: " << plr.get_mmr() << endl;
    cout << "WINS: " << plr.get_wins() << endl;
    cout << "LOSSES: " << plr.get_losses() << endl;
    cout << "========================" << endl;
}

void db_save_player(Player& plr) {
    cout << "Saving player.." << endl;
    print_player(plr);
    sqlite3* db; 
    int exit = 0, rc = 0; 
    char* message_error;

    exit = sqlite3_open(dir.c_str(), &db);

    static string guid("UPDATE PLAYERS SET "
    "NAME = '%s', " 
    "IP = '%s', "
    "MMR = '%.2f', " 
    "WINS = '%d', "
    "LOSSES = '%d' " 
    "WHERE GUID = '%s';");

    static string ip("UPDATE PLAYERS SET "
    "GUID = '0', "
    "NAME = '%s', " 
    "MMR = '%.2f', " 
    "WINS = '%d', "
    "LOSSES = '%d' " 
    "WHERE IP = '%s';");

    if (plr.get_guid() == "0") {
        char input[1024];
        snprintf(input, sizeof(input), ip.c_str(), 
        plr.get_name().c_str(), plr.get_mmr(), plr.get_wins(), plr.get_losses(), plr.get_ip().c_str());
        rc = sqlite3_exec(db, input, 0, NULL, &message_error);
        if (rc != 0) {
            cout << "Error in saving by guid: " << rc << endl;
            cout << plr.get_ip() << endl;
            return;
        }
    }
    else {
        char input[1024];
        snprintf(input, sizeof(input), guid.c_str(), 
        plr.get_name().c_str(), plr.get_ip().c_str(), plr.get_mmr(), plr.get_wins(), plr.get_losses(), plr.get_guid().c_str());
        rc = sqlite3_exec(db, input, 0, NULL, &message_error);
        if (rc != 0) {
            cout << "Error in saving by guid: " << rc << endl;
            cout << plr.get_ip() << endl;
            return;
        }
    }
}