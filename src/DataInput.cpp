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
#include <sstream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "Socket.hpp"
#include "Player.hpp"
#include "StringProcessing.hpp"
#include "EventHandling.hpp"
#include "include/sqlite/sqlite3.h"
#include "Database.hpp"
#include "DataInput.hpp"

using namespace std;

volatile static bool running_process = false;

std::mutex m;

std::mutex worker_m;

vector<int> numlist;

std::condition_variable cv;

std::queue <std::function<void(string, vector<Player>&)>> functions;

void worker();

void sigintHandler(int sigint) 
{ 
    signal(SIGINT, sigintHandler); 

    running_process = true;
    fflush(stdout);
} 

void _funnel(string line, vector<Player> &playerlist) {

    //cout << "In funnel(), ADD: "  << &playerlist << endl;
    if (is_number(line.substr(0, line.find(":")))) {
        event_say(line, playerlist);
        return;
    }
    else if (line.find("Player", 0) == 0) {
        event_player(line, playerlist);
    }
    else if (line.find("broad", 0) == 0) {
        event_broadcast(line, playerlist);
    }
    else if (line.find("Kill", 0) == 0) {
        event_kill(line, playerlist);
    }
    else if (line.find("ClientDisconnect:", 0) == 0) {
        event_clientdisconnect(line, playerlist);
    }
    else if (line.find("ShutdownGame", 0) == 0) {
        event_shutdowngame(playerlist);
    }
}


void read_server_stdout(string cmd, vector<Player> &playerlist) {

    std::thread t(worker);
    t.detach();
    string data;
    signal(SIGINT, sigintHandler);
    int a = 3;
    FILE* stream;
    const int max_buffer = 1024;
    char buffer[max_buffer];
    cmd.append(" 2>&1");
    stream = popen(cmd.c_str(), "r");

    if (stream) {
        while (!feof(stream)) {
            fgets(buffer, max_buffer, stream);
            data = string(buffer);
            functions.push(std::bind(_funnel, data.erase(0, 4), std::ref(playerlist)));
            cv.notify_all();
            
            if (running_process)
            {
                cout << "Terminating Server.." << endl;
                pclose(stream);
                cout << "Server terminated.";
                t.~thread();
                exit(EXIT_SUCCESS);
            }
        }
        pclose(stream);
    }
}

void read_tail_stdout(string path, vector<Player> &playerlist) {

    send_stuff("Loading Elo..");

    std::thread t(worker);
    t.detach();

    signal(SIGINT, sigintHandler);
    //handler for cleanup after ctrl+c

    FILE* stream;
    const int max_buffer = 1024;
    char buffer[max_buffer];
    string data;
    string cmd = "tail -f " + path;
    stream = popen(cmd.c_str(), "r");



    if (stream) {
        send_stuff("Elo loaded, restarting round..");
        send_stuff("newround");
        while (!feof(stream)) {
            fgets(buffer, max_buffer, stream);
            data = string(buffer);
            cout << data;
            functions.push(std::bind(_funnel, data, std::ref(playerlist)));
            cv.notify_all();
            
            if (running_process)
            {
                cout << "Terminating Program.." << endl;
                cout << "Program terminated.";
                t.~thread();
                exit(EXIT_SUCCESS);
            }
        }
        pclose(stream);
    }
}

void worker() {
    static string arg1;
    static vector<Player> arg2;

    while(1) {
        std::unique_lock<std::mutex> w_lock(worker_m);
        unsigned int sz = functions.size();
        if (sz == 0) {
            cv.wait(w_lock);
        }
        std::unique_lock<std::mutex> lock(m);
        auto x = functions.front();
        functions.pop();
        lock.unlock();
        x(arg1, arg2);
        w_lock.unlock();
    }
}