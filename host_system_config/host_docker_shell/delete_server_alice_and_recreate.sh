#!/bin/bash
docker ps -a | grep 'server' | awk '{print $1}' | sed '1d' | xargs docker rm -f
docker ps -a | grep 'alice' | awk '{print $1}' | sed '1d' | xargs docker rm -f

for((i=1;i<=72;i++))
do

port=$(($i+10000))
docker run -d -i --net=my-net$i --name=Server$i -v /home/dsns-server/Desktop/project3/Server:/home/student/:ro -p $port:8888 32bit/ubuntu16-server:version1 "/usr/bin/ncat -kl 8888 -ve /home/student/project3"
docker run -d -i --net=my-net$i --name=Alice$i -v /home/dsns-server/Desktop/project3/Alice:/home/Alice:ro 32bit/ubuntu16-alice:version1 /bin/bash
docker run -d -i --net=my-net$i --name=Bob$i 32bit/ubuntu:xenial /bin/bash

done
