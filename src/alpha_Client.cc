#include "connection.h"
#include "connectionclosedexception.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

Connection conn;

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

void connect(int argc, char* argv[]){
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
	
	conn =  Connection(argv[1], port);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}
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
void listNewsgroup(){
	cout << "0 -Create new newsgruop" << endl;
	//ska lista newsgruop och presentera
}

int main(int argc, char* argv[]) {
	bool alwaysHelpText = true;

	connect(argc,argv);
	printwelcome(argv);
	listNewsgroup();
	printhelp(alwaysHelpText);

	string inCom;
	while (cin >> inCom) {
		switch (inCom[0]){
			case 'h': printhelp(true);
				continue;
			case 'H':
				continue;
			case 'n':
				listNewsgroup();
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