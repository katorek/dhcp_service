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