#!/bin/bash
echo FIRST
echo
x=1
while [ $x -le 25 ]
do
    time ./salaries <./testcases/salaries/input$x.txt > output$x.txt
    diff -q ./testcases/salaries/output$x.txt output$x.txt
    echo 
    echo
    x=$(( $x + 1 ))
done
rm output*
echo 
echo
echo
echo SECOND
echo
x=1
while [ $x -le 22 ]
do
    time ./relay <./testcases/relay/input$x.txt > output$x.txt
    diff -q ./testcases/relay/output$x.txt output$x.txt
    echo 
    echo
    x=$(( $x + 1 ))
done
rm output*