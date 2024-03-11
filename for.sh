## standard
for 变量 in 值1 值2 值3 ...
do
...
done


for((初始值;循环控制条件;变量变化))
do
...
done


## for example

#!/bin/bash

for((j=0;j<100;j++))
do
echo "j=$j"
done
