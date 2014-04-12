#ifndef BACKENDSQL_H
#define BACKENDSQL_H
#include <string>
#include <vector>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


class BackEndSql : public BackEnd {
public:
private:
	void connect();
	void disconnect();
	bool ngExists(int ngId);

	sql::Driver *driver;
	sql::Connection *con;
	bool connectedFlag
;};

#endif
