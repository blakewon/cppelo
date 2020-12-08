
#pragma once

#include <vector>
#include <string>
#include "Player.hpp"
#include "Database.hpp"

void event_player(std::string&, std::vector<Player>&);

void event_broadcast(std::string&, std::vector<Player>&);

void event_say(std::string&, std::vector<Player>&);

void event_say_chill(std::string&, std::vector<Player>&);

void event_say_elo(string&, vector<Player>&);

void event_broadcast_duel(std::string&, vector<Player>&);

void event_broadcast_name(std::string&, vector<Player>&);

void event_broadcast_name_split(string&,string&,string&,string&,string::size_type);

void event_kill (std::string&, std::vector<Player>&);

void event_clientdisconnect(std::string&, std::vector<Player>&);

void event_shutdowngame(std::vector<Player>&);