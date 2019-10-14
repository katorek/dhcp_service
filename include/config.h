#ifndef CONFIG_H
#define CONFIG_H

    // Time max val 255 in 0x00 format (1 hour = 3600 sec)
    #define TOTAL_TIME_3 0x00
    #define TOTAL_TIME_2 0x00
    #define TOTAL_TIME_1 0x00
    #define TOTAL_TIME_0 0x78

    #define USED_INTERFACE "enp0s9"

    #define DNS1_IP "8.8.8.8"
    #define DNS2_IP "8.8.8.9"

    #define IP_POOL_START "10.11.1.10"
    #define IP_POOL_END "10.11.1.20"
    // subnt mast 255.255.255.0
    #define SUBNET_IP "255.255.255.0"
    
    #define SERVER_IP "10.11.1.1"

    #define LEASE_FILENAME "dhcp.lease"

    #define MAX_MSG_LEN 512

#endif