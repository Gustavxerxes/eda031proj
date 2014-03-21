#ifndef BACKEND_H
#define BACKEND_H

#include "newsgroup.h"
#include <string>
#include <vector>
#include <map>

class BackEnd {
	public;
	bool listNG(std::vector<std::pair<int, std::string>>& ngs);
	bool addNG(const std::string& name);
	bool removeNG(const int id);
	bool listArticle(std::vector<std::pair<int, std::string>>& arts);
	bool addArticle(const std::string& title, const std::string& author, const std::string& textbody);
	bool removeArticle(const int id);
	
	private:
	std::map<int, NewsGroup::NewsGroup>;
	int NGcount;

};

#endif
