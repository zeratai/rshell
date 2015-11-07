#rshell
A command shell written in C++.

#Limitations
All single commands without a connector(;, &&, ||, #) works with all tested cases.

Exit command works. When using the script commands in test cases, need to type exit multiple times,
but outside of script, exit is flawless as tested with script command.

Commands with comments work as tested.

#Bugs
After a combination of && and || in a line, if exiting, multiple exit lines must be typed from
two to three exit(e.g exit,enter, then exit, enter)
