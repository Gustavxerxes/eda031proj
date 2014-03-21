#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class Article{
public:
	Article(int& idIn) : id(id) {}

	int getId() const { return id;}

	std::string getTitle() const { return title;} 
private:
	int id;
	std::string title;
	std::string author;
	std::string articleText;
};
#endif