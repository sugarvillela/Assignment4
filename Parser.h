/* David Swanson CS100 Fall 2016 */
#ifndef PARSER_H
#define PARSER_H
#include "RShell.h"
#include "UT.h"

class RShell;

class Parser{
	/*	Parser reads input string, populating a composite object
		with connector objects AND, OR and SemiColon. Connector objects
		are populated with Cmd, Tester, CD or more Composite objects 
	*/	
	public:
		/* Constructor needs dirs reference from main for CD object */
		Parser( string setStr, string setDirs[] ) : 
		  str( setStr ), dirs( setDirs ), errNo( 0 ) {}
		
		/* Populate is public interface; calls protected functions below */
		void populate( RShell* composite );
		int getErrNo();
		string getErrTxt();
	protected:
		string str;
		string* dirs;
		int errNo;		//integer error code
		
		/* Convert string to formatted vector. On heap, deleted by caller */
		vector< string >* strToV( string in );
		
		/* Something like a factory pattern: create connectors based on &|;*/
		RShell* makeConnector( char );
		
		/* Factory again: create Cmd, Tester, CD or composite, depending on
			bool functions looking for certain syntax.
			The bool functions also prepare data for the various constructors
		*/
		RShell* makeCmd( string );
		
		/* Find syntax and prep */
		bool cmdIsTest1( string &in, vector< string > &t1 );
		bool cmdIsTest2( string &in, vector< string > &t1 );
		bool cmdIsTest3( vector<string> &t1, char &testType, string &fileName );
		bool cmdIsCd1( vector< string > &t1 );
		bool cmdIsCd2( vector<string> &t1, string &fileName );
		bool cmdIsParenth( string &in );
};


#endif