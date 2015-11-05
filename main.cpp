#include <unistd.h>	//chdir() fork() exec() gethostname() pid_t
#include <stdio.h>	//perror()
#include <stdlib.h>	//exit() execvp() EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>
#include <string>	//getline()
#include <cstring>
#include <sys/wait.h>	//waitpid() and macros
#include <sys/types.h>
#include <vector>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <algorithm>   //copy
#include <iterator>    //back_inserter()
#include <list>

using namespace std;
using namespace boost;

vector<string> parse(string inputLine);
int execute(vector<string> &args);
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
  return 0;
}
 
bool execute(vector<char*> &oneCommand)  //from izbicki sample
{
  char **command = &oneCommand[0]; 
		
  if(oneCommand.size() < 2) 
    return true;
	
  if(strcmp(command[0], "exit") == 0)
    exit(0);
	
  else 
  {
    int pid = fork();
    if(pid == -1) 
    {
      perror("fork() errrr");
      exit(1);
    } 
    else if(pid == 0) 
    {	//child process
      if(-1 == execvp(command[0], command))
      {
        perror("execvp() errrr");
      }
      oneCommand.clear();		//clear vector so it can hold next command.
      return false;
     }
     if(wait(0) == -1)
     {
       return false;
       perror("wait() errrr");
     }
		//parent 
     oneCommand.clear();
     return true;
   }
}

int execute(vector<string> &args)
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
  
  vector<char *> commandVector;
    for(size_t i = 0; i < args.size(); ++i)
      commandVector.push_back(const_cast<char*>(args[i].c_str() ));
      
  commandVector.push_back(NULL);	//push NULL to end of vect execvp expects null
  char** command = &commandVector[0];
  
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
   /*
    vector<char *> commandVector;
    for(size_t i = 0; i < args.size(); ++i)
      commandVector.push_back(const_cast<char*>(args[i].c_str() ));
   */
   
 //   commandVector.push_back(NULL);	//push NULL to end of vect execvp expects null

 //   char** command = &commandVector[0]; 

     if(execvp(command[0], command) == -1 )
     {
       perror("execvp errrrr ");
     }
     
     commandVector.clear();         //clear to hold next cmd
     exit(1);	//exit failure
   
   }
   else if (pid > 0)  //parent process
   {
     do
     {
       waitpid(pid, &status, WUNTRACED);
     }while(!WIFEXITED(status) && !WIFSIGNALED(status));   
     
     commandVector.clear();
   } 

  return 1;
}  

string getInput()
{
  string input;
  getline(cin, input);

  return input;
}

vector<string> parse(string inputLine)  //for no connectors
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

//not used
int parseExec(string inputLine )   //from checked ; --parses then calls execute
{
  int status;
 
  vector<string> commandVect;
  char_separator<char> sep(" \r\a\t\n;");
  tokenizer<char_separator<char> > tokens(inputLine, sep);

  BOOST_FOREACH(string t, tokens)
  {
    commandVect.push_back(t);
  }
 
  vector<string>::const_iterator it = commandVect.begin();
  do
  {
    if (*it == "cd")
    { 
      vector<string> oneCommand;
      oneCommand.push_back(*it);
      vector<string>::const_iterator cpy = it;
      cpy++;
      oneCommand.push_back(*cpy);
     // ownCd(oneCommand);        

      const char* cmds[] = {"cd", "exit"};
      vector<string> ownCommands(cmds, cmds + 2);
      for(size_t i = 0; i < ownCommands.size(); i++)
      {
      // if( commandVect[0] == ownCommands[i] )
        return(*ownCommandFuncs[i]) (oneCommand); 
      }
    }
/*
    if (*it != "cd")
    {
      for(vector<string>::const_iterator i = commandVect.begin(); i != commandVect.end(); i++)
      {
        vector<string> oneCommand;
        oneCommand.push_back(*i);
        status = execute(oneCommand);
      }
    } */

    else
    {
      goto next; 
 /*     for(vector<string>::const_iterator i = commandVect.begin(); i != commandVect.end(); i++)
        {
        vector<string> oneCommand;
        oneCommand.push_back(*i);
        status = execute(oneCommand);
        }*/  
    }
  }while(it++ != commandVect.end() );

next:
    for(vector<string>::const_iterator i = commandVect.begin(); i != commandVect.end(); i++)
    {
      vector<string> oneCommand;
      oneCommand.push_back(*i);

      status = execute(oneCommand);
     }


  return status;
}

void addOneSpace(string &input, const string &connector) //finds particular connector then adds space before n after
{
  size_t length = connector.length();
  size_t position;
  position = input.find(connector, 0);   	//finds connector and returns position

  while(position != string::npos)   		//npos -- until the end of the string
  {
    input.insert(position, " ");	       	//one space before
    input.insert(position + 1 + length, " ");	 //and one space after
    position = input.find(connector, position + 1 + length);
  }
	
}

