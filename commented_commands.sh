#!/bin/bash

ls #This is ls
sleep 1

pwd #This is pwd
sleep 1

ls -a; ls -l; # ls -a then ls -l
sleep 1

echo "hello" && echo "goodbye" #echoes
sleep 1

git status || echo "again" #you should not see "again"

exit
