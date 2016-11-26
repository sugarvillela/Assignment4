/* David Swanson CS100 Fall 2016 */

#include<iostream>
#include <string>
using namespace std;
#include "Parser.h"
#include "RShell.h"

string prompt( string cwd ){
	/* Output bash-style prompt, with current directory */
	cout << cwd << " $ ";
	string str;
	getline(cin, str);
	return str;
}

int main( int argc, char* argv[] ){
	/* Dirs is array length 3:
		0=current working directory
		1=previous directory
		2=home directory
	*/
	string dirs[]={ "", "", UT::getHomeDir() };
	UT::updateDirs( dirs );
	
	/* Accept command line args or console input:
		First check for args
	*/
	if ( argc > 1 ){
		Composite* C=new Composite(  UT::cmdLineToStr( argc, argv ), dirs );
		if( !C->execute() ){
			cout << C->getErrTxt();
		}
		delete C;
	}
	else{
		do{
			/* Get user input and send it to new Composite */
			Composite* C=new Composite( prompt( dirs[0] ), dirs );
			if( !C->execute() ){
				cout << C->getErrTxt();
			}
			delete C;
			UT::updateDirs( dirs );//see if cd was used
		}
		while( true );//let exit command call exit( 0 );
	}
	return 0;
}

