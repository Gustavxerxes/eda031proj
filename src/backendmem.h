// ------------------------------------------------------------------
//
//						News System Server
//					Backend memory class header
//
//			Written by Viktor, Martin and Niklas
//
// Change log
//	Apr 10, 2014	NH	Initial version
//	
//
//
// ------------------------------------------------------------------

#ifndef BACKENDMEM_H
#define BACKENDMEM_H

#include "newsgroup.h"
#include "backend.h"

#include <string>
#include <vector>
#include <map>

class BackEndMem : public BackEnd {
public:
	BackEndMem() : ng_count(0) {};
	void listNG(std::vector<std::pair<int, std::string> >& ngs);
	bool addNG(const std::string& name);
	bool removeNG(const int id);
	bool listArticles(const int ngId, std::vector<std::pair<int, std::string> >& arts);
	bool addArticle(const int ngId, const std::string& title, const std::string& author, const std::string& textbody);
	unsigned char removeArticle(const int ngId, const int artId);
	unsigned char getArticle(const int ngId, const int artId, std::vector<std::string>& article);
	
private:
	std::map<int, NewsGroup> database;
	int ng_count;
};

#endif
