#rshell
A command shell written in C++.

#Limitations
All single commands without a connector(;, &&, ||, #) works with all tested cases.

Exit command works, but when previous commands with a combination of && and ||, must type exit
multiple times.

Commands with comments work, but with the nuances of bugs with exit.

#Bugs
After a combination of && and || in a line, if exiting, multiple exit lines must be typed from
two to three exit(e.g exit,enter, then exit, enter)
