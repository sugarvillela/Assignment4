/* David Swanson CS100 Fall 2016 */

#ifndef RSHELL_H
#define  RSHELL_H
#include<iostream>
#include <string>
#include <vector>
#include <unistd.h>//Linux
//#include <sys/types.h>//not needed?
#include <sys/wait.h>//Missing on my system
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sys/stat.h>//for -e -f -d
#include <sys/types.h>//for -e -f -d
#include <sys/param.h>//for cd?
using namespace std;
#include "Parser.h"

class Parser;

/* Abstract Base Class */
class RShell{
	public:
		virtual bool execute()=0;
		virtual void addCommand( RShell* newCmd )=0;
		virtual ~RShell(){}
};

/* Leaf class to implement commands */
class Cmd : public RShell{
	public:
		Cmd( string setCmd );
		bool execute();
		void addCommand( RShell* newCmd );
		string getErrTxt(){return "";}
	protected:
		string cmd;
};
/* Leaf class to implement tests */
class Tester : public RShell{
	public:
		Tester( char test, string file );
		//~Tester(void);
		bool execute();
		void addCommand( RShell* newCmd );
	protected:
		char testType;
		string fileName;
		bool d();
		bool e();
		bool f();
};
/* Leaf class for cd command*/
class CD : public RShell{
	public:
		CD( string file );
		bool execute();
		void addCommand( RShell* newCmd );
	protected:
		string fileName;
		//bool cd();
};

/* Abract parent for AND, OR, SemiColon */
class Connector : public RShell{
	public:
		Connector() : left( NULL ), right( NULL){}
		~Connector(void);
		virtual bool execute()=0;
		void addCommand( RShell* newCmd );
	protected:
		RShell* left;
		RShell* right;
};
/* Implementations of Connector */
class AND : public Connector{
	public:
		AND() : Connector() {}
		bool execute();
};
class OR : public Connector{
	public:
		OR() : Connector() {}
		bool execute();
};
class SemiColon : public Connector{
	public:
		SemiColon() : Connector() {}
		bool execute();
};

/* Composite: concrete class that runs the shell.
	Composite is the head of a lopsided tree structure, 
	which executes preorder, following short-circuit 
	rules for AND and OR
*/
class Composite : public RShell{
	public:
		Composite( string setStr, string dirs[] );
		~Composite(void);
		bool execute();
		void addCommand( RShell* newCmd );
		string getErrTxt();
	protected:
		RShell* connector;
		Parser* P;
};

#endif