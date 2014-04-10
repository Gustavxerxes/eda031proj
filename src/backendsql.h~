	#ifndef BACKEND_H
#define BACKEND_H
#include <string>
#include <vector>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


class BackEnd {
public:
	BackEnd();
	~BackEnd();
	void listNG(std::vector<std::pair<int, std::string> >& ngs);
	bool addNG(const std::string& name);
	bool removeNG(const int id);
	bool listArticles(const int ngId, std::vector<std::pair<int, std::string> >& arts);
	bool addArticle(const int ngId, const std::string& title, const std::string& author, const std::string& textbody);
	unsigned char removeArticle(const int ngId, const int artId);
	unsigned char getArticle(const int ngId, const int artId, std::vector<std::string>& article);
	enum {
		NO_ERR = 0,
		ERR_NG = 1,
		ERR_ART = 2
	};

private:
	void connect();
	void disconnect();
	bool ngExists(int ngId);

	sql::Driver *driver;
	sql::Connection *con;
	bool connectedFlag;
	sql::Statement *stmt;
	sql::ResultSet *res;

	
};

#endif
