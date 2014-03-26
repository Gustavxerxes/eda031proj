/*
 * FRONTEND for handling the special Protocol
 * 
 * 'Cause parsing is f-ing "yääääääy!"-fun
 */

#include <string>


#include "frontend.h"
#include "protocol.h"

using namespace std;
using LINE = vector<char>;

int readInt(const shared_ptr<Connection>& conn) {
	conn->read(); // read PAR_NUM byte
	unsigned char b1 = conn->read();
	unsigned char b2 = conn->read();
	unsigned char b3 = conn->read();
	unsigned char b4 = conn->read();
	return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

string readString(const shared_ptr<Connection>& conn) {
	conn->read(); // read PAR_STRING byte
	// Read the four N bytes
	unsigned char b1 = conn->read();
	unsigned char b2 = conn->read();
	unsigned char b3 = conn->read();
	unsigned char b4 = conn->read();
	int n = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;

	string s;
	for (int i = 0; i < n; ++i) {
		string += conn->read();
	}
	return s;
}

void writeInt(const shared_ptr<Connection>& conn, int value) {
	conn->write(Protocol::PAR_NUM);
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8)	 & 0xFF);
	conn.write(value & 0xFF);
}

void writeString(const shared_ptr<Connection>& conn, const string& s) {
	conn->write(Protocol::PAR_STRING);

	// Write N
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8)	 & 0xFF);
	conn.write(value & 0xFF);

	// Write chars
	for (auto it = s.begin(); it != s.end(); ++it) {
		conn->write(*it);
	}
}

void listNewsGroup(const shared_ptr<Connection>& conn) {
	conn->read(); // Read the COM_END byte
	vector<pair<int,string>> ngs;
	backend.listNG(ngs);

	// Answer
	conn->write(Protocol::ANS_LIST_NG);
	writeInt(conn, ngs.size());
	for (auto it = ngs.begin(); it != ngs.end(); ++it) {
		writeInt(it->first);
		writeString(it->second);
	}
}

void createNewsGroup(const shared_ptr<Connection>& conn) {
	string name = readString(conn);
	conn->read(); // read the COM_END byte
	bool succ = backend.addNG(name);

	// Answer
	conn->write(Protocol::ANS_CREATE_NG);
	if (succ) {
		conn->write(Protocol::ANS_ACK);
	} else {
		conn->write(Protocol::AND_NAK);
		conn->write(Protocol::ERR_NG_ALREADY_EXISTS);
	}
	conn->write(Protocol::ANS_END);
}

void deleteNewsGroup(const shared_ptr<Connection>& conn) {
	bool succ = backend.removeNG(readInt(conn));
	conn->read(); // read COM_END byte

	// Answer
	conn->write(Protocol::ANS_DELETE_NG);
	if (succ) {
		conn->write(Protocol::ANS_ACK);
	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void listArticles(const shared_ptr<Connection>& conn) {
	int ng_id = readInt(conn);
	conn->read(); // read COM_END byte
	vector<pair<int,string>> arts;
	bool succ = backend.listArticle(arts);

	// Answer
	conn->write(Protocol::ANS_LIST_ART);
	if (succ) {
		conn->write(Protocol::ANS_ACK);
		writeInt(conn, arts.size());
		for (auto it = arts.begin(); it != arts.end(); ++it) {
			writeInt(it->first);
			writeString(it->second);
		}
	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void createArticle(const shared_ptr<Connection>& conn) {
	bool succ = backend.addArticle(readInt(conn), readString(conn), readString(conn), readString(conn));
	conn->read(); // read COM_END byte

	// Answer
	conn->write(Protocol::ANS_CREATE_ART);
	if (succ) conn->write(Protocol::ANS_ACK);
	else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void deleteArticle(const shared_ptr<Connection>& conn) {
	int succ = backend.removeArticle(readInt(conn), readInt(conn));
	conn->read(); // read COM_END byte

	// Answer
	conn->write(Protocol::ANS_DELETE_ART);
	switch (conn)
	{
	case BackEnd::NO_ERR :
		conn->write(Protocol::ANS_ACK);
		break;
	case BackEnd::ERR_NG :
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
		break;
	case BackEnd::ERR_ART :
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
		break;
	default :
		break;
	}
	conn->write(Protocol::ANS_END);
}

void getArticle(const shared_ptr<Connection>& conn) {
	vector<string> article;
	auto succ = backend.getArticle(readInt(conn), readInt(conn), article);
	conn->read(); // read COM_END byte

	// Answer
	conn->write(Protocol::ANS_GET_ART);
	switch (succ)
	{
	case BackEnd::NO_ERR :
		conn->write(Protocol::ANS_ACK);
		writeString(article[0]);
		writeString(article[1]);
		writeString(article[2]);
		break;
	case BackEnd::ERR_NG :
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
		break;
	case BackEnd::ERR_ART :
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
		break;
	default :
		break;
	}
	conn->write(Protocol::ANS_END);
}

void FrontEnd::readAndReply(const shared_ptr<Connection>& conn) {
	unsigned char cmd = conn->read();
	switch (cmd)
	{
	case Protocol::COM_LIST_NG :
		listNewsGroup(conn);
		break;

	case Protocol::COM_CREATE_NG :
		createNewsGroup(conn);
		break;

	case Protocol::COM_DELETE_NG :
		deleteNewsGroup(conn);
		break;

	case Protocol::COM_LIST_ART :
		listArticles(conn);
		break;

	case Protocol::COM_CREATE_ART :
		createArticle(conn);
		break;

	case Protocol::COM_DELETE_ART :
		deleteArticle(conn);
		break;

	case Protocol::COM_GET_ART :
		getArticle(conn);
		break;
	default :
		break;
	}
}