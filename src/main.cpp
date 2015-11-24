#include <unistd.h>	//chdir() fork() exec() gethostname() pid_t
#include <stdio.h>	//perror()
#include <stdlib.h>	//exit() execvp() EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>
#include <string>	//getline()
#include <cstring>
#include <sys/wait.h>	//waitpid() and macros
#include <sys/types.h>
#include <sys/stat.h>   //stat() successful completion 0 shall be returned. Otherwise, -1 shall be returned and errno set to indicate the error.
#include <dirent.h>     //DIR, struct dirent
#include <errno.h>
#include <vector>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <algorithm>   //copy
#include <iterator>    //back_inserter()
#include <list>

using namespace std;
using namespace boost;

vector<string> parse(string inputLine);
int execute(vector<string> args);
string getInput();

//written own commands
int ownCd(vector<string> args);
int ownExit(vector<string> args);

const char* cmds[] = {"cd", "exit"};
vector<string> ownCommands(cmds, cmds + 2);
int (*ownCommandFuncs[]) (vector<string>) = {&ownCd, &ownExit};

int ownCd(vector<string> args)
{
 if(args.size() < 1)  //checks if only cd is in args as opposed to cd something
   cout << "need something to cd to" << endl;
 else
 { 
   string forCommand = args[1];
   char const* command = forCommand.c_str();
  // char *command = &args[1];	//make args to char** for execvp

   if( chdir(command) != 0 )
     perror("error in cd"); 
 } 
 return 1;
}

int ownExit(vector<string> args)
{
  cout << "This is our own custom exit \n";
  return 0;
}

string getInput() {
	string input;
	getline(cin, input);
	
	return input;
}

int execute(vector<string> args)
{
  if (args.empty() == true)
    return 1;		//return status of 1
  else
  { //check for built in command and execute them
    const char* cmds[] = {"cd", "exit"};
    vector<string> ownCommands(cmds, cmds + 2);
    for(size_t i = 0; i < ownCommands.size(); i++)
    {
     if( args[0] == ownCommands[i] )
       return(*ownCommandFuncs[i]) (args); 
    }
  }
  
  // do execvp
   pid_t pid;
   int status;
    
   pid = fork();       //error forking
   if (pid < 0)    //error forking
   {
     perror("fork failed");
     exit(1);	//exit failure
   }
   else if (pid == 0)  //child process
   {
    vector<char *> commandVector;
    for(size_t i = 0; i < args.size(); ++i)
      commandVector.push_back(const_cast<char*>(args[i].c_str() ));

    commandVector.push_back(NULL);	//push NULL to end of vect execvp expects null

    char** command = &commandVector[0]; 

     if(execvp(command[0], command) == -1 )
     {
       perror("execvp errrrr ");
     }
       status = -1; 
       exit(1);	//exit failure    
   }
   else if (pid > 0)  //parent process
   {
     do
     {
       waitpid(pid, &status, WUNTRACED);
       if(WEXITSTATUS(status) == 0)
       {
       	return 1;
       }
       else
       	return -1; // Program failed
     }while(!WIFEXITED(status) && !WIFSIGNALED(status));   
   } 

  return 1;
}

bool andConnector = false;
bool secondAndConnector = false;
bool orConnector = false;
bool secondOrConnector = false;
bool semiConnector = false;
bool commentConnector = false;
bool testProgram = false;
bool openParen = false;
bool endParen = false;
int connectorAndCount = 0;
int connectorOrCount = 0;
int testRun = 0;

void whichConnector(string c) {
	
   // cout << "which connector function\n";
   //cout << c << "\n";
   if( c == "&") {
		andConnector = true;
		//cout << "This is an and connector\n";
   }
   else if( c == "|") {
      	orConnector = true;
      	//cout << "This is an or connector\n";
   }
   else if( c == ";") {
   	   semiConnector = true;
   	   //cout << "This is a semicolon connector\n";
   }
   else if( c == "#") {
       commentConnector = true;
       //cout << "This is a comment connector\n";
   }
   else if( c == "test" || c == "[") {
   		testProgram = true;
   		//cout << "This is a test program\n";
   }
   else if( c == "(") {
   		openParen = true;
   		//cout << "This is a parenthesis connector\n";
   }
   else if( c == ")") {
   		endParen = true;
   		openParen = false;
   }
}

