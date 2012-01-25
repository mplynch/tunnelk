#!/bin/sh

for arg in xml java h cpp pde
do 
echo $arg
find . -name \*.$arg | xargs wc -l | tail -1
done
