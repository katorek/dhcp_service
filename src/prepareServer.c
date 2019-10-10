#include "custom_function.h"


void createLeaseFileIfNotExists() {
   info_message("Creating file if not exists for lease info");
   FILE *p;
   p = fopen(LEASE_FILENAME, "rb+");
   if( p == NULL) p = fopen(LEASE_FILENAME, "wb");
}

int serverIpConfigured() {          
	int sock;
	struct ifreq ifr;

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        error_message("serverIpConfigured(): ");
        close(sock);
		return -1;
	}

	strncpy(ifr.ifr_name, USED_INTERFACE, 16);
	ifr.ifr_name[16 - 1] = 0;

	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
        error_message("serverIpConfigured(): IP not configured");
        close(sock);
        return -1;
	}
    return 0;
}

void setIp(int sfd, char* ip) {
    info_message("Setting IP");
    // int sfd;
    struct ifreq ifr;
    struct sockaddr_in* sin;

    // sfd = socket(PF_INET, SOCK_DGRAM, 0);
    strncpy(ifr.ifr_name, USED_INTERFACE, strlen(USED_INTERFACE) + 1);
    sin = (struct sockaddr_in*) &ifr.ifr_addr;
    memset(sin, 0, sizeof(struct sockaddr_in));
    sin->sin_family = AF_INET;
    sin->sin_port = 0;
    sin->sin_addr.s_addr = inet_addr(ip);
    ioctl(sfd, SIOCSIFADDR, &ifr);
    ioctl(sfd, SIOCGIFFLAGS, &ifr);
    ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
    ioctl(sfd, SIOCSIFFLAGS, &ifr);
}

void checkIfServerIpIsConfigured(int sockDesc) {
    if(serverIpConfigured < 0) setIp(sockDesc, SERVER_IP);
}

void prepareServer(int sockDesc) {
    createLeaseFileIfNotExists();
    checkIfServerIpIsConfigured(sockDesc);
}