//Check if there are any Connectors
bool isConnector(vector<string> args) {
   char const* connector_list[] = {"&", "|", ";"}; //Create our connector list
   vector<string> ownConnectors(connector_list, connector_list + 3); //Convert our connector list into a vector<string> to compare our args input
   for(size_t i = 0; i < ownConnectors.size(); i++) {
      for(size_t j = 0; j < args.size(); j++) {
         if( args[j] == ownConnectors[i]) {
            whichConnector(ownConnectors[i]); //determind which type of connector for further parsing
            //cout << ownConnectors[i] << "\n";
            return true;
         }
      }
   }
   return false;
}

// Check if there is a connector within the parsed vector
bool hasConnector(string c) {
	//cout << "running has connector function\n";
	if( c == "&") {
	  //cout << "has connector detects and connector\n";
	  //cout << connectorAndCount << "\n";
	  if(connectorAndCount == 2) {
	  	secondAndConnector = true;
	  	andConnector = true;
	  	connectorAndCount = 0;
	  }
      return true; // set andConnector to true
   }
   else if( c == "|") {
   	if(connectorOrCount == 2) {
	  	secondOrConnector = true;
	  	orConnector = true;
	  	connectorOrCount = 0;
	  }
      return true;
   }
   else if( c == ";") {
       semiConnector = true;
   	   return true;
   }
   else if( c == "#") {
       commentConnector = true;
       return true;
   }
   else if (c == "(") {
   		openParen = true;
   		return true;
   }
   else if(c == ")") {
   		endParen = true;
   		openParen = false;
   		return true;
   }
   return false;
}

vector<string> parse(string inputLine)
{
  vector<string> vectForTokens;
  char_separator<char> sep(" \r\a\t\n");
  tokenizer<char_separator<char> > tokens(inputLine, sep);

  BOOST_FOREACH(string t, tokens)
  {
   vectForTokens.push_back(t);
 //  cout<<"inside parse "  << t << endl;  //to check 
  }
  
  return vectForTokens;
}

