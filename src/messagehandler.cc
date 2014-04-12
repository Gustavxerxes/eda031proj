#include <string>
#include <memory>

#include "messagehandler.h"
#include "connection.h"
#include "protocol.h"
#include "invalidprotocolexception.h"

void MessageHandler::writeInt(const shared_ptr<Connection>& conn, const int value) {
	conn->write(Protocol::PAR_NUM);
	conn->write((value >> 24) & 0xFF);
	conn->write((value >> 16) & 0xFF);
	conn->write((value >> 8) & 0xFF);
	conn->write(value & 0xFF);
}

void MessageHandler::writeString(const shared_ptr<Connection>& conn, const string& s) {
	conn->write(Protocol::PAR_STRING);

	int n = s.size();
	// Write N
	conn->write((n >> 24) & 0xFF);
	conn->write((n >> 16) & 0xFF);
	conn->write((n >> 8) & 0xFF);
	conn->write(n & 0xFF);

	// Write chars
	for (auto it = s.begin(); it != s.end(); ++it) {
		conn->write(*it);
	}
}

void MessageHandler::writeByte(const shared_ptr<Connection>& conn, const unsigned char byte) {
	conn->write(byte);
}

int MessageHandler::readInt(const shared_ptr<Connection>& conn) {
	if (conn->read() != Protocol::PAR_NUM) throw InvalidProtocolException();
	unsigned char b1 = conn->read();
	unsigned char b2 = conn->read();
	unsigned char b3 = conn->read();
	unsigned char b4 = conn->read();
	return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

string MessageHandler::readString(const shared_ptr<Connection>& conn) {
	if (conn->read() != Protocol::PAR_STRING) throw InvalidProtocolException();
	// Read the four N bytes
	unsigned char b1 = conn->read();
	unsigned char b2 = conn->read();
	unsigned char b3 = conn->read();
	unsigned char b4 = conn->read();
	int n = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;

	string s;
	for (int i = 0; i < n; ++i) {
		s += conn->read();
	}
	return s;
}

unsigned char MessageHandler::readByte(const shared_ptr<Connection>& conn) {
	return conn->read();
}
