#include "frontend.h"
#include "backend.h"
#include "backendsql.h"
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "invalidprotocolexception.h"
#include "mysql_connection.h"
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: zerver port-number" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	Server server(port);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
		exit(1);
	}

 	BackEndSql be = BackEndSql();
	FrontEnd fe(&be);
	cout << "Server started" << endl;

	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			try {
				fe.readAndReply(conn);
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			} catch (InvalidProtocolException&) {
				server.deregisterConnection(conn);
				cout << "Invalid use of protocol" << endl;
			}
		} else {
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}
}