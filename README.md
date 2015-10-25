#rshell
A command shell written in C++

Added connector.cpp which contains the Abstract Base Class for Connector.
   - For now the orConnector class inherits the Connector class which will takes the argument of the array of cmd. It will then use strtok to check if there is the || connector and return true if there is one.

   - In the connector.cpp, we wrote a test function with some simple commands to test the strtok and parsing features.

#Limitations


#Bugs
