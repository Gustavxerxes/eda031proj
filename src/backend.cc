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

bool BackEnd::addNG(const std::string& name){ //Måste fixas då det inte får finnas dubletter i newsgroup. Dvs de får inte ha samma namn.
	return (database.emplace(++ng_count, NewsGroup(ng_count, name))).second;
}

bool BackEnd::removeNG(const int id){
	auto ng = database.find(id);
	if (ng == database.end()) return false;
	database.erase(ng);
	return true;
}


bool BackEnd::listArticles(const int ngId, std::vector<std::pair<int, std::string>>& arts){
	auto it = database.find(ngId);
	if (it != database.end()) {
		return (it->second).listArticles(arts);
	} else {
		return false;
	}
}

bool BackEnd::addArticle(const int ngId, const std::string& title, const std::string& author, const std::string& textbody){
	auto it = database.find(ngId);
	if (it != database.end()) {
		(it->second).addArticle(title,author,textbody);
		return true;
	} else {
		return false;
	}
}

unsigned char BackEnd::removeArticle(int ngId, int artId){
	auto it = database.find(ngId);
	if (it != database.end()) {
		if ((it->second).removeArticle(artId)) return NO_ERR;
		return ERR_ART;
	} else {
		return ERR_NG;
	}
}

unsigned char BackEnd::getArticle(const int ngId, const int artId, vector<string>& article) {
	auto it = database.find(ngId);
	if (it != database.end()) {
		Article a;
		if ((it->second).fetchArticle(artId,a)) {
			article.push_back(a.getTitle());
			article.push_back(a.getAuthor());
			article.push_back(a.getText());
			return NO_ERR;
		}
		return ERR_ART;
	} else {
		return ERR_NG;
	}
}