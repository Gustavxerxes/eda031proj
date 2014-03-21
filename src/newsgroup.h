#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include 

class Newsgroup {

public: 
	Newsgroup(int& idIn, std::string& titleIn) : id(idIn), title(titleIn) {}

	int getId() const { return id; }

	std::string getTitle() const { return title; }



private:
	int id;
	std::string title;
	std::map<int, Article> articleMap;

};
#endif