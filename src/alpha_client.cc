#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include "invalidprotocolexception.h"

using namespace std;


string line(int count){
	string s ="";
	int i;
	for(i= 0; i<count; ++i){
		s+="-";
	}
	return s;
}

string line(){
	return line(40);
}

void helperWrite(const Connection& conn, int value){
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8)	 & 0xFF);
	conn.write(value & 0xFF);
}

void writeInt(const Connection& conn, int value) {
	conn.write(Protocol::PAR_NUM);
	helperWrite(conn,value);
}

void writeString(const Connection& conn, const string& s) {
	conn.write(Protocol::PAR_STRING);

	int n = s.size();
	// Write N
	helperWrite(conn,n);

	// Write chars
	for (auto it = s.begin(); it != s.end(); ++it) {
		conn.write(*it);
	}
}

int helperRead(const Connection& conn){
	unsigned char b1 = conn.read();
	unsigned char b2 = conn.read();
	unsigned char b3 = conn.read();
	unsigned char b4 = conn.read();
	return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

int readInt(const Connection& conn) {
	if(conn.read()!=Protocol::PAR_NUM) throw InvalidProtocolException(); // read PAR_NUM byte
	return helperRead(conn);
}

string readString(const Connection& conn) {
	if(conn.read()!=Protocol::PAR_STRING) throw InvalidProtocolException(); // read PAR_STRING byte
	// Read the four N bytes
	int n = helperRead(conn);

	string s;
	for (int i = 0; i < n; ++i) {
		s += conn.read();
	}
	return s;
}

Connection connect(int argc, char* argv[]){

	if (argc != 3) {
		cerr << "Usage:" << argv[0] <<" host-name port-number" << endl;
		exit(1);
	}
	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}
	if(conn.isConnected()) cout << "conneted!" << endl;
return conn;

}

void printwelcome(char* argv[]){
	cout << "\n" << line() << endl;
	cout << ".:Weclome to Alpha_client:." <<endl;
	cout << "You are now connected to " << argv[1] << " on port " << argv[2] << endl;
	cout << line() << "\n" << endl;
}

void printhelp(bool b){
	if(b){
	cout << "\n\n\n" << line() << endl;
	cout << ".:Help and commands:." << endl;
	cout << "Navigate in list by the number in the list\n" 
	<< "or use one of the following commands \n" << endl;
	
	cout << "(h) -This helptext and list of commands" << endl;
	cout << "(H) -Extended help text" << endl;
	cout << "(n) -List newsgroups" << endl;
	cout << "(d) -Delite newsgroups" << endl;	
	cout << "(l) -List articles in curent newsgroup" << endl;
	cout << "(c) -Create new article" << endl;
	cout << "(r) -Remove article" << endl;
	cout << "(s) -Toggle constant printing off this helptext" << endl;
	cout << "(q) -Quit program" << endl;
	cout << "(0) -Create new newsgruop" << endl;
	cout << line() << "\n" << endl;
	}
	else{
		cout << "\n\n\n"  << line() << "\n" <<
		"(h) -print helptext and list of commands\n" << line() << endl;
	}

}

void createNewsgroup(const Connection& conn){
	cout << "\nTitle for new Newsgroup?: " << endl;
	conn.write(Protocol::COM_CREATE_NG);
	string title;
	std::getline (std::cin,title);
	std::getline (std::cin,title);
	writeString(conn, title); 
	conn.write(Protocol::COM_END);
	if(conn.read() !=Protocol::ANS_CREATE_NG) throw InvalidProtocolException(); //check correctness
	char ans = conn.read();
	switch(ans){
		case Protocol::ANS_NAK:
		 cout << "ANS_NAK" << endl;
		 if(conn.read() == Protocol::ERR_NG_ALREADY_EXISTS) cout << "ERR_NG_ALREADY_EXISTS" << endl;
		break;
		case Protocol::ANS_ACK: cout << "ANS_ACK" << endl;
		break;
	}
	if(conn.read() !=Protocol::ANS_END) throw InvalidProtocolException(); //check correctness
}

void listArt(const Connection& conn, int currentNewsgroup){

}

void deliteNewsgroup(const Connection& conn){

}

void removeArt(const Connection& conn){

}

void createArt(const Connection& conn){

}

void listNewsgroup(const Connection& conn){
	cout << "List of Newsgroup on the server:" << endl;
	conn.write(Protocol::COM_LIST_NG);
	conn.write(Protocol::COM_END);
	int numbOf;
	if(conn.read()!=Protocol::ANS_LIST_NG) throw InvalidProtocolException(); //check correctness
	numbOf = readInt(conn);
	if(numbOf == 0) cout << "No newsgroups exist" << endl; 
	for(int i = 0; i < numbOf; ++i){
		cout << "(" << readInt(conn) << ")" << ": ";
		cout << readString(conn) << endl;
	}
	if(conn.read() !=Protocol::ANS_END) throw InvalidProtocolException(); //check correctness
	cout << "\n(0) -Create new newsgruop" << endl;
	cout << line() << endl;
}
void printExtednHelp(){
	printhelp(true);
}

int main(int argc, char* argv[]) {
	bool alwaysHelpText = true;
	bool skipOneH = false;
 	int currentNewsgroup = -1;
	string inCom;
	Connection conn = connect(argc, argv); //connetion skapas här
	
	printwelcome(argv);
	printhelp(alwaysHelpText);
	listNewsgroup(conn);
	while (cin >> inCom) {
		try{
			switch (inCom[0]){
				case 'h': printhelp(true);
					continue;
				case '0': createNewsgroup(conn);
					continue;
				case 'H': printExtednHelp();
					continue;
				case 'n': 
				printhelp(alwaysHelpText);
				listNewsgroup(conn);
					continue;
				case 'd': deliteNewsgroup(conn);
					continue;
				case 'l': 
					if(currentNewsgroup<1){
						printhelp(alwaysHelpText);
						cout << "You are not inside any newsgroup" << endl;
					}else{
						listArt(conn, currentNewsgroup);
					}
					continue;
				case 'c': createArt(conn);
					continue;
				case 'r': removeArt(conn);
					continue;
				case 's':
					alwaysHelpText = !alwaysHelpText;
					alwaysHelpText? cout << "help on" << endl : cout<< "help off" << endl;
					continue;
				case 'q': exit(0);
					break;
				case 't':
					printhelp(alwaysHelpText); 
					if(conn.isConnected()) cout << "connected" << endl;
					else cout << "not connected" << endl;
					continue;
			}
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		} catch (InvalidProtocolException&) {
			cout << "Protocol failed try to reconnect" << endl;
			exit(1);
		}


		// flytta detta ?
		try {
			currentNewsgroup = stoi(inCom);

		} catch (exception& e) {
			cout << "Not a valid command, try again" << endl;
			continue;
		}
		listArt(conn, currentNewsgroup);
		printhelp(alwaysHelpText);
	
	}
}