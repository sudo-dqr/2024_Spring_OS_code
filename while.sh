## standard
while [ condition ]
do
...
done



## for exmaple
#!/bin/bash

i=100
zero=0

while [ $i -ge $zero ]
do
echo "i=$i"
let i=i-1
done
