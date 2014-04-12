/*
 * FRONTEND for handling the special Protocol
 * 
 * Parsing FTW!!11
 */

#include <string>


#include "frontend.h"
#include "protocol.h"
#include "invalidprotocolexception.h"
#include "messagehandler.h"

using namespace std;
using MH = MessageHandler;
using P = Protocol;

void FrontEnd::listNewsGroup(const shared_ptr<Connection>& conn) {
	if (MH::readByte(conn) != P::COM_END) throw InvalidProtocolException();
	vector<pair<int,string>> ngs;
	backend->listNG(ngs);

	// Answer
	conn->write(P::ANS_LIST_NG);
	int str_len = ngs.size();
	writeInt(conn, str_len);
	for (auto it = ngs.begin(); it != ngs.end(); ++it) {
		writeInt(conn, it->first);
		writeString(conn, it->second);
	}
		conn->write(P::ANS_END);
}

void FrontEnd::createNewsGroup(const shared_ptr<Connection>& conn) {
	string name = readString(conn);
	if (MH::readByte(conn) != P::COM_END) throw InvalidProtocolException();
	bool succ = backend->addNG(name);

	// Answer
	conn->write(P::ANS_CREATE_NG);
	if (succ) {
		conn->write(P::ANS_ACK);
	} else {
		conn->write(P::ANS_NAK);
		conn->write(P::ERR_NG_ALREADY_EXISTS);
	}
	conn->write(P::ANS_END);
}

void FrontEnd::deleteNewsGroup(const shared_ptr<Connection>& conn) {
	bool succ = backend->removeNG(readInt(conn));
	if (MH::readByte(conn) != P::COM_END) throw InvalidProtocolException();

	// Answer
	conn->write(P::ANS_DELETE_NG);
	if (succ) {
		conn->write(P::ANS_ACK);
	} else {
		conn->write(P::ANS_NAK);
		conn->write(P::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(P::ANS_END);
}

void FrontEnd::listArticles(const shared_ptr<Connection>& conn) {
	int ng_id = readInt(conn);
	if (MH::readByte(conn) != P::COM_END) throw InvalidProtocolException();
	vector<pair<int,string>> arts;
	bool succ = backend->listArticles(ng_id, arts);

	// Answer
	conn->write(P::ANS_LIST_ART);
	if (succ) {
		conn->write(P::ANS_ACK);
		int str_len = arts.size();
		writeInt(conn, str_len);
		for (auto it = arts.begin(); it != arts.end(); ++it) {
			writeInt(conn, it->first);
			writeString(conn, it->second);
		}
	} else {
		conn->write(P::ANS_NAK);
		conn->write(P::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(P::ANS_END);
}

void FrontEnd::createArticle(const shared_ptr<Connection>& conn) {
	int ng_id = readInt(conn);
	string title = readString(conn), author = readString(conn), textbody = readString(conn);
	bool succ = backend->addArticle(ng_id, title, author, textbody);
	if (MH::readByte(conn) != P::COM_END) throw InvalidProtocolException();

	// Answer
	conn->write(P::ANS_CREATE_ART);
	if (succ) conn->write(P::ANS_ACK);
	else {
		conn->write(P::ANS_NAK);
		conn->write(P::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(P::ANS_END);
}

void FrontEnd::deleteArticle(const shared_ptr<Connection>& conn) {
	int ng = readInt(conn);
	int art = readInt(conn);
	auto succ = backend->removeArticle(ng, art);
	if (MH::readByte(conn) != P::COM_END) throw InvalidProtocolException();

	// Answer
	conn->write(P::ANS_DELETE_ART);
	switch (succ)
	{
	case BackEnd::NO_ERR :
		conn->write(P::ANS_ACK);
		break;
	case BackEnd::ERR_NG :
		conn->write(P::ANS_NAK);
		conn->write(P::ERR_NG_DOES_NOT_EXIST);
		break;
	case BackEnd::ERR_ART :
		conn->write(P::ANS_NAK);
		conn->write(P::ERR_ART_DOES_NOT_EXIST);
		break;
	default :
		break;
	}
	conn->write(P::ANS_END);
}

void FrontEnd::getArticle(const shared_ptr<Connection>& conn) {
	vector<string> article;
	int ng = readInt(conn);
	int art = readInt(conn);
	auto succ = backend->getArticle(ng, art, article);
	if (MH::readByte(conn) != P::COM_END) throw InvalidProtocolException();

	// Answer
	conn->write(P::ANS_GET_ART);
	switch (succ)
	{
	case BackEnd::NO_ERR :
		conn->write(P::ANS_ACK);
		writeString(conn, article[0]);
		writeString(conn, article[1]);
		writeString(conn, article[2]);
		break;
	case BackEnd::ERR_NG :
		conn->write(P::ANS_NAK);
		conn->write(P::ERR_NG_DOES_NOT_EXIST);
		break;
	case BackEnd::ERR_ART :
		conn->write(P::ANS_NAK);
		conn->write(P::ERR_ART_DOES_NOT_EXIST);
		break;
	default :
		break;
	}
	conn->write(P::ANS_END);
}

void FrontEnd::readAndReply(const shared_ptr<Connection>& conn) {
	unsigned char cmd = MH::readByte(conn);
	switch (cmd)
	{
	case P::COM_LIST_NG :
		listNewsGroup(conn);
		break;

	case P::COM_CREATE_NG :
		createNewsGroup(conn);
		break;

	case P::COM_DELETE_NG :
		deleteNewsGroup(conn);
		break;

	case P::COM_LIST_ART :
		listArticles(conn);
		break;

	case P::COM_CREATE_ART :
		createArticle(conn);
		break;

	case P::COM_DELETE_ART :
		deleteArticle(conn);
		break;

	case P::COM_GET_ART :
		getArticle(conn);
		break;
	default :
		throw InvalidProtocolException();
		break;
	}
}