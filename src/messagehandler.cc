#include <string>
#include <memory>

#include "messagehandler.h"
#include "connection.h"
#include "protocol.h"
#include "invalidprotocolexception.h"


int helperRead(const std::shared_ptr<Connection>& conn){
	unsigned char b1 = conn->read();
	unsigned char b2 = conn->read();
	unsigned char b3 = conn->read();
	unsigned char b4 = conn->read();
	return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

void helperWrite(const std::shared_ptr<Connection>& conn, const int value){
	conn->write((value >> 24) & 0xFF);
	conn->write((value >> 16) & 0xFF);
	conn->write((value >> 8)	 & 0xFF);
	conn->write(value & 0xFF);
}

void MessageHandler::writeInt(const std::shared_ptr<Connection>& conn, const int value) {
	conn->write(Protocol::PAR_NUM);
	helperWrite(conn, value);
	/*
	conn->write((value >> 24) & 0xFF);
	conn->write((value >> 16) & 0xFF);
	conn->write((value >> 8) & 0xFF);
	conn->write(value & 0xFF);
	*/
}

void MessageHandler::writeString(const std::shared_ptr<Connection>& conn, const std::string& s) {
	conn->write(Protocol::PAR_STRING);

	int n = s.size();
	// Write N
	/*
	conn->write((n >> 24) & 0xFF);
	conn->write((n >> 16) & 0xFF);
	conn->write((n >> 8) & 0xFF);
	conn->write(n & 0xFF);
	*/
	helperWrite(conn,n);


	// Write chars
	for (auto it = s.begin(); it != s.end(); ++it) {
		conn->write(*it);
	}
}

void MessageHandler::writeCode(const std::shared_ptr<Connection>& conn, const unsigned char byte) {
	conn->write(byte);
}

int MessageHandler::readInt(const std::shared_ptr<Connection>& conn) {
	if (conn->read() != Protocol::PAR_NUM) throw InvalidProtocolException();
	/*
	unsigned char b1 = conn->read();
	unsigned char b2 = conn->read();
	unsigned char b3 = conn->read();
	unsigned char b4 = conn->read();
	return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
	*/
	return helperRead(conn);
}

std::string MessageHandler::readString(const std::shared_ptr<Connection>& conn) {
	if (conn->read() != Protocol::PAR_STRING) throw InvalidProtocolException();
	// Read the four N bytes
	/*
	unsigned char b1 = conn->read();
	unsigned char b2 = conn->read();
	unsigned char b3 = conn->read();
	unsigned char b4 = conn->read();
	int n = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
*/
	int n = helperRead(conn);
	std::string s;
	for (int i = 0; i < n; ++i) {
		s += conn->read();
	}
	return s;
}

unsigned char MessageHandler::readCode(const std::shared_ptr<Connection>& conn) {
	return conn->read();
}

