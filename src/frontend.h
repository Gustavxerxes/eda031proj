#ifndef FRONTEND_H
#define FRONTEND_H

#include "connection.h"
#include "backend.h"

#include <memory>

class FrontEnd {
public:
	FrontEnd(BackEnd& be) : backend(be) {}
	void readAndReply(const std::shared_ptr<Connection>& conn);
private:
	BackEnd backend;

	void writeInt(const std::shared_ptr<Connection>& conn, int&	 value);
	void writeString(const std::shared_ptr<Connection>& conn, const std::string& s);
	void listNewsGroup(const std::shared_ptr<Connection>& conn);
	void createNewsGroup(const std::shared_ptr<Connection>& conn);
	void deleteNewsGroup(const std::shared_ptr<Connection>& conn);
	void listArticles(const std::shared_ptr<Connection>& conn);
	void createArticle(const std::shared_ptr<Connection>& conn);
	void deleteArticle(const std::shared_ptr<Connection>& conn);
	void getArticle(const std::shared_ptr<Connection>& conn);
	int readInt(const std::shared_ptr<Connection>& conn);
	std::string readString(const std::shared_ptr<Connection>& conn);

};

#endif