#include messagehandler.h;
#include <string>
#include "connection.h"




void Messagehandler::helperWrite(const Connection& conn, int value){
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8)	 & 0xFF);
	conn.write(value & 0xFF);
}

void Messagehandler::writeInt(const Connection& conn, int value) {
	conn.write(Protocol::PAR_NUM);
	helperWrite(conn,value);
}

void Messagehandler::writeString(const Connection& conn, const string& s) {
	conn.write(Protocol::PAR_STRING);

	int n = s.size();
	// Write N
	helperWrite(conn,n);

	// Write chars
	for (auto it = s.begin(); it != s.end(); ++it) {
		conn.write(*it);
	}
}

int Messagehandler::helperRead(const Connection& conn){
	unsigned char b1 = conn.read();
	unsigned char b2 = conn.read();
	unsigned char b3 = conn.read();
	unsigned char b4 = conn.read();
	return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

int Messagehandler::readInt(const Connection& conn) {
	if(conn.read()!=Protocol::PAR_NUM) throw InvalidProtocolException(); // read PAR_NUM byte
	return helperRead(conn);
}

string Messagehandler::readString(const Connection& conn) {
	if(conn.read()!=Protocol::PAR_STRING) throw InvalidProtocolException(); // read PAR_STRING byte
	// Read the four N bytes
	int n = helperRead(conn);

	string s;
	for (int i = 0; i < n; ++i) {
		s += conn.read();
	}
	return s;
}