#include "custom_function.h"

struct in_addr getMyIpAddr()
{          
	int sock;
	struct sockaddr_in sin;
	struct ifreq ifr;

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
			exit_error("getMyIpAddr(): socket() error");
	}

	strncpy(ifr.ifr_name, USED_INTERFACE, 16);
	ifr.ifr_name[16 - 1] = 0;

	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
			exit_error("getMyIpAddr(): ioctl() error");
	}

	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
	return sin.sin_addr;
}

uint32_t getIPForClient() {
	uint32_t startIpPool, endIpPool, currentIp;
	startIpPool = inet_network(IP_POOL_START);
	endIpPool = inet_network(IP_POOL_END);

	struct in_addr addr;
	char* strIp;

	for (currentIp = startIpPool; currentIp <= endIpPool; currentIp++) {
		addr.s_addr = ntohl(currentIp);
		strIp = inet_ntoa(addr);
		
		if(checkBusyIp(strIp)) continue;

		if(!checkBusyIp(strIp) && checkAvailableHost(strIp)) return addr.s_addr;
	}
	
	return 0;
}

void printMAC(struct dhcp_msg* packet) {
	struct dhcp_header header = packet->hdr;

	char mac[30];

	sprintf(mac, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", 
        header.chaddr[0], 
        header.chaddr[1], 
        header.chaddr[2], 
        header.chaddr[3], 
        header.chaddr[4], 
        header.chaddr[5]
    );

	printf("MAC [%s]\n", mac);
}