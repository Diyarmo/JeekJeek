#pragma once
#include <vector>
#include <string>
#include "defines.hpp"

using namespace std;

class Exception
{
private:
	string error;
public:
	Exception(string _error) { error = _error; }
	string get_error() { return error; }
};


class Notifiction
{};

class User
{
private:
	string id;
	string username;
	string display_name;
	string password;
	USERS followers;
	USERS followings;
	STRINGS notifictions;
	STRINGS jeeks_ids;
public:
	User(string _id, string _username, string _display_name, string _password);
	string get_id() { return id; };
	string get_username() { return username; };
	string get_password() { return password; };
	string get_display_name() { return display_name; };
	STRINGS get_jeeks_ids() { return jeeks_ids; }
	void follow(User * following);
	void unfollow(User * following);
	bool has_followed(User * following);
	void being_followed(User * follower);
	void being_unfollowed(User * follower);
	void notify(string text) { notifictions.push_back(text); }
	USERS get_followers() { return followers; }
	void add_jeek(string jeek_id) { jeeks_ids.push_back(jeek_id);};



};