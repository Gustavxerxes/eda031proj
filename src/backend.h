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
	unsigned char removeArticle(const int ng_id, const int art_id);
	unsigned char getArticle(const int ng_id, const int art_id, string[]& article);
	enum {
		unsigned char NO_ERR = 0:
		unsigned char ERR_NG = 1;
		unsigned char ERR_ART = 2;
	}

	private:
	std::map<int, NewsGroup::NewsGroup>;
	int NGcount;
};

#endif
