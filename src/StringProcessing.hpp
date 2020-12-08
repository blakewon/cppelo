
#pragma once

#include <string>

std::string get_player_guid(const std::string&);

int get_player_id(const std::string&);

std::string get_player_name(const std::string&);

std::string get_player_ip(const std::string&);

bool is_number(const std::string&);

void pos_substr(std::string&, std::string&, std::string::size_type, std::string::size_type);