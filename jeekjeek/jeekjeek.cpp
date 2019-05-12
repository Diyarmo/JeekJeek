#include "jeekjeek.hpp"


string Jeekjeek::new_session_id()
{
	srand(time(NULL));
	string session_id;
	do
	{
		session_id = to_string( (rand() % START_OF_SESSION_IDS) + START_OF_SESSION_IDS) ;
	}while (is_session_id_used(session_id));
	return session_id;
}

bool Jeekjeek::is_session_id_used(string session_id)
{
	for (SESSION_IDS::iterator logged_in = logged_in_users.begin(); logged_in != logged_in_users.end(); logged_in++)
		if ( logged_in->first == session_id)
			return true;
	return false;
}

string Jeekjeek::get_last_session_id()
{
	return last_session_id;
}

Jeekjeek::Jeekjeek()
{
	database = new Database;
}

bool Jeekjeek::sign_up(string username, string password, string display_name)
{
	try 
	{
		database->add_user(username, password, display_name);
		return true;
	}catch(...){return false;}
}

bool Jeekjeek::login(string username, string password)
{
	try 
	{
		string session_id = new_session_id();
		logged_in_users[session_id] = database->login(username, password);
		last_session_id = session_id;
		return true;
	}catch(...){return false;}
}

void Jeekjeek::logout(string session_id)
{
	logged_in_users.erase(session_id);

}

bool Jeekjeek::add_jeek(string session_id, string text, STRINGS tags, STRINGS mentions)
{
	try 
	{
		database->add_jeek(logged_in_users[session_id],text, tags, mentions);
		return true;
	}catch(...){return false;}
}

CONTENTS Jeekjeek::search_user(string session_id)
{
	return database->get_user_jeeks(logged_in_users[session_id]->get_username());
}

CONTENTS Jeekjeek::search_jeeks(string keyword)
{
	return database->find_jeeks_by_keyword(keyword);
}

void Jeekjeek::search_tag(string session_id,string tag)
{
	CONTENTS jeeks = database->find_jeeks_by_tag(tag);
	// show jeeks
}

void Jeekjeek::comment(string session_id,string jeek_id, string text)
{
	database->add_comment(logged_in_users[session_id],jeek_id, text);
}

void Jeekjeek::rejeek(string session_id, string jeek_id)
{
	try
	{
		database->add_rejeek(logged_in_users[session_id],jeek_id);
	}catch(Exception ex){cout << ex.get_error() << endl; }
}

void Jeekjeek::like(string session_id, string jeek_id)
{
	database->like(logged_in_users[session_id], jeek_id);
}

string Jeekjeek::get_display_name(string session_id)
{
	if (logged_in_users[session_id])
		return logged_in_users[session_id]->get_display_name();
	else
		throw Exception("You are not logged in");
}

Jeek * Jeekjeek::find_jeek_by_id(string id)
{	
	return static_cast<Jeek *> (database->find_content_by_id(id));
}

User * Jeekjeek::get_user_by_session_id(string session_id)
{
	for (SESSION_IDS::iterator logged_in = logged_in_users.begin(); logged_in != logged_in_users.end(); logged_in++)
		if ( logged_in->first == session_id)
			return logged_in->second;
	return NULL;
}

CONTENTS Jeekjeek::get_user_jeeks(string username)
{
	return database->get_user_jeeks(username);
}
