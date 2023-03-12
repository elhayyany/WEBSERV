#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
// struct addrinfo {
//     int              ai_flags;     /* AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST */
//     int              ai_family;    /* AF_INET, AF_INET6, AF_UNSPEC */
//     int              ai_socktype;  /* SOCK_STREAM, SOCK_DGRAM */
//     int              ai_protocol;  /* use 0 for "any" */
//     size_t           ai_addrlen;   /* size of ai_addr in bytes */
//     struct sockaddr *ai_addr;      /* struct sockaddr_in or _in6 */
//     char            *ai_canonname; /* full canonical hostname */

//     struct addrinfo *ai_next;      /* linked list, next node */
// };
int main()
{
    // int socket(int domain, int type, int protocol);
    struct addrinfo hints, *res;
    int status;
    char ipstr[INET6_ADDRSTRLEN];
    int sockfd;
    struct addrinfo *record;

    if(sockfd == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((status = getaddrinfo(NULL, "http", &hints, &res)) != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
    }
    for(record = res; record != NULL;  record = record->ai_next)
    {
        sockfd = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
        if(sockfd == -1)
            continue;
        if(connect(sockfd, record->ai_addr, record->ai_addrlen) != -1)
        {
            std::cout << "\033[1;32m" << "Connection established" << "\033[0m" << std::endl;
            break;
        }
        close(sockfd);
    }
    //listen from the socket
    char buffer[1024];
    int bytes_read = recv(sockfd, buffer, 1024, 0);
    std::cout << "Bytes read: " << bytes_read << std::endl;
    std::cout << "Data read: " << buffer << std::endl;
    std::cout << "SOCKET TYPE IS " << res->ai_socktype << std::endl;
    std::cout << "SOCKET FAMILY IS " << res->ai_family << std::endl;
    std::cout << "SOCKET PROTOCOL IS " << res->ai_protocol << std::endl;
    std::cout << "SOCKET ADDRESS LENGTH IS " << res->ai_addrlen << std::endl;

}