#include "pch.h"
#include "core.h"

#define DEFAULT_CLIENT_PORT "194"
#define DEFAULT_SERVER_PORT "193"

int _InitWinsock();
int _ConnectSocket(SOCKET ConnectSocket, addrinfo* ptr);
SOCKET _CreateSocket(PCSTR pNodeName, _Out_ addrinfo** addr, PCSTR DesPort);

int ClientOpen(PCSTR pNodeName, PCSTR pDesPort)
{
	try
	{
		_InitWinsock();
		addrinfo* addr = nullptr;
		SOCKET socket = _CreateSocket(pNodeName, &addr, pDesPort);
		if (socket == 0) {
			throw std::invalid_argument("socket is invaild, maybe you input an wrong url");
		}
		_ConnectSocket(socket, addr);

		return 0;
	}
	catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	
}
int _InitWinsock()
{
	std::wcout << L"[Info]INetCore is loading...\n";
	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult) {
		printf("WSAStartup failed: %d\n", iResult);
		throw std::invalid_argument("mem:");
		return 0;
	}
	return 1;
}
SOCKET _CreateSocket(PCSTR pNodeName, _Out_ addrinfo** addr, PCSTR DesPort)
{
	
	int iResult;

	struct addrinfo* result = nullptr, * ptr = nullptr, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(pNodeName, DesPort, &hints, &result);
	if (iResult != 0) {
		std::wcerr << L"getaddrinfo failed: " << iResult << L"\n";
		WSACleanup();
		return 0;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 0;
	}

	*addr = ptr;
	return ConnectSocket;
}
int _ConnectSocket(SOCKET ConnectSocket, addrinfo* ptr) {
	int iResult;
	addrinfo* result = ptr;

	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		throw std::invalid_argument("Unable to connect to server!");
	}
	return 0;
}

int ServerOpen() {
	return 0;
}
