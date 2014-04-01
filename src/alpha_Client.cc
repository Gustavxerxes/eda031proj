#include "connection.h"
#include "connectionclosedexception.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

string line(int i){
	return string("-----------");
	 
}

void connect(){
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
}
void printwelcome(){
	cout << "Weclome to " << argv[0] <<endl();
	cout << "You are now connected to " << argv[1] << " on port " << argv[2] << endl();


}
void printhelp(bool b){
	if(b){
	cout << <<"\n" << line(10) << endl();
	cout << "Help and commands" << endl();
	cout << line(10) << endl();
	cout << "Navigate in list by the number in the list\n 
	or use one of the following commands \n" << endl();
	
	cout << "h -This helptext and list of commands" << endl();
	cout << "H -Extended help text" << endl();
	cout << "n -List newsgroups" << endl();
	cout << "d -Delite newsgroups" << endl();	
	cout << "l -List articles in curent newsgroup" << endl();
	cout << "c -Create new article" << endl();
	cout << "r -Remove article" << endl();
	cout << "r -Remove article" << endl();
	cout << "s -Toggle printing off this helptext" << endl();
	cout << "q -Quit program" << endl();
	cout << line(10) << "\n" << endl();
	}
	else{
		cout << "h -print helptext and list of commands" << endl();
	}

}
void listNewsgroup(){
	cout << "0  -Create new newsgruop" << endl();
	//ska lista newsgruop och presentera
}

int main(int argc, char* argv[]) {
	bool alwaysHelpText = true;

	connect();
	printwelcome();
	listNewsgroup();
	printhelp(alwaysHelpText);
}