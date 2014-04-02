#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

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

void writeInt(const Connection& conn, int value) {
	conn.write(Protocol::PAR_NUM);
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8)	 & 0xFF);
	conn.write(value & 0xFF);
}

void writeString(const Connection& conn, const string& s) {
	conn.write(Protocol::PAR_STRING);

	int n = s.size();
	// Write N
	conn.write((n >> 24) & 0xFF);
	conn.write((n >> 16) & 0xFF);
	conn.write((n >> 8)	 & 0xFF);
	conn.write(n & 0xFF);

	// Write chars
	for (auto it = s.begin(); it != s.end(); ++it) {
		conn.write(*it);
	}
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
	cout << "\n" << line() << endl;
	cout << ".:Weclome to Alpha_client:." <<endl;
	cout << "You are now connected to " << argv[1] << " on port " << argv[2] << endl;
	cout << line() << "\n" << endl;
}

void printhelp(bool b){
	if(b){
	cout << "\n" << line() << endl;
	cout << ".:Help and commands:." << endl;
	cout << "Navigate in list by the number in the list\n" 
	<< "or use one of the following commands \n" << endl;
	
	cout << "h -This helptext and list of commands" << endl;
	cout << "H -Extended help text" << endl;
	cout << "n -List newsgroups" << endl;
	cout << "d -Delite newsgroups" << endl;	
	cout << "l -List articles in curent newsgroup" << endl;
	cout << "c -Create new article" << endl;
	cout << "r -Remove article" << endl;
	cout << "s -Toggle constant printing off this helptext" << endl;
	cout << "q -Quit program" << endl;
	cout << line() << "\n" << endl;
	}
	else{
		cout << line() << "\n" <<
		"h -print helptext and list of commands\n" << line() << endl;
	}

}
void listNewsgroup(const Connection& conn){

	cout << "0 -Create new newsgruop" << endl;
	//ska lista newsgruop och presentera
}

int main(int argc, char* argv[]) {
	bool alwaysHelpText = true;

	Connection conn = connect(argc,argv);
	printwelcome(argv);
	listNewsgroup(conn);
	printhelp(alwaysHelpText);

	string inCom;
	while (cin >> inCom) {
		switch (inCom[0]){
			case 'h': printhelp(true);
				continue;
			case 'H':
				continue;
			case 'n':
				listNewsgroup(conn);
				continue;
			case 'd':
				continue;
			case 'l':
				continue;
			case 'c':
				continue;
			case 'r':
				continue;
			case 's':
				alwaysHelpText = !alwaysHelpText;
				alwaysHelpText? cout << "help on" << endl : cout<< "help off" << endl;
				continue;
			case 'q': exit(0);
				break;			
		}
			
		int inComnbr = -1;
		try {
			inComnbr = stoi(inCom);
		} catch (exception& e) {
			cout << "Not a valid command, try again" << endl;
			continue;
		}

	printhelp(alwaysHelpText);

	}
}