int parseMultipleExec(string inputLine) 
{
	int status;
	vector<string> commandVect;
	vector<string> parsedVector;
	//cout << "using multiple parse exec function";
	char_separator<char> sep(" \r\a\t\n]", ";&|#()");
 	tokenizer<char_separator<char> > tokens(inputLine, sep);

  	BOOST_FOREACH(string t, tokens)
  	{
    	commandVect.push_back(t);
    	//cout << t << "\n";
  	}
  	for(size_t i = 0; i < commandVect.size(); i++) // for loop logic that will run commands 1 at a time based on our parsing 
  	{
              //  int statusTest;
    
  		//cout << status << "\n";
  		whichConnector(commandVect[i]);
  		if(testProgram)
                {
  			//parsedVector.clear();
  			//cout << "You are in test program \n";
  			// run once for first cmd
  			if(hasConnector(commandVect[i])) // if it has another connector and status is not -1, run it 
  			{
  				for(size_t i = 0; i < parsedVector.size(); i++) {
  				 // cout << "Parsed Vector in test: " << parsedVector[i] << "\n";
  				}
  				//cout << "has connector stop pushing and execute test \n";
  				//status = execute(parsedVector);
  				if(parsedVector.size() == 2)   //only test /some/thing, not test -'flag' /some/thing
  				{
    				  struct stat buf;
    				  status = stat(parsedVector[1].c_str(), &buf);  //stat returns 0 if succesful
    				  cout << "status stat " << status << endl;
  				}
        
   				if(parsedVector.size() == 3)   //there are 3 args: test -'flag' /some/bull/shit
   				{
         			  struct stat buf;
         			  status = stat(parsedVector[2].c_str(), &buf);
    
         				if(parsedVector[1] == "-e")
         				{
         					cout << "status stat " << status << endl;
         				}
    
         				else if(parsedVector[1] == "-f")
         				{   
                     				cout << "status stat " << status << endl;
    			                      if( S_ISREG(buf.st_mode ) == 0 ) // checks for regular file
              					cout << "Is not a regular file " << endl;
            				      else
              					cout << "Is a regular file " << endl; 
         				}
      
         				else if(parsedVector[1] == "-d")
         				{
           					cout << "status stat " << status << endl;
    
           					if(S_ISDIR(buf.st_mode ) == 0)  //checks for dir
             					cout << "Is not a dir " << endl;
           					else
             					cout << "Is a dir " << endl;
         				}
         				else if(parsedVector[1] != "-e" || parsedVector[1] != "-f" || parsedVector[1] != "-d")
         				{
           					cout << "wrong flag syntax, defaulting to -e " << endl;
               				        cout << "status stat " << status << endl;
         				}  
   				}
   				testProgram = false;
   				parsedVector.clear();
  			}
  			else if(i+1 >= commandVect.size()) {
                                
  				parsedVector.push_back(commandVect[i]);
  				for(size_t i = 0; i < parsedVector.size(); i++) {
  				  //cout << "Parsed Vector in test: " << parsedVector[i] << "\n";
  				}
  				//cout << "has connector stop pushing and execute test \n";
  				//status = execute(parsedVector);
  				if(parsedVector.size() == 2)   //only test /some/thing, not test -'flag' /some/thing
  				{
    				  struct stat buf;
    				  status = stat(parsedVector[1].c_str(), &buf);  //stat returns 0 if succesful
    				  cout << "status stat " << status << endl;
  				}
        
   				if(parsedVector.size() == 3)   //there are 3 args: test -'flag' /some/bull/shit
   				{
         			  struct stat buf;
         			  status = stat(parsedVector[2].c_str(), &buf);
    
         				if(parsedVector[1] == "-e")
         				{
         					cout << "status stat " << status << endl;
         				}
    
         				else if(parsedVector[1] == "-f")
         				{   
            				       cout << "status stat " << status << endl;
    			                     if( S_ISREG(buf.st_mode ) == 0 ) // checks for regular file
              					cout << "Is not a regular file " << endl;
            			             else
              					cout << "Is a regular file " << endl; 
         				}
      
         				else if(parsedVector[1] == "-d")
         				{
           					cout << "status stat " << status << endl;
    
           				    if(S_ISDIR(buf.st_mode ) == 0)  //checks for dir
             					cout << "Is not a dir " << endl;
           			            else
             					cout << "Is a dir " << endl;
         				}
         				else if(parsedVector[1] != "-e" || parsedVector[1] != "-f" || parsedVector[1] != "-d")
         				{
           					cout << "wrong flag syntax, defaulting to -e " << endl;
               				        cout << "status stat " << status << endl;
         				} 
         			}
         		testProgram = false;
   				parsedVector.clear();
   				break;
  			}
  			//else
  			//	parsedVector.push_back(commandVect[i]);
  		}
  		 if(commentConnector) 
  		{
  			//cout << "is commentConnector";
  			status = execute(parsedVector);
  			while(i < commandVect.size()) 
  			{
  						parsedVector.push_back(commandVect[i]);
  						//cout << "Position j: " << j << "\n" << "command Vector j: " << commandVect[j] << "\n";
  						i++;
  			} 
  			parsedVector.clear(); // empty out the rest of the cmds and arguments after #
  			commentConnector = false; // default back to false for further cmds inputs
  		}
  		else if(openParen) {
  			if(hasConnector(commandVect[i])) // if it has another connector and status is not -1, run it 
  			{
  				status = execute(parsedVector);
  				parsedVector.clear();
  			}
  			else {
  				parsedVector.push_back(commandVect[i]);
  			}
  		}
  		else if(andConnector && !openParen) // Take parsedVector and run the command 
  		{
  			//cout << "in and connector \n";
  			// run once for first cmd
  			if(connectorAndCount == 0) 
  			{
  				//cout << "executing parsedVector\n";
  				status = execute(parsedVector);
  				//cout << status << "\n";
  			}
  			//clear vector to be rewritten parsedVector
  			parsedVector.clear();
  			if(hasConnector(commandVect[i]) && status != -1) // if it has another connector and status is not -1, run it 
  			{
  				connectorAndCount++;
  				//cout << "and connector count: " << connectorAndCount++ << "\n";
  				if(hasConnector(commandVect[i]) && secondAndConnector && status != -1) 
  				{
  					status = execute(parsedVector);
  					andConnector = false; // reset andConnector to check if cmd ends or if there are more commands
  					secondAndConnector = false;
  				} 
  				else if(hasConnector(commandVect[i]) && endParen && status != -1) {
  					status = execute(parsedVector);
  					andConnector = false;
  					secondAndConnector = false;
  				}
  			}
  			else if(status == -1) 
  			{
  				connectorAndCount++;
  				//cout << "status is -1, do not run second command\n";
  				i = i + 2;
  				//cout << i << "\n";
  				//cout << j << "\n";
  				while(!hasConnector(commandVect[i]) && commandVect[i] != ";" && i < commandVect.size()) 
  				{
  					parsedVector.push_back(commandVect[i]);
  					//cout << "Position i: " << i << "\n" << "command Vector i: " << commandVect[i] << "\n";
  					i++;
  				}
  				parsedVector.clear();
  				//cout << commandVect[i] << "\n";
  				whichConnector(commandVect[i]);
  				size_t j = i;
  				//cout << "Position j at semicolon check: " << j << "\n";
  				if(commandVect[i] == ";" || commandVect[j] == ")") 
  				{
  				    //cout << "has semicolon \n";
  					j = j + 1;
  				}
  				else j = j + 2;
  				//cout << "Position j after semicolon check: " << j << "\n";
  				while(!hasConnector(commandVect[j]) && commandVect[j] != ";" && j < commandVect.size())
  				{
  					parsedVector.push_back(commandVect[j]);
  					//cout << "Position j: " << j << "\n" << "command Vector j: " << commandVect[j] << "\n";
  					j++;
  				} 
  				// push next command
  				// parsedVector.push_back(commandVect[i]);
  				andConnector = false;
  				secondAndConnector = false;
  			}
  			else
  				parsedVector.push_back(commandVect[i]);
  		}
  		else if(orConnector && !openParen) // Take parsedVector and run the command 
  		{
  			//cout << "in or connector \n" << "Current commandVect: " << commandVect[i] << "\n";
  			// run once for first cmd
  			if(connectorOrCount == 0) 
  			{
  				//cout << "executing parsedVector in orConnector\n";
  				status = execute(parsedVector);
  				//cout << status << "\n";
  			}
  			//clear vector to be rewritten parsedVector
  			parsedVector.clear();
  			if(status == -1) 
  			{
  				connectorOrCount++;
  				//cout << "or connector count: " << connectorOrCount++ << "\n" << "Status: " << status << "\n";
  				if(hasConnector(commandVect[i]) && secondOrConnector && status == -1) 
  				{
  						//cout << "Run orConnector cmd\n";
  						//cout << "prasedVector: " << parsedVector[i] << "\n";
  						status = execute(parsedVector);
  						orConnector = false; // reset andConnector to check if cmd ends or if there are more commands
  						secondOrConnector = false;
  				}
  				else if(hasConnector(commandVect[i]) && endParen && status != -1) {
  					status = execute(parsedVector);
  					andConnector = false;
  					secondAndConnector = false;
  				}
  			}
  			else if(hasConnector(commandVect[i]) && status == -1) // if it has another connector and status is -1, run it 
  				{
  					connectorOrCount++;
  					//cout << "or connector count: " << connectorOrCount++ << "\n" << "Status: " << status << "\n";
  					if(hasConnector(commandVect[i]) && secondOrConnector && status == -1) 
  					{
  						//cout << "Run orConnector cmd\n";
  						//cout << "prasedVector: " << parsedVector[i] << "\n";
  						status = execute(parsedVector);
  						orConnector = false; // reset andConnector to check if cmd ends or if there are more commands
  						secondOrConnector = false;
  					} 
  				}
  			else if(status != -1) 
  				{
  					connectorOrCount++;
  					//cout << "status is -1, do not run second command\n";
  					if(i + 2 < commandVect.size() && !commandVect[i+2].empty()) {
  					i = i + 2;
                                        }
  					//cout << i << "\n";
					//cout << commandVect[i] << "\n";
  					//cout << j << "\n";
  					//Push all the failed commands following after the orConnector fails
  					while(i < commandVect.size() && !commandVect[i].compare(";")) 
  					{
  						parsedVector.push_back(commandVect[i]);
  						//cout << "Position i: " << i << "\n" << "command Vector i: " << commandVect[i] << "\n";
  						i++;
  					}
  					parsedVector.clear();
  					//cout << commandVect[i] << "\n";
  					whichConnector(commandVect[i]);
  					size_t j = i;
  					//cout << "Position j at semicolon check: " << j << "\n";
  					if(!commandVect[j].empty()) 
  					{
  					    //cout << "has semicolon \n";
  					    if(commandVect[j].compare(";") || commandVect[j].compare(")")) {
  						j = j + 1;
					    }
  					}
  					else j = j + 2;
					if(j >= commandVect.size()) break;
  					//cout << "Position j after semicolon check: " << j << "\n";
  					while(!hasConnector(commandVect[j]) && j < commandVect.size() && !commandVect[j].compare(";"))
  					{
  						parsedVector.push_back(commandVect[j]);
  						//cout << "Position j: " << j << "\n" << "command Vector j: " << commandVect[j] << "\n";
  						j++;
  					} 
  					// push next command
  					// parsedVector.push_back(commandVect[i]);
  					orConnector = false;
  					secondOrConnector = false;
                                        break;
  				}
  			else 
  			{
  				//cout << "Pushing into prasedVector: " << parsedVector[i] << "\n";
  				parsedVector.push_back(commandVect[i]);
  			}
  		}
  		else if(semiConnector) 
  		{
  			//cout << "is SemiConnector";
  			status = execute(parsedVector);
  			parsedVector.clear();
  			//cout << commandVect[i] << "\n";
  			whichConnector(commandVect[i]);
  			semiConnector = false; // default back to false for looping
  		}
  		else 
  		{
  			//cout << "pushing onto parsedVector \n";
  			parsedVector.push_back(commandVect[i]);
  		}
  	}
  	
  	if(!parsedVector.empty() && parsedVector.size() > 0) 
  	{
  		//cout << "Executing final cmd without semicolon\n";
  		status = execute(parsedVector);
  	}
  	secondAndConnector = false;
  	secondOrConnector = false;
  	connectorOrCount = 0;
  	connectorAndCount = 0;
  	return status;
}

