// ------------------------------------------------------------------
//
//						News System Server
//						Backend sql class 
//
//			Written by Viktor, Martin and Niklas
//
// Change log
//	Apr 10, 2014	NH	Initial version
//
//	
//
//
// ------------------------------------------------------------------

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

BackEndSql::BackEndSql(string user, string pw, string url, string db){

	driver = get_driver_instance();
	connectedFlag = false;
	connect(user, pw, url, db);
}


BackEndSql::~BackEndSql(){
	disconnect();
}

void BackEndSql::connect(string user, string pw, string url, string db){
	try{
		con = driver->connect(url, user, pw);
		con->setSchema(db);
		connectedFlag = true;
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}


void BackEndSql::disconnect(){
	connectedFlag = false;
	delete con;

}


bool BackEndSql::ngExists(int ngId){

	PreparedStatement *stmt;
	ResultSet *res;
	try{
		stmt = con->prepareStatement("select * from NewsGroup where NgId = ?");
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
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
		return false;
	}
}

void BackEndSql::listNG(std::vector<std::pair<int, std::string> >& ngs){



	PreparedStatement *stmt; 
	ResultSet *res;
	try{
		stmt = con->prepareStatement("select NgID, title from newsgroup");	
		res = stmt->executeQuery();
		while(res->next()){
			ngs.push_back(make_pair(res->getInt("NgID"), res->getString("title")));
		}
		delete res;
		delete stmt;
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}

bool BackEndSql::addNG(const std::string& name){

	ResultSet *res;
	PreparedStatement *stmt;

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
		return true;
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
		return false;
	}
}

bool BackEndSql::removeNG(const int id){
	PreparedStatement *stmt;
	bool deleted;
	try{
		stmt = con->prepareStatement("delete from Article where ngId = ? ");
		stmt->setInt(1,id);
		stmt->executeUpdate();
		delete stmt;
		stmt = con->prepareStatement("delete from NewsGroup where NgId = ?");
		stmt->setInt(1,id);

		deleted = (0 != stmt->executeUpdate());
		delete stmt;

	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
		deleted = false;
	}
	return deleted;
}


bool BackEndSql::listArticles(const int ngId, std::vector<std::pair<int, std::string> >& arts){


	if(!ngExists(ngId)){
		return false;
	}

	PreparedStatement *stmt;
	ResultSet *res;
	try{
		stmt = con->prepareStatement("select artId,title from Article where NgId = ?");
		stmt->setInt(1, ngId);
		res = stmt->executeQuery();
		while(res->next()){
			arts.push_back(make_pair(res->getInt("artId"), res->getString("title")));
		}
		delete res;
		delete stmt;

	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
		return false;
	}
return true;
}

bool BackEndSql::addArticle(const int ngId, const std::string& title, const std::string& author, const std::string& textbody){


	

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
		return true;
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
		return false;
	}

}

unsigned char BackEndSql::removeArticle(int ngId, int artId){

	if(!ngExists(ngId)){
		return ERR_NG;
	}

	PreparedStatement *stmt;
	try{
		stmt = con->prepareStatement("delete from Article where ngId = ? and ArtId = ?");
		stmt->setInt(1,ngId);
		stmt->setInt(2, artId);

		if(0 == stmt->executeUpdate()){
			delete stmt;
			return ERR_ART;
		}
		delete stmt;
		return NO_ERR;
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
		return ERR_NG;
	}

}

unsigned char BackEndSql::getArticle(const int ngId, const int artId, vector<string>& article) {

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
		delete res;
		delete stmt;
		return ERR_ART;
	} catch(sql::SQLException &e){
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl
		<< "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << " )" << endl;
		return ERR_NG;
	}
}

