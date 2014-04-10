/*
 * FRONTEND for handling the special Protocol
 * 
 * Parsing FTW!!11
 */

#include <string>


#include "frontendsql.h"
#include "protocol.h"
#include "invalidprotocolexception.h"

using namespace std;
using LINE = vector<char>;

int FrontEnd::readInt(const shared_ptr<Connection>& conn) {
	if (conn->read() != Protocol::PAR_NUM) throw InvalidProtocolException();
	unsigned char b1 = conn->read();
	unsigned char b2 = conn->read();
	unsigned char b3 = conn->read();
	unsigned char b4 = conn->read();
	return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

string FrontEnd::readString(const shared_ptr<Connection>& conn) {
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

void FrontEnd::writeInt(const shared_ptr<Connection>& conn, int& value) {
	conn->write(Protocol::PAR_NUM);
	conn->write((value >> 24) & 0xFF);
	conn->write((value >> 16) & 0xFF);
	conn->write((value >> 8) & 0xFF);
	conn->write(value & 0xFF);
}

void FrontEnd::writeString(const shared_ptr<Connection>& conn, const string& s) {
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

void FrontEnd::listNewsGroup(const shared_ptr<Connection>& conn) {
	if (conn->read() != Protocol::COM_END) throw InvalidProtocolException();
	vector<pair<int,string>> ngs;
	backend.listNG(ngs);

	// Answer
	conn->write(Protocol::ANS_LIST_NG);
	int str_len = ngs.size();
	writeInt(conn, str_len);
	for (auto it = ngs.begin(); it != ngs.end(); ++it) {
		writeInt(conn, it->first);
		writeString(conn, it->second);
	}
		conn->write(Protocol::ANS_END);
}

void FrontEnd::createNewsGroup(const shared_ptr<Connection>& conn) {
	string name = readString(conn);
	if (conn->read() != Protocol::COM_END) throw InvalidProtocolException();
	bool succ = backend.addNG(name);

	// Answer
	conn->write(Protocol::ANS_CREATE_NG);
	if (succ) {
		conn->write(Protocol::ANS_ACK);
	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_ALREADY_EXISTS);
	}
	conn->write(Protocol::ANS_END);
}

void FrontEnd::deleteNewsGroup(const shared_ptr<Connection>& conn) {
	bool succ = backend.removeNG(readInt(conn));
	if (conn->read() != Protocol::COM_END) throw InvalidProtocolException();

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

void FrontEnd::listArticles(const shared_ptr<Connection>& conn) {
	int ng_id = readInt(conn);
	if (conn->read() != Protocol::COM_END) throw InvalidProtocolException();
	vector<pair<int,string>> arts;
	bool succ = backend.listArticles(ng_id, arts);

	// Answer
	conn->write(Protocol::ANS_LIST_ART);
	if (succ) {
		conn->write(Protocol::ANS_ACK);
		int str_len = arts.size();
		writeInt(conn, str_len);
		for (auto it = arts.begin(); it != arts.end(); ++it) {
			writeInt(conn, it->first);
			writeString(conn, it->second);
		}
	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void FrontEnd::createArticle(const shared_ptr<Connection>& conn) {
	int ng_id = readInt(conn);
	string title = readString(conn), author = readString(conn), textbody = readString(conn);
	bool succ = backend.addArticle(ng_id, title, author, textbody);
	if (conn->read() != Protocol::COM_END) throw InvalidProtocolException();

	// Answer
	conn->write(Protocol::ANS_CREATE_ART);
	if (succ) conn->write(Protocol::ANS_ACK);
	else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void FrontEnd::deleteArticle(const shared_ptr<Connection>& conn) {
	int ng = readInt(conn);
	int art = readInt(conn);
	auto succ = backend.removeArticle(ng, art);
	if (conn->read() != Protocol::COM_END) throw InvalidProtocolException();

	// Answer
	conn->write(Protocol::ANS_DELETE_ART);
	switch (succ)
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

void FrontEnd::getArticle(const shared_ptr<Connection>& conn) {
	vector<string> article;
	int ng = readInt(conn);
	int art = readInt(conn);
	auto succ = backend.getArticle(ng, art, article);
	if (conn->read() != Protocol::COM_END) throw InvalidProtocolException();

	// Answer
	conn->write(Protocol::ANS_GET_ART);
	switch (succ)
	{
	case BackEnd::NO_ERR :
		conn->write(Protocol::ANS_ACK);
		writeString(conn, article[0]);
		writeString(conn, article[1]);
		writeString(conn, article[2]);
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
		throw InvalidProtocolException();
		break;
	}
}