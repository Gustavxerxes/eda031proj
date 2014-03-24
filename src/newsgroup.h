#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>

#include "Article"

class NewsGroup {

public: 
	NewsgGroup(int& idIn, std::string& titleIn) : id(idIn), title(titleIn), preArtId(0) {}

	int getId() const { return id; }

	std::string getTitle() cheonst { return title; }

	Article getArticle(int& id) const;

	bool listArticles(std::vector<std::pair<int, std::string>>& arts) const; 
	
	bool addArticle(const std::string& title, const std::string& author, const std::string& textbody);
	
	bool removeArticle(int artId);

	bool makeArticle(std::string& titleIn, std::string& authorIn, std::string& textIn);

	bool removeArticle(int& id);

private:
	int id;
	int preArtId;
	std::string title;
	std::map<int, Article> articleMap;

};
#endif
