#include<iostream>
#include<string.h>
#include<winsock2.h>
#include<ws2tcpip.h>
// #define _WINSOCK_DEPRECATED_NO_WARNINGS
// #define _CRT_SECURE_NO_WARNINGS
// #define NI_MAXHOST 1025


#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    // Initialize Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {

        cerr << "Cannot initialize winsock. Quitting !!" << endl;
        return 1;
    }

    //create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

    if (listening == INVALID_SOCKET) {

        cerr << "Cannot create a socket. Quitting !!" << endl;
        return 1;
    }

    //bind the socket to an ip address and port

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;  //could also use inet_pton
    bind(listening, (sockaddr*)&hint, sizeof(hint));

    //tell winsock the socket is for listening

    listen(listening, SOMAXCONN);

    //wait for a connection

    sockaddr_in client;
    int clientsize = sizeof(client);
    SOCKET clientsocket = accept(listening, (sockaddr*)&client, &clientsize);
    // if (clientsocket == INVALID_SOCKET) //Do something 
    // {

    // }
    char host[NI_MAXHOST]; // client remote name
    char service[NI_MAXSERV]; // servuce(i.e port ) the client is connect on  

    ZeroMemory(host, NI_MAXHOST); // same as  memset(host, 0, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        cout << host << "Connected on port " << service << endl;
    }
    else {

        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << "Connected on port " <<
            ntohs(client.sin_port) << endl; // ntohs = network to host short
    }

    //close listening socket
    closesocket(listening);

    //while loop: accept and echo message back to client
    char buf[4096];
    while (true) {
        ZeroMemory(buf, 4096);
        //wait for client to send data.

        int bytesReceived = recv(clientsocket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            cerr << " Error in recv(). Quitting " << endl;
            break;
        }
        if (bytesReceived == 0) {
            cout << " Client Disconnected. " << endl;
            break;
        }
        send(clientsocket, buf, bytesReceived + 1, 0);


        //echo message back to the client 
    }

    //close the socket
      
    closesocket(clientsocket);

    //Cleanup winsock

    WSACleanup();
}