// ------------------------------------------------------------------
//
//						News System Server
//						BackEnd interface 
//
//			Written by Viktor, Martin and Niklas
//
// Change log
//	Mar 21, 2014	ML	Initial version
//	
//
//
// ------------------------------------------------------------------

#ifndef BACKEND_H
#define BACKEND_H
#include <string>
#include <vector>

class BackEnd {
public:
	virtual void listNG(std::vector<std::pair<int, std::string> >& ngs) = 0;
	virtual bool addNG(const std::string& name) = 0;
	virtual bool removeNG(const int id) = 0;
	virtual bool listArticles(const int ngId, std::vector<std::pair<int, std::string> >& arts) = 0;
	virtual bool addArticle(const int ngId, const std::string& title, const std::string& author, const std::string& textbody) = 0;
	virtual unsigned char removeArticle(const int ngId, const int artId) = 0;
	virtual unsigned char getArticle(const int ngId, const int artId, std::vector<std::string>& article) = 0;
	enum {
		NO_ERR = 0,
		ERR_NG = 1,
		ERR_ART = 2
	};
private:
	
};

#endif
