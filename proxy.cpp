#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#define CLIENT_PORT 8080
#define SERVER_PORT 80
#define BUFFER_SIZE 4500

using namespace std;

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(CLIENT_PORT);

    // Binding socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "Server started at port " << CLIENT_PORT << ". Waiting for connections..." << endl;

    

    while(true) {
        //Receving the request from the client
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        char buffer[BUFFER_SIZE];

        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        

        cout << buffer << endl;

        //Sending the request to the server

        //Server-side socket creation
        int server_socket = 0;
        server_socket = socket(AF_INET, SOCK_STREAM, 0) ;
        if (server_socket == -1){
            cout << "Socket creation failed !" << endl ;
        }

        // Set server address details
        struct sockaddr_in server_address ;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(SERVER_PORT) ;

        

        if (bytes_received > 0) {

            buffer[bytes_received] = '\0';

            string request(buffer);

        
            if(request.substr(0, 3) != "GET"){
                continue;
            }

            size_t host_pos = request.find("Host:");
            if (host_pos != string::npos) {
                size_t end_pos = request.find("\r\n", host_pos);
                string host = request.substr(host_pos + 6, end_pos - host_pos - 6);
                

                
                
        
                hostent * record = gethostbyname(host.c_str());
                in_addr * s_address = (in_addr * )record->h_addr;
                server_address.sin_addr.s_addr = inet_addr(inet_ntoa(* s_address));
                int status = connect(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
                if (status == -1){
                    cout << "connection failed!" << endl;
                    return -1 ;
                }

                send(server_socket, request.c_str(), request.length(), 0);

                char buffer2[4096] = {0};
                string receivedData;
                int bytesReceived2 = 0;
                while ((bytesReceived2 = recv(server_socket, buffer2, 4096, 0)) > 0) {
                    receivedData.append(buffer2, bytesReceived2);
                }


                //string response(buffer2);

                size_t content_pos = receivedData.find("\r\n\r\n");
                size_t frog_pos = content_pos;

                while((frog_pos = receivedData.find("frog", frog_pos)) != string::npos){
                    receivedData.replace(frog_pos, 4, "fred");
                    frog_pos += 4;
                }

                frog_pos = content_pos;

                while((frog_pos = receivedData.find("Frog", frog_pos)) != string::npos){
                    receivedData.replace(frog_pos, 4, "Fred");
                    frog_pos += 4;
                }


                size_t jpg_pos = content_pos;

                while((jpg_pos = receivedData.find(".jpg", jpg_pos)) != string::npos){
                    size_t start_pos = receivedData.rfind("\"", jpg_pos);
                    string frogue = "http://pages.cpsc.ucalgary.ca/~jcleahy/Frogue.jpg\" ";
                    receivedData.replace(start_pos + 1, jpg_pos - start_pos + 5, frogue);

                    jpg_pos += start_pos + frogue.length();
                }

                cout << endl;
                cout <<receivedData << endl;
                
                if(send(client_socket, receivedData.c_str(), receivedData.size(), 0) == -1){
                    perror("Send back to client failed!");
                }


                
                


                


            }

            
        }
        
        close(server_socket);

    }
    
    close(client_socket);
    

    return 0;
}
