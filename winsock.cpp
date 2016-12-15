struct Winsock
{
private:
	// ÍÀÑÒÐÎÉÊÈ
	#define DEFAULT_IP "127.0.1.0"
	static const ushort DEFAULT_PORT = 6881;
	static const ushort WINS_VERSION = 0x0101;
	static const uint   BUFFER_SIZE  = 1024 * 1024;
	static const uint   MAX_CLIENTS  = 8;

	char
		bufferRead[BUFFER_SIZE],
		bufferWrite[BUFFER_SIZE];
	uint
		bufferReadIndex  = 0,
		bufferWriteIndex = 0;

	// Client/server socket
	SOCKET Socket = INVALID_SOCKET;
	
	// Start winsock
	bool initialize()
	{
		// WSAStartup
		if (WSAStartup(WINS_VERSION, (WSADATA *)bufferRead) != NO_ERROR) {
			showError("WSAStartup");
			return false;
		}

		// Create socket
		if ((Socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			showError("socket");
			WSACleanup();
			return false;
		}

		return true;
	}

	// Show error
	void showError(string function)
	{
		showMessage("Error " + function + " " + str(WSAGetLastError()));
	}

public:
	// Clients sockets for server
	SOCKET clientSockets[MAX_CLIENTS] = { INVALID_SOCKET };

	// Ïàöàíû ãî, ÿ ñîçäàë
	bool startServer(ushort port = DEFAULT_PORT)
	{
		// Initialize WS
		if (!initialize()) {
			return false;
		}

		// Local address
		SOCKADDR_IN localhost;
		localhost.sin_family      = AF_INET;
		localhost.sin_port        = htons(port);
		localhost.sin_addr.s_addr = INADDR_ANY;
		
		// Bind socket
		if (bind(Socket, (sockaddr *)&localhost, sizeof(localhost)) == SOCKET_ERROR) {
			showError("bind");
			closeSocket();
			return false;
		}

		// Wait for clients
		if (listen(Socket, MAX_CLIENTS) == SOCKET_ERROR) {
			showError("listen");
			closeSocket();
			return false;
		}

		return true;
	}

	// Connect to server
	bool startClient(char *ip = DEFAULT_IP, ushort port = DEFAULT_PORT)
	{
		// Initialize WS
		if (!initialize()) {
			return false;
		}

		// Destination address
		SOCKADDR_IN destination;
		destination.sin_family      = AF_INET;
		destination.sin_port        = htons(port);
		destination.sin_addr.s_addr = inet_addr(ip);

		// Change "127.0.1.0" to 0x7F000100
		if (destination.sin_addr.s_addr == INADDR_NONE) {
			showMessage("Invalid address " + string(ip));
			WSACleanup();
			return false;
		}

		// Connect to server
		if (connect(Socket, (sockaddr *)&destination, sizeof(destination)) == SOCKET_ERROR) {
			showError("connect");
			closeSocket();
			return false;
		}

		return true;
	}

	// Close client/server socket
	void closeSocket()
	{
		closeSocket(Socket);
		WSACleanup();
	}

	// Close concrete socket
	void closeSocket(SOCKET &Socket)
	{
		const int SD_BOTH = 2;
		if (shutdown(Socket, SD_BOTH) == SOCKET_ERROR) {
			showError("shutdown");
		}

		while (recv(Socket, bufferRead, BUFFER_SIZE, 0) != SOCKET_ERROR);

		if (closesocket(Socket) == SOCKET_ERROR) {
			showError("closesocket");
		}
		Socket = INVALID_SOCKET;
	}

	// Accept new client 
	SOCKET acceptSocket()
	{
		timeval time;
		time.tv_sec = time.tv_usec = 0;

		fd_set readFDs;
		FD_ZERO(&readFDs);
		FD_SET(Socket, &readFDs);

		// If there is a new one
		int sel = select(0, &readFDs, NULL, NULL, &time);
		if (sel == SOCKET_ERROR) {
			showError("select");
			return INVALID_SOCKET;
		}

		if (sel > 0) {
			// Accept new one
			SOCKADDR_IN client_addr;
			int client_addr_size = sizeof(client_addr);

			SOCKET SocketNew = accept(Socket, (sockaddr *)&client_addr, &client_addr_size);
			if (SocketNew == INVALID_SOCKET) {
				showError("accept");
				return INVALID_SOCKET;
			}

			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (clientSockets[i] == INVALID_SOCKET) {
					return clientSockets[i] = SocketNew;
				}
			}
		}

		return INVALID_SOCKET;
	}

	/****************************************/
	/************** ÑÎÎÁÙÅÍÈß ***************/
	/****************************************/

	// Clear buffer
	inline void clearBuffer()
	{
		bufferWriteIndex = 0;
	}

	// Receive message from server/client
	int getMessage()
	{
		return getMessage(Socket);
	}

	// Receive message from concrete socket
	int getMessage(SOCKET Socket)
	{
		bufferReadIndex = 0;

		timeval time;
		time.tv_sec = time.tv_usec = 0;

		fd_set readSet;
		FD_ZERO(&readSet);
		FD_SET(Socket, &readSet);

		int n = select(Socket + 1, &readSet, NULL, NULL, &time);
		if (n == SOCKET_ERROR) {
			showError("select");
		}
		if (n > 0) {
			n = recv(Socket, bufferRead, BUFFER_SIZE, 0);
		}
		return n;
	}

	// Send message to server/client
	void sendMessage()
	{
		sendMessage(Socket);
	}

	// Send message to all clients
	void sendMessageAll()
	{
		for (int i = 0; i < MAX_CLIENTS; i++) {
			sendMessage(clientSockets[i]);
		}
	}

	// Send message to all clients except one
	void sendMessageAllExcept(SOCKET Socket)
	{
		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (clientSockets[i] != Socket) {
				sendMessage(clientSockets[i]);
			}
		}
	}

	// Send message to concrete socket
	void sendMessage(SOCKET Socket)
	{
		if (Socket != INVALID_SOCKET) {
			if (bufferWriteIndex > 0) {
				if (send(Socket, bufferWrite, bufferWriteIndex, 0) == SOCKET_ERROR) {
					showMessage("Error send " + str(WSAGetLastError()));
				}
			}
		}
	}

	/****************************************/
	/****************  READ  ****************/
	/****************************************/

	char read1() {
		return bufferRead[bufferReadIndex++];
	}

	short read2() {
		bufferReadIndex += 2;
		return *(short *)&bufferRead[bufferReadIndex - 2];
	}

	int read4() {
		bufferReadIndex += 4;
		return *(int *)&bufferRead[bufferReadIndex - 4];
	}

	float read4f() {
		bufferReadIndex += 4;
		return *(float *)&bufferRead[bufferReadIndex - 4];
	}

	char *readStr() {
		char *text = (char *)&bufferRead[bufferReadIndex];
		bufferReadIndex += strlen(text) + 1;
		return text;
	}

	/****************************************/
	/**************** WRITE *****************/
	/****************************************/

	void write1(char a) {
		bufferWrite[bufferWriteIndex++] = a;
	}

	void write2(short a) {
		for (char i = 0; i < 2; i++) {
			bufferWrite[bufferWriteIndex++] = *((char *)&a + i);
		}
	}

	void write4(int a) {
		for (char i = 0; i < 4; i++) {
			bufferWrite[bufferWriteIndex++] = *((char *)&a + i);
		}
	}

	void write4f(float a) {
		for (char i = 0; i < 4; i++) {
			bufferWrite[bufferWriteIndex++] = *((char *)&a + i);
		}
	}

	void writeStr(string text) {
		for (uchar i = 0, length = text.size(); i <= length; i++) {
			bufferWrite[bufferWriteIndex++] = text[i];
		}
	}

} winsock;