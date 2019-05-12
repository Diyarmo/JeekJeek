#include "server.hpp"
#include <cstdlib> // for rand and srand
#include <ctime>   // for time
#include <iostream>
#define NO_POS -1

using namespace std;

void insert(string &body, string text, string key)
{
  int firstOfKey = body.find(key);
  if (firstOfKey == NO_POS)
    return;
  body.insert(firstOfKey + key.size(), text);

}

string operationsHtml(string jeekID, bool has_liked)
{
  string html;
  html += "<div class=\"operations\"> \n";
  html += "<div class=\"column\"> \n";
  html += "<form action=\"/like\" method=\"post\"> \n";
  html += "<input type=\"hidden\" name=\"jeekID\" value=\"" + jeekID + "\"> \n";
  if (has_liked)
    html += "<button type=\"submit\" name=\"like\"></button> \n";
  else
    html += "<button type=\"submit\" name=\"dislike\"></button> \n";   
  html += "</form></div>\n";
  html += "<div class=\"column\"><form action=\"/rejeek\" method=\"post\"> \n ";
  html += "<input type=\"hidden\" name=\"jeekID\" value=\"" + jeekID + "\"> \n"; 
  html += "<button type=\"submit\" name=\"rejeek\">REJEEK it!</button></form></div></div> \n";
  return html;
}

string ShowJeekHtmlMaker(Jeekjeek * jeekjeek, Request* req)
{
  string sessionId = req->getSessionId();
  Jeek * jeek = jeekjeek->find_jeek_by_id(req->getBodyParam("jeekID"));
  string body = readFile("static/jeekPage.html");
  setVariable(body, "displayName", jeek->get_publisher()->get_display_name());
  setVariable(body, "showProfile", "<input type=\"hidden\" name=\"username\" value=\"" + jeek->get_publisher()->get_username() + "\">");
  setVariable(body, "text", jeek->get_text());
  STRINGS tags = jeek->get_tags();
  USERS mentions = jeek->get_mentions();
  string tagsString, mentionsString;
  for (int j = 0; j < tags.size(); j++)
    tagsString += "#" + tags[j] + " ";
  for (int j = 0; j < mentions.size(); j++)
    mentionsString += "@" + mentions[j]->get_username() + " ";
  setVariable(body, "tags", tagsString);
  setVariable(body, "mentions", mentionsString);
  setVariable(body, "likes", to_string(jeek->count_likes()));
  setVariable(body, "rejeeks", to_string(jeek->count_rejeeks()));
  insert(body, operationsHtml(jeek->get_id(), jeek->has_liked(jeekjeek->get_user_by_session_id(sessionId) ) ), "<!-- OPERATIONS -->");
  if (jeekjeek->is_session_id_used(sessionId))
      setVariable(body, "name", jeekjeek->get_display_name(sessionId) );
  return body;

}

string JeeksToHTML(CONTENTS jeeks)
{
  string jeeksHTML;
  for (int i = 0 ; i < jeeks.size(); i++)
  {
    jeeksHTML +="    <div class=\"jeek\"> \n";
    jeeksHTML +="    <div class=\"column\" style=\"width: 85%\"> \n";
    jeeksHTML +="        <p class=\"text\"> <b>@" + jeeks[i]->get_publisher()->get_display_name() + ": </b> ";
    jeeksHTML += jeeks[i]->get_text() ;
    STRINGS tags = (static_cast<Jeek *> (jeeks[i]))->get_tags();
    USERS mentions = (static_cast<Jeek *> (jeeks[i]))->get_mentions();
    for (int j = 0; j < tags.size(); j++)
      jeeksHTML += " #" + tags[j];
    for (int j = 0; j < mentions.size(); j++)
      jeeksHTML += " @" + mentions[j]->get_username();
    jeeksHTML += "</p></div> \n";
    jeeksHTML +="    <div class=\"column\" style=\"width: 15%\"> \n";
    jeeksHTML +=" <form action=\"/showJeek\" method=\"post\">\n";
    jeeksHTML +="        <input type= \"hidden\" name=\"jeekID\" value=\"" + jeeks[i]->get_id() + "\">";
    jeeksHTML +="        <button target=\"_blank\" href=\"/showJeek\" class=\"more\" >More</button>\n";           
    jeeksHTML +=" </form></div> </div> \n \n";
  }
  return jeeksHTML;
}

