#!/bin/bash
echo FIRST
echo
x=1
while [ $x -le 20 ]
do
    time ./contagion <./testcases/contagion/input$x.txt > output$x.txt
    diff -q ./testcases/contagion/output$x.txt output$x.txt
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
while [ $x -le 20 ]
do
    time ./shortcuts <./testcases/shortcuts/input$x.txt > output$x.txt
    diff -q ./testcases/shortcuts/output$x.txt output$x.txt
    echo 
    echo
    x=$(( $x + 1 ))
done
rm output*