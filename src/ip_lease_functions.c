#include "custom_function.h"

struct lease** ipLeaseList = NULL;
int ipLeaseListSize;

int readIpLease(struct lease*** _ipLeaseList, int* countIp) {
    FILE* fp;
    char str[100];
    int count = 0;
    struct lease * ipLeaseListTmp[40];
    struct lease* ipLease;

    if((fp = fopen(LEASE_FILENAME, "r+"))) {
        while (feof(fp)) {
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