// ------------------------------------------------------------------
//
//						News System Server
//					Backend sql class header
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

#ifndef BACKENDSQL_H
#define BACKENDSQL_H
#include <string>
#include <vector>
#include "backend.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class BackEndSql : public BackEnd {
public:	
	BackEndSql(const std::string user, const std::string pw, const std::string url, const std::string db);
	~BackEndSql();
	void listNG(std::vector<std::pair<int, std::string> >& ngs);
	bool addNG(const std::string& name);
	bool removeNG(const int id);
	bool listArticles(const int ngId, std::vector<std::pair<int, std::string> >& arts);
	bool addArticle(const int ngId, const std::string& title, const std::string& author, const std::string& textbody);
	unsigned char removeArticle(const int ngId, const int artId);
	unsigned char getArticle(const int ngId, const int artId, std::vector<std::string>& article);

private:
	void connect(const std::string user, const std::string pw, const std::string url, const std::string db);
	void disconnect();
	bool ngExists(int ngId);

	sql::Driver *driver;
	sql::Connection *con;
	bool connectedFlag;
};

#endif