class LoginHandler : public RequestHandler {
  private:
  Jeekjeek * jeekjeek;
public:
  LoginHandler(Jeekjeek * jj) {jeekjeek = jj;}
  Response* callback(Request* req) 
  {
    Response *res;
    string username = req->getBodyParam("username");
    string password = req->getBodyParam("password");
    cout<< "Received Data: "<< username << " - " << password<<endl;
    if (jeekjeek->login(username, password) )
    {
      res = Response::redirect("/");
      res->setSessionId(jeekjeek->get_last_session_id());
    }
    else
    {
      res = new Response;
      res->setHeader("Content-Type","text/html");
      string body = readFile("static/login.html");
      setVariable(body, "Message", "Invalid Login");
      res->setBody(body);
    }
    return res;
  }
};

class LogoutHandler : public RequestHandler {
  private:
  Jeekjeek * jeekjeek;
public:
  LogoutHandler(Jeekjeek * jj) {jeekjeek = jj;}
  Response* callback(Request* req) 
  {
    jeekjeek->logout(req->getSessionId()); 
    Response * res = Response::redirect("/");
    return res;
  }
};



class SearchHandler : public RequestHandler {
  private:
  Jeekjeek * jeekjeek;
public:
  SearchHandler(Jeekjeek * jj) {jeekjeek = jj;}
  Response* callback(Request* req) 
  {
    Response *res = new Response;
    string sessionId = req->getSessionId();
    res->setHeader("Content-Type", "text/html");  
    string body = readFile("static/mainPage.html");
    if (jeekjeek->is_session_id_used(sessionId))
      setVariable(body, "name", jeekjeek->get_display_name(sessionId));

    string jeeksInHTML = JeeksToHTML(jeekjeek->search_jeeks(req->getBodyParam("searchKeyword")));
    insert(body, jeeksInHTML, "<!-- New Post -->");
    res->setBody(body);
    return res;
  }
};

class ShowJeekPage : public RequestHandler {
private:
  Jeekjeek * jeekjeek;
public:
  ShowJeekPage(Jeekjeek * jj) {jeekjeek = jj;}
  Response* callback(Request* req) 
  {
    Response *res ;
    if (!jeekjeek->find_jeek_by_id(req->getBodyParam("jeekID")))
    {
      res = Response::redirect("/404");
      return res;
    }
    res = new Response;
    res->setHeader("Content-Type", "text/html"); 
    res->setBody(ShowJeekHtmlMaker(jeekjeek, req));
    return res;
  }
};

class LikeHandler : public RequestHandler {
private:
  Jeekjeek * jeekjeek;
public:
  LikeHandler(Jeekjeek * jj) {jeekjeek = jj;}
  Response* callback(Request* req) 
  {
    Response *res ;
    User * user = jeekjeek->get_user_by_session_id(req->getSessionId());
    Jeek * jeek = jeekjeek->find_jeek_by_id(req->getBodyParam("jeekID")); 
    if (!jeek)
    {
      res = Response::redirect("/404");
      return res;
    }
    if (user)
    { 
      if (jeek->has_liked(user))
        jeek->dislike(user);
      else
        jeek->like(user);
    }
    res = new Response;
    res->setHeader("Content-Type", "text/html"); 
    res->setBody(ShowJeekHtmlMaker(jeekjeek, req));
    return res;
  }
};

class RejeekHandler: public RequestHandler {
private:
  Jeekjeek * jeekjeek;
public:
  RejeekHandler (Jeekjeek * jj) {jeekjeek = jj;}
  Response* callback(Request* req) 
  {
    Response *res ;
    string jeekID = req->getBodyParam("jeekID");
    if (!jeekjeek->find_jeek_by_id(jeekID))
    {
      res = Response::redirect("/404");
      return res;
    }
    if (jeekjeek->get_user_by_session_id(req->getSessionId()))
    { 
      jeekjeek->rejeek(req->getSessionId(), jeekID);
    }
    res = new Response;
    res->setHeader("Content-Type", "text/html"); 
    res->setBody(ShowJeekHtmlMaker(jeekjeek, req));
    return res;
  }
};



class ShowHomePage : public RequestHandler {
  private:
  Jeekjeek * jeekjeek;
public:
  ShowHomePage(Jeekjeek * jj) {jeekjeek = jj;}
  Response* callback(Request* req) 
  {
    Response *res = new Response;
    string sessionId = req->getSessionId();
    res->setHeader("Content-Type", "text/html");
    string body;  
    if (jeekjeek->is_session_id_used(sessionId))
    {
      body = readFile("static/mainPage.html");
      setVariable(body, "name", jeekjeek->get_display_name(sessionId));
      insert(body, JeeksToHTML(jeekjeek->search_user(sessionId)), "<!-- New Post -->");
    }
    else
      body = readFile("static/home.html");
    res->setBody(body);
    return res;
  }
};

