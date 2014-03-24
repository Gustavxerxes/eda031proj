#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class Article{ 

public:
	Article(int& idIn, std::string& titleIn, std::string& authorIn, std::string& textIn)
	 : id(id), title(titleIn), author(authorIn), text(textIn) {}

	int& getId() const { return id;}

	std::string& getTitle() const { return title; } 

	std::string& getAuthor() const { return author; }

	std::string& getArticle() const { return articleText; }
private:
	int id;
	std::string title;
	std::string author;
	std::string text;
};
#endif
