# NCTU Network Security Course 2016 Autumn Project 3,
# ARP Spoofing via Software Exploit
# Simple CTF like homework


## Requirement
* Environment : Ubuntu 16.04 32bits

* System setting
1. prepare a host install docker
2. create 3 container in host
    * Server
    * Alice
    * Bob
3. in container 'Server'
    1. put all file in host_system_config/machine_Server/* in container 'Server' unfer '/home/student/project3/'
    2. run server_config.sh
4. run host server
    * run server_start.sh (need to be modify docker_shell)

## Exercise
the file 'project3' is the same as our target file.
You can exploit 'project3' first.

Homework spec
===
NS_Project_3_SPEC.pdf

Homework simple spec
===
## Description
In this project, you need to do ARP spoofing and exploit the Format-String(FS) and Buffer-Overflow(BOF) vulnerabilities in the target machine.

## Part 1 - Software Exploit
1. format string vulnerability
2. buffer overflow
3. buffer overflow and using shell code

## Part 2 - ARP Spoofing
1. `nmap`
2. `arping`

## Flag format
FLAG = {xxxxxxxxxx}

Appendix - host_system_config/prepare_files
===
* project3.c
    * exploit target

* etherdump.c
    * ethernet dump

* recv_ether_proto.c
    * receive ether net packet

* shellcode_for_testing.c
    * testing wthether the shellcode is executable
