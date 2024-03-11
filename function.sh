## standard
function fun_name() {
    body...
    return int_value;
}


## for example

#!/bin/bash
function func1() {
        zero=0
        para1=$1
        while [ $para1 -gt $zero ]
        do
                echo "para1=$para1"
                let para1=para1-1
        done
        return $para1
}

func1 10
echo "return value is $?"
