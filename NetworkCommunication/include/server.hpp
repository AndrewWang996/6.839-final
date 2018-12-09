#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}


namespace network_communication {


    void socket_send(char * data, uint size) {
        int sockfd, portno, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;



        portno = 11000;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            error("ERROR opening socket");
        server = gethostbyname("127.0.0.1");
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            exit(0);
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,
              (char *)&serv_addr.sin_addr.s_addr,
              server->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
            error("ERROR connecting");


        //std::cout << "COMPARE" << std::endl;

        //sprintf(buf, "%s", data);
        n = write(sockfd,data,(size_t)size);
        if (n < 0)
            error("ERROR writing to socket");
        close(sockfd);
        free(data);
        usleep(70000); //sleep for 70 ms

    }
}