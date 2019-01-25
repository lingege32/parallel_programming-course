#!/bin/bash
function Test {
 if [ $# -eq 0 ]
 then
     echo "At least 1 argument"
 else
     mkfifo input
     cat $1 > input&
     time ./histogram
     diff $2.out "$1.out" > /dev/null
     if [ $? -eq 0 ]
     then
         echo $1 is correct
     fi
     rm input $2.out
 fi
}

if [ $# -eq 0 ]
then
    echo "Input your student ID"
else
    Test "/home/net/input-307M" $1
    Test "/home/net/input-1200M" $1
    Test "/home/net/input-2400M" $1
fi
