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
string vertspce(int count){
	string s ="";
	int i;
	for(i= 0; i<count; ++i){
		s+="\n";
	}
	return s;
}

string vertspce(){
	return vertspce(100);
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
	cout << vertspce() << line() << endl;
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
		cout << vertspce() << line() << "\n" <<
		"(h) -print helptext and list of commands\n" << line() << endl;
	}

}

void listNewsgroup(const Connection& conn, const int currentNewsgroup, string& currentTitel, int& currentArticle){
	currentArticle = -1;
	currentTitel = "";
	cout << "List of Newsgroup on the server:\n" << endl;
	conn.write(Protocol::COM_LIST_NG);
	conn.write(Protocol::COM_END);
	int numbOf;
	if(conn.read()!=Protocol::ANS_LIST_NG) throw InvalidProtocolException(); //check correctness
	numbOf = readInt(conn);
	if(numbOf == 0) cout << "No newsgroups exist" << endl; 
	int index;
	string titel;
	for(int i = 0; i < numbOf; ++i){
		index = readInt(conn);
		if(index == currentNewsgroup) cout << "[";
		cout << "(" << index << ")" << ": ";
		titel = readString(conn);
		cout << titel;
		if(index == currentNewsgroup){
			cout << "]";
			currentTitel = titel;
		}
		cout << endl;
	}
	if(conn.read() !=Protocol::ANS_END) throw InvalidProtocolException(); //check correctness
	cout << "\n(0) -Create new newsgruop" << endl;
	cout << line() << endl;
	if(currentNewsgroup > 0) cout << "Newsgroup selected: " << currentNewsgroup << endl;
}

void createNewsgroup(const Connection& conn){
	cout << "\nTitle for new Newsgroup?: " << endl;
	conn.write(Protocol::COM_CREATE_NG);
	string title;
	std::getline (std::cin,title);
	writeString(conn, title); 
	conn.write(Protocol::COM_END);
	if(conn.read() !=Protocol::ANS_CREATE_NG) throw InvalidProtocolException(); //check correctness
	char ans = conn.read();
	switch(ans){
		case Protocol::ANS_NAK:
		 if(conn.read() == Protocol::ERR_NG_ALREADY_EXISTS) cout << "Newsgroup does already exist!" << endl;
		break;
		case Protocol::ANS_ACK: cout << "The new newsgruop... " << title << " ...is now created" << endl;
		break;
	}
	if(conn.read() !=Protocol::ANS_END) throw InvalidProtocolException(); //check correctness
	cout << "\npress enter to continue";
	cin.ignore(10000,'\n');
}

void listArt(const Connection& conn, int& currentNewsgroup, string& currentTitel, int& currentArticle){
	if(currentArticle < 0) currentArticle = 0;
	if(currentNewsgroup < 1){ 
		cout << "Newsgroup to list? ";
		cin >> currentNewsgroup;
		cin.ignore(10000,'\n');
	}
	conn.write(Protocol::COM_LIST_ART);
	writeInt(conn, currentNewsgroup); 
	conn.write(Protocol::COM_END);
	int numbOf;
	if(conn.read()!=Protocol::ANS_LIST_ART) throw InvalidProtocolException(); //check correctness
	char ans = conn.read();
	switch (ans){
		case Protocol::ANS_ACK:
		{
			printhelp(true);
			numbOf = readInt(conn);
			if(numbOf == 0){
				cout << "No articles exist" << endl;
				currentArticle = -1;
			}
			int index;
			for(int i = 0; i < numbOf; ++i){
				index = readInt(conn);
				cout << "(" << index << ")" << ": ";
				cout << readString(conn);
				cout << endl;
			}
			if(numbOf > 0) cout << "\n type n to list newsgroups again" << endl;
		}
		break;
		case Protocol::ANS_NAK: 
			if(conn.read() == Protocol::ERR_NG_DOES_NOT_EXIST) cout << "Newsgroup does not exist!" << endl;
			currentArticle = -1;
		break;
	} 
	if(conn.read() !=Protocol::ANS_END) throw InvalidProtocolException(); //check correctness
	if(currentArticle < 0) {
		cout << "\npress enter to continue";
		cin.ignore(10000,'\n');
		printhelp(true);
		listNewsgroup(conn, currentNewsgroup, currentTitel, currentArticle);			
	}
}

void helperDelite(const Connection& conn, const int currentNewsgroup){
	conn.write(Protocol::COM_DELETE_NG);
	writeInt(conn, currentNewsgroup); 
	conn.write(Protocol::COM_END);
	if(conn.read() !=Protocol::ANS_DELETE_NG) throw InvalidProtocolException(); //check correctness
	char ans = conn.read();
	switch(ans){
		case Protocol::ANS_NAK:
		 if(conn.read() == Protocol::ERR_NG_DOES_NOT_EXIST) cout << "Newsgroup does not exist!" << endl;
		break;
		case Protocol::ANS_ACK: cout << "Newsgroup delited!" << endl;
		break;
	}
	if(conn.read() !=Protocol::ANS_END) throw InvalidProtocolException(); //check correctness
	cout << "\npress enter to continue";
	cin.ignore(10000,'\n');
}

