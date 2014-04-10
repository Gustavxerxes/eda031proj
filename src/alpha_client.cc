// ------------------------------------------------------------------
//
//						Newsgroup client 
//
//			Written by Viktor, Martin and Niklas
//
//	
//
//
// ------------------------------------------------------------------

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

string fixline = line(60);

string space(int count){
	string s ="";
	int i;
	for(i= 0; i<count; ++i){
		s+=" ";
	}
	return s;
}

string fixspace = space(15);

string vertspace(int count){
	string s ="";
	int i;
	for(i= 0; i<count; ++i){
		s+="\n";
	}
	return s;
}

string fixvertspace = vertspace(100);

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
return conn;

}

void printwelcome(char* argv[]){
	cout << "\n" << fixline << endl;
	cout << fixspace << ".:Weclome to Alpha_client:." <<endl;
	cout << space(10) << "You are now connected to " << argv[1] << " on port " << argv[2] << endl;
	cout << fixline << "\n" << endl;
}

void printhelp(bool b){
	if(b){
	cout << fixvertspace << fixline << endl;
	cout << fixspace <<".:Help and commands:." << endl;
	cout << space(5) << "Navigate in list by the number in the list\n" 
	<< space(5) << "or use one of the following commands \n" << endl;
	
	cout << "(h) -Print this list of commands" << endl;
	cout << "(n) -List newsgroups" << endl;
	cout << "(c) -Create new newsgruop" << endl;
	cout << "(d) -Delite newsgroups" << endl;	
	cout << "(l) -List articles in curent newsgroup" << endl;
	cout << "(w) -Write new article" << endl;
	cout << "(r) -Remove article" << endl;
	cout << "(s) -Toggle constant printing off this helptext" << endl;
	cout << "(0) -Go back" << endl;
	cout << "(q) -Quit program" << endl;
	cout << fixline << "\n" << endl;
	}
	else{
		cout << fixvertspace << fixline << "\n" <<
		"(h) -print helptext with list of commands\n" << fixline << endl;
	}

}

void listNewsgroup(const Connection& conn, int& currentNewsgroup, string& currentTitel, int& currentArticle){
	currentArticle = -1;
	currentTitel = "";
	cout << "List of Newsgroup on the server:\n" << endl;
	conn.write(Protocol::COM_LIST_NG);
	conn.write(Protocol::COM_END);
	int numbOf;
	if(conn.read()!=Protocol::ANS_LIST_NG) throw InvalidProtocolException(); //check correctness
	numbOf = readInt(conn);
	if(numbOf == 0) cout << " No newsgroups exist yet" << endl;
	int index;
	bool exist = false;
	string titel;
	for(int i = 0; i < numbOf; ++i){
		index = readInt(conn);
		if(index == currentNewsgroup) cout << "[ ";
		cout << "(" << index << ")" << ": ";
		titel = readString(conn);
		cout << titel;
		if(index == currentNewsgroup){
			cout << " ] ";
			currentTitel = titel;
			exist = true;
		}
		cout << endl;
	}
	if(!exist) currentNewsgroup = -1; 
	if(conn.read() !=Protocol::ANS_END) throw InvalidProtocolException(); //check correctness
	cout << "\n(c) -Create new newsgruop" << endl;
	cout << fixline << endl;
	if(currentNewsgroup > 0) cout << "Newsgroup selected: " << currentNewsgroup << ", press enter to list articles or type a command from list" << endl;
	else if(numbOf > 0) cout << "Select Newsgroup with number or type a command from list" << endl;
	else cout << "List is empty" << endl;
}