int singleTest(string inputLine)
{
  int status = -1;
  vector<string> commandVect;
  vector<string> parsedVector;

  char_separator<char> sep(" \r\a\t\n");
  tokenizer<char_separator<char> > tokens(inputLine, sep);

  BOOST_FOREACH(string t, tokens)
  {
    commandVect.push_back(t);
    cout << t << endl;
  }

  if(commandVect.size() == 2)   //only test /some/thing, not test -'flag' /some/thing
  {
    struct stat buf;
    status = stat(commandVect[1].c_str(), &buf);  //stat returns 0 if succesful
    cout << "status stat " << status << endl;
  }
        
   if(commandVect.size() == 3)   //there are 3 args: test -'flag' /some/bull/shit
   {
         struct stat buf;
         status = stat(commandVect[2].c_str(), &buf);
    
         if(commandVect[1] == "-e")
         {
          // status = stat(commandVect[2].c_str(), &buf);  //stat returns 0 if succesful
           cout << "status stat " << status << endl;
         }
    
         else if(commandVect[1] == "-f")
         {   
          //  status = stat(commandVect[2].c_str(), &buf);  //stat returns 0 if succesful
            cout << "status stat " << status << endl;
    
             // stat(commandVect[2].c_str(), &buf);
            if( S_ISREG(buf.st_mode ) == 0 ) // checks for regular file
              cout << "Is not a regular file " << endl;
            else
              cout << "Is a regular file " << endl; 
         }
      
         else if(commandVect[1] == "-d")
         {
          // status = stat(commandVect[2].c_str(), &buf);  //stat returns 0 if succesful
           cout << "status stat " << status << endl;
    
           if(S_ISDIR(buf.st_mode ) == 0)  //checks for dir
             cout << "Is not a dir " << endl;
           else
             cout << "Is a dir " << endl;
         }
         else if(commandVect[1] != "-e" || commandVect[1] != "-f" || commandVect[1] != "-d")
         {
           cout << "wrong flag syntax, defaulting to -e " << endl;
    
          // status = stat(commandVect[2].c_str(), &buf);  //stat returns 0 if succesful
           cout << "status stat " << status << endl;
    
           //cout << "wrong flag syntax, defaulting to -e " << endl;
          //exit(1); // exit failure
         }  
       
   }
   return status;
}

