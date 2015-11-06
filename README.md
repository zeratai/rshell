#rshell
A command shell written in C++

Multiple Commands:
   Semicolon (;): This is working and chains correctly even with a failed command, it will run the next command
   
   And Connector (&&): This is working and chains correctly, but has a few limitations (listed under limitations)
   
   Or Connector (||): This is working and chains correctly, but also has a few limitations (listed under limitations)

#Limitations

   And Connector (&&): Running a failed cmd with the && connector will prevent the commands from continuing to run even with semicolons
   
   Or Connector (||): Running an executable cmd that fails will prevent the or Connector function from continuing to run, even though it should

#Bugs

   Not sure if the script we are making runs in the OS bash rather than our rshell, we have created a script and .sh for comparison