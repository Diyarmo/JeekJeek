#include "content.hpp"

Jeek::Jeek(string text, string id, User * publisher, STRINGS _tags, USERS _mentions)
{
	set_text(text);
	set_id(id);
	set_publisher(publisher);
	tags = _tags;
	mentions = _mentions;
}

bool Jeek::has_tag(string tag)
{
	for (int i = 0; i < tags.size(); i++)
		if (tags[i] == tag)
			return true;

	return false;
}

Jeek::Jeek( Jeek *jeek)
{
	tags = jeek->tags;
	mentions = jeek->mentions;
	set_text(jeek->get_text());
}

void Jeek::like(User * user)
{
	if (has_liked(user))
		throw Exception("User has liked this jeek before\n");
	else
		likes.push_back(user);

}

bool Jeek::has_liked(User * user)
{
	for (int i = 0; i < likes.size(); i++)
		if (user == likes[i])
			return true;
	return false;
}

void Jeek::dislike(User * user)
{
	if (!has_liked(user))
		throw Exception("User hasn't liked this jeek before\n");
	else
	{
		for (int i = 0; i < likes.size(); i++)
			if (user == likes[i])
			{
				likes.erase(likes.begin() + i);
				return;
			}
	}
}

bool Jeek::has_keyword(string keyword)
{
	if (this->get_publisher()->get_username().find(keyword) != NO_POS)
		return true;
	if (this->get_text().find(keyword) != NO_POS)
		return true;
	if (this->has_tag(keyword))
		return true;
	if (this->has_mention(keyword))
		return true;

	return false;
}

bool Jeek::has_mention(string username)
{
	for (int i = 0; i < mentions.size(); i++)
		if (mentions[i]->get_username() == username)
			return true;
	return false;
}