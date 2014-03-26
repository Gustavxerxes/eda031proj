#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <map>
#include <vector>

#include "article.h"

class NewsGroup {

public: 
	NewsGroup(int idIn, std::string& titleIn) : id(idIn), title(titleIn), preArtId(0) {}

	int getId() const { return id; }

	std::string getTitle() const { return title; }

	Article& getArticle(int idin);

	bool listArticles(std::vector< std::pair<int, std::string> >& arts) ; 
	
	bool addArticle(const std::string& title, const std::string& author, const std::string& textbody);
	
	bool removeArticle(int artId);

private:
	int id;
	int preArtId;
	std::string title;
	std::map<int, Article> articleMap;

};
#endif
