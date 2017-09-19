import os
import socket, sys
import fcntl
import struct


def get_ip_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(s.fileno(), 0x8915, struct.pack('256s', ifname[:15]))[20:24])


alice_ip =  get_ip_address("eth0")
bob_ip = alice_ip[0:-1]+str('4')

ping_command = str('ping -c 10 ')+str(bob_ip)

message0 = "INITIALIZE"
server_address0 = (bob_ip, 54210)
sock0 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
i=0
while i<100:
    sock0.sendto(message0, server_address0)
    i += 1
sock0.close()

sock1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address1 = (alice_ip, 9527)
sock1.bind(server_address1)
sock1.listen(1)

while True:
    # Wait for a connection
    print >>sys.stderr, 'waiting for a connection'
    connection, client_address = sock1.accept()

    try:
        print >>sys.stderr, 'connection from', client_address

        # Receive the data in small chunks and retransmit it
        while True:
	    connection.sendall("Alice : Hi Bob! How's going on?\n")
            connection.sendall("[TA's whispering] Input your REAL STUDENT ID : ")
            data = connection.recv(16)
            print >>sys.stderr, 'received "%s"' % data
            if data:

                print >>sys.stderr, 'sending data back to the client'

                try:
                    arp_command = str('arp -a | grep \'')+str(bob_ip)+str('\'')
                    sarp_response = os.popen(arp_command, "r").readline().split()
                    target = sarp_response [ sarp_response.index('at')+1 ]
                    print target
                    target_dec = int(target.replace(':', ''), 16)
                except:                    
                    sock0 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                    i=0
                    while i<100:
                        sock0.sendto(message0, server_address0)
                        i += 1
                    sock0.close()
                    connection.sendall("[TA's MESSAGE] SERVER IS SOMETHING WRONG, PLEASE TRY AGAIN.\n")
                    connection.sendall("[TA's MESSAGE] THIS IS NOT THE PART OF PROJECT, YOU CAN CONNECT ALICE IMMEDIATELY\n")

		data =  data[0:-1]
                if data.isdigit():
                    flag = int(target_dec)*int(data)
                    print flag
                    message = str("FINAL FLAG = {") + str(flag) + str("}")

                    server_address2 = (bob_ip, 54210)
                    sock2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                    i = 0
                    while i<100:
                        sock2.sendto(message, server_address2)
                        i += 1
                    sock2.close()

                    connection.sendall("Bob, Secreat message has been send complete! BYE!\n")
		
                else :
                    connection.sendall("[ERROR] Wrong imput.\n")
                    continue;
			

            else:
                print >>sys.stderr, 'no more data from', client_address
                break
            
    finally:
        # Clean up the connection
        connection.close()
	
