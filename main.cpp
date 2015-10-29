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
  return 0;
}

int execute(vector<string> args)
{
  if (args.empty() == true)
    return 1;		//return status of 1
  else
  { //check for built in command and execute them
    const char* cmds[] = {"cd", "exit"};
    vector<string> ownCommands(cmds, cmds + 2);
    for(int i = 0; i < ownCommands.size(); i++)
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
    //char* command[100];
    //copy(args.begin(), args.end(), command);
    // const char* command = forCommand.c_str();
    
    //Convert vector string to char array for execvp
/*    vector<char *> command(args.size() + 1 ); // Added +1 for the null terminator
    for( size_t i = 0; i != args.size(); i++)
    {
    	command[i] = &args[i][0];
    }

    cout << "in execute "; //to check
    for(vector<char*>::const_iterator it = command.begin(); it != command.end(); it++)
      cout << *it << " ";	//to check execute
*/  
    vector<char *> commandVector;
    for(size_t i = 0; i < args.size(); ++i)
      commandVector.push_back(const_cast<char*>(args[i].c_str() ));

   // for(vector<string>::const_iterator it = args.begin(); it != args.end(); it++)
   // {
    //  commandVector.push_back(*it);
   // }
   // copy(args.begin(), args.end(), back_inserter(commandVector[0]) );
    commandVector.push_back(NULL);	//push NULL to end of vect execvp expects null

    char** command = &commandVector[0]; 

    // char **command = &args[0];	//make args to char** for execvp
     if(execvp(command[0], command) == -1 )
     {
       perror("execvp errrrr ");
     }
       exit(1);	//exit failure
     
 //  execvp(command[0], command.data() );
   }
   else if (pid > 0)  //parent process
   {
     do
     {
       waitpid(pid, &status, WUNTRACED);
     }while(!WIFEXITED(status) && !WIFSIGNALED(status));   
   } 

  return 1;
}

string getInput()
{
  string input;
  getline(cin, input);

  return input;
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

bool andConnector = false;
bool orConnector = false;

void whichConnector(string c) {
   if( c == "&&") {
      andConnector = true;
      //cout << "This is an and connector\n";
   }
   else if( c == "||") {
      orConnector = true;
      //cout << "This is an or connector\n";
   }
}

//Check if there are any Connectors
bool isConnector(vector<string> args) {
   char const* connector_list[] = {"&&", "||", ";"}; //Create our connector list
   vector<string> ownConnectors(connector_list, connector_list + 3); //Convert our connector list into a vector<string> to compare our args input
   for(int i = 0; i < ownConnectors.size(); i++) {
      for(int j = 0; j < args.size(); j++) {
         if( args[j] == ownConnectors[i]) {
            whichConnector(ownConnectors[i]); //determind which type of connector for further parsing
            //cout << ownConnectors[i] << "\n";
            return true;
         }
      }
   }
   return false;
}

int parseExec(string inputLine )   //from checked ; --parses then calls execute
{
  int status;
 // bool semicolon = false;
//  list<string> parsed;
  vector<string> commandVect;
  char_separator<char> sep(" \r\a\t\n;");
  tokenizer<char_separator<char> > tokens(inputLine, sep);

  BOOST_FOREACH(string t, tokens)
  {
    commandVect.push_back(t);
  }
  /*
  list<string> commands;
  while (!(parsed.empty()) ) //push cmds in commands, connectors still in parsed list
  {
    if (parsed.front() == ";")
      parsed.pop_front();
    else
    {
      commandVect.push_back(parsed.front());
      commands.push_back(parsed.front()); 
      parsed.pop_front();
  //    cout << "in parseExec "; //to check
  //    for(vector<string>::const_iterator it = commandVect.begin(); it != commandVect.end(); it++)
  //      cout << *it << " ";	//to check args
    }
  
  }*/
/*  vector<string> firstOneCommand;
  vector<string> secondOneCommand;
  vector<string> thirdOneCommand;
  firstOneCommand.push_back(commandVect[0]);
  secondOneCommand.push_back(commandVect[1]);
  thirdOneCommand.push_back(commandVect[2]);

  status = execute(firstOneCommand);
  status = execute(secondOneCommand);
  status = execute(thirdOneCommand);  */

  for(vector<string>::const_iterator it = commandVect.begin(); it != commandVect.end(); it++)
  {
    vector<string> oneCommand;
    oneCommand.push_back(*it);

    status = execute(oneCommand);
  }
/*
  do 
  {
   
    cout << "in parseExec "; //to check
      for(vector<string>::const_iterator it = commandVect.begin(); it != commandVect.end(); it++)
        cout << *it << " ";	//to check args
    status = execute(commandVect);
    commandVect.erase(commandVect.begin());  
  }while(!(commandVect.empty()) );
*/
 /* 
  while (!(commands.empty()) )
  {
   if (parsed.front() == ";")
   {
     semicolon = true;
     parsed.pop_front();
   }
   else
   {
     status = execute(commandVect);
     commands.pop_front();
     break;
   }

   if (semicolon == true)
   {
     status = execute(commandVect);
     commands.pop_front();
    
     cout << "in parseExec1 "; //to check
     for(vector<string>::const_iterator it = commandVect.begin(); it != commandVect.end(); it++)
      cout << *it << " ";	//to check args
   }
 
  }*/
  return status;

}

void shell()
{
  string inputLine;
  vector<string> args;  //before char** args
  int status;
/* 
  char hostName[128];			//buffer to hold name from gethostname
  if( (gethostname(hostName, sizeof hostName) == -1) )	//gethostname returns 0=success or -1=error
    perror("gethostname");		//print system error with program name
  else 
    cout << "[" << getlogin() << "@ " << hostName << "]" << "$ ";
   
  inputLine = getInput();
  if ( inputLine.find(";") != string::npos)  //looks for ;
  { 
    status =  parseExec(inputLine);   //parseExec parses then calls execute()
     // continue;
  } 
*/
  do
  {
jump:
  char hostName[128];			//buffer to hold name from gethostname
  if( (gethostname(hostName, sizeof hostName) == -1) )	//gethostname returns 0=success or -1=error
    perror("gethostname");		//print system error with program name
  else 
    cout << "[" << getlogin() << "@ " << hostName << "]" << "$ ";
   
  inputLine = getInput();		//reads user input then saves to inputLine


  if ( inputLine.find(";") != string::npos)  //looks for ;
  { 
    status =  parseExec(inputLine);   //parseExec parses then calls execute()
    goto jump;
     // continue;
  } 
//   continue;			//goes back top of loop

    if ( inputLine.find(";") != string::npos)  //looks for ;
    { 
      status =  parseExec(inputLine);   //parseExec parses then calls execute()
     
    } 

  args = parse(inputLine);		//parses inputLine for commands to be executed
/*
    cout << "args "; //to check
    for(vector<string>::const_iterator it = args.begin(); it != args.end(); it++)
      cout << *it << " ";	//to check args
*/

    //Test isConnector function, if there is a connector - use multiparser recursive function to execute
    if(isConnector(args)) {
       //cout << "There is a connector\n";
       if(andConnector) {
          //cout << "There is an and connector\n";
       }
       else if(orConnector) {
          //cout << "There is an or connector\n";
       }
    }
    else
       status = execute(args); 		//executes commands and sets status to continue running if 1
   }while(status);
}


int main(int argc, char** argv)
{
  shell();		//function that reads, parse, and execute

  return 0;
}
