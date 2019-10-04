#include "custom_function.h"

int main(int argc, char const *argv[]) {
    int sockDesc = createSocketBC();

    struct dhcp_msg *message;
    struct in_addr addr;
    info_message("Start dhcp service");

    while(0) {
       message = receivePacketDHCP(sockDesc);

        switch (message->option[2]) {
            case DHCP_INFORM:
                if (message -> hdr.op !=0 ){
                    info_message("Received DHCP_INFORM");
                    sendPacketDHCP(DHCP_IACK, sockDesc, message);
                    printf("Send DHCP_ACK\n");
                }
            break;
            
            case DHCP_RELEASE:
                info_message("Received DHCP_RELEASE");

                addr.s_addr = message -> hdr.ciaddr;
                char* ipStr = inet_ntoa(addr);

                // todo remove from list
            break;

            case DHCP_DISCOVER:
                info_message("Received DHCP_DISCOVER");

                if((message->hdr.yiaddr = getIPForClient())) {
                    sendPacketDHCP(DHCP_OFFER, sockDesc, message);
                } else {
                    error_message("IP pool error. All IP are used!");
                }
            break;

            case DHCP_REQUEST:
                info_message("Received DHCP_REQUEST");

                if(message-> hdr.ciaddr != 0) {
                    addr.s_addr = message->hdr.yiaddr = message -> hdr.ciaddr;
                    printf("Renew IP: %s\n", inet_ntoa(addr));
                    // todo remove ip from lease list
                } else {
                    addr.s_addr = message -> hdr.yiaddr = getIPForClient();
                    printf("Allocated IP: %s\n", inet_ntoa(addr));
                }

                // todo write ips to list
                sendPacketDHCP(DHCP_ACK, sockDesc, message);
            break;
        }
        
        free(message);
    }
    
    return 0;
}