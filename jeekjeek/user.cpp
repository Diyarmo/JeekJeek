#include "user.hpp"


User::User(string _id, string _username, string _display_name, string _password)
{
	id = _id;
	username = _username;
	display_name = _display_name;
	password = _password;
}

void User::follow(User * following)
{
	if (has_followed(following))
	{
		throw Exception("User has been followed before \n");
	}
	else
	{
		followings.push_back(following);
	}
}

void User::unfollow(User * following)
{
	if (!has_followed(following))
		throw Exception("User hasn't been followed before\n");
	else
	{
		for (int i = 0; i < followings.size(); i++)
			if (following == followings[i])
			{
				followings.erase(followings.begin() + i);
				return;
			}
	}
}

void User::being_followed(User * follower)
{
	followers.push_back(follower);
}

void User::being_unfollowed(User * follower)
{
	for (int i = 0; i < followers.size(); i++)
		if (follower == followers[i])
		{
			followers.erase(followers.begin() + i);
			return;
		}
}

bool User::has_followed(User * following)
{
	for (int i = 0; i < followings.size(); i++)
		if (following == followings[i])
			return true;
	return false;
}