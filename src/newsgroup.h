#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>

#include "Article"

class Newsgroup {

public: 
	Newsgroup(int& idIn, std::string& titleIn) : id(idIn), title(titleIn), preArtId(0) {}

	int getId() const { return id; }

	std::string getTitle() const { return title; }



private:
	int id;
	int preArtId;
	std::string title;
	std::map<int, Article> articleMap;

};
#endif