void addSpacesForConnectors(string &input)
{
  addOneSpace(input, ";");
  addOneSpace(input, "&&");
  addOneSpace(input, "||");
  addOneSpace(input, "#");   
}

//if connectors detected
void parseMultipleExec(string inputLine) 
{
  bool status = true;	 
	
  bool semiConnector;
  bool andConnector;
  bool orConnector;
  bool commentConnector;

  bool checkAllConnector;   //to check if there's a connector
  
  addSpacesForConnectors(inputLine);
 
/* 
  vector<string> commandVect1;
  char_separator<char> sep(" \r\a\t\n", ";&|");    //keeps ;&|
  tokenizer<char_separator<char> > tokens(inputLine, sep);
	
 a BOOST_FOREACH(string t, tokens)
  {
    commandVect1.push_back(t);          // commandVect has ; or & or||
  }
  
  vector<char*> commandVect;          //convert vector<string> commandVect1 to vector<char*> commandVect
  for(int i = 0; i < commandVect1.size(); i++)
  {
    char *convert;
    strcpy(convert, commandVect1[i].c_str());
    commandVect.push_back(convert);
    cout << commandVect[i];
	  
  }
  */
  
  //Parsing here boost char_separator creates problems 
  char *charInputLine = new char[inputLine.length()+1];	
  strcpy(charInputLine, inputLine.c_str());

  char *token = strtok(charInputLine, " \t\n\r\a");
  vector<char*> commandVect;

  while(token != NULL)
  {	
    commandVect.push_back(token);
    token = strtok(NULL, " \t\n");
  }   
 
  char semi[] = ";";		//add ;, bc wont execute if no ;(ls; ls; pwd) -- pwd won't execute
  commandVect.push_back(semi);

  vector<char*> oneCommand;
  //vector<string> onecommand;
  oneCommand.clear();
  
  size_t i = 0;
  while(i < commandVect.size()) 
  {
	  //whichConnector(commandVect[i]);
/*	  semiConnector = commandVect[i].compare(";") == 0;
	  andConnector = commandVect[i].compare("&&") == 0;
	  orConnector = commandVect[i].compare("||") == 0;
    commentConnector = commandVect[i].compare("#") == 0;    */
    
     semiConnector = strcmp(commandVect.at(i), ";") == 0;   //check bools
     andConnector = strcmp(commandVect.at(i), "&&") == 0;
     orConnector = strcmp(commandVect.at(i), "||") == 0;
     commentConnector = strcmp(commandVect.at(i), "#") == 0;
    //check if either one of the connector inside commandVect[i]	  
     checkAllConnector = semiConnector || andConnector || orConnector || commentConnector;

 //    if(commentConnector)
   //    goto end;
		
     if(!checkAllConnector) 
       oneCommand.push_back(commandVect.at(i));    //oneCommand contains 1 cmd from commandVect no connector
	
     else  //if connector found
     {   
       oneCommand.push_back(NULL);        	 //push NULL for execvp, expects null termitated 
       status = execute(oneCommand);    	 // execute 1 cmd(oneCommand already contains no connector)
            
       if(semiConnector)  
         oneCommand.clear();  			 //clear oneCommand for the next command

       else if(andConnector && status) 
         oneCommand.clear();
        
       else if(orConnector && !status) 
         oneCommand.clear();
     /*   
       else if(commentConnector && status)
       {
         oneCommand.clear();
         break;
       }
                   */
       else 
         break;		
     }
     i++;
  }
//end:	
  delete[] charInputLine;
  	
  //return status;
}

void shell()
{
  string inputLine;
  vector<string> args;  //before char** args
  int status;

 do
  {
jump:
    char hostName[128];			//buffer to hold name from gethostname
    if( (gethostname(hostName, sizeof hostName) == -1) )	//gethostname returns 0=success or -1=error
      perror("gethostname");		//print system error with program name
    else 
      cout << "[" << getlogin() << "@ " << hostName << "]" << "$ ";
     
    inputLine = getInput();		//reads user input then saves to inputLine

    if( inputLine.find(";") != string::npos || inputLine.find("&&") != string::npos
       || inputLine.find("||") != string::npos || inputLine.find("#") != string::npos ) 
    {
      parseMultipleExec(inputLine);
      goto jump;
    }

    args = parse(inputLine);		//parses inputLine for commands to be executed
/*
    cout << "args "; //to check
    for(vector<string>::const_iterator it = args.begin(); it != args.end(); it++)
      cout << *it << " ";	//to check args
*/
    status = execute(args); 		//executes commands and sets status to continue running if 1
  }while(status);

}

int main(int argc, char** argv)
{
  shell();		//function that reads, parse, and execute

  return 0;
}