int singleBracTest(string inputLine)  //cmd: [ -'flag' some/bull/shit ]
{
  int status = -1;
  vector<string> commandVect;
  vector<string> parsedVector;

  char_separator<char> sep(" \r\a\t\n[]");
  tokenizer<char_separator<char> > tokens(inputLine, sep);

  BOOST_FOREACH(string t, tokens)
  {
    commandVect.push_back(t);
    cout << t << endl;
  }
  	
  if(commandVect.size() == 1)   //only test:  /some/thing, not -'flag' /some/thing
  {
    struct stat buf;
    status = stat(commandVect[0].c_str(), &buf);  //stat returns 0 if succesful
    cout << "status stat " << status << endl;
  }
        
   if(commandVect.size() == 2)   //there are 2 args:  -'flag' /some/bull/shit
   {    
          
        //S_ISREG(m) and S_ISDIR macros return 0 if false, non-zero if true
        //m = buf.st_mode
         struct stat buf;
         status = stat(commandVect[1].c_str(), &buf);
    
         if(commandVect[0] == "-e")
         {
          // status = stat(commandVect[2].c_str(), &buf);  //stat returns 0 if succesful
           cout << "status stat " << status << endl;
         }
    
         else if(commandVect[0] == "-f")
         {   
          //  status = stat(commandVect[2].c_str(), &buf);  //stat returns 0 if succesful
            cout << "status stat " << status << endl;
    
             // stat(commandVect[2].c_str(), &buf);
            if( S_ISREG(buf.st_mode ) == 0 ) // checks for regular file
              cout << "Is not a regular file " << endl;
            else
              cout << "Is a regular file " << endl; 
         }
      
         else if(commandVect[0] == "-d")
         {
          // status = stat(commandVect[2].c_str(), &buf);  //stat returns 0 if succesful
           cout << "status stat " << status << endl;
    
           if(S_ISDIR(buf.st_mode ) == 0)  //checks for dir
             cout << "Is not a dir " << endl;
           else
             cout << "Is a dir " << endl;
         }
         else if(commandVect[0] != "-e" || commandVect[0] != "-f" || commandVect[0] != "-d")
         {
           cout << "wrong flag syntax, defaulting to -e " << endl;
    
          // status = stat(commandVect[2].c_str(), &buf);  //stat returns 0 if succesful
           cout << "status stat " << status << endl;
    
           //cout << "wrong flag syntax, defaulting to -e " << endl;
          //exit(1); // exit failure
         }  
       
   }
   return status;
}

