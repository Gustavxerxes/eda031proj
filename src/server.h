// ------------------------------------------------------------------
//
//				   Client/Server communication package
//
//						   Server header file
//
// Change log
//	950323	RH	Initial version
//	951212	RH	Modified to allow subclassing of class Connection
//	970127	RH	Changed "private" to "protected"
//	990125	PH	Changed function names: Read -> read, etc.
//	000114	PH	int -> bool, virtual destructors, other minor changes
//	010129	PH	added void type to initConnection
//	011212	PH	changed char* arguments to const char*
//				changed connection closed handling to exception
//				unsigned char instead of char/int in write/read
//	020102	PH	split into separate file for each class
//	040421	PH	added namespace, new casts, cleaned up a lot
//	050113	PH	added deregisterConnection, new registration (vector),
//				added check for server shutdown, many more changes
//  130521  PH  removed namespace, shared pointers instead of raw
//              pointers
//
// ------------------------------------------------------------------

#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <memory>
#include "connection.h"

/* A server listens to a port and handles multiple connections */
class Server {
public:
	/* Creates a server that listens to a port */
	explicit Server(int port);
	
	/* Removes all registered connections */
	virtual ~Server();
	
	/* Returns true if the server has been initialized correctly */
	bool isReady() const;
	
	/* Waits for activity on the port. Returns a previously registered
	   connection object if an existing client wishes to communicate,
	   nullptr if a new client wishes to communicate */
	std::shared_ptr<Connection> waitForActivity() const;
	
	/* Registers a new connection */
	void registerConnection(const std::shared_ptr<Connection>& conn);
	
	/* Deregisters a connection */
	void deregisterConnection(const std::shared_ptr<Connection>& conn);
	
protected:
	/* The number of the communication socket */
	int my_socket;
	
	/* List of registered connections */
	std::vector<std::shared_ptr<Connection>> connections;
	
	/* Socket for a connection waiting to be registered */
	mutable int pending_socket;
	
	/* Prints error message and exita */
	void error(const char* msg) const;
};

#endif
