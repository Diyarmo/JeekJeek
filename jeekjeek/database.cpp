#include "database.hpp"
#define ID_REFRENCE "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%&*-+"
#define SIZE_OF_ID 16
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

User * Database::find_user_by_id(string id)
{
	for (int i = 0; i < users.size(); i++)
		if (users[i]->get_id() == id)
			return users[i];
	return NULL;
}

User * Database::find_user_by_username(string username)
{
	for (int i = 0; i < users.size(); i++)
		if (users[i]->get_username() == username)
			return users[i];
	return NULL;
}

void Database::add_user(string username, string password, string display_name)
{
	if (find_user_by_username(username))
	{
		throw Exception("username was duplicated\n");
	}
	string new_id = generate_id();
	User * user = new User(new_id, username, display_name, password);
	users.push_back(user);
}

string Database::generate_id()
{
	string ref = ID_REFRENCE;
	string new_id;
	srand(clock());
	do
	{
		new_id = "";
		for (int i = 0; i < SIZE_OF_ID; i++)
			new_id.push_back(ref[rand() % ref.size()]);

	} while (find_user_by_id(new_id));
	used_ids.push_back(new_id);
	return new_id;
}

User * Database::login(string username, string password)
{
	User * user = find_user_by_username(username);
	if (user && user->get_password() == password)
		return user;
	else
		throw Exception("Invalid Username and Password\n");
}

USERS Database::get_mentioned_users(STRINGS mentions)
{
	USERS mentioned_users;
	User * user;
	for (int i = 0; i < mentions.size(); i++)
	{
		user = find_user_by_username(mentions[i]);
		if (user)
			mentioned_users.push_back(user);
		else
			throw Exception("Wrong Username in Mentions\n");
	}

	return mentioned_users;

}

void Database::add_jeek(User * user, string text, STRINGS tags, STRINGS mentions)
{
	if (text.size() > 140)
		throw Exception("Text size is invalid.");
	string jeek_id = generate_id();
	USERS mentioned_users = get_mentioned_users(mentions);
	Content * jeek = new Jeek(text, jeek_id, user, tags, mentioned_users);
	contents.push_back(jeek);
	user->add_jeek(jeek_id);
	USERS followers = user->get_followers();
	for (int i = 0; i < followers.size(); i++)
		followers[i]->notify(user->get_username() + " jeeked " + jeek->get_id());
}

CONTENTS Database::get_user_jeeks(string username)
{
	CONTENTS jeeks;
	STRINGS jeeks_ids;
	User * user = find_user_by_username(username);
	if (user)
		jeeks_ids = user->get_jeeks_ids();
	for (int i = 0; i < jeeks_ids.size() ; i++)
		jeeks.push_back(find_content_by_id(jeeks_ids[i]));
	return jeeks;
}

CONTENTS Database::find_jeeks_by_keyword(string keyword)
{
	CONTENTS jeeks;
	Jeek * jeek;
	for (int i = contents.size() - 1; i >= 0; i--)
	{
		jeek = static_cast<Jeek *> (contents[i]);
		if (jeek && jeek->has_keyword(keyword))
			jeeks.push_back(jeek);
	}
	return jeeks;
}


Content * Database::find_content_by_id(string id)
{
	for (int i = 0; i < contents.size(); i++)
		if (contents[i]->get_id() == id)
			return contents[i];
	return NULL;
}

CONTENTS Database::find_jeeks_by_tag(string tag)
{
	CONTENTS jeeks;
	for (int i = 0; i < contents.size(); i++)
		if (static_cast<Jeek *> (contents[i])->has_tag(tag))
			jeeks.push_back(contents[i]);
	return jeeks;
}

void Database::add_comment(User * user, string jeek_id, string text)
{
	Jeek * jeek = static_cast<Jeek *> (find_content_by_id(jeek_id));
	if (jeek)
	{
		Comment * comment = new Comment(user, jeek, generate_id(), text);
		contents.push_back(comment);
		jeek->comment(comment);
		jeek->get_publisher()->notify(user->get_username() + " commented on	 " + jeek->get_id());
	}
	else
		throw Exception("Wrong Jeek ID\n");
}

void Database::add_reply(User *user, string content_id, string text)
{
	Content * content = find_content_by_id(content_id);
	Comment * comment = static_cast<Comment *> (content);
	Reply * reply = static_cast<Reply *> (content);

	Reply * new_reply = new Reply(user, content, generate_id(), text);
	if (comment)
	{
		comment->reply(new_reply);
		comment->get_publisher()->notify(user->get_username() + " replied " + comment->get_id());
	}
	else if (reply)
	{
		reply->reply(new_reply);
		reply->get_publisher()->notify(user->get_username() + " replied " + reply->get_id());
	}
	else
	{
		throw Exception("Wrong Reply or Comment ID\n");
		delete(new_reply);
		return;
	}
	contents.push_back(new_reply);

}

void Database::add_rejeek(User * user, string jeek_id)
{
	Jeek * jeek = static_cast<Jeek *> (find_content_by_id(jeek_id));
	if (jeek)
	{
		Jeek * rejeek = new Jeek(jeek);
		rejeek->set_id(generate_id());
		rejeek->set_publisher(user);
		rejeek->set_text("Rejeeked: \n" + jeek->get_text());
		contents.push_back(rejeek);
		user->add_jeek(rejeek->get_id());
		jeek->add_rejeek(rejeek);

		jeek->get_publisher()->notify(user->get_username() + " rejeeked " + jeek->get_id());
	}
	else
	{
		throw Exception("Wrong Jeek ID \n");
	}
}

void Database::like(User * user, string jeek_id)
{
	Jeek * jeek = static_cast<Jeek *> ( find_content_by_id(jeek_id) );
	if (jeek)
	{
		jeek->like(user);
		jeek->get_publisher()->notify(user->get_username() + " liked " + jeek->get_id());
	}
	else
	{
		throw Exception("Wrong Jeek ID \n");
	}
}

void Database::dislike(User * user, string jeek_id)
{
	Jeek * jeek = static_cast<Jeek *> (find_content_by_id(jeek_id));
	if (jeek)
	{
		jeek->dislike(user);
		jeek->get_publisher()->notify(user->get_username() + " disliked " + jeek->get_id());
	}
	else
	{
		throw Exception("Wrong Jeek ID \n");
	}
}

void Database::follow(User * user, string following_id)
{
	User * following = find_user_by_id(following_id);
	if (user == following)
	{
		throw Exception("Can't follow yourself\n");
	}
	if (following)
	{
		user->follow(following);
		following->being_followed(user);
	}
	else
	{
		throw Exception("Wrong Following User ID \n");
	}
}

void Database::unfollow(User * user, string following_id)
{
	User * following = find_user_by_id(following_id);
	if (user == following)
	{
		throw Exception("Can't unfollow yourself\n");
	}
	if (following)
	{
		user->unfollow(following);
		following->being_unfollowed(user);
	}
	else
	{
		throw Exception("Wrong Following User ID \n");
	}
}