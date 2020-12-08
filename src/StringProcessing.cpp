#include "StringProcessing.hpp"  

#include <string>
#include <iostream>
#include <string.h>

using namespace std;
string get_player_guid(const string& s) {
    static string query = "ja_guid\\";
    string::size_type pos = s.find(query,0);
    if (pos == string::npos) {
        return "0";
    }

    string::size_type pos1 = pos + query.size();
    string::size_type pos2 = s.find("\\", pos1);

    return s.substr(pos1,pos2 - pos1);
}

int get_player_id(const string& s) {
    try {
    static string query = " ";
    string::size_type pos1 = s.find(query, 0) + query.size();
    string::size_type pos2 = s.find(" ", pos1);

    return stoi(s.substr(pos1,pos2-pos1));
    }
    catch (int e) {
        cout << "Error in that stoi shit.";
        cout << s;

    }
    exit(EXIT_SUCCESS);
    return 2; 
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

string get_player_name(const string& s) {

    static string query = "name\\";
    string::size_type pos1 = s.find(query, 0) + query.size();
    string::size_type pos2 = s.find("\\", pos1);

    return s.substr(pos1,pos2 - pos1);
}

string get_player_ip(const string& s) {
    
    static string query = "ip\\";
    string::size_type pos1 = s.find(query, 0) + query.size();
    string::size_type pos2 = s.find(":", pos1);

    
    return s.substr(pos1,pos2 - pos1);
}

void pos_substr(string& string_src, string& string_out, string::size_type pos1, string::size_type pos2) {
    for (auto x = pos1; x != pos2; x++) {
        string_out.push_back(string_src[x]);
    }
}