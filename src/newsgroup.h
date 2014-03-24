#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>

#include "Article"

class Newsgroup {

public: 
	Newsgroup(int& idIn, std::string& titleIn) : id(idIn), title(titleIn), preArtId(0) {}

	int getId() const { return id; }

	std::string getTitle() const { return title; }

	Article getArticle(int& id) const;

	bool makeArticle(std::string& titleIn, std::string& authorIn, std::string& textIn);

	bool removeArticle(int& id);

private:
	int id;
	int preArtId;
	std::string title;
	std::map<int, std::shared_ptr<Article>> articleMap;

};
#endif