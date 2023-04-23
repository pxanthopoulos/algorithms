#!/bin/bash
echo FIRST
echo
x=1
while [ $x -le 20 ]
do
    time ./cereal <./testcases/cereal/input$x.txt > output$x.txt
    diff -q ./testcases/cereal/output$x.txt output$x.txt
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
while [ $x -le 25 ]
do
    time ./shops2 <./testcases/shops2/input$x.txt > output$x.txt
    diff -q ./testcases/shops2/output$x.txt output$x.txt
    echo 
    echo
    x=$(( $x + 1 ))
done
rm output*