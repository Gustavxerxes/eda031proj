#include <map>
#include <vector>
#include <string>

#include "newsgroup.h"
#include "article.h"


	bool NewsGroup::listArticles(std::vector<std::pair<int, std::string>>& arts) {
		if ( articleMap.empty() ){
			return false;
		}
		for(auto it = articleMap.begin() ; it != articleMap.end() ; ++it){
			arts.push_back(  make_pair( it->first, (it->second).getTitle() ) );
		}
	return true;
	} 
	
	bool NewsGroup::addArticle(const std::string& title, const std::string& author, const std::string& textbody) {
		articleMap[++preArtId] = Article(preArtId, title, author, textbody);		

	}
	
	bool NewsGroup::removeArticle(int artId){
		auto it = articleMap.find(artId); 
		if(it !=  articleMap.end()){
			articleMap.erase(it);
			return true;
		}
		return false;

	}

