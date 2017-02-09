#ifndef PRACTICAL_H_
#define PRACTICAL_H_

#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>

// Handle error with user msg
void DieWithUserMessage(const char *msg, const char *detail);
// Handle error with sys msg
void DieWithSystemMessage(const char *msg);
// Print socket address
void PrintSocketAddress(const struct sockaddr *address, FILE *stream);
// Test socket address equality
bool SockAddrsEqual(const struct sockaddr *addr1, const struct sockaddr *addr2);
// Create, bind, and listen a new TCP server socket
int SetupTCPServerSocket(const char *service);
// Accept a new TCP connection on a server socket
int AcceptTCPConnection(int servSock);
// Handle new TCP client
void HandleTCPClient(int clntSocket, int N);
// Create and connect a new TCP client socket
int SetupTCPClientSocket(const char *server, const char *service);

HttpMessage readRequest(int sockfd); //what is the logic inside readRequest?

HttpRequest parseRequest(HttpMessage m); // what logic?

HttpResponse serveRequest(HttpRequest req);

returnResponse(sockfd, res);

enum sizeConstants {
  MAXSTRINGLENGTH = 128,
  BUFSIZE = 8192,
  MAXHEADERS = 100,
};


// struct InitialLine{
// 	char* method[2] = "GET";
// 	char* uri;
// 	char* httpVersion[9] = "TritonHTTP"
// };
// typedef struct InitialLine InitialLine;

// struct HttpHeader{
// 	char* fieldName[MAXSTRINGLENGTH];
// 	char* fieldValue[MAXSTRINGLENGTH];
// };
// typedef struct HttpHeader HttpHeader;

struct HttpRequest{
	/* GET /images/myimg.jpg HTTP/1.1 */
	char * messageTokens[5];

	char * method = messageTokens[0];
	char * uri = messageTokens[1];
	char * httpVersion= messageTokens[2];
	char * host = messageTokens[4];
	
	//	HttpHeader headers[MAXHEADERS];
};
typedef struct HttpRequest HttpRequest;

struct HttpResponse{
	/* HTTP/1.1 200 OK */
	char * status;
	char * server;
	char * lastModified; 	// Last-Modified: <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT (required only if return type is 200)
	char * contentType; 	// (required if return type is 200; if you create a custom error page, you can set this to ‘text/html’)
	char * contentLength; // (required if return type is 200; if you create a custom error page, you can set this to the length of that page)
	
	//HttpHeader headers[MAXHEADERS];
	/*
	200 OK: The request was successful
	400 Client Error: The client sent a malformed or invalid request that the server doesn’t understand
	403 Forbidden: The request was not served because the client wasn’t allowed to access the requested content
	404 Not Found: The requested content wasn’t there
	500 Server Error: An error occured internal to the server. In this project we do not implement plugins, server-side scripting, or other extensions, and so you shouldn’t expect to use this return code
	*/
};
typedef struct HttpResponse HttpResponse;

struct HttpMessage{

//	uint16_t currRead;
	uint16_t numBytes;
	uint8_t error;
	char* buffer[BUFSIZE];
};
typedef struct HttpMessage HttpMessage;

#endif // PRACTICAL_H_
