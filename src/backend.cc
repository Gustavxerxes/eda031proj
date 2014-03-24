#include <map>
#include <string>
#include "backend.h"


using namespace sdt;


void listNG(std::vector<std::pair<int, std::string>>& ngs){
	
	for(auto it = database.begin() ; it != database.end() ; ++it){
		vector.push_back(make_pair(it->first, it->second));
	}
	return;
}

bool BackEnd::addNG(const std::string& name){
		
		return database.emplace(Ngcount++, name);
}

bool removeNG(const int id){
		return database.remove(id);
}


bool listArticle(int ngId, std::vector<std::pair<int, std::string>>& arts){
	
	map<int,NewsGroup>::iterator it;
	
	((it = database.find(ngId)) == database.end()) ? 
	
}

bool addArticle(const std::string& title, const std::string& author, const std::string& textbody){
	
	
}

bool removeArticle(int ngId, int artId){
	
}