void createNewsgroup(const Connection& conn){
	cout << "\nTitle for new Newsgroup?: " << endl;
	conn.write(Protocol::COM_CREATE_NG);
	string title;
	getline (cin,title);
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
	if(currentNewsgroup < 1){ 
		cout << "Newsgroup to list? ";
		cin >> currentNewsgroup;
		cin.ignore(10000,'\n');
		printhelp(true);
	}else if(currentArticle <0){
		currentArticle = 0;
		printhelp(true);

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
			cout << "Articels in newsgroup " << currentNewsgroup << ", " << currentTitel << ":\n" << endl;
			numbOf = readInt(conn);
			if(currentArticle < 1) {
			cout << "[ (0) -Go back ]\n" << endl;
			}else cout << "(0) -Go back\n" << endl;
			if(numbOf == 0){
				cout << " Newsgroup is empty..." << endl;
				currentArticle = -2;
			}
			int index;
			string titel;
			for(int i = 0; i < numbOf; ++i){
				index = readInt(conn);
				if(index == currentArticle) cout << "[ ";
				cout << "(" << index << ")" << ": ";
				titel = readString(conn);
				cout << titel;
				if(index == currentArticle){
					cout << " ]";
				}
				cout << endl;
			} 
			cout << "\n(w) -write new articles" << endl;
			cout << fixline << endl;
	
			if(numbOf > 0) cout << "Select number to read or type a command from list" << endl;
		}
		break;
		case Protocol::ANS_NAK: 
			if(conn.read() == Protocol::ERR_NG_DOES_NOT_EXIST) cout << "Newsgroup does not exist!" << endl;
			currentArticle = -2;
			currentNewsgroup = -1;
		break;
	} 
	if(conn.read() !=Protocol::ANS_END) throw InvalidProtocolException(); //check correctness
	
}

