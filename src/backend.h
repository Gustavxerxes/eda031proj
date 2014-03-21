#ifndef BACKEND
#define BACKEND

#include "newsgroup.h"
#indlude <string>
#include <vector>
#include <map>

class BackEnd (
	public;
	bool listNG(std::vector<std::pair<int, std::string>>&);
	bool addNG(std::string name);
	bool removeNG(int id);
	bool listArticle(std::vector<std::pair<int, std::string>>&);
	bool addArticle(std::vector<std::string<);
	bool removeArticle(int id);
	
	private:
	std::map<int, NewsGroup::NewsGroup>;
	int NGcount;
	

);

#endif
