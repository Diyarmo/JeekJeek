#include <string>
#include <vector>
#include "user.hpp"
#include "defines.hpp"

using namespace std;

class Comment;
class Reply;
class Content
{
private:
	string text;
	string content_id;
	User * publisher;

public:
	void set_text(string _text) { text = _text; }
	void set_id(string id) { content_id = id; }
	void set_publisher(User * user) { publisher = user; }
	string get_text() { return text; }
	string get_id() { return content_id; }
	User * get_publisher() { return publisher; }

};

class Jeek : public Content
{
private:
	USERS likes;
	JEEKS rejeeks;
	COMMENTS comments;
	STRINGS tags;
	USERS mentions;
public:
	Jeek(string text, string id, User * publisher, STRINGS _tags, USERS _mentions);
	Jeek( Jeek * jeek);
	bool has_tag(string tag);
	bool has_mention(string username);
	void add_rejeek(Jeek * rejeek) { rejeeks.push_back(rejeek);}
	void comment(Comment * comment) { comments.push_back(comment); }
	void like(User * user);
	void dislike(User * user);
	bool has_liked(User * user);
	STRINGS get_tags(){return tags;}
	USERS get_mentions(){return mentions;}
	bool has_keyword(string keyword);
	int count_likes() { return likes.size();}
	int count_rejeeks() { return rejeeks.size();}
};


class Comment : public Content
{
private:
	Jeek * parent_content;
	REPLYS replys;
public:
	Comment(User * publisher, Jeek * parent, string id, string _text) : parent_content(parent) { set_text(_text); set_id(id); set_publisher(publisher); }
	void reply(Reply * reply) { replys.push_back(reply); }
};

class Reply : public Content
{
private:
	Content * parent_content;
	REPLYS replys;
public:
	Reply(User * publisher, Content * parent, string id, string _text) : parent_content(parent) { set_text(_text); set_id(id); set_publisher(publisher); }
	void reply(Reply * reply) { replys.push_back(reply); }

};