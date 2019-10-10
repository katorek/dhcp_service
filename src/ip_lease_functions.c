#include "custom_function.h"

struct lease** ipLeaseList = NULL;
int ipLeaseListSize;

int readIpLease(struct lease*** _ipLeaseList, int* countIp) {
    FILE* fp;
    char str[100];
    int count = 0;
    struct lease * ipLeaseListTmp[40];
    struct lease* ipLease;
    printf("readIpLease\n");

    if((fp = fopen(LEASE_FILENAME, "r+"))) {
        printf("\t2\n");
        while (feof(fp)) {
            printf("\twhile\n");
            memset(str, 0 , sizeof(str));

            if((fgets(str, 1024, fp) != 0) && (strlen(str) > 16)){
                printf("\t\"%s\"\n", str);
                ipLease = (struct lease*) malloc(sizeof(struct lease));
                memset(ipLease, 0 , sizeof(struct lease));
                strcpy(ipLease -> ipaddr, strtok(str, ";"));
                strcpy(ipLease -> macaddr, strtok(NULL, ";"));
                strcpy(ipLease -> ttime, strtok(NULL, ";"));
                
                ipLeaseListTmp[count] = ipLease;
                count++;
            }
        }
    }

    printf("readIpLease_%d\n", count);

    fclose(fp);
    *_ipLeaseList = (struct lease**) malloc(sizeof(struct lease) * count);

    for(int i=0; i< count; ++i) {
        _ipLeaseList[0][i] = ipLeaseListTmp[i];
    }

    *countIp = count;
    return 1;

}

void freeIpLeaseList() {
    if(ipLeaseList) {
        for (int i = 0; i < ipLeaseListSize; i++) {
            free(ipLeaseList[i]);
        }

        free(ipLeaseList);
        ipLeaseListSize = 0;
        ipLeaseList = NULL;
    }
}

int writeIpLease(struct lease* ipLease) {
    if(checkBusyIp(ipLease -> ipaddr)) {
        error_message("writeIpLease(): IP is used");
        return 0;
    }

    FILE *fp;

    if(!(fp = fopen(LEASE_FILENAME, "a+"))) {
        error_message("writeIpLease(): Error opening file for appending");
        return 0;
    }

    time_t t;
    t = time(NULL);
    sprintf(ipLease -> ttime, "%ld", t);

    fprintf(fp, "%s;%s;%s\n", ipLease -> ipaddr, ipLease -> macaddr, ipLease -> ttime);
    fclose(fp);
    freeIpLeaseList();
    
    return 1;
}


int readStandardIpLeaseList() {
    return readIpLease(&ipLeaseList, &ipLeaseListSize);
}

int checkBusyIp(char *ip) {
    if(!ipLeaseList) {
        readStandardIpLeaseList();
    }

    for (int i = 0; i < ipLeaseListSize; i++)
    {
        if(!strcmp(ip, ipLeaseList[i]->ipaddr)) {
            return 1;
        }
    }
    return 0;
}

int removeIpFromLeaseList(char* ip) {
    readStandardIpLeaseList();
    if(ipLeaseListSize == 0) {
        return 1;
    }

    FILE* fp;

    if(!(fp = fopen(LEASE_FILENAME, "w+"))) {
        error_message("removeIpFromLeaseList(): Error open file for write");
        return 0;
    }

    time_t t = time(NULL);

    for (int i = 0; i < ipLeaseListSize; i++) {
        if(strcmp(ip, ipLeaseList[i]->ipaddr)) {
            sprintf(ipLeaseList[i]->ttime, "%ld", t);
            fprintf(fp, "%s;%s;%s\n", ipLeaseList[i]->ipaddr, ipLeaseList[i]->macaddr, ipLeaseList[i]->ttime);
        }
    }
    fclose(fp);
    freeIpLeaseList();

    return 1;
}

struct lease * getLeaseDataFromDhcpHeader(struct dhcp_header header)
{
    struct in_addr addr;
    struct lease* ipLease;
    
    ipLease = (struct lease*) malloc(sizeof(struct lease));
    memset(ipLease, 0, sizeof(struct lease));

    if (header.ciaddr != 0) {
        addr.s_addr = header.ciaddr;
    } else if (header.yiaddr != 0) {
        addr.s_addr = header.yiaddr;
    }

    strcpy(ipLease->ipaddr, inet_ntoa(addr));
    sprintf(ipLease->macaddr, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", 
        header.chaddr[0], 
        header.chaddr[1], 
        header.chaddr[2], 
        header.chaddr[3], 
        header.chaddr[4], 
        header.chaddr[5]
    );

    return ipLease;
}