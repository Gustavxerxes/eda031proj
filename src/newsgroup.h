// ------------------------------------------------------------------
//
//						News System Server
//				Newsgroup class for inmemory database
//
//			Written by Viktor, Martin and Niklas
//
// Change log
//	Mar 24, 2014	VN	Initial version
//	
//
//
// ------------------------------------------------------------------

#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <map>
#include <vector>

#include "article.h"

class NewsGroup {

public: 
	NewsGroup(int& idIn, const std::string& titleIn) : id(idIn), preArtId(0), title(titleIn)  {}

	int getId() const { return id; }

	std::string getTitle() const { return title; }
	

	bool fetchArticle(int idin, Article& art);

	bool listArticles(std::vector< std::pair<int, std::string> >& arts) ; 
	
	void addArticle(const std::string& title, const std::string& author, const std::string& textbody);
	
	bool removeArticle(int artId);

private:
	int id;
	int preArtId;
	std::string title;
	std::map<int, Article> articleMap;

};
#endif