//int exceParenstat;
vector<string> cmdsInsideParen;
int findClosingParen(string inputLine, size_t openPos)  
{
    
    size_t closePos = openPos;
  /* if(closePos == inputLine.size() )
     return closePos;    */
    //cout << "init closePos " << closePos << endl;  
   // else
 //  {
        string text = inputLine;
       // char text[inputLine.length()];
      //  strcpy(text, inputLine.c_str());
      
        //int closePos = openPos;
        int counter = 1;
	while (counter > 0) 
        { 
            //string c = text[++closePos];
            closePos++;
            string c = text.substr(closePos,1);
            //cout << " c: " << c ;
            if (c == "(") 
                counter++;
            
            else if (c == ")") 
                counter--;
            
        }
        //cout << endl;
       // char insideParen[closePos-1];          //insideParen should contain cmds inside
        //int number_of_chars = closePos - (openPos + 1);
        //strncpy ( insideParen, text + (openPos + 1), number_of_chars);
        //insideParen[strlen(insideParen)] = '\0';
        string insideParen = text.substr(openPos+1,(closePos-openPos)-1); 
  //      for(int i=0; i < insideParen.length(); i++)
   //        cout << insideParen[i] ;
        
         //cout<< "closePos" << closePos << endl;
        //convert  insideParen to string
        //string toVect(insideParen);
        //cout << "insideParen: " << insideParen << endl;
        //push toVect to vector cmdsInsideParen
        cmdsInsideParen.push_back(insideParen);
        
       /* 
        inputLine.erase (openPos,closePos);
        if(inputLine.find("(") != string::npos)
        {
            size_t openPos = inputLine.find("(");
            closePos = 0;
       	    closePos = findClosingParen(inputLine, openPos);
        }     */
        
        return closePos;
   //}
  
}

