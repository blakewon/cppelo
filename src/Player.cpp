#include "Player.hpp" 

#include <string>
#include <vector>
#include <math.h>
#include <sstream>

#include "Socket.hpp"

using namespace std;


void Player::duel_start(vector<Player>& playerlist, int opponent) {
	Player* p2 = find_by_id(playerlist, opponent);
			if (m_chill) {
				send_stuff("svtell " + std::to_string(p2->get_id()) + " Your opponent has disabled ranked dueling.");
				return;
			}

			if (p2->chill()) {
				send_stuff("svtell " + std::to_string(m_id) + " Your opponent has disabled ranked dueling.");
				return;
			}

			if (!p2) {
				cout << "Opponent " << opponent << "couldn't be found, returning.";
				return;
			}

			if (!(this->in_duel()) && !(p2->in_duel())) {

				this->set_opponent(p2->get_id());
				this->flip_duel();

				cout <<this->get_name() << " IN DUEL: " << this->in_duel() << endl;

				p2->set_opponent(m_id);
				p2->flip_duel();

                std::ostringstream stream1;
                std::ostringstream stream2;

                stream1 << this->get_mmr();
                stream2 << p2->get_mmr();

				cout << p2->get_name()<< " IN DUEL: " << p2->in_duel() << endl;
                send_stuff("svtell " + std::to_string(this->get_id()) + " ^7VS " + p2->get_name() + "[" + "^3" + stream2.str() + "^7]");
                send_stuff("svtell " + std::to_string(p2->get_id()) + " ^7VS " + this->get_name() + "[" + "^3" + stream1.str() + "^7]");

			}
			else
			{
				cout << "One or more players are already in a duel, returning.";
			}
			cout << "Duel Started between " + this->get_name() << " and " << p2->get_name() << endl;

}
void Player::duel_win(vector<Player>& playerlist, int opponent_id) {
    int k = 10;
    if(m_opponent_id != opponent_id) {
        cout << "Invalid opponent id";
        return;
    }
    Player* ptr = find_by_id(playerlist, m_opponent_id);


    double e1 = pow(10, m_mmr/400);
    double e2 = pow(10, ptr->get_mmr()/400);

    double difference = fabs(roundf((k *(0 - e2 / (e1 + e2))) * 100) / 100);
    cout << "Diff: " << difference << endl;

    this->set_mmr(m_mmr + difference);
    ptr->set_mmr(ptr->get_mmr() - difference);
    this->increment_win();
    ptr->increment_loss();

    std::ostringstream stream;
    stream << difference;

    send_stuff("svtell " + std::to_string(this->get_id()) + " ^2+" + stream.str());
    send_stuff("svtell " + std::to_string(ptr->get_id()) + " ^1-" + stream.str());


    this->flip_duel();
    ptr->flip_duel();
    return;
}

Player::Player(string name, string ip, string guid, int id, int wins, int losses, double mmr, bool chill)
{   //constructor for an existing player in the database.

    m_name = name;
    m_ip = ip;
    m_guid = guid;
    m_id = id;
    m_wins = wins;
    m_losses = losses;
    m_mmr = mmr;
    m_chill = chill;
}


Player::Player(string name, string ip, string guid, int id) : 
m_in_duel(false), m_wins(0), m_losses(0), m_mmr(1500), m_chill(false)
{
    m_name = name;
    m_ip = ip;
    m_guid = guid;
    m_id = id;
}

void Player::construct_from_db(char* argv []) {
    m_mmr = (double)strtod(argv[3], NULL);
    m_wins = (int)stoi(argv[4], NULL);
    m_losses = (int)stoi(argv[5], NULL);
}

void Player::flip_chill() {
    m_chill = !m_chill;

    if (m_chill)
        send_stuff("svtell "+ to_string(m_id) + " ^7Chill: ^2ON");
    else
        send_stuff("svtell "+ to_string(m_id) + " ^7Chill: ^1OFF");
}
void Player::flip_duel() {
    m_in_duel = !m_in_duel;
}
void Player::set_name(const string& name) {
	m_name = name;
}
void Player::set_opponent(int opponent) {
    m_opponent_id = opponent;
}
bool Player::in_duel() {
    if (m_in_duel)
    return true;
    else
    return false;
}
bool Player::chill() {
    return m_chill;
}
double Player::get_mmr() {
    return m_mmr;
}
void Player::set_mmr(double mmr) {
    m_mmr = mmr;
}
string Player::get_name(){
    return m_name;
}
string Player::get_ip(){
    return m_ip;
}
string Player::get_guid(){
    return m_guid;
}
int Player::get_wins() {
    return m_wins;
}
int Player::get_losses() {
    return m_losses;
}
int Player::get_id() {
    return m_id;
}
void Player::set_ip(const string& ip) {
    m_ip = ip;
}

void Player::increment_win() {
    m_wins +=1;
}
void Player::increment_loss() {
    m_losses+=1;
}

Player::Player()
{

}

Player::~Player()
{

}

Player* find_by_name(vector<Player>& list, string& name) {
    for (auto& x: list) {
        if (x.get_name() == name)
            return &x;
    }
    return nullptr;
}

Player* find_by_id(vector<Player>& playerlist, int id) {
    for (auto& x: playerlist) {
        if (x.get_id() == id)
            return &x;
    }
    return nullptr;
}