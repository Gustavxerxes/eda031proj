/*
 * Class with static methods for low-level protocol handling
 *
 * Author: NH
 */

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <string>
#include <memory>

#include "connection.h"
#include "protocol.h"

class MessageHandler {
public: 
	static void writeInt(const std::shared_ptr<Connection>& conn, const int value);
	static void writeString(const std::shared_ptr<Connection>& conn, const std::string& s);
	static void writeCode(const std::shared_ptr<Connection>& conn, const unsigned char code);
	static int readInt(const std::shared_ptr<Connection>& conn);
	static std::string readString(const std::shared_ptr<Connection>& conn);
	static unsigned char readCode(const std::shared_ptr<Connection>& conn);

private:
};

#endif