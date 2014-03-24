#ifndef BACKEND_H
#define BACKEND_H

#include "newsgroup.h"
#include <string>
#include <vector>
#include <map>

class BackEnd {
	public:
	Backend() : NGcount(0) {}
	void listNG(std::vector<std::pair<int, std::string>>& ngs);
	bool addNG(const std::string& name);
	bool removeNG(const int id);
	bool listArticle(int ngId, std::vector<std::pair<int, std::string>>& arts);
	bool addArticle(int ngId, const std::string& title, const std::string& author, const std::string& textbody);
	bool removeArticle(int ngId, int artId);
	
	private:
	std::map<int, NewsGroup::NewsGroup> database;
	int NGcount;

};

#endif
