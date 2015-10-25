#include <stdio.h>
#include <string.h>
// Abstract base class for the connector
class Connector {
public:
Connector() {}
~Connector() {}
virtual bool isConnector() = 0;
};

// orConnector class that inherhits from the Abstract Base Class of connector
// Connector class is used once a command is passed to check whether there is a connector within the line
class orConnector : public Connector {
public:
bool isOrConnector;
orConnector() {}
orConnector(char *line);
~orConnector() {}
};

orConnector::orConnector(char *line) : Connector() {
   char* word;
   while ((word = strtok(line, "||")) != NULL) {
      isOrConnector = true;
   }
   isOrConnector = false;
}

// Testing strtok
int main() {
   char cmd[] = "cd /home/cs100 || ls";
   char* word;
   
   // Gets the first word from the cmd, which is separated by space
   word = strtok(cmd, " ");
   printf("1st word: %s\n", word);

   // NULL must be used to get tokens from the previous string, and second word is separated by space
   word = strtok(NULL, "/");
   printf("2nd word: %s\n", word);

   // Loop the rest of the command until a connector operator
   while((word = strtok(NULL, "||")) != NULL) {
      printf("Next: %s\n", word);
   }

   return 0;
}