void deliteNewsgroup(const Connection& conn, int& currentNewsgroup){
	bool delite = true;
	char ans;
	while(delite){
		if(currentNewsgroup > 0){
			cout << "Delite newsgroup ID: " << currentNewsgroup <<"?  (y)es / (n)o / (o)therID" << endl;
			cin >> ans;
			cin.ignore(10000,'\n');
			cin.clear();
			switch (ans){
				case 'y': helperDelite(conn, currentNewsgroup);
				currentNewsgroup = -1;
				return;
				case 'n': return;
				case 'o': break;
				default: return;
			}
		}
		cout << "ID to delite? ";
		cin >> currentNewsgroup;
		if(currentNewsgroup < 0) delite = false;
	}
}
void getArticle(const Connection& conn, const int currentNewsgroup, const int currentArticle){

}

void removeArt(const Connection& conn, const int currentNewsgroup, const string& currentTitel, const int currentArticle){

}

void createArt(const Connection& conn, int& currentNewsgroup){
	if(currentNewsgroup < 1){ 
		cout << "Newsgroup to creat Articel in? ";
		cin >> currentNewsgroup;
		cin.ignore(10000,'\n');
	}	
	conn.write(Protocol::Protocol::COM_CREATE_ART);
	writeInt(conn, currentNewsgroup);
	cout << "\nTitle for new article: ";
	string in;
	std::getline (std::cin,in);
	writeString(conn, in); 
	cout << "\nAuther: ";
	std::getline (std::cin,in);
	writeString(conn, in); 
	cout << "\nText: " << endl;
	std::getline (std::cin,in);
	writeString(conn, in); 
	conn.write(Protocol::COM_END);
	if(conn.read() !=Protocol::ANS_CREATE_ART) throw InvalidProtocolException(); //check correctness
	char ans = conn.read();
	switch(ans){
		case Protocol::ANS_NAK:
		  if(conn.read() == Protocol::ERR_NG_DOES_NOT_EXIST) cout << "Newsgroup does not exist!" << endl;
		break;
		case Protocol::ANS_ACK: cout << "Articel created" << endl;
		break;
	}
	if(conn.read() !=Protocol::ANS_END) throw InvalidProtocolException(); //check correctness
	cout << "\npress enter to continue";
	cin.ignore(10000,'\n');
}

void printExtednHelp(){
	printhelp(true);
}

int main(int argc, char* argv[]) {
	bool alwaysHelpText = true;
	bool skipOneH = false;
 	int currentNewsgroup = -1;
 	int currentArticle = -1;
 	string currentTitel = "";
	string inCom;
	Connection conn = connect(argc, argv); //connetion skapas här
	printwelcome(argv);
	printhelp(alwaysHelpText);
	listNewsgroup(conn, currentNewsgroup, currentTitel, currentArticle);
	while (cin >> inCom) {
		cin.ignore(10000,'\n');
		cin.clear();
		try{
			switch (inCom[0]){
				case 'h': printhelp(true);
					continue;
				case '0': 
					createNewsgroup(conn);
					printhelp(alwaysHelpText);
					listNewsgroup(conn, currentNewsgroup, currentTitel, currentArticle);
					continue;
				case 'H': printExtednHelp();
					continue;
				case 'n': 
				printhelp(alwaysHelpText);
				listNewsgroup(conn, currentNewsgroup, currentTitel, currentArticle);
					continue;
				case 'd': deliteNewsgroup(conn, currentNewsgroup);
					printhelp(alwaysHelpText);
					listNewsgroup(conn, currentNewsgroup, currentTitel, currentArticle);
					continue;
				case 'l': 
					listArt(conn, currentNewsgroup, currentTitel, currentArticle);	
					continue;
				case 'c': 
					listArt(conn, currentNewsgroup, currentTitel, currentArticle);
					createArt(conn, currentNewsgroup);
					continue;
				case 'r': removeArt(conn, currentNewsgroup, currentTitel, currentArticle);
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
		try {
			if(currentArticle<0) currentNewsgroup = stoi(inCom);
			else currentArticle = stoi(inCom);

		} catch (exception& e) {
			cout << "Not a valid command, try again" << endl;
			continue;
		}
		printhelp(alwaysHelpText);
		if(currentArticle < 1) listNewsgroup(conn, currentNewsgroup, currentTitel, currentArticle);
		else { 
			listArt(conn, currentNewsgroup, currentTitel, currentArticle);
			getArticle(conn, currentNewsgroup, currentArticle);
		}

	}
}