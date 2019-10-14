#include "custom_function.h"

char* ptr;

extern struct settings* settings; 

void allocateMem() {
    settings = (struct settings*) malloc(sizeof(struct settings));
    settings->subnetmask = malloc (5 * sizeof(int));
    settings->serverip = malloc (5 * sizeof(int));
    settings->dns1 = malloc (5 * sizeof(int));
    settings->dns2 = malloc (5 * sizeof(int));
    info_message("Allocated memory");
}

void initDNSs() {
    int lenDNS1 = strlen(DNS1_IP);
    int lenDNS2 = strlen(DNS2_IP);

    char dns1[lenDNS1];
    strcpy(dns1, DNS1_IP);
    ptr = strtok(dns1, ".");
    for (int i = 0; i < 4; i++) {
        settings->dns1[i] = strtol(ptr, (char**) NULL, 10);
        ptr = strtok( NULL, ".");
    }

    char dns2[lenDNS2];
    strcpy(dns2, DNS2_IP);
    ptr = strtok(dns2, ".");
    for (int i = 0; i < 4; i++) {
        settings->dns2[i] = strtol(ptr, (char**) NULL, 10);
        ptr = strtok( NULL, ".");
    }

    info_message("initDNSs");
}

void initSubnet() {
    int length  = strlen(SUBNET_IP);

    char mask[length];
    strcpy(mask, SUBNET_IP);

    char* ptr = strtok(mask, ".");
    for (int i = 0; i < 4; i++) {
        settings->subnetmask[i] = strtol(ptr, (char**) NULL, 10);
        ptr = strtok( NULL, ".");
    }
    info_message("initSubnet");
}

void initServerIp() {
    int length = strlen(SERVER_IP);

    char serv[length];
    strcpy(serv, SERVER_IP);
    
    ptr = strtok(serv, ".");
    for (int i = 0; i < 4; i++) {
        settings->serverip[i] = strtol(ptr, (char**) NULL, 10);
        ptr = strtok( NULL, ".");
    }
    info_message("initServerIp");
}

void printIP(int *ip) {
    printf("%d %d %d %d\n", 
        *(ip+0),
        *(ip+1),
        *(ip+2),
        *(ip+3)
    );
}

void printSettings() {
    printIP(settings->serverip);
    printIP(settings->subnetmask);
    printIP(settings->dns1);
    printIP(settings->dns2);
}

void initSettings() {
    info_message("Initializing settings");
    allocateMem();
    
    initSubnet();
    initServerIp();
    initDNSs();
    printSettings();
}

