#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <string>
#include "connection.h"
#include "protocol.h"

class Messagehandler {

public: 

	Messagehandler(const Connection connIn): conn = connIn {};
	void writeInt(const int value);

	void writeString(const string& s);

	void wrriteCode(const unsigned char code);

	int readInt();

	string readString();

private:

	int helperRead();

	void helperWrite(int value);

	Connection conn;

};
#endif