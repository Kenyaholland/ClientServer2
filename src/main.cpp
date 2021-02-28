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

    //get length of host name for gethostname function
    struct hostent* host = gethostbyname(argv[1]);

    //ensure host exists
    if (!host) {
        printf("ERROR! No such host: %s\n", argv[1]);
        return 1;
    }

    //creating the TCP socket
    int tcp_client_socket;                                    //Socket descriptor
    tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0);      //Calling the socket function - args: socket domain, socket stream type, TCP protocol (default)

    //specify address and port of the remote socket
    struct sockaddr_in tcp_server_address;             //declaring a structure for the address
    tcp_server_address.sin_family = AF_INET;           //Structure Fields' definition: Sets the address family of the address the client would connect to
    tcp_server_address.sin_port = htons(atoi(argv[2]));        //Specify and pass the port number to connect - converting in right network byte order
    memcpy(&tcp_server_address.sin_addr.s_addr, host->h_addr_list[0], host->h_length);  //Connecting to whatever host name was inputted in command line

    //connecting to the remote socket
    int connection_status = connect(tcp_client_socket, (struct sockaddr *) &tcp_server_address, sizeof(tcp_server_address));     //params: which socket, cast for address to the specific structure type, size of address
    if (connection_status == -1){                                                                                         //return value of 0 means all okay, -1 means a problem
        printf(" Problem connecting to the socket! Sorry!! \n");
    }

    char tcp_server_response[2048];
    std::string href_link = "";
    std::string get_request_ending = "\r\n\r\n";

    do{
        //these strings are subject to change so they will stay in the loop
        std::string get_request_1 = "GET /";
        std::string get_request_2 = " HTTP/1.1\r\nHost: ";

        //If there is a next link then it will be added after "GET /"
        if(href_link != ""){
            get_request_1.append(href_link);
            href_link.clear();
        }

        //Adding the host name and spacings on get request ending
        get_request_2.append(host->h_name);
        get_request_2.append(get_request_ending);

        //combining the two lines
        std::string full_get_request = get_request_1 + get_request_2;
        //std::cout << full_get_request << std::endl;

        //Copying the new get_request to the char that will be passed into the send function
        char get_request[2048];
        strcpy(get_request, full_get_request.c_str());
        //printf("%s \n", get_request);

        //sending get request to server and receiving its response
        send(tcp_client_socket, get_request, sizeof(get_request), 0);
        recv(tcp_client_socket, &tcp_server_response, sizeof(tcp_server_response), 0);   // params: where (socket), what (string), how much - size of the server response, flags (0)
        printf("%s \r\n", tcp_server_response);

        //make a new string variable to store server response to use find function
        std::string server_response_string(tcp_server_response);

        //Find the link within the server response
        std::string find_link_start = "href=\"/";

        std::size_t found_link_start = server_response_string.find(find_link_start);
        if(found_link_start != std::string::npos){
            //this line will take the position from found_link_start and read the next 100 character into href_link
            href_link = server_response_string.substr(found_link_start + 7, 100);
        }

        //std::cout << "Link: " + href_link << std::endl;

    }while(true);

    //Output, as received from Server
    //printf("\n\n Server says: %s \n", tcp_server_response);

    //closing the socket
    close(tcp_client_socket);
    return 0;
}