void helperDelite(const Connection& conn, int& currentNewsgroup){
	conn.write(Protocol::COM_DELETE_NG);
	writeInt(conn, currentNewsgroup); 
	conn.write(Protocol::COM_END);
	if(conn.read() !=Protocol::ANS_DELETE_NG) throw InvalidProtocolException(); //check correctness
	char ans = conn.read();
	switch(ans){
		case Protocol::ANS_NAK:
		 if(conn.read() == Protocol::ERR_NG_DOES_NOT_EXIST) cout << "Newsgroup does not exist!" << endl;
		 currentNewsgroup = -1;
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

void getArticle(const Connection& conn, int& currentNewsgroup, int& currentArticle){
	conn.write(Protocol::COM_GET_ART);
	writeInt(conn, currentNewsgroup);
	writeInt(conn, currentArticle);
	conn.write(Protocol::COM_END);
	if(conn.read() !=Protocol::ANS_GET_ART) throw InvalidProtocolException(); //check correctness
	char ans = conn.read();
	cout << fixvertspace << endl;
	switch(ans){
		case Protocol::ANS_ACK:
			cout << "Titel: " << readString(conn) << endl; 
			cout << "Auther: " << readString(conn) << "\n" << endl;
			cout << readString(conn) << endl;
		break;
		case Protocol::ANS_NAK:
			ans = conn.read();
			if(ans == Protocol::ERR_NG_DOES_NOT_EXIST){
				cout << "Newsgroup: " << currentNewsgroup << "does not exist" << endl;
				currentNewsgroup = -2;
			}
			if(ans == Protocol::ERR_ART_DOES_NOT_EXIST) {
				cout << "Articel " << currentArticle << "does not exist" << endl;
				currentArticle = -2;
			}
		break;
	}
	if(conn.read() !=Protocol::ANS_END) throw InvalidProtocolException(); //check correctness
	cout << "\npress enter to continue";
	cin.ignore(10000,'\n');
}

void helperRemove(const Connection& conn, int& currentNewsgroup, int& currentArticle){
	conn.write(Protocol::COM_DELETE_ART);
	writeInt(conn, currentNewsgroup); 
	writeInt(conn, currentArticle); 
	conn.write(Protocol::COM_END);
	if(conn.read() !=Protocol::ANS_DELETE_ART) throw InvalidProtocolException(); //check correctness
	char ans = conn.read();
	switch(ans){
		case Protocol::ANS_NAK:
		 	ans = conn.read();
			if(ans == Protocol::ERR_NG_DOES_NOT_EXIST) {
				cout << "Newsgroup: " << currentNewsgroup << "does not exist" << endl;
				currentNewsgroup=-1;
			}
			if(ans == Protocol::ERR_ART_DOES_NOT_EXIST) {
				cout << "Articel " << currentArticle << "does not exist" << endl;
				currentArticle=-1;
			}
			break;
		case Protocol::ANS_ACK: cout << "Articel removed!" << endl;
		currentArticle = 0;
		break;
	}
	if(conn.read() !=Protocol::ANS_END) throw InvalidProtocolException(); //check correctness
	cout << "\npress enter to continue";
	cin.ignore(10000,'\n');
}

void removeArt(const Connection& conn, int& currentNewsgroup, int& currentArticle){
	if(currentNewsgroup < 1){ 
		cout << "Newsgroup to remove Articel from? ";
		cin >> currentNewsgroup;
		cin.ignore(10000,'\n');
	}
	if(currentArticle < 1){ 
		cout << "Articel to remove?";
		cin >> currentArticle;
		cin.ignore(10000,'\n');
	}
	bool delite = true;
	char ans;
	while(delite){
		if(currentArticle > 0){
			cout << "Delite artcel ID: " << currentArticle << "?  (y)es / (n)o / (o)therID" << endl;
			cin >> ans;
			cin.ignore(10000,'\n');
			cin.clear();
			switch (ans){
				case 'y': helperRemove(conn, currentNewsgroup, currentArticle);
				return;
				case 'n': return;
				case 'o': break;
				default: return;
			}
		}
		cout << "ID to delite? ";
		cin >> currentArticle;
		if(currentArticle < 0) delite = false;
	}
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
	getline (cin,in);
	writeString(conn, in); 
	cout << "\nAuther: ";
	getline (cin,in);
	writeString(conn, in); 
	cout << "\nText (end input with blank newline): " << endl;
	getline (cin,in);
	string result = in;
	while (in.length() != 0)
	{
		getline(cin, in);
	    result = result + "\n" + in;
	}
	writeString(conn, result); 
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

int main(int argc, char* argv[]) {
	bool alwaysHelpText = true;
	int currentNewsgroup = -1;
 	int currentArticle = -1;
 	string currentTitel = "";
	string inCom;
	Connection conn = connect(argc, argv); //connetion skapas här
	printwelcome(argv);
	cout << "\npress enter to continue";
	cin.ignore(10000,'\n');
	printhelp(alwaysHelpText);
	listNewsgroup(conn, currentNewsgroup, currentTitel, currentArticle);
	while (getline(cin,inCom)) {
		try{
			switch (inCom[0]){
				case 'h': printhelp(true);
					continue;
				case 'c': 
					createNewsgroup(conn);
				case 'n': 
					printhelp(alwaysHelpText);
					listNewsgroup(conn, currentNewsgroup, currentTitel, currentArticle);
					continue;
				case 'd': 
					deliteNewsgroup(conn, currentNewsgroup);
					printhelp(alwaysHelpText);
					listNewsgroup(conn, currentNewsgroup, currentTitel, currentArticle);
					continue;
				case 'w': 
					cout << fixvertspace << endl;
					createArt(conn, currentNewsgroup);
					printhelp(alwaysHelpText);
					listArt(conn, currentNewsgroup, currentTitel, currentArticle);
					continue;
				case 'r': 
					removeArt(conn, currentNewsgroup, currentArticle);
				case 'l':
					currentArticle = -1;
					listArt(conn, currentNewsgroup, currentTitel, currentArticle);
					continue;
				case 's':
					alwaysHelpText = !alwaysHelpText;
					alwaysHelpText? cout << "help on" << endl : cout<< "help off" << endl;
					continue;
				case 'q': exit(0);
					break;
			}
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		} catch (InvalidProtocolException&) {
			cout << "Protocol failed try to reconnect" << endl;
			exit(1);
		}
		if(inCom != ""){
			try {
				if( currentArticle < 0 ) currentNewsgroup = stoi(inCom);
				else currentArticle = stoi(inCom);
			} catch (exception& e) {
				cout << "Not a valid command, try again" << endl;
				continue;
			}
		}
		printhelp(alwaysHelpText);
		if( (inCom == "") && (0 < currentNewsgroup) && (0 > currentArticle) && (-2 != currentArticle) ) listArt(conn, currentNewsgroup, currentTitel, currentArticle);
		else if(currentArticle < 1) listNewsgroup(conn, currentNewsgroup, currentTitel, currentArticle);
		else { 
			listArt(conn, currentNewsgroup, currentTitel, currentArticle);
			getArticle(conn, currentNewsgroup, currentArticle);
			printhelp(alwaysHelpText);
			listArt(conn, currentNewsgroup, currentTitel, currentArticle);
		}
	}
}