# Assignment4

Student Assignment to implement some features of a bash interpreter, including parsing of user input text, execution of bin commands, testing file types and changing directories.

Features in this version:
* Support for && || and ; connectors.
* Support for parentheses, including nested parentheses
* Test function using keyword 'test' or traditional [] brackets.
* -d -e -f arguments check directory, existence and file
* cd, cd -, cd dirName, move to home, previous and named directory
* Can type test commands in any order eg. 'test filename -e' or '[-e filename]
* Test function supports multi-word filename in double quotes

Fixed coding issues:
* Vastly simplified parser
* Tokenize on &&. (Previously tokenized on double or single &)
* Adoption of a tree structure for command objects (instead of vectors)
* Addition of abstract-factory-like functions to control object creation
* Better organization overall
* Proper precedence for && || mixes without parentheses
