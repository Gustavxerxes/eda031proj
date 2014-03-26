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
	bool listArticle(const int ngId, std::vector<std::pair<int, std::string>>& arts);
	bool addArticle(const std::string& title, const std::string& author, const std::string& textbody);
	unsigned char removeArticle(const int ngId, const int artId);
	unsigned char getArticle(const int ngId, const int artId, std::vector<std::string>& article);
	enum {
		unsigned char NO_ERR = 0:
		unsigned char ERR_NG = 1;
		unsigned char ERR_ART = 2;
	}

	private:
	std::map<int, NewsGroup::NewsGroup> database;
	int NGcount;
};

#endif

herp derp a clerp