int executeParen(string inputLine)  
{
   int stat;
   //erase "(" and ")"
   inputLine.erase( remove(inputLine.begin(), inputLine.end(), '('), inputLine.end() );
   inputLine.erase( remove(inputLine.begin(), inputLine.end(), ')'), inputLine.end() );

//   cout << cmdsInsideParen.size() << endl;
  // for(size_t i = 0; i < cmdsInsideParen.size(); i++)
    // cout << "in executeParen, cmdsInsideParen "<<cmdsInsideParen[i] << endl;   
  
   for(size_t i = 0; i < cmdsInsideParen.size(); i++)
   { 
     cmdsInsideParen[i].erase( remove(cmdsInsideParen[i].begin(),cmdsInsideParen[i].end(), ')' ), cmdsInsideParen[i].end() );
     //cout << "in executeParen, cmdsInsideParen "<<cmdsInsideParen[i] << endl;   
   
   }
 
   //erase stuff from cmdsInsideParen vector from inputLine
   for(size_t i = 0; i < cmdsInsideParen.size(); i++)
   {
      size_t j = inputLine.find(cmdsInsideParen[i]);
        
      if (j != string::npos)
           inputLine.erase(j, cmdsInsideParen[i].length());
   }
   string erased = inputLine;
  // cout << erased << endl;  //to check

   vector<string> connectorVect;

   char_separator<char> sep(" \r\a\t\n");
   tokenizer<char_separator<char> > tokens(erased, sep);

   BOOST_FOREACH(string t, tokens)
   {
     connectorVect.push_back(t);
     //cout << "connectorVect " << t << endl;
   }

   for(size_t i = 0; i < cmdsInsideParen.size(); i++)
   {
     
     stat = parseMultipleExec(cmdsInsideParen[i]); 
   }

  /*
   for(vector<string>::iterator i = cmdsInsideParen.begin(); i !=  cmdsInsideParen.end(); i++)
  // for(vector<string>::iterator i = connectorVect.begin(); i !=  connectorVect.end(); i++)
   {
       if(*i == "&&")
       {
         stat = parseMultipleExec(cmdsInsideParen[i-connectorVect.begin()]);
         if(stat != 1)
         {
           cmdsInsideParen.erase(i+1);
           continue;
         }
         else
           stat = parseMultipleExec(cmdsInsideParen[i-connectorVect.begin()+1] ); 
       }
       
       else if (*i == "||")
       {
         stat = parseMultipleExec(cmdsInsideParen[i-connectorVect.begin()] );
         if(stat != -1 ) //succeed
           continue;
         else
           stat = parseMultipleExec(cmdsInsideParen[i-connectorVect.begin()+1]);
       }
        
       else
         stat = parseMultipleExec(cmdsInsideParen[i-connectorVect.begin()]);
   }
    */  
   return stat;
}



