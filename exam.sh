#!/bin/bash
mkdir test
cp -r code test
cat code/14.c
cd test/code
gcc -c 0.c -o 0.o
gcc -c 1.c -o 1.o
gcc -c 2.c -o 2.o
gcc -c 3.c -o 3.o
gcc -c 4.c -o 4.o
gcc -c 5.c -o 5.o
gcc -c 6.c -o 6.o
gcc -c 7.c -o 7.o
gcc -c 8.c -o 8.o
gcc -c 9.c -o 9.o
gcc -c 10.c -o 10.o
gcc -c 11.c -o 11.o
gcc -c 12.c -o 12.o
gcc -c 13.c -o 13.o
gcc -c 14.c -o 14.o
gcc -c 15.c -o 15.o
gcc *.o -o ../hello
cd ..
./hello 2> err.txt
mv err.txt ../
cd ..
chmod "u=rw" err.txt
chmod "g=r" err.txt
chmod "o=xr" err.txt
chmod "a=x" err.txt

x=1
y=1
if [ $# -eq 2 ]
then
x=$1
y=$2
elif [ $# -eq 1 ]
then
x=$1
fi
n=$(($x+$y))
sed -n "$np" err.txt >&2  
