#include<stdio.h>
#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h> 
#include<string>
#include<string.h>

#include "ConfigParse.hpp"

using namespace std;
#define PORT 29070
extern Config cfg;

void send_stuff(std::string cmd) {
    int valread;
	static string prefix = "\xff\xff\xff\xffrcon " + cfg.rconpass + " ";
    cmd = prefix + cmd;
	char buffer[1024] = {0};
	struct sockaddr_in serv_addr; 
	static string ip =  cfg.ip;
	static int port =  cfg.port;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	inet_pton(AF_INET, ip.c_str() , &serv_addr.sin_addr);

	connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	send(sockfd , cmd.c_str() , cmd.size() , 0);
    valread = read(sockfd ,buffer, 1024);
}
