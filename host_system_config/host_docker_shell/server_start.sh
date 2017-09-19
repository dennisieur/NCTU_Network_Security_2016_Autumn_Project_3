#!/bin/bash
./net.sh $1
port=$(($1+10000))
docker run -d -i --net=my-net$1 --name=Server$1 -v /home/dsns-server/Desktop/project3/Server:/home/student/:ro -v /proc/:/writable-proc/ -p $port:8888 32bit/ubuntu16-server:version1 /usr/bin/ncat -kl 8888 -ve /home/student/project3
#docker run -d -i --net=my-net$1 --name=Server$1 -v /home/dsns-server/Desktop/project3/Server:/home/student/:ro -v /proc/:/writable-proc/ -p $port:8888 32bit/ubuntu16-server:version1 /bin/bash
docker run -d -i --net=my-net$1 --name=Alice$1 -v /home/dsns-server/Desktop/project3/Alice:/home/Alice:ro 32bit/ubuntu16-alice:version1 /bin/bash -c "exec /usr/bin/python2.7 /home/alice/alice.py"
docker run -d -i --net=my-net$1 --name=Bob$1 32bit/ubuntu:xenial /bin/bash

