	#ifndef BACKEND_H
#define BACKEND_H

#include "newsgroup.h"
#include <string>
#include <vector>
#include <map>

class BackEnd {
public:
	BackEnd() : ng_count(0) {}
	void listNG(std::vector<std::pair<int, std::string>>& ngs);
	bool addNG(const std::string& name);
	bool removeNG(const int id);
	bool listArticles(const int ngId, std::vector<std::pair<int, std::string>>& arts);
	bool addArticle(const int ngId, const std::string& title, const std::string& author, const std::string& textbody);
	unsigned char removeArticle(const int ngId, const int artId);
	unsigned char getArticle(const int ngId, const int artId, std::vector<std::string>& article);
	enum {
		NO_ERR = 0,
		ERR_NG = 1,
		ERR_ART = 2
	};

private:
	std::map<int, NewsGroup> database;
	int ng_count;




};

#endif
