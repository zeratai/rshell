
Student 1: Adrian Dy

Student 2: Justin Moy

#rshell
A command shell written in C++.

All single commands without a connector(;, &&, ||, #) works with all tested cases.

Commands with a variety of connectors work. &&'s chained with ||'s work as well as commands chained with ;.

Exit command works. When using the script commands in test cases, need to type exit multiple times,
but outside of script, exit is flawless as tested with script command.

Commands with comments work as tested.



Exit command works. When using the script commands in test cases, need to type exit multiple times,
but outside of script, exit is flawless as tested with script command.

Commands with comments work as tested.
=======

#Limitations
- cannot run sh scripts
- no cd yet
- no piping yet
- seem to output only execute errors




#Limitations
- cannot run sh scripts
- no cd yet
- no piping yet
- seem to output only execute errors

#Bugs
The echo command outputs the quotes.

No other bugs currently found as tested. Readme updated when more bugs are found.


November 6th, 2015 Bugs

echo "testing with quotes" #this will output "testing with quotes" with the quotation marks - will need to remove later

ls -l && pwd && echo yes! || echo dont output && echo this will fail ; ls -a && pwd #mixing an || following
an && connector will cause the command to run the && command and will not run the commands after the ; #The above bug for || connector logic has been fixed, but running the ls -a && pwd will cause errors for pwd

ls && pwd && echo yes && echo done || echo no ; ls -l #these commands work but our rshell will try to execute a final command
causing a perror, this needs further investigation

for ():
cmds should be in () like --- (echo a) && (echo b && echo c) or (echo a && echo b) && (echo c)
if echo a && (echo b && echo c) seg faults
