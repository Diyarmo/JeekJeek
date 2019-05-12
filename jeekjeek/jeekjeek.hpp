#include "database.hpp"
#include <iostream>
#include <map>
#include <cstdlib>
#define START_OF_SESSION_IDS 10000
#define SESSION_IDS map <string ,User *>
using namespace std;

class Jeekjeek
{
private:
	Database * database;
	SESSION_IDS logged_in_users;
	string last_session_id;
	string new_session_id();

public:
	Jeekjeek();
	bool sign_up(string username, string password, string display_name);
	bool login(string username, string password);
	void logout(string session_id);
	bool add_jeek(string session_id, string text, STRINGS tags, STRINGS mentions);
	CONTENTS search_user(string session_id);
	void search_tag(string session_id, string tag);
	CONTENTS search_jeeks(string keyword);
	void comment(string session_id,string jeek_id, string text);
	void rejeek(string session_id, string jeek_id);
	void like(string session_id, string jeek_id);
	string get_last_session_id();
	string get_display_name(string session_id);
	bool is_session_id_used(string session_id);
	Jeek * find_jeek_by_id(string id);
	User * get_user_by_session_id(string session_id);
	CONTENTS get_user_jeeks(string username);



};