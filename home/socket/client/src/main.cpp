#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <string>
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

int main(int argc, char* argv[])
{
    //----------------------
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        wprintf(L"WSAStartup failed with error: %ld\n", iResult);
        return 1;
    }

    //----------------------
    // Create a SOCKET for connecting to server
    SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_port = htons(20131);

    //----------------------
    // Connect to server.
    iResult = connect(ConnectSocket, (SOCKADDR*)& addrServer, sizeof(addrServer));
    if (iResult == SOCKET_ERROR)
    {
        closesocket (ConnectSocket);
        printf("Unable to connect to server: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct SOCKET_PACK_T
    {
        int  MsgID;
        SOCKADDR_IN addrClient;
        char recvBuf[1024 * 60];
    };
    SOCKET_PACK_T stSocket = {0};
    /// char buf[1024 + 1];
    //以一个无限循环的方式，不停地接收输入，发送到server
    //// while (1)
    /// {
        /// int count = _read (0, buf, 1024);//从标准输入读入
        /// if (count <= 0)break;
        int count = sizeof(SOCKET_PACK_T);
        struct st
        {
            int i;
            char acNumber[16];
        };

        st  stTemp = {99, "4564sdfsd65"};
        memcpy_s(stSocket.recvBuf, 60 * 1024, &stTemp, sizeof(st));

        char acBuf[sizeof(SOCKET_PACK_T)] = {0};
        memcpy_s(acBuf, sizeof(SOCKET_PACK_T), &stSocket, sizeof(SOCKET_PACK_T));
        int sendCount, currentPosition = 0;
        while (count > 0 && (sendCount = send(ConnectSocket, acBuf + currentPosition, count, 0)) != SOCKET_ERROR)
        {
            count -= sendCount;
            currentPosition += sendCount;
        }
        /// if (sendCount == SOCKET_ERROR)break;

        char acRet[16] = {0};
        count = recv(ConnectSocket, acRet, 16, 0);
        if (count == 0 || count == SOCKET_ERROR) /// 被对方关闭或收到错误消息
        {
            closesocket(ConnectSocket);
            WSACleanup();
        }

        // 收到server应答
        if (0 == strcmp(acRet, "true")) system("pause");//return true;

        closesocket(ConnectSocket);
        WSACleanup();

    //}
    //结束连接

    return 0;
}