void shell() 
{
	string inputLine;
	vector<string> args;
	int status;
	do {
	jump:
		char hostName[128];
		if(gethostname(hostName, sizeof hostName) == -1)
			perror("gethostname");
		else
			cout << "[" << getlogin() << "@" << hostName << "]" << "$ ";
		
		inputLine = getInput();
		/*
		if( inputLine.find(";") != string::npos || inputLine.find("&&") != string::npos || inputLine.find("||") != string::npos
                    || inputLine.find("#") != string::npos) 
		{
			//cout << "use multiple parse\n";
			status = parseMultipleExec(inputLine);
			goto jump;
		}    */
		
                //also find [, for bracket test
	    //------------------------------------------------------------------------
               if( inputLine.find("(") != string::npos)
	       {
		    size_t openPos = inputLine.find("(");
		    size_t closeParen =  findClosingParen(inputLine, openPos);
		    
		    string cutInput = inputLine.substr(openPos,closeParen+1);  //cut analyzed() in inputLine
                    //cout << "cutInput " << cutInput << endl;
                    size_t i = inputLine.find(cutInput);
                    string newInput;
                    if(i != string::npos)
                       newInput = inputLine.erase(i, cutInput.length() );
                    //cout << "newInput: " << newInput << endl;
		    if(newInput.find("(") != string::npos) //part of inputLine is cut(analyzed part) then check "(" again
		    {
		        size_t openPos1 = newInput.find("(");
		        findClosingParen(newInput, openPos1);
		    }
		    
                  //erase "(" and ")"
                 inputLine.erase( remove(inputLine.begin(), inputLine.end(), '('), inputLine.end() );
                 inputLine.erase( remove(inputLine.begin(), inputLine.end(), ')'), inputLine.end() );
                // cout << "inputLine erase: " << inputLine << endl;
                 //erase stuff from cmdsInsideParen vector from inputLine
                  for(size_t i = 0; i < cmdsInsideParen.size(); i++)
                  {
                    size_t j = inputLine.find(cmdsInsideParen[i]);
                
                     if (j != string::npos)
                       inputLine.erase(j, cmdsInsideParen[i].length());
                  }
                   //cout << "inputLine erase (), cmdsInsideParen: " << inputLine << endl;

                   //parse for connectors outside ()
                   string erased = inputLine;
                   vector<string> connectors;
        	//cout << "using multiple parse exec function";
        	   char_separator<char> sep(" \r\a\t\n");
         	   tokenizer<char_separator<char> > tokens(erased, sep);
        
          	    BOOST_FOREACH(string t, tokens)
          	    {
            	     connectors.push_back(t);
		       //cout <<"connectors: " << t << "\n";
          	     }
            
            	    for(size_t i = 0; i < connectors.size(); i++)
            	    { 	 
                	if(connectors[i] == "&&")
                	{	
                          status = parseMultipleExec(cmdsInsideParen[i]);
                    	  if(status == -1)
                      	    cmdsInsideParen.erase (cmdsInsideParen.begin()+1);
                    	else
                      	    status = parseMultipleExec(cmdsInsideParen[i+1]);
                	}		
                	else if(connectors[i] == "||")
                	{
                	    status = parseMultipleExec(cmdsInsideParen[i]);
                    	    if(status != -1)
                      		cmdsInsideParen.erase (cmdsInsideParen.begin()+1);
                    
                	}
            	    }
           	 //if there's no connectors outside ()
		    for(size_t i = 0; i < cmdsInsideParen.size(); i++)
		    {
		        status = parseMultipleExec(cmdsInsideParen[i]);
		    }
                    cmdsInsideParen.clear();
		    //status = executeParen(inputLine);
		    goto jump;
		}	
                //also find [, for bracket test
        else if( inputLine.find("[") != string::npos || inputLine.find(";") != string::npos || inputLine.find("&&") != string::npos
                     || inputLine.find("||") != string::npos || inputLine.find("#") != string::npos) 
		{
			//cout << "use multiple parse\n";
			status = parseMultipleExec(inputLine);
			goto jump;
		}      
     
		//look for test cmd, single cmd without connectors
		else if( inputLine.find("test") != string::npos)
		{
			//cout << "It is running single test cmd\n";
		    singleTest(inputLine);
		    goto jump;
		}
                
               	//look for [] test cmd, single cmd without connectors
		else if( inputLine.find("[") != string::npos && inputLine.find("]") != string::npos)
		{
		    singleBracTest(inputLine);
		    goto jump;
		}
		
		args = parse(inputLine);
		
		status = execute(args);
		}
	while(status);
}

int main (int argc, char** argv) 
{
	shell();
	return 0;
}
