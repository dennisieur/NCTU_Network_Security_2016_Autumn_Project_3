/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 */

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>

//#define DEST_MAC0   0x02
//#define DEST_MAC1   0x42
//#define DEST_MAC2   0x61
//#define DEST_MAC3   0x79
//#define DEST_MAC4   0xc5
//#define DEST_MAC5   0xbb

#define ETHER_TYPE  0x0800

#define DEFAULT_IF  "eth0"
#define BUF_SIZ     1024

int main(int argc, char *argv[])
{
    // for project 3
    char dest_mac[18];
    char *delim = ":";
    char *pch;

    int DEST_MAC0;
    int DEST_MAC1;
    int DEST_MAC2;
    int DEST_MAC3;
    int DEST_MAC4;
    int DEST_MAC5;
    //--------------

    char sender[INET6_ADDRSTRLEN];
    int sockfd, ret, i;
    int sockopt;
    ssize_t numbytes;
    struct ifreq ifopts;    /* set promiscuous mode */
    struct ifreq if_ip;     /* get ip addr */
    struct sockaddr_storage their_addr;
    uint8_t buf[BUF_SIZ];
    char ifName[IFNAMSIZ];

    /* Get interface name */
    if (argc == 3 ){
        strcpy(dest_mac, argv[1]);
        strcpy(ifName, argv[2]);
    }
    else{
        printf("[ERROR] Please follow the format : \n");
        printf("./etherdump [MY MAC ADDRESS with ':', e.g. 01:23:45:67:89:ab ] [INTERNET INTERFACE]\n");
        return 0;
    }

    pch = strtok(dest_mac,delim);
    if( pch != NULL)
        DEST_MAC0 = (int)strtol(pch, NULL, 16);
    pch = strtok(NULL, delim);
    if( pch != NULL)
        DEST_MAC1 = (int)strtol(pch, NULL, 16);
    pch = strtok(NULL, delim);
    if( pch != NULL)
        DEST_MAC2 = (int)strtol(pch, NULL, 16);
    pch = strtok(NULL, delim);
    if( pch != NULL)
        DEST_MAC3 = (int)strtol(pch, NULL, 16);
    pch = strtok(NULL, delim);
    if( pch != NULL)
        DEST_MAC4 = (int)strtol(pch, NULL, 16);
    pch = strtok(NULL, delim);
    if( pch != NULL)
        DEST_MAC5 = (int)strtol(pch, NULL, 16);

    printf("YOUR MAC ADDRESS: ");
    printf("%x:", DEST_MAC0);
    printf("%x:", DEST_MAC1);
    printf("%x:", DEST_MAC2);
    printf("%x:", DEST_MAC3);
    printf("%x:", DEST_MAC4);
    printf("%x\n", DEST_MAC5);

    /* Header structures */
    struct ether_header *eh = (struct ether_header *) buf;
    struct iphdr *iph = (struct iphdr *) (buf + sizeof(struct ether_header));
    struct udphdr *udph = (struct udphdr *) (buf + sizeof(struct iphdr) + sizeof(struct ether_header));

    memset(&if_ip, 0, sizeof(struct ifreq));

    /* Open PF_PACKET socket, listening for EtherType ETHER_TYPE */
    if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) == -1) {
        perror("listener: socket"); 
        return -1;
    }

    /* Set interface to promiscuous mode - do we need to do this every time? */
    strncpy(ifopts.ifr_name, ifName, IFNAMSIZ-1);
    ioctl(sockfd, SIOCGIFFLAGS, &ifopts);
    ifopts.ifr_flags |= IFF_PROMISC;
    ioctl(sockfd, SIOCSIFFLAGS, &ifopts);
    /* Allow the socket to be reused - incase connection is closed prematurely */
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1) {
        perror("setsockopt");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    /* Bind to device */
    if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ-1) == -1)  {
        perror("SO_BINDTODEVICE");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

repeat: printf("listener: Waiting to recvfrom...\n");
    numbytes = recvfrom(sockfd, buf, BUF_SIZ, 0, NULL, NULL);
    printf("listener: got packet %lu bytes\n", numbytes);

    /* Check the packet is for me */
    if (eh->ether_dhost[0] == DEST_MAC0 &&
            eh->ether_dhost[1] == DEST_MAC1 &&
            eh->ether_dhost[2] == DEST_MAC2 &&
            eh->ether_dhost[3] == DEST_MAC3 &&
            eh->ether_dhost[4] == DEST_MAC4 &&
            eh->ether_dhost[5] == DEST_MAC5) {
        printf("Correct destination MAC address\n");
    } else {
        printf("Wrong destination MAC: %x:%x:%x:%x:%x:%x\n",
                        eh->ether_dhost[0],
                        eh->ether_dhost[1],
                        eh->ether_dhost[2],
                        eh->ether_dhost[3],
                        eh->ether_dhost[4],
                        eh->ether_dhost[5]);
        ret = -1;
        goto done;
    }

    /* Get source IP */
    ((struct sockaddr_in *)&their_addr)->sin_addr.s_addr = iph->saddr;
    inet_ntop(AF_INET, &((struct sockaddr_in*)&their_addr)->sin_addr, sender, sizeof sender);

    /* Look up my device IP addr if possible */
    strncpy(if_ip.ifr_name, ifName, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFADDR, &if_ip) >= 0) { /* if we can't check then don't */
        printf("Source IP: %s\n My IP: %s\n", sender, 
                inet_ntoa(((struct sockaddr_in *)&if_ip.ifr_addr)->sin_addr));
        /* ignore if I sent it */
        if (strcmp(sender, inet_ntoa(((struct sockaddr_in *)&if_ip.ifr_addr)->sin_addr)) == 0)  {
            printf("but I sent it :(\n");
            ret = -1;
            goto done;
        }
    }

    /* UDP payload length */
    ret = ntohs(udph->len) - sizeof(struct udphdr);

    /* Print packet */
    printf("\tData:");
    for (i=0; i<numbytes; i++) printf("%02x:", buf[i]);
    printf("\n");
//    printf(" GREAT!!! YOU HAVE DONE the ARP SPOOFING!!!!\n YOU CAN DELIVER THE FLAG WHAT YOU SNIFFER IN TCPDUMP!!!!\n");

done:   goto repeat;

    close(sockfd);
    return ret;

}
