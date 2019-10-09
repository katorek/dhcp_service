#ifndef CONFIG_H
#define CONFIG_H

    // Time max val 255 in 0x00 format (1 hour = 3600 sec)
    #define TOTAL_TIME_3 0x00
    #define TOTAL_TIME_2 0x00
    #define TOTAL_TIME_1 0x00
    #define TOTAL_TIME_0 0x78

    #define USED_INTERFACE "wlp2s0"

    #define IP_POOL_START "192.168.56.10"
    #define IP_POOL_END "192.168.56.20"
    // subnt mast 255.255.255.0
    #define SUBNET_1 0xff
    #define SUBNET_2 0xff
    #define SUBNET_3 0xff
    #define SUBNET_4 0x00

    #define LEASE_FILENAME "dhcp.lease"


    #define MAX_MSG_LEN 512

#endif