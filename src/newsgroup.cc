// ------------------------------------------------------------------
//
//						News System Server
//				Newsgroup class for inmemory database
//
//			Written by Viktor, Martin and Niklas
//
// Change log
//	Mar 24, 2014	VN	Initial version
//	
//
//
// ------------------------------------------------------------------

#include <map>
#include <vector>
#include <string>

#include "newsgroup.h"
#include "article.h"

	bool NewsGroup::listArticles(std::vector< std::pair<int, std::string> >& arts) {
		if ( articleMap.empty() ){
			return true;
		}
		for(auto it = articleMap.begin() ; it != articleMap.end() ; ++it){
			arts.push_back(  make_pair( it->first, (it->second).getTitle() ) );
		}
	return true;
	} 
	
	void NewsGroup::addArticle(const std::string& title, const std::string& author, const std::string& textbody) {
		++preArtId;
		articleMap[preArtId] = Article(preArtId, title, author, textbody);		

	}
	
	bool NewsGroup::removeArticle(int artId){

		return (articleMap.erase(artId) > 0);
	}
	bool NewsGroup::fetchArticle(int idin, Article& art){
		auto it = articleMap.find(idin); 

		if(it !=  articleMap.end()){
			art = it->second;
			return true;
		}
		return false;
	}


