#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class Article{ 

public:
	Article(const int& idIn, const std::string& titleIn, const std::string& authorIn, const std::string& textIn)
	 : id(id), title(titleIn), author(authorIn), text(textIn) {}

	int getId() const { return id;}

	std::string getTitle() const { return title; } 

	std::string getAuthor() const { return author; }

	std::string getArticle() const { return text; }
private:
	int id;
	std::string title;
	std::string author;
	std::string text;
};
#endif
