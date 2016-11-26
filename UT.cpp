/* David Swanson CS100 Fall 2016 */
#include "UT.h"

/* See .h file for general descriptions */

void UT::updateDirs( string dirs[] ){
	/* Dirs is array length 3:
		0=current working directory
		1=previous directory
		2=home directory
	*/
	char buffer[128];
	char *cwd = getcwd(buffer, sizeof(buffer));
	if ( cwd ){
		/* Got current directory 
			If no change, don't do anything
			If change, copy current to previous and
			update current
		*/
		string cwdStr=string( cwd );
		if( dirs[0] != cwdStr ){//skip if nothing has changed
			dirs[1]=dirs[0];
			dirs[0]=string( cwd );
		}
	}
	else{
		cout << "Can't find current working directory..." << endl;
		dirs[0]="";
	}
}
string UT::getHomeDir(){
	/* Get and return as string */
	struct passwd* pw = getpwuid( getuid() );
	return string( pw->pw_dir );
}

char* const* UT::toNullTermArray( vector< string > &v ){
	int i, n=v.size();
	char** argv = new char*[ n+1 ];
	for ( i = 0; i<n; i++ ){
        argv[i] = new char[ v.at(i).length() + 1];
		strcpy( argv[i], v.at(i).c_str() );
	}
	argv[i]=NULL;
	return argv;
}
void UT::printNullTermArray( char* const* argv ){
	cout << "printNullTermArray:" << endl;
	for ( int i = 0; true; i++ ){
		if( argv[i] == NULL ){
			//cout << endl ;//<< "break on null" << endl;
			break;
		}
		for ( int j = 0; true; j++ ){
			if( argv[i][j] == 0 ){
				cout << endl;//": endline on null" << endl;
				break;
			}
			cout << argv[i][j];
		}
	}
	cout << "end printNullTermArray:" << endl;
}
void UT::deleteNullTermArray( char* const* &argv ){
	for ( int i = 0; true; i++ ){
		if( argv[i] == NULL ){
			break;
		}
		delete argv[i];
	}
	delete argv;
	argv=NULL;
}
void UT::trunc( char at, string &in ){
	int n=in.length();
	for( int i = 0; i<n; i++ ){
		if( in[i]==at ){
			in=in.substr( 0, i );
			return;
		}
	}
}
string UT::trm( string in ){
	int n=in.length(), top=-1, end=-1;
	for( int i = 0, j=n-1; i<n; i++, j-- ){
		if( in[i]!=' ' && top==-1 ){
			top=i;
		}
		if( in[j]!=' ' && in[i]!='\n' && end==-1 ){
			end=j+1;
		}
	}
	return ( top!=-1 && end!=-1 )? in.substr( top, end-top ) : "";
}
string UT::trm( string in, char opener, char closer ){
	int n=in.length(), top=-1, end=-1;
	for( int i = 0, j=n-1; i<n; i++, j-- ){
		if( top==-1 && in[i]!=' ' && in[i]!=opener ){
			top=i;
		}
		if( end==-1 && in[j]!=' ' && in[i]!='\n' && in[j]!=closer ){
			end=j+1;
		}
	}
	return ( top!=-1 && end!=-1 )? in.substr( top, end-top ) : "";
}
bool UT::inStr( char needle, string haystack ){
	int n=haystack.length();
	for( int i = 0; i<n; i++ ){
		if( haystack[i]==needle ){
			return true;
		}
	}
	return false;
}
bool UT::inV( string needle, vector<string> &haystack ){
	int hSize=haystack.size();
	for( int i = 0; i<hSize; i++ ){
		if( haystack.at( i ) == needle ){
			return true;
		}
	}
	return false;
}
bool UT::inV( string needles[], int nSize, vector<string> &haystack ){
	int hSize=haystack.size();
	for( int i = 0; i<hSize; i++ ){
		for( int j = 0; j<nSize; j++ ){
			if( haystack.at( i ) == needles[j] ){
				return true;
			}
		}
	};
	return false;
}
void UT::tok( char at, string in, vector< string > &t1 ){
	string pushMe="";
	int n = in.length(), curr=0, i=0;
	for( i = 0; i<n; i++ ){
		if( in[i]==at ){
			pushMe=trm( in.substr( curr, i-curr ) );
			if( pushMe.length() ){
				t1.push_back( pushMe );
			}
			curr=i+1;
		}
	}
	pushMe=trm( in.substr( curr, i-curr ) );
	if( pushMe.length() ){
		t1.push_back( pushMe );
	}
}
void UT::tok( char at, char groupOn, char groupOff, string in, vector< string > &t1 ){
	bool inGroup=false;
	int n = in.length(), curr=0, i;
	string pushMe="";
	/* Parse input string one char at a time */
	for( i = 0; i<n; i++ ){
		/* Doesn't differentiate between groupOn and groupOff. 
			It just toggles on either char.  You could 
			type )this( and it would toggle the same as (this) */
		if( in[i] == groupOn || in[i] == groupOff ){
			inGroup=!inGroup;
		}
		if( !inGroup && in[i]==at ){
			pushMe=trm( in.substr( curr, i-curr ) );
			if( pushMe.length() ){
				t1.push_back( pushMe );
			}
			curr=i+1;
		}
	}
	/* On loop finish, dump the rest of the string into the vector */
	if( i-curr ){
		pushMe=UT::trm( in.substr( curr, i-curr ) );
		if( pushMe.length() ){//Couple of steps to avoid pushing empty string
			t1.push_back( pushMe );
		}
	}
}
string UT::cmdLineToStr( int argc, char* argv[] ){
	string out="";
	for( int i = 1; i<argc; i++ ){
		string s=string( argv[i] );
		out += s;
		if( i!= argc-1){
			out += " ";
		}
	}
	return out;
}
int UT::maxStrLen( vector< string >* v ){
	unsigned int n=v->size(), max=0;
	for( unsigned int i = 0; i<n; i++ ){
		if( v->at( i ).length() > max ){
			max=v->at( i ).length();
		}
	}
	return max;
}
void UT::dispV( vector< string >* v ){/* For dev */
	int n=v->size();
	for( int i = 0; i<n; i++ ){
		cout << ">>>" << v->at( i ) << "<<<" << endl;
	}
}
