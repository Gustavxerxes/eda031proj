#include "newsgroup.h"


	bool NewsGroup::listArticles(std::vector<std::pair<int, std::string>>& arts) const{
		for(auto it = database.begin() ; it != database.end() ; ++it){
			vector.push_back(make_pair(it->first, it->second));
		}
	return;
		
		
		
	} 
	
	bool NewsGroup::addArticle(const std::string& title, const std::string& author, const std::string& textbody);
	
	bool NewsGroup::removeArticle(int artId);
