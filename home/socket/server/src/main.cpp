#include <winsock2.h>
#include <stdio.h>
#include <vector>
#include <memory>
#include <iostream>
#include <windows.h>
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

bool TCPSocketInit(SOCKET &ListenSocket)
{
    //----------------------
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        wprintf(L"WSAStartup failed with error: %ld\n", iResult);
        return false;
    }
    //----------------------
    // Create a SOCKET for listening for
    // incoming connection requests.
    /// SOCKET ListenSocket;
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET)
    {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return false;
    }
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_addr.s_addr = htonl(INADDR_ANY); //实际上是0
    addrServer.sin_port = htons(20131);

    //绑定套接字到一个IP地址和一个端口上
    if (bind(ListenSocket, (SOCKADDR*)& addrServer, sizeof (addrServer)) == SOCKET_ERROR)
    {
        wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return false;
    }

    //将套接字设置为监听模式等待连接请求
    //----------------------
    // Listen for incoming connection requests.
    // on the created socket
    if (listen(ListenSocket, 1) == SOCKET_ERROR)
    {
        wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return false;
    }

    return true;
}

bool SocketWaitConnect(SOCKET &ListenSocket, SOCKET& AcceptSocket, SOCKADDR_IN &addrClient)
{
    int len = sizeof(SOCKADDR);
    /// 以一个无限循环的方式，不停地接收客户端socket连接
    /// while (1)
    /// {
    //请求到来后，接受连接请求，返回一个新的对应于此次连接的套接字
    AcceptSocket = accept(ListenSocket, (SOCKADDR*)& addrClient, &len);
    if (AcceptSocket == INVALID_SOCKET) return false; //出错

    return true;
}

struct SOCKET_PACK_T
{
    // SOCKET_PACK_T() {}
    ~SOCKET_PACK_T()
    {
        std::cout << "SOCKET_PACK_T is deleted. Address is ";
        std::cout << inet_ntoa(addrClient.sin_addr) << std::endl;
        std::cout << recvBuf << std::endl;
    }
    int  MsgID;
    SOCKADDR_IN addrClient;
    char recvBuf[1024 * 60];
};

struct st
{
    int i;
    char acNumber[16];
};

bool TCPSocketRecv(SOCKET &ListenSocket, SOCKET& AcceptSocket, SOCKADDR_IN& addrClient, std::vector<std::shared_ptr<SOCKET_PACK_T>> &vstSocketPack)
{
    //接收缓冲区的大小是60K个字符
    SOCKET_PACK_T stRec = {0};
    char acBuf[sizeof(SOCKET_PACK_T)] = {0};

    int count = recv(AcceptSocket, acBuf, 50, 0);
    if (count == 0) return false;//被对方关闭
    if (count == SOCKET_ERROR)return false;//错误count<0

    memcpy_s(&stRec, sizeof(SOCKET_PACK_T), acBuf, sizeof(SOCKET_PACK_T));

    auto pstRecvPack = std::make_shared<SOCKET_PACK_T>(stRec);
    vstSocketPack.push_back(pstRecvPack);

    // 返回已经收到的消息
    int sendCount, currentPosition = 0;
    char acRecvBuf[16] = {0};
    strcpy_s(acRecvBuf, sizeof(acRecvBuf), "true");
    while (count > 0 && (sendCount = send(AcceptSocket, acRecvBuf + currentPosition, count, 0)) != SOCKET_ERROR)
    {
        count -= sendCount;
        currentPosition += sendCount;
    }
    /// 发送失败
    if (sendCount == SOCKET_ERROR); // break;

    //结束连接
    closesocket(AcceptSocket);
}

void TCPSocketClose(SOCKET &ListenSocket)
{
    closesocket(ListenSocket);
    WSACleanup();
}

int main(int argc, char* argv[])
{
    {
    SOCKET AcceptSocket;
    SOCKADDR_IN addrClient;
    SOCKET ListenSocket;
    std::vector<std::shared_ptr<SOCKET_PACK_T>> vstSocketPack;

    while (true)
    {
        TCPSocketInit(ListenSocket);
        SocketWaitConnect(ListenSocket, AcceptSocket, addrClient);
        TCPSocketRecv(ListenSocket, AcceptSocket, addrClient, vstSocketPack);
        TCPSocketClose(ListenSocket);
        break;
    }

    for (auto it : vstSocketPack)
    {
        // std::cout << " recv: " << it->recvBuf << std::endl;
        st stTemp = {0};
        memcpy_s(&stTemp, sizeof(stTemp), it->recvBuf, sizeof(stTemp));
        std::cout << stTemp.acNumber << "\t" << stTemp.i << std::endl;
    }

    }
    system("pause");

    return 0;
}
