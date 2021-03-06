/**
 * @brief Main function for the program. Gets called like this
 * src/hackServer <hostname> <port>
 * where hostname is the hostname of the server to attack and 
 * port is the port to attack
 * 
 * @param argc number of arguments
 * @param argv values
 * @return int 0 if everything is fine
 */

#include <stdio.h>          //Standard library
#include <stdlib.h>         //Standard library
#include <sys/socket.h>     //API and definitions for the sockets
#include <sys/types.h>      //more definitions
#include <netinet/in.h>     //Structures to store address information
#include <unistd.h>         //close function
#include <netdb.h>          //gethostbyname
#include <string.h>         //memcpy
#include <string>

#include <iostream>

int main(int argc, char **argv)
{
    //Default port is 80 unless specified in argv[2]
    int port = 80;
    if(argv[2] != NULL){
        port = atoi(argv[2]);
    }

    //get host name in a way the socket can use
    struct hostent* server = gethostbyname(argv[1]);

    //ensure host exists
    if (server == NULL)      {
        printf("ERROR, no such host, code %i", errno);
        return errno;
    };             

    //creating the TCP socket
    int tcp_client_socket;                                    //Socket descriptor
    tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0);      //Calling the socket function - args: socket domain, socket stream type, TCP protocol (default)

    //specify address and port of the remote socket
    struct sockaddr_in tcp_server_address;             //declaring a structure for the address
    tcp_server_address.sin_family = AF_INET;           //Structure Fields' definition: Sets the address family of the address the client would connect to
    tcp_server_address.sin_port = htons(port);        //Specify and pass the port number to connect - converting in right network byte order
    memcpy(&tcp_server_address.sin_addr.s_addr, server->h_addr, server->h_length);  //Connecting to whatever host name was inputted in command line

    //connecting to the remote socket
    int connection_status = connect(tcp_client_socket, (struct sockaddr *) &tcp_server_address, sizeof(tcp_server_address));     //params: which socket, cast for address to the specific structure type, size of address
    if (connection_status == -1){                                                                                         //return value of 0 means all okay, -1 means a problem
        printf(" Problem connecting to the socket! Sorry!! \n");
    }

    char tcp_server_response[1024];
    char get_request[1024];
    char href_link[1024];
    char message[1024];

    do{
        //Clear chars to ensure old info will not be used
        memset(tcp_server_response, 0, sizeof(tcp_server_response));
        memset(get_request, 0, sizeof(get_request));

        //seperation of full get request to add reference link and host name
        std::string get_request_1 = "GET /";
        std::string get_request_2 = " HTTP/1.1\r\nHost: ";
        std::string get_request_ending = "\r\n\r\n";

        //Add the href link if there is one after "Get /"
        if(sizeof(href_link) != 0){
            get_request_1.append(href_link);
            //clear link to ensure old onfo wont be used
            memset(href_link, 0, sizeof(href_link));
        }

        //combining the two lines
        std::string full_get_request = get_request_1 + get_request_2 + server->h_name + get_request_ending;

        //Copying the new get_request to the char that will be passed into the send function
        strcpy(get_request, full_get_request.c_str());

        //sending get request to server and receiving its response
        send(tcp_client_socket, get_request, strlen(get_request), 0);
        recv(tcp_client_socket, &tcp_server_response, sizeof(tcp_server_response), 0);   // params: where (socket), what (string), how much - size of the server response, flags (0)

        //Find the link or server within the server response
        char find_link_start[50] = "href=\"/";
        char find_message_start[50] = "<p>";
        char find_message_end[50] = "</p>";

        //strstr points to beginning of string
        char* pointer_to_href;
        char* pointer_to_message_start;
        char* pointer_to_message_end;
        
        //find where href begins
        pointer_to_href = strstr(tcp_server_response, find_link_start);
        if(pointer_to_href != NULL){
            memcpy(href_link, pointer_to_href + 7, 100);
        }
        else{
            //Since there is no link, find message beginning and end pointers
            pointer_to_message_start = strstr(tcp_server_response, find_message_start);
            pointer_to_message_end = strstr(tcp_server_response, find_message_end);

            //get length of message
            int index_start = pointer_to_message_start - tcp_server_response;
            int index_end = pointer_to_message_end - tcp_server_response;
            int length_of_message = index_end - index_start - 2;

            //store message to char
            memcpy(message, pointer_to_message_start + 3, length_of_message);

            //To ensure no random characters will be added
            message[length_of_message - 1] = '\0';

            break;
        }

    }while(true);

    //Print message
    printf("%s \n", message);

    //closing the socket
    close(tcp_client_socket);
    return 0;
}