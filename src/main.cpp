
//Basic TCP Client: socket() creation > connect() > receive() > display > close

#include <stdio.h>      //Standard library
#include <stdlib.h>     //Standard library
#include <sys/socket.h> //API and definitions for the sockets
#include <sys/types.h>  //more definitions
#include <netinet/in.h> //Structures to store address information
#include <unistd.h>     //close function
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <string.h>     /* memcpy, memset */

// https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response
int main(int argc, char **argv)
{
    int portno = atoi(argv[2]);
    char *host = argv[1];
    char message_fmt[128] = "GET /%s HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n";
    char send_buffer[512];
    char link_buffer[1028] = "";

    //creating the TCP socket
    int tcp_client_socket;                               //Socket descriptor
    tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0); //Calling the socket function - args: socket domain, socket stream type, TCP protocol (default)

    //lookup host
    struct hostent *server = gethostbyname(host);
    if (server == NULL)
    {
        printf("ERROR, no such host");
        return -1;
    };

    //specify address and port of the remote socket
    struct sockaddr_in tcp_server_address;       //declaring a structure for the address
    tcp_server_address.sin_family = AF_INET;     //Structure Fields' definition: Sets the address family of the address the client would connect to
    tcp_server_address.sin_port = htons(portno); //Specify and pass the port number to connect - converting in right network byte order
    memcpy(&tcp_server_address.sin_addr.s_addr, server->h_addr, server->h_length);

    //connecting to the remote socket
    int connection_status = connect(tcp_client_socket, (struct sockaddr *)&tcp_server_address, sizeof(tcp_server_address)); //params: which socket, cast for address to the specific structure type, size of address
    if (connection_status == -1)
    { //return value of 0 means all okay, -1 means a problem
        printf(" Problem connecting to the socket! Sorry!! \n");
    }

    bool lastPage = false;
    while (!lastPage)
    {
        // Send request
        snprintf(send_buffer, sizeof(send_buffer), message_fmt, link_buffer);
        send(tcp_client_socket, send_buffer, strlen(send_buffer), 0);

        // Get resp
        char tcp_server_response[2048];
        recv(tcp_client_socket, &tcp_server_response, sizeof(tcp_server_response), 0); // params: where (socket), what (string), how much - size of the server response, flags (0)
        char *newLink = strstr(tcp_server_response, "<a href=\"/");

        if (newLink != NULL)
        {
            memcpy(link_buffer, newLink + 10, 100);
            link_buffer[100] = 0;
            //Output, as received from Server
            // printf("%s\n", link_buffer);
        }
        else
        {
            char *begin = strstr(tcp_server_response, "<p>") + 3;
            char *end = strstr(tcp_server_response, "</p>");
            memcpy(link_buffer, begin, end - begin);
            link_buffer[end - begin] = 0;
            printf("%s\n", link_buffer);
            lastPage = true;
        }
    }

    //closing the socket
    close(tcp_client_socket);

    return 0;
}
