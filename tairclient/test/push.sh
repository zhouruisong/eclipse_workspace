#!/bin/bash

count=10000;
name=prog1.mp4
ip=10.1.1.41
i=1

for i in `seq 1 $count`
do
    curl -d @pget http://127.0.0.1:8080/tair & 
    #sleep 0.02
    echo "--------------------$i\n"
done
echo "done\n"
