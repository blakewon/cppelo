#include "ConfigParse.hpp"
#include <string>


Config::Config() {
    myfile.open("mb2elo.cfg");
    int max_size = 1024;
    char buffer[max_size];
    std::vector<std::string> lines;
    std::string s;
    if (myfile.is_open()) {
        while ((myfile.getline(buffer, max_size, '\n'))) {
            s = std::string(buffer);
            if (s.find("#", 0) != 0 && s.size())
                lines.push_back(std::string(buffer));
        }
        for (auto x: lines) {
            std::cout << "Settings: " << std::endl;
            std::cout << x << std::endl;
        }  
        port = stoi(init_value("port", lines));
        path_to_file = init_value("logpath", lines);
        rconpass = init_value("rcon", lines);
        ip = init_value("ip", lines);

    }
    else {
        std::cout << "Could not open config file, proceeding with default values.." << std::endl;
    }
}

std::string init_value(const std::string& query, const std::vector<std::string>& lines) {
    for (auto x: lines) {
        if (x.find(query, 0) == 0) {
            std::string::size_type pos = x.find("= ", 0) + 2;
            std::string rt = x.substr(pos, x.size() - pos);
            std::cout <<"|" << rt << "|" << std::endl;
            return rt;
        }
    }
}
