#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "Practical.h"

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int SetupTCPServerSocket(const char *service) {
  // Construct the server address structure
  struct addrinfo addrCriteria;                   // Criteria for address match
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
  addrCriteria.ai_family = AF_UNSPEC;             // Any address family
  addrCriteria.ai_flags = AI_PASSIVE;             // Accept on any address/port
  addrCriteria.ai_socktype = SOCK_STREAM;         // Only stream sockets
  addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol

  struct addrinfo *servAddr; // List of server addresses
  int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
  if (rtnVal != 0)
    DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

  int servSock = -1;
  for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
    // Create a TCP socket
    servSock = socket(addr->ai_family, addr->ai_socktype,
        addr->ai_protocol);
    if (servSock < 0)
      continue;       // Socket creation failed; try next address

    // Bind to the local address and set socket to listen
    if ((bind(servSock, addr->ai_addr, addr->ai_addrlen) == 0) &&
        (listen(servSock, MAXPENDING) == 0)) {
      // Print local address of socket
      struct sockaddr_storage localAddr;
      socklen_t addrSize = sizeof(localAddr);
      if (getsockname(servSock, (struct sockaddr *) &localAddr, &addrSize) < 0)
        DieWithSystemMessage("getsockname() failed");
      fputs("Binding to ", stdout);
      PrintSocketAddress((struct sockaddr *) &localAddr, stdout);
      fputc('\n', stdout);
      break;       // Bind and listen successful
    }

    close(servSock);  // Close and try again
    servSock = -1;
  }

  // Free address list allocated by getaddrinfo()
  freeaddrinfo(servAddr);

  return servSock;
}

int AcceptTCPConnection(int servSock) {
  struct sockaddr_storage clntAddr; // Client address
  // Set length of client address structure (in-out parameter)
  socklen_t clntAddrLen = sizeof(clntAddr);

  // Wait for a client to connect
  int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
  if (clntSock < 0)
    DieWithSystemMessage("accept() failed");

  // clntSock is connected to a client!

  fputs("Handling client ", stdout);
  PrintSocketAddress((struct sockaddr *) &clntAddr, stdout);
  fputc('\n', stdout);

  return clntSock;
}

void sendAll(int clntSocket, char* buffer, ssize_t len) {
  ssize_t sent_cnt = 0;
  while (sent_cnt < len) {
    ssize_t numBytesSent = send(
        clntSocket, buffer + sent_cnt, len - sent_cnt, 0);
    printf("Sent %d bytes.\n", (int)numBytesSent);
    if (numBytesSent < 0)
      DieWithSystemMessage("send() failed");
    else if (numBytesSent == 0)
      DieWithUserMessage("send()", "failed to send anything");
    sent_cnt += numBytesSent;
  }
}

ssize_t recvTillNull(int clntSocket, char* buffer, ssize_t buf_size) {
  ssize_t recv_cnt = 0;
  printf("Prepare to recv.\n");
  while (true) {
    ssize_t recv_cnt_prev = recv_cnt;
    ssize_t numBytesRcvd = recv(
        clntSocket, buffer + recv_cnt, buf_size - recv_cnt, 0);
    if (numBytesRcvd <= 0)
      DieWithSystemMessage("recv() failed");
    recv_cnt += numBytesRcvd;
    for (ssize_t i = recv_cnt_prev; i < recv_cnt; ++i) {

      if(buffer[i]=='\0'){
        return i;
      }
    }
  }
  return -1;
}

void ParseClientRequest(char* buffer){
  strtok();
}

void HandleTCPClient(int clntSocket, int N) {
  
  char buffer[BUFSIZE]; // Buffer for echo string

  // Receive message from client
  ssize_t numBytesRcvd = recvTillNull(clntSocket, buffer, BUFSIZE);
  if (numBytesRcvd <= 0)
    DieWithSystemMessage("recv() failed");
/*  
  HTTPMessage m = readRequest(sockfd);
  • HTTPRequest req = parseRequest(m)
  • HTTPResponse res = serveRequest(req)
  • returnResponse(sockfd, res);
*/





  // Send it back N times
  for (int i = 0; i < N; ++i) {
    sendAll(clntSocket, buffer, numBytesRcvd);
  }

  close(clntSocket); // Close client socket
}
