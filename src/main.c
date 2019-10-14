#include "custom_function.h"

int main(int argc, char const *argv[]) {
    errno = 0;
    int sockDesc = createSocketBC();

    struct dhcp_msg *message;
    struct in_addr addr;
    prepareServer(sockDesc);
    
    info_message("Start dhcp service");


    while(1) {
       message = receivePacketDHCP(sockDesc);
       

        switch (message->option[2]) {
            case DHCP_INFORM:
                if (message -> hdr.op !=0 ){
                    info_message("Received DHCP_INFORM");
                    printMAC(message);
                    sendPacketDHCP(DHCP_IACK, sockDesc, message);
                    printf("Send DHCP_ACK\n");
                }
            break;
            
            case DHCP_RELEASE:
                info_message("Received DHCP_RELEASE");
                printMAC(message);

                addr.s_addr = message -> hdr.ciaddr;
                char* ipStr = inet_ntoa(addr);

                if(removeIpFromLeaseList(ipStr)) {
                    printf("%s removed from lease list\n", ipStr);
                }
            break;

            case DHCP_DISCOVER:
                info_message("Received DHCP_DISCOVER");
                printMAC(message);

                if((message->hdr.yiaddr = getIPForClient())) {
                    sendPacketDHCP(DHCP_OFFER, sockDesc, message);
                } else {
                    error_message("IP pool error. All IP are used!");
                }
            break;

            case DHCP_REQUEST:
                info_message("Received DHCP_REQUEST");
                printMAC(message);

                if(message-> hdr.ciaddr != 0) {
                    addr.s_addr = message->hdr.yiaddr = message -> hdr.ciaddr;
                    printf("Renew IP: %s\n", inet_ntoa(addr));
                    removeIpFromLeaseList(inet_ntoa(addr));
                } else {
                    addr.s_addr = message -> hdr.yiaddr = getIPForClient();
                    printf("Allocated IP: %s\n", inet_ntoa(addr));
                }
                writeIpLease(getLeaseDataFromDhcpHeader(message->hdr));
                sendPacketDHCP(DHCP_ACK, sockDesc, message);
            break;
        }
        
        free(message);
    }
    
    return 0;
}
