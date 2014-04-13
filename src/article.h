// ------------------------------------------------------------------
//
//						News System Server
//							Article class 
//
//			Written by Viktor, Martin and Niklas
// Change log
//	Mar 21, 2014	VN	Initial version
//
//
// ------------------------------------------------------------------

#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class Article{ 

public:
	Article() {}

	Article(const int& idIn, const std::string& titleIn, const std::string& authorIn, const std::string& textIn)
	 : id(idIn), title(titleIn), author(authorIn), text(textIn) {}

	int getId() const { return id;}

	std::string getTitle() const { return title; } 

	std::string getAuthor() const { return author; }

	std::string getText() const { return text; }
private:
	int id;
	std::string title;
	std::string author;
	std::string text;
};
#endif
