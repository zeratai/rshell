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

using namespace std;
using namespace boost;

vector<string> parse(string inputLine);
int execute(vector<string> args);
string getInput();

//written own commands
int ownCd(vector<string> args);
int ownExit(vector<string> args);

vector<string> ownCommands = {"cd", "exit"};
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
    vector<string> ownCommands = {"cd", "exit"};
    for(int i = 0; i < ownCommands.size(); i++)
    {
     if( args[0] == ownCommands[i] )
       return(*ownCommandFuncs[i]) (args); 
    }
  }
  
  // do execvp
   pid_t pid;
   int status;
    
   pid = fork();    if (pid < 0)    //error forking
   if (pid < 0)    //error forking
   {
     perror("fork failed");
     exit(1);	//exit failure
   }
   else if (pid == 0)  //child process
   {
     char* command[100]; 
     copy(args.begin(), args.end(), command);
    // const char* command = forCommand.c_str();

    // char **command = &args[0];	//make args to char** for execvp
     if(execvp(command[0], command) == -1 )
       perror("execvp failed");

     exit(1);	//exit failure
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
  char_separator<char> sep("\t\n");
  tokenizer<char_separator<char>> tokens(inputLine, sep);

  BOOST_FOREACH(string t, tokens)
  {
   vectForTokens.push_back(t);
   cout<<"inside parse "  << t << endl;  //to check 
  }
  
  return vectForTokens;
}

void shell()
{
  string inputLine;
  vector<string> args;  //before char** args
  int status;

    do
    {
    char hostName[128];			//buffer to hold name from gethostname
    if( (gethostname(hostName, sizeof hostName) == -1) )	//gethostname returns 0=success or -1=error
      perror("gethostname");		//print system error with program name
    else 
      cout << "[" << getlogin() << "@ " << hostName << "]" << "$ ";
   
    inputLine = getInput();		//reads user input then saves to inputLine

    args = parse(inputLine);		//parses inputLine for commands to be executed

    cout << "args "; //to check
    for(vector<string>::const_iterator it = args.begin(); it != args.end(); it++)
      cout << *it << " ";	//to check args

    status = execute(args); 		//executes commands and sets status to continue running if 1
   }while(status);

}


int main(int argc, char** argv)
{
  shell();		//function that reads, parse, and execute

  return 0;
}
