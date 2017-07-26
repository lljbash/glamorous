#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "app.h"
#include "log.h"

using namespace glamorous;

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 3) {
        Log::error("ERROR, no port/database provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        Log::error("ERROR opening socket");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        Log::error("ERROR on binding");
        exit(1);
    }
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        Log::error("ERROR on accept");
        exit(1);
    }
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0) {
        Log::error("ERROR reading from socket");
        exit(1);
    }
    Log::info("Here is the message: %s\n", buffer);
    
    char temp[2] = {buffer[0], '\0'};
    int style = atoi(temp);
    std::string text_adj(&buffer[1]);
    int split = text_adj.find_first_of('#');
    std::string text = std::string(text_adj, 0, split);
    std::string adj = std::string(text_adj, split + 1);
    
    GlamorousApp app;
    app.initialize(argv[2]);
    std::string ret = app.transfer(text.c_str(), style, adj.c_str());
    bzero(buffer, 256);
    memcpy(buffer, ret.c_str(), ret.length());
    
    n = write(newsockfd, buffer, 256);
    if (n < 0) {
        Log::error("ERROR reading from socket");
        exit(1);
    }
    close(newsockfd);
    close(sockfd);
    return 0; 
}

