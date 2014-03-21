#ifndef FRONTEND_H
#define FRONTEND_H

#include "connection.h"

class FrontEnd {
public:
	Frontend(BackEnd& be) : backend(be) {}
	void readAndReply(const shared_ptr<Connection>& conn);
private:
	BackEnd backend;

	//lots of private methods
};

#endif