#include "content.hpp"

class Database
{
private:
	vector <Content *> contents;
	vector <User * > users;
	vector <string> used_ids;
	string generate_id();

public:
	void add_user(string username, string password, string display_name);
	User * login(string username, string password);
	USERS get_mentioned_users(STRINGS mentions);
	void add_jeek(User * user, string text, STRINGS tags, STRINGS mentions);
	User * find_user_by_id(string id);
	User * find_user_by_username(string username);
	CONTENTS get_user_jeeks(string username);
	Content * find_content_by_id(string id);
	CONTENTS find_jeeks_by_tag(string tag);
	CONTENTS find_jeeks_by_keyword(string keyword);
	void add_comment(User * user, string jeek_id, string text);
	void add_reply(User * user, string content_id, string text);
	void add_rejeek(User * user, string jeek_id);
	void like(User * user, string jeek_id);
	void dislike(User * user, string jeek_id);
	void follow(User * user, string follower_id);
	void unfollow(User * user, string follower_id);

};