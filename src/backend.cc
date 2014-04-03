#include <map>
#include <vector>
#include <string>
#include "backend.h"
#include "article.h"
#include "newsgroup.h"


using namespace std;


void BackEnd::listNG(std::vector<std::pair<int, std::string>>& ngs){
	
	for(auto it = database.begin() ; it != database.end() ; ++it){
		ngs.push_back(make_pair(it->first, it->second.getTitle()));
	}
	return;
}

bool BackEnd::addNG(const std::string& name){
	return (database.emplace(ng_count++, NewsGroup(ng_count, name))).second;
}

bool BackEnd::removeNG(const int id){
	auto ng = database.find(id);
	return database.erase(ng) != database.end();
}


bool BackEnd::listArticles(const int ngId, std::vector<std::pair<int, std::string>>& arts){
	
	map<int,NewsGroup>::iterator it;
	
	((it = database.find(ngId)) != database.end()) ? return it->listArticles(arts) : return false;
	
}

bool BackEnd::addArticle(intndId, const std::string& title, const std::string& author, const std::string& textbody){

	map<int,NewsGroup>::iterator it;
	
	((it = database.find(ngId)) != database.end()) ? return it->addArticle(title,author,textbody) : return false;
	
	
}

bool BackEnd::removeArticle(int ngId, int artId){
	
	map<int,NewsGroup>::iterator it;
	
	((it = database.find(ngId)) != database.end()) ? return it->removeArticle(artId) : return false;
}
