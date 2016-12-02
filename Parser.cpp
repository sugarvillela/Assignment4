/* David Swanson CS100 Fall 2016 */

#include "Parser.h"
/* See header for general descriptions */

int Parser::getErrNo(){
	return errNo;
}
string Parser::getErrTxt(){
	/* Returns text for parsing errors */
	switch( errNo ){
		/* 0=no error; 1=empty string, not reported */
		case 2:
			return "Check syntax near '&& || ;'\n";
		default:
			return "";
	}
}
RShell* Parser::makeConnector( char symb ){
	if( symb=='&' ){
		return new AND();
	}
	if( symb=='|' ){
		return new OR();
	}
	if( symb==';' ){
		return new SemiColon();
	}
	/* This'll never happen */
	cout << "Parser::makeConnector: bad symbol caused NULL return" << endl;
	return NULL;
}
RShell* Parser::makeCmd( string in ){
	vector< string > t1;
	char test=' ';
	string file="";
	if( 
		( cmdIsTest1( in, t1 ) || cmdIsTest2( in, t1 ) ) &&
			cmdIsTest3( t1, test, file )
		){
		return new Tester( test, file );
	}
	else if( cmdIsCd1( t1 )  &&	cmdIsCd2( t1, file ) ){
		return new CD( file );
	}
	else if( cmdIsParenth( in ) ){
		return new Composite( in, dirs );
	}
	else{
		return new Cmd( in );
	}
}
bool Parser::cmdIsTest1( string &in, vector< string > &t1 ){
	/* Checks for test brackets; if so, tokenizes string to vector */
	string trimmed=UT::trm( in, '[', ']' );
	if( trimmed.length() != in.length() ){
		in=trimmed;
		UT::tok( ' ', '"', '"', trimmed, t1 );
		return true;
	}
	return false;
}
bool Parser::cmdIsTest2( string &in, vector< string > &t1 ){
	/* Tokenizes string to vector and checks for keyword 'test' */
	UT::tok( ' ', '"', '"', in, t1 );
	if( t1.size() && t1.at( 0 ) == "test" ){
		t1.erase( t1.begin() );
		return true;
	}
	return false;
}
bool Parser::cmdIsTest3( vector<string> &t1, char &testType, string &fileName ){
	/* Checks for args -e -f -d. Sets test type and filename */
	int n=t1.size();
	testType=' ';
	fileName="";
	for( int i = 0; i<n; i++ ){
		if( t1.at( i ) == "-e" ){
			testType='e';
		}
		else if( t1.at( i ) == "-f" ){
			testType='f';
		}
		else if( t1.at( i ) == "-d" ){
			testType='d';
		}
		else{
			fileName=UT::trm( t1.at( i ), '"', '"' );
		}
	}
	return testType && fileName.length();
}
bool Parser::cmdIsCd1( vector< string > &t1 ){
	/* t1 is tokenized string; check for keyword 'cd' */
	int n=t1.size();
	for( int i = 0; i<n; i++ ){
		if( t1.at( i ) == "cd" ){
			t1.erase( t1.begin() + i );
			return true;
		}
	}
	return false;
}
bool Parser::cmdIsCd2( vector<string> &t1, string &fileName ){
	/* Checks for args '-'' or filename */
	int n=t1.size();
	fileName="";
	for( int i = 0; i<n; i++ ){
		if( t1.at( i ) == "-" ){
			fileName=dirs[1];
		}
		else{
			fileName=UT::trm( t1.at( i ), '"', '"' );
		}
	}
	if( !fileName.length() ){
		fileName=dirs[2];
	}
	return fileName.length()>0;
}
bool Parser::cmdIsParenth( string &in ){
	/* Check for ( ) and delete them */
	int len=in.length();
	if( len>2 && in[0]=='(' && in[len-1]==')' ){
		in=in.substr( 1, in.length()-2 );
		return true;
	}
	return false;
}
vector< string >* Parser::strToV( string in ){
	/* Tokenizes input string on connector chars, ignoring 
		first occurrence of same char; trims whitespace.
		Creates vector* t1 which needs to be deleted
		For the input string: "cmd1 arg1 && cmd2;" Vector will hold:
			cmd1 arg1
			&
			cmd2
			;
		For: "a && (b || c)" Vector will hold:
			a
			&
			(b || c)
			;
	*/
	string pushMe="";
	vector< string >* t1=new vector< string >;
	char lastChar=' ';
	int n = in.length(), curr=0, stackLevel=0, i;
	/* Parse input string one char at a time */
	for( i = 0; i<n; i++ ){
		if( in[i]=='(' ){//parenthes beginning; ignore &|; chars
			stackLevel++;
		}
		else if( in[i]==')' ){//parenthes end; stop ignoring &|; chars
			stackLevel--;
		}
		else if( !stackLevel ){
			/* If here, not inside parentheses */
			if( lastChar==in[i] && ( in[i]=='&' || in[i]=='|' ) ){
				/* If here, it's the second &| char. So dump the 
					previous part of the string into the vector 
				*/
				pushMe=UT::trm( in.substr( curr, i-curr-1 ) );
				if( pushMe.length() ){//if nothing is there, don't push it
					t1->push_back( pushMe );
				}
				t1->push_back( in.substr( i, 1 ) );//push the current &| char
				curr=i+1;//set current so the next dump starts here
			}
			else if( in[i]==';' ){//semicolon splits on first occurrence
				pushMe=UT::trm( in.substr( curr, i-curr ) );
				if( pushMe.length() ){
					t1->push_back( pushMe );
				}
				t1->push_back( in.substr( i, 1 ) );//push the current ; char
				curr=i+1;//set current so the next dump starts here
			}		
		}
		lastChar=in[i];
	}
	/* On loop finish, dump the rest of the string into the vector */
	if( i-curr+1 ){
		pushMe=UT::trm( in.substr( curr, i-curr ) );
		if( pushMe.length() ){//Couple of steps to avoid pushing empty string
			t1->push_back( pushMe );
		}
	}
	/* ; is optional in bash, so add it if not there */
	if( t1->at( t1->size()-1 ) != ";" ){
		t1->push_back( ";" );
	}
	//UT::dispV( t1 );
	return t1;
}
void Parser::populate( RShell* composite ){
	/* First, parse the string and deal with parse errors */
	UT::trunc( '#', str );//Get rid of comments
	if( !str.length() ){
		errNo=1;//empty string, abort, no error message
		return;
	}
	vector< string >* t1=strToV( str );//tokenize string to vector
	int n = t1->size();
	if( !n || n%2 != 0 ){
		/* If here, user typed something like & & */
		errNo=2;//syntax error
		return;
	}
	/* Variables:
		symb, cmdTxt: current &|; and command
	*/
	string symb="", cmdTxt="";
	RShell* curr=NULL;
	RShell* prev=NULL;
	RShell* cmd=makeCmd( t1->at( 0 ) );//only used for first iteration
	/* t1 length is multiples of 2 */
	for( int i = 2; i<n; i+=2 ){
		symb=t1->at( i-1 );
		cmdTxt=t1->at( i );
		if( prev ){//subsequent iterations
			curr=makeConnector( symb[0] );
			curr->addCommand( prev );
			curr->addCommand( makeCmd( cmdTxt ) );
			prev=curr;
		}
		else {//first iteration
			prev=makeConnector( symb[0] );
			prev->addCommand( cmd );
			cmd=makeCmd( cmdTxt );
			prev->addCommand( cmd );
		}
	}
	if( prev ){
		/* final dump after loop */
		composite->addCommand( prev );
	}
	else{
		/* Here only if loop didn't run (single command) */
		prev=makeConnector( ';' );
		prev->addCommand( cmd );
		composite->addCommand( prev );
	}
	delete t1;
}
