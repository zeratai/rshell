#include <unistd.h>	//chdir() fork() exec() gethostname() pid_t
#include <stdio.h>	//perror()
#include <stdlib.h>	//exit() execvp() EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>
#include <string>	//getline()
#include <cstring>
#include <sys/wait.h>	//waitpid() and macros
#include <sys/types.h>
#include <vector>

using namespace std;

char** parse(string inputLine);
int execute();
string getInput();

void shell()
{
  string inputLine;
  char** args;
  int status;

 // do
 // {
    char hostName[128];			//buffer to hold name from gethostname
    if( (gethostname(hostName, sizeof hostName) == -1) )	//gethostname returns 0=success or -1=error
      perror("gethostname");		//print system error with program name
    else 
      cout << "[" << getlogin() << "@ " << hostName << "]" << "$ ";
   
 //   inputLine = getInput();		//reads user input then saves to inputLine

 //   args = parse(inputLine);		//parses inputLine for commands to be executed

 //   status = execute(); 		//executes commands and sets status to continue running if 1
//  }while(status)

}


int main(int argc, char** argv)
{
  shell();		//function that reads, parse, and execute

  return 0;
}
