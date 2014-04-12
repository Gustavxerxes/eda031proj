#include "mysql_connection.h"
#include "backendsql.h"
//mysql c++ connector, see http://dev.mysql.com/doc/connector-cpp/en/connector-cpp-installation-source.html for more info on these includes

//build commands:
//g++ -std=c++11  -I/usr/local/include -I/usr/local/boost/include zerversql.cc connection.cc  server.cc frontend.cc backendsql.cc 
//-L/usr/local/lib -L/usr/local/mysql/lib  -o zerversql  -lmysqlcppconn -g

#include <vector>
#include <string>
#include <algorithm>
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
	try{
		con = driver->connect("puccini.cs.lth.se", "db06", "kodord2");
		con->setSchema("db06");
		connectedFlag = true;
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
}
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
	try{
		stmt = con->prepareStatement("select * from NewsGroup where NgId = ?");
		stmt->setInt(1,ngId);
		res = stmt->executeQuery();
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;

}
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



	PreparedStatement *stmt = 
	Resultset *res;
	try{
		stmt = con->prepareStatement("select NgID, title from newsgroup");	
		res = stmt->executeQuery();
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
}

while(res->next()){
	ngs.push_back(make_pair(res->getInt("NgID"), res->getString("title")));
}
delete res;
delete stmt;
}

bool BackEnd::addNG(const std::string& name){

	ResultSet *res;
	PreparedStatement *stmt 

	try{
		stmt = con->prepareStatement("select title from NewsGroup where title = ?");
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
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
}
}

}

bool BackEnd::removeNG(const int id){
	PreparedStatement *stmt;
	try{
		stmt = con->prepareStatement("delete from Article where ngId = ? ");
		stmt->setInt(1,id);
		stmt->executeUpdate();
		delete stmt;
		stmt = con->prepareStatement("delete from NewsGroup where NgId = ?");
		stmt->setInt(1,id);

		bool deleted = (0 != stmt->executeUpdate());
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
}
delete stmt;
return deleted;
}


bool BackEnd::listArticles(const int ngId, std::vector<std::pair<int, std::string> >& arts){


	if(!ngExists(ngId)){
		return false;
	}

	PreparedStatement *stmt;
	ResultSet *res;
	try{
		stmt = con->prepareStatement("select artId,title from Article where NgId = ?");
		stmt->setInt(1, ngId);
		res = stmt->executeQuery();

	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
}
while(res->next()){
	arts.push_back(make_pair(res->getInt("artId"), res->getString("title")));
}
delete res;
delete stmt;
return true;
}

bool BackEnd::addArticle(const int ngId, const std::string& title, const std::string& author, const std::string& textbody){


	

	PreparedStatement *stmt;
	ResultSet *res;
	try{
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

		stmt = con->prepareStatement("update NewsGroup set nextArtId = nextArtId+1 where ngId = ?");
		stmt->setInt(1, ngId);
		stmt->executeUpdate();
		delete stmt;
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
}

}

unsigned char BackEnd::removeArticle(int ngId, int artId){

	if(!ngExists(ngId)){
		return ERR_NG;
	}

	PreparedStatement *stmt;
	ResultSet *res;
	try{
		stmt = con->prepareStatement("delete from Article where ngId = ? and ArtId = ?");
		stmt->setInt(1,ngId);
		stmt->setInt(2, artId);

		if(0 == stmt->executeUpdate()){
			delete stmt;
			return ERR_ART;
		}
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
}
delete stmt;
return NO_ERR;
}

unsigned char BackEnd::getArticle(const int ngId, const int artId, vector<string>& article) {

	if(!ngExists(ngId)){
		return ERR_NG;;
	}

	PreparedStatement *stmt;
	ResultSet *res;
	try{
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
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
}
}

delete res;
delete stmt;
return ERR_ART;
}
