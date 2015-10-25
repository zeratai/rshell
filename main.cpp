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
int execute();
string getInput();

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

 // do
 // {
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

 //   status = execute(); 		//executes commands and sets status to continue running if 1
//  }while(status)

}


int main(int argc, char** argv)
{
  shell();		//function that reads, parse, and execute

  return 0;
}
