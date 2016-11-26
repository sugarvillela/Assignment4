/* David Swanson CS100 Fall 2016 */

#include "RShell.h"
#include "Parser.h"


Composite::Composite( string setStr, string dirs[] ){
	P=new Parser( setStr, dirs );
	connector=NULL;
	P->populate( this );
}
Composite::~Composite(void){
	if( P ){
		delete P;
	}
	if( connector ){
		delete connector;
	}
}
bool Composite::execute(){
	/* Returns false on parse error */
	if( P->getErrNo()>1 || !connector ){
		return false;
	}
	return connector->execute();
}
void Composite::addCommand( RShell* newConnector ){
	connector=newConnector;
}
string Composite::getErrTxt(){
	return P->getErrTxt();
}

Connector::~Connector(void){
	if( left ){
		delete left;
	}
	if( right ){
		delete right;
	}
}
void Connector::addCommand( RShell* newCommand ){
	if( left ){
		right=newCommand;
	}
	else{
		left=newCommand;
	}
}

bool AND::execute(){
	/* Follows 'short circuit rules for evaluation' 
		Any false quits loop and returns false
	*/
	//cout << "AND execute" << endl;
	bool result=false;
	if( left ){
		result=left->execute();
	}
	if( right && result ){
		result=right->execute();
	}
	//cout << "end AND execute" << endl;
	return result;
}
bool OR::execute(){
	/* Follows 'short circuit rules for evaluation' */
	//cout << "OR execute" << endl;
	bool result=false;
	if( left ){
		result=left->execute();
	}
	if( right && !result ){
		result=right->execute();
	}
	//cout << "end OR execute" << endl;
	return result;
}
bool SemiColon::execute(){
	/* Evaluates all */
	//cout << "SemiColon execute" << endl;
	bool result=true;
	if( left ){
		result&=left->execute();
	}
	if( right ){
		result&=right->execute();
	}
	//cout << "end SemiColon execute" << endl;
	return result;
}

Cmd::Cmd( string setCmd ) : cmd( setCmd ){}
void Cmd::addCommand( RShell* newCommand ){}
bool Cmd::execute(){
	//cout << "Cmd execute: cmd=" << cmd << endl;
	if( cmd=="exit" || cmd=="quit" ){
		exit( 0 );
	}
	int status=0;
	pid_t pid = fork();
	if (pid == -1){
		perror( "Could not start child process" );
	}
	//If here, the process has split in two, with parent and child both running this code.
	//Check id to see which is which
	else if (pid == 0){//This is the child process; run the child task
		vector< string > t1;
		UT::tok( ' ', cmd, t1 );
		char* const* argv=UT::toNullTermArray( t1 );
		//UT::printNullTermArray( argv );
		execvp( *argv, argv );//execvp lets you specify just the file name; execv wants the whole path
		UT::deleteNullTermArray( argv );
		_exit( 1 );
		// exec only returns on error; else when the called program finishes, the child is terminated
		//automatically and the called program's exit status shows up in the parent's waitpid
	}
	else {//This is the parent process; wait for child
		waitpid( pid, &status, 0 );
		if( status ){
			perror( "Command exited with status > 0" );
		}
	}
	return ( status == 0 );
}

Tester::Tester( char test, string file ){
	testType=test;
	fileName=file;
}

/* d, e, f execute based on testType, if any */
bool Tester::d() {
	const char *path=fileName.c_str();
    struct stat s;
	return !stat( path,&s ) && ( s.st_mode & S_IFDIR );
}
bool Tester::e() {
	const char *path=fileName.c_str();
    struct stat s;
	return !stat( path,&s );
}
bool Tester::f(){
	const char *path=fileName.c_str();
    struct stat s;
	return !stat( path,&s ) && ( s.st_mode & S_IFREG );
}
bool Tester::execute(){
	bool result=( testType=='d' && d() ) ||
				( testType=='e' && e() ) ||
				( testType=='f' && f() );
	if( result){
		cout << "(True)" << endl;
		return true;
	}
	else{
		cout << "(False)" << endl;
		return false;
	}
}
void Tester::addCommand( RShell* newConnector ){}


CD::CD( string file ) : fileName( file ) {}
bool CD::execute(){
	//cout << "execute cd: fileName="<< fileName << endl;
    if ( chdir( fileName.c_str() ) < 0) {
    	perror( "chdir fail");
		return false;
    }
	return true;
}
void CD::addCommand( RShell* newConnector ){}