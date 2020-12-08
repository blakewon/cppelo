#pragma once

#include <iostream>
#include <string>
#include "Player.hpp"



void db_init();

void db_create_table();

void db_create_player(Player&);

void db_save_player(Player&);

bool db_find_player(Player&);


