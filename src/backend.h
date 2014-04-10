#ifndef BACKEND_H
#define BACKEND_H
#include <string>
#include <vector>

class BackEnd {
public:
	virtual BackEnd();
	virtual ~BackEnd();
	virtual void listNG(std::vector<std::pair<int, std::string> >& ngs);
	virtual bool addNG(const std::string& name);
	virtual bool removeNG(const int id);
	virtual bool listArticles(const int ngId, std::vector<std::pair<int, std::string> >& arts);
	virtual bool addArticle(const int ngId, const std::string& title, const std::string& author, const std::string& textbody);
	virtual unsigned char removeArticle(const int ngId, const int artId);
	virtual unsigned char getArticle(const int ngId, const int artId, std::vector<std::string>& article);
	enum {
		NO_ERR = 0,
		ERR_NG = 1,
		ERR_ART = 2
	};
private:
	
};

#endif