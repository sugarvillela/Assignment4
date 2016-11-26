/* David Swanson CS100 Fall 2016 */
#ifndef UT_H
#define UT_H
#include<string>
#include<vector>
#include<iostream>
#include <stdio.h>
//#include <stdlib.h>//for getenv()
#include <unistd.h>//for getcwd()
#include <string.h>
#include <pwd.h>
using namespace std;


/*	This is the c++ equivalent to PHP static classes
	If there's a function that's general enough to be used in several
	different classes, just give it global scope (protected by a namespace)
*/
namespace UT{//Utility
	/*	Manage path-working-directory info	*/
	void updateDirs( string dirs[] );
	string getHomeDir();

	/*	Converts vector<string> to array of char arrays to comply with execvp.
		Adds NULL element at end
		These are heap pointers: need to delete
		Use deleteNullTermArray()
	*/
	char* const* toNullTermArray( vector< string > &v );

	/* Deallocates the above array by iterating and deleting */
	void deleteNullTermArray( char* const* &argv );

	/* This one just prints for debugging */
	void printNullTermArray( char* const* argv );

	/* Tokenizes string to the referenced vector on 'at' char
		Assume empty vector to start 
	*/
	void tok( char at, string in, vector< string > &t1 );

	/* As above, but groups quoted or bracketed text to one element with 
		groupOn/groupOff char included 
	*/
	void tok( char at, char groupOn, char groupOff, string in, vector< string > &t1 );

	/* Truncates referenced string on 'at' char */
	void trunc( char at, string &in );

	/* Kills whitespace before and after text; returns altered string */
	string trm( string in );

	/* like above, but removes opener and closer characters too. 
		For killing enclosing parentheses, brackets, quotes etc */
	string trm( string in, char opener, char closer );

	/* Like find, but returns bool */
	bool inStr( char needle, string haystack );

	/* Find a string in a string vector* */
	bool inV( string needle, vector<string> &haystack );

	/* Find one of a group of strings in a string vector* */
	bool inV( string needles[], int nSize, vector<string> &haystack );

	/* Convert array of char arrays to string, skipping first element */
	string cmdLineToStr( int argc, char* argv[] );

	/* Returns length of longest string in the vector. */
	int maxStrLen( vector< string >* v );

	/* Dev tool; Quick display of vector contents */
	void dispV( vector< string >* v );
}

#endif

