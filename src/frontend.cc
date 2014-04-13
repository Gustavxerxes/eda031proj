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
	if (MH::readCode(conn) != P::COM_END) throw InvalidProtocolException();
	vector<pair<int,string>> ngs;
	backend->listNG(ngs);

	// Answer
	MessageHandler::writeCode(conn, P::ANS_LIST_NG);
	int str_len = ngs.size();
	MH::writeInt(conn, str_len);
	for (auto it = ngs.begin(); it != ngs.end(); ++it) {
		MH::writeInt(conn, it->first);
		MH::writeString(conn, it->second);
	}
		MessageHandler::writeCode(conn, P::ANS_END);
}

void FrontEnd::createNewsGroup(const shared_ptr<Connection>& conn) {
	string name = MH::readString(conn);
	if (MH::readCode(conn) != P::COM_END) throw InvalidProtocolException();
	bool succ = backend->addNG(name);

	// Answer
	MessageHandler::writeCode(conn, P::ANS_CREATE_NG);
	if (succ) {
		MessageHandler::writeCode(conn, P::ANS_ACK);
	} else {
		MessageHandler::writeCode(conn, P::ANS_NAK);
		MessageHandler::writeCode(conn, P::ERR_NG_ALREADY_EXISTS);
	}
	MessageHandler::writeCode(conn, P::ANS_END);
}

void FrontEnd::deleteNewsGroup(const shared_ptr<Connection>& conn) {
	bool succ = backend->removeNG(MH::readInt(conn));
	if (MH::readCode(conn) != P::COM_END) throw InvalidProtocolException();

	// Answer
	MessageHandler::writeCode(conn, P::ANS_DELETE_NG);
	if (succ) {
		MessageHandler::writeCode(conn, P::ANS_ACK);
	} else {
		MessageHandler::writeCode(conn, P::ANS_NAK);
		MessageHandler::writeCode(conn, P::ERR_NG_DOES_NOT_EXIST);
	}
	MessageHandler::writeCode(conn, P::ANS_END);
}

void FrontEnd::listArticles(const shared_ptr<Connection>& conn) {
	int ng_id = MH::readInt(conn);
	if (MH::readCode(conn) != P::COM_END) throw InvalidProtocolException();
	vector<pair<int,string>> arts;
	bool succ = backend->listArticles(ng_id, arts);

	// Answer
	MessageHandler::writeCode(conn, P::ANS_LIST_ART);
	if (succ) {
		MessageHandler::writeCode(conn, P::ANS_ACK);
		int str_len = arts.size();
		MH::writeInt(conn, str_len);
		for (auto it = arts.begin(); it != arts.end(); ++it) {
			MH::writeInt(conn, it->first);
			MH::writeString(conn, it->second);
		}
	} else {
		MessageHandler::writeCode(conn, P::ANS_NAK);
		MessageHandler::writeCode(conn, P::ERR_NG_DOES_NOT_EXIST);
	}
	MessageHandler::writeCode(conn, P::ANS_END);
}

void FrontEnd::createArticle(const shared_ptr<Connection>& conn) {
	int ng_id = MH::readInt(conn);
	string title = MH::readString(conn), author = MH::readString(conn), textbody = MH::readString(conn);
	bool succ = backend->addArticle(ng_id, title, author, textbody);
	if (MH::readCode(conn) != P::COM_END) throw InvalidProtocolException();

	// Answer
	MessageHandler::writeCode(conn, P::ANS_CREATE_ART);
	if (succ) MessageHandler::writeCode(conn, P::ANS_ACK);
	else {
		MessageHandler::writeCode(conn, P::ANS_NAK);
		MessageHandler::writeCode(conn, P::ERR_NG_DOES_NOT_EXIST);
	}
	MessageHandler::writeCode(conn, P::ANS_END);
}

void FrontEnd::deleteArticle(const shared_ptr<Connection>& conn) {
	int ng = MH::readInt(conn);
	int art = MH::readInt(conn);
	auto succ = backend->removeArticle(ng, art);
	if (MH::readCode(conn) != P::COM_END) throw InvalidProtocolException();

	// Answer
	MessageHandler::writeCode(conn, P::ANS_DELETE_ART);
	switch (succ)
	{
	case BackEnd::NO_ERR :
		MessageHandler::writeCode(conn, P::ANS_ACK);
		break;
	case BackEnd::ERR_NG :
		MessageHandler::writeCode(conn, P::ANS_NAK);
		MessageHandler::writeCode(conn, P::ERR_NG_DOES_NOT_EXIST);
		break;
	case BackEnd::ERR_ART :
		MessageHandler::writeCode(conn, P::ANS_NAK);
		MessageHandler::writeCode(conn, P::ERR_ART_DOES_NOT_EXIST);
		break;
	default :
		break;
	}
	MessageHandler::writeCode(conn, P::ANS_END);
}

void FrontEnd::getArticle(const shared_ptr<Connection>& conn) {
	vector<string> article;
	int ng = MH::readInt(conn);
	int art = MH::readInt(conn);
	auto succ = backend->getArticle(ng, art, article);
	if (MH::readCode(conn) != P::COM_END) throw InvalidProtocolException();

	// Answer
	MessageHandler::writeCode(conn, P::ANS_GET_ART);
	switch (succ)
	{
	case BackEnd::NO_ERR :
		MessageHandler::writeCode(conn, P::ANS_ACK);
		MH::writeString(conn, article[0]);
		MH::writeString(conn, article[1]);
		MH::writeString(conn, article[2]);
		break;
	case BackEnd::ERR_NG :
		MessageHandler::writeCode(conn, P::ANS_NAK);
		MessageHandler::writeCode(conn, P::ERR_NG_DOES_NOT_EXIST);
		break;
	case BackEnd::ERR_ART :
		MessageHandler::writeCode(conn, P::ANS_NAK);
		MessageHandler::writeCode(conn, P::ERR_ART_DOES_NOT_EXIST);
		break;
	default :
		break;
	}
	MessageHandler::writeCode(conn, P::ANS_END);
}

void FrontEnd::readAndReply(const shared_ptr<Connection>& conn) {
	unsigned char cmd = MH::readCode(conn);
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