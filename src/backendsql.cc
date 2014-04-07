#include <vector>
#include <string>
#include "backendsql.h"
#include <algorithm>
//mysql c++ connector, see http://dev.mysql.com/doc/connector-cpp/en/ for more info on these includes
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


using namespace std;
using namespace sql;

BackEnd::BackEnd(){



	driver = get_driver_instance();
	connectedFlag = false;
	connect();
	

	//construct sql driver here
}


BackEnd::~BackEnd(){
	disconnect();


}

void BackEnd::connect(){

	con = driver->connect("puccini.cs.lth.se", "db06", "kodord");
	con->setSchema("db06");
	connectedFlag = true;
}


void BackEnd::disconnect(){
	connectedFlag = false;
	delete res;
	delete stmt;
	delete con;

}


bool BackEnd::ngExists(int ngId){

	PreparedStatement *stmt;
	ResultSet *res;
	stmt = con->prepareStatement("select NgId from NewsGroup where NgId = ?");
	stmt->setInt(1,ngId);
	res = stmt->executeQuery();
	if(res->next()){
		delete res;
		delete stmt;
		return true;
	}
	delete res;
	delete stmt;
	return false;

}

void BackEnd::listNG(std::vector<std::pair<int, std::string> >& ngs){

	PreparedStatement *stmt = con->prepareStatement("select NgID, title from newsgroup");


	res = stmt->executeQuery();
	
	while(res->next()){
		ngs.push_back(make_pair(res->getInt("NgID"), res->getString("title")));
	}
	delete res;
	delete stmt;
}

bool BackEnd::addNG(const std::string& name){
	ResultSet *res;
	PreparedStatement *stmt = con->prepareStatement("select title from NewsGroup where title = ?");
	stmt->setString(1,name);
	res = stmt->executeQuery();
	if(res->next()){
		delete res;
		delete stmt;
		return false;
	}
	delete res;
	delete stmt;
	
	stmt = con->prepareStatement("insert into NewsGroup(NgId, nextArtId, title) values(default,1,?)");
	stmt->setString(1, name);
	stmt->executeUpdate();
	delete stmt;
}

bool BackEnd::removeNG(const int id){
	PreparedStatement *stmt;
	stmt = con->prepareStatement("delete from NewsGroup where NgId = ?");
	stmt->setInt(1,id);
	stmt->executeUpdate();
	bool deleted = (0 == stmt->getUpdateCount());
	delete stmt;
	return deleted;
}


bool BackEnd::listArticles(const int ngId, std::vector<std::pair<int, std::string> >& arts){


	if(!ngExists(ngId)){
		return false;
	}

	PreparedStatement *stmt;
	ResultSet *res;

	stmt = con->prepareStatement("select Artid,Article.title from Article where NgId = ?");
	stmt->setInt(1, ngId);
	res = stmt->executeQuery();

	while(res->next()){
		arts.push_back(make_pair(res->getInt("ArtId"), res->getString("Article.title")));
	}
	delete res;
	delete stmt;
	return true;
}

bool BackEnd::addArticle(const int ngId, const std::string& title, const std::string& author, const std::string& textbody){


	

	PreparedStatement *stmt;
	ResultSet *res;

	stmt = con->prepareStatement("select nextArtId from NewsGroup where NgId = ?");
	stmt->setInt(1,ngId);
	res = stmt->executeQuery();
	if(!res->next()){
		delete res;
		delete stmt;
		return false;
	}
	int artnbr = res->getInt("nextArtId");
	delete res;
	delete stmt;



	stmt = con->prepareStatement("insert into Article(ngId, ArtId, title, author, artText) values(?,?,?,?,?)");
	stmt->setInt(1, ngId);
	stmt->setInt(2,artnbr);
	stmt->setString(3, title);
	stmt->setString(4, author);
	stmt->setString(5, textbody);
	stmt->executeUpdate();
	delete stmt;

	stmt = con->prepareStatement("update NewsGroup set nextArtId = nextArtId+1 where NewsGroup = ?");
	stmt->setInt(1, ngId);
	stmt->executeUpdate();
	delete res;
	delete stmt;

}

unsigned char BackEnd::removeArticle(int ngId, int artId){

	PreparedStatement *stmt;
	ResultSet *res;

	stmt = con->prepareStatement("delete from Article where NgId = ? and ArtId = ?");

	bool removedArt = (0 != stmt->getUpdateCount());
	delete res;
	delete stmt;
	
	return removedArt;
}

unsigned char BackEnd::getArticle(const int ngId, const int artId, vector<string>& article) {

	if(!ngExists(ngId)){
		return ERR_NG;;
	}

	PreparedStatement *stmt;
	ResultSet *res;

	stmt = con->prepareStatement("select title, author, artText from Article where NgId = ? and ArtId = ?");
	stmt->setInt(1,ngId);
	stmt->setInt(2, artId);
	res = stmt->executeQuery();
	if(res->next()){
		article.push_back(res->getString("title"));
		article.push_back(res->getString("author"));
		article.push_back(res->getString("artText"));
		delete res;
		delete stmt;
		return NO_ERR;
	}

	delete res;
	delete stmt;
	return ERR_ART;
}