class ProfileHandler : public RequestHandler {
  private:
  Jeekjeek * jeekjeek;
public:
  ProfileHandler(Jeekjeek * jj) {jeekjeek = jj;}
  Response* callback(Request* req) 
  {
    Response *res = new Response;
    string sessionId = req->getSessionId();
    res->setHeader("Content-Type", "text/html");
    string body = readFile("static/mainPage.html"); 
    if (jeekjeek->is_session_id_used(sessionId))
      setVariable(body, "name", jeekjeek->get_display_name(sessionId));

    insert(body, JeeksToHTML(jeekjeek->get_user_jeeks(req->getBodyParam("username"))), "<!-- New Post -->");
    res->setBody(body);
    return res;
  }
};

class SignupHandler : public RequestHandler {
private:
  Jeekjeek * jeekjeek;
public:
  SignupHandler(Jeekjeek * jj) {jeekjeek = jj;}
  Response* callback(Request* req) 
  {
    Response *res ;
    string displayName = req->getBodyParam("displayName");
    string username = req->getBodyParam("username");
    string password = req->getBodyParam("password");
    string repeatPassword = req->getBodyParam("repeatPassword");
    string body = readFile("static/signup.html");

    if (password != repeatPassword )
      setVariable(body, "Error", "Passwords do not match");
    else if (password.size() < 6)
      setVariable(body, "Error", "Password's length should be at least six");
    else 
    {
      if (jeekjeek->sign_up(username, password, displayName))
      {
        res = Response::redirect("/");
        return res;
      }
      else
        setVariable(body, "Error", "Username and Password are not correct");
    }
    res = new Response;
    res->setHeader("Content-Type", "text/html");
    res->setBody(body);
    return res;
  }
};


class NewJeekHandler : public RequestHandler {
  private:
  Jeekjeek * jeekjeek;
public:
  NewJeekHandler(Jeekjeek * jj) {jeekjeek = jj;}
  Response* callback(Request* req) 
  {
    Response *res = new Response;
    res->setHeader("Content-Type","text/html");
    string body = readFile("static/newJeek.html");
    if (jeekjeek->add_jeek(req->getSessionId(), req->getBodyParam("text"), strToVectorOFStr(req->getBodyParam("tags")),
     strToVectorOFStr(req->getBodyParam("mentions")) ) )
    {
      setVariable(body, "Message", "You jeeked succesfully!");
    }
    else
    {
      setVariable(body, "Message", "Something went worng bro!");
    }
    setVariable(body,"name", jeekjeek->get_display_name( req->getSessionId() ));
    res->setBody(body);
    return res;
  }
};

class ShowNewJeek : public RequestHandler {
  private:
  Jeekjeek * jeekjeek;
public:
  ShowNewJeek(Jeekjeek * jj) {jeekjeek = jj;}
  Response* callback(Request* req) 
  {
    Response *res;
    string sessionId = req->getSessionId();
    if (!jeekjeek->is_session_id_used(sessionId))
    {
      res = Response::redirect("/");
      return res;
    }
    res = new Response;
    res->setHeader("Content-Type","text/html");
    string body = readFile("static/newJeek.html");
    setVariable(body,"name", jeekjeek->get_display_name(sessionId));
    res->setBody(body);
    return res;
  }
};

int main(int argc, char **argv) {
  srand(time(NULL)); // for rand
  try {
    Jeekjeek * jeekjeek = new Jeekjeek();
    Server server(argc > 1 ? atoi(argv[1]) : 5000, "static/404.html");
    server.get("/login", new ShowPage("static/login.html"));
    server.get("/signup", new ShowPage("static/signup.html"));
    server.get("/newJeek", new ShowNewJeek(jeekjeek));
    server.post("/showJeek", new ShowJeekPage(jeekjeek));
    server.get("/logout", new LogoutHandler(jeekjeek));
    server.post("/login", new LoginHandler(jeekjeek));
    server.post("/signup", new SignupHandler(jeekjeek));
    server.post("/newJeek", new NewJeekHandler(jeekjeek));
    server.post("/search", new SearchHandler(jeekjeek));
    server.post("/like", new LikeHandler(jeekjeek));
    server.post("/rejeek", new RejeekHandler(jeekjeek));
    server.post("/showProfile", new ProfileHandler(jeekjeek));
    server.get("/home.png", new ShowImage("static/home.png"));
    server.get("/search.png", new ShowImage("static/search.png"));
    server.get("/like.png", new ShowImage("static/like.png"));
    server.get("/dislike.png", new ShowImage("static/dislike.png"));
    server.get("/", new ShowHomePage(jeekjeek));
    server.run();
  } catch (Server::Exception e) {
    cerr << e.getMessage() << endl;
  }
}
