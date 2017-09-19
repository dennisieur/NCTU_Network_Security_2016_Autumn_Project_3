#!/bin/bash
docker ps -a | awk '{print $1}' | sed '1d' | xargs docker rm -f
