#pragma once

#include <string>
#include <vector>

#include "Socket.hpp"
using namespace std;


class Player  
{
	public:
		Player();
		//constructor for an existing player by ip
		

		//constructor
		Player(string name, string ip, string guid, int id);


		//temporary constructor
		Player(string name, string ip, string guid, int id, int wins, int losses, double mmr, bool chill);

		//non-member indexing functions
		friend Player* find_by_name(vector<Player>&, string& name);
		friend Player* find_by_id(vector<Player>&, int id);

		//duel management functions..
		void duel_start(vector<Player>&, int opponent_id);
		void duel_win(vector<Player>&, int opponent_id);

		// getters and setters..
		string get_name();
		string get_ip();
		string get_guid();
		int get_wins();
		int get_losses();
		int get_id();
		double get_mmr();
		bool in_duel(); //returns the current in_duel flag
		bool chill();	//returns the current chill flag

		void set_name(const string& name);
		void set_ip(const string& ip);
		void set_opponent(int opponent);
		void set_mmr(double mmr);
		void increment_win();
		void increment_loss();
		void flip_chill(); 	//flips the chill flag and notifies the in-game user regarding it.
		void flip_duel();	//flips the in_duel flag

		//partial "constructors" used in database header, converts char* array to appropriate values
		void construct_from_db(char* []); //

		~Player();

private:
		string m_name;
		string m_ip;
		string m_guid = "0";
		int m_id;
		bool m_in_duel = false;
		int m_wins = 0;
		int m_losses = 0;
		double m_mmr = 1500;
		bool m_chill = false;
		int m_opponent_id;
};
//Player* find_by_name(vector<Player>&, string&);

