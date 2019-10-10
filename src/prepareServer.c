#include "custom_function.h"


void createLeaseFileIfNotExists() {
   info_message("Creating file if not exists for lease info");
   FILE *p;
   p = fopen(LEASE_FILENAME, "rb+");
   if( p == NULL) p = fopen(LEASE_FILENAME, "wb");
   close(p);
}

int serverIpConfigured() {          
	int sock;
	struct sockaddr_in sin;
	struct ifreq ifr;

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		return -1;
	}

	strncpy(ifr.ifr_name, USED_INTERFACE, 16);
	ifr.ifr_name[16 - 1] = 0;

	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
        return -1;
		error_message("Check if server`s IP is configured properly");
		exit_error("getMyIpAddr(): ioctl() error");
	}
    return 0;
}

void checkIfServerIpIsConfigured(int sockDesc) {
    if(serverIpConfigured != 0) set_ip(sockDesc, USED_INTERFACE, SERVER_IP, SERVER_IP);
}

void prepareServer(int sockDesc) {
    createLeaseFileIfNotExists();
    checkIfServerIpIsConfigured(sockDesc);
}

// help functions
int generic_ioctrlcall(int sockfd, u_long *flags, struct ifreq *ifr) {

    if (ioctl(sockfd, (long unsigned int)flags, &ifr) < 0) {
        exit_error(flags);
        // fprintf(stderr, "ioctl: %s\n", (char *)flags);
    }
    return 1;
}

int set_route(int sockfd, char *gateway_addr,  struct sockaddr_in *addr) {
    struct rtentry route;
    int err = 0;
    memset(&route, 0, sizeof(route));
    addr = (struct sockaddr_in*) &route.rt_gateway;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(gateway_addr);
    addr = (struct sockaddr_in*) &route.rt_dst;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr("0.0.0.0");
    addr = (struct sockaddr_in*) &route.rt_genmask;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr("0.0.0.0");
    route.rt_flags = RTF_UP | RTF_GATEWAY;
    route.rt_metric = 100;
    err = ioctl(sockfd, SIOCADDRT, &route);
    if ((err) < 0) {
        fprintf(stderr, "ioctl: %s\n",  "mahdi MOAHMMADI Error");
     return -1;
    }
    return 1;
}

int set_ip(int sockfd, char *iface_name, char *ip_addr, char *gateway_addr) {
    if(!iface_name) return -1;
    struct ifreq ifr;
    struct sockaddr_in sin;

    sin.sin_family = AF_INET;

    // Convert IP from numbers and dots to binary notation
    inet_aton(ip_addr,&sin.sin_addr.s_addr);

    /* get interface name */
    strncpy(ifr.ifr_name, iface_name, IFNAMSIZ);

    /* Read interface flags */
    generic_ioctrlcall(sockfd, (u_long *)"SIOCGIFFLAGS", &ifr);
    /*
    * Expected in <net/if.h> according to
    * "UNIX Network Programming".
    */
    #ifdef ifr_flags
    # define IRFFLAGS       ifr_flags
    #else   /* Present on kFreeBSD */
    # define IRFFLAGS       ifr_flagshigh
    #endif
    // If interface is down, bring it up
    if (ifr.IRFFLAGS | ~(IFF_UP)) {
        ifr.IRFFLAGS |= IFF_UP;
        generic_ioctrlcall(sockfd, (u_long *)"SIOCSIFFLAGS", &ifr);
    }
    // Set route
    set_route(sockfd, gateway_addr    ,  &sin);
    memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr)); 
    // Set interface address
    if (ioctl(sockfd, SIOCSIFADDR, &ifr) < 0) {
        exit_error("Cannot set IP address. ");
    }             
    #undef IRFFLAGS 

    return 0;
}