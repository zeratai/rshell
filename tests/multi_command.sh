ls -l; pwd; echo hello world;
sleep 2
ls -l ; pwd; echo spaces test
sleep 2
echo testing and connector && echo works!
sleep 2
echo testing and with semi && echo yay!; pwd && ls;
sleep 2
echo testing failed or || echo dont run;
sleep 2
this wont run || echo the or works! && pwd; ls -a
sleep 2
this wont run || echo this works! && mkdir testdir; ls -l && rmdir testdir;
sleep 2
echo testing connectors with spaces &&     pwd     && echo it works!  ; 
sleep 2
echo testing && echo and && echo chaining   && echo with spaces; echo and semicolons
sleep 2
ls l || echo this works! && mkdir testdir; ls -l && rmdir testdir; pwd
sleep 2
ls a || echo failed ls command should run this echo
sleep 2
echo chaining semicolon; echo with a failed cmd; this will fail; echo this should work!
sleep 2
ls a && echo this will wont echo || echo this should echo; pwd
exit