// Winsock
namespace Winsock {
	// Settings
	constexpr char*  DEFAULT_IP   = "127.0.1.0";
	constexpr ushort DEFAULT_PORT = 6881;
	constexpr ushort WINS_VERSION = 0x0101;
	constexpr uint   BUFFER_SIZE  = 1024 * 1024;

	// Read and write buffers
	char bufferRead[BUFFER_SIZE];
	char bufferWrite[BUFFER_SIZE];

	int bufferReadSize   = 0;
	int bufferReadIndex  = 0;
	int bufferWriteIndex = 0;

	// Client/server socket
	SOCKET serverSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;

	// Show error
	void showError(string function) {
		showMessage("Error " + function + " #" + to_string(WSAGetLastError()));
	}

	// Close concrete socket
	void closeSocket(SOCKET &Socket = clientSocket) {
		if (Socket != INVALID_SOCKET) {
			// Do not process server Socket
			if (Socket != serverSocket) {
				constexpr int SD_BOTH = 2;
				if (::shutdown(Socket, SD_BOTH) == SOCKET_ERROR) {
					showError("shutdown");
				}
			}

			while (recv(Socket, bufferRead, BUFFER_SIZE, 0) != SOCKET_ERROR);

			if (closesocket(Socket) == SOCKET_ERROR) {
				showError("closesocket");
			}

			Socket = INVALID_SOCKET;
		}
	}

	// Initialize
	void initialize() {
		// WSAStartup
		if (WSAStartup(WINS_VERSION, (WSADATA*)bufferRead) != NO_ERROR) {
			showError("WSAStartup");
		}
	}

	// Start winsock
	SOCKET createSocket() {
		// Create socket
		SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		if (Socket == INVALID_SOCKET) {
			showError("socket");
			return INVALID_SOCKET;
		}

		return Socket;
	}

	// Create server
	bool startServer(ushort port = DEFAULT_PORT) {
		// Initialize WS
		if (serverSocket != INVALID_SOCKET || clientSocket != INVALID_SOCKET) {
			return false;
		}
		if ((serverSocket = createSocket()) == INVALID_SOCKET)
			return false;

		// Local address
		SOCKADDR_IN localhost;
		localhost.sin_family = AF_INET;
		localhost.sin_port = htons(port);
		localhost.sin_addr.s_addr = INADDR_ANY;

		// Bind socket
		if (bind(serverSocket, (sockaddr*)&localhost, sizeof(localhost)) == SOCKET_ERROR) {
			showError("bind");
			// closeSocket(); // throws shutdown error also, after previous message

			if (closesocket(serverSocket) == SOCKET_ERROR) {
				showError("closesocket");
			}
			serverSocket = INVALID_SOCKET;

			return false;
		}

		// Wait for clients
		if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
			showError("listen");
			closeSocket();
			return false;
		}

		return true;
	}

	// Connect to server
	bool startClient(char* ip = DEFAULT_IP, ushort port = DEFAULT_PORT) {
		// Initialize WS
		if (clientSocket != INVALID_SOCKET)
			return false;
		if ((clientSocket = createSocket()) == INVALID_SOCKET)
			return false;

		// Destination address
		SOCKADDR_IN destination;
		destination.sin_family = AF_INET;
		destination.sin_port = htons(port);
		destination.sin_addr.s_addr = inet_addr(ip);

		// Change "127.0.1.0" to 0x7F000100
		if (destination.sin_addr.s_addr == INADDR_NONE) {
			showMessage("Invalid address " + string(ip));
			return false;
		}

		// Connect to server
		if (connect(clientSocket, (sockaddr*)&destination, sizeof(destination)) == SOCKET_ERROR) {
			showError("connect");
			closeSocket();
			return false;
		}

		return true;
	}

	// Shutdown server
	bool shutdown() {
		if (serverSocket == INVALID_SOCKET) {
			return false;
		}

		closeSocket(serverSocket);
		return true;
	}

	// Disconnect from server
	bool disconnect() {
		if (clientSocket == INVALID_SOCKET) {
			return false;
		}

		closeSocket(clientSocket);
		return true;
	}

	// Accept new client 
	SOCKET acceptSocket() {
		timeval time;
		time.tv_sec = time.tv_usec = 0;

		fd_set readFDs;
		FD_ZERO(&readFDs);
		FD_SET(serverSocket, &readFDs);

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

			SOCKET SocketNew = accept(serverSocket, (sockaddr*)&client_addr, &client_addr_size);
			if (SocketNew == INVALID_SOCKET) {
				showError("accept");
				return INVALID_SOCKET;
			}

			return SocketNew;
		}

		return INVALID_SOCKET;
	}

	/****************************************/
	/************** MESSAGES ****************/
	/****************************************/

	// Clear write buffer
	inline void clearBuffer() {
		bufferWriteIndex = 0;
	}

	// Check read buffer
	inline bool checkBuffer() {
		return bufferReadIndex < bufferReadSize;
	}

	// Receive message from concrete socket
	int getMessage(SOCKET Socket = clientSocket) {
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
		return bufferReadSize = n;
	}

	// Send message to concrete socket
	void sendMessage(SOCKET Socket = clientSocket) {
		if (Socket != INVALID_SOCKET) {
			if (bufferWriteIndex > 0) {
				if (send(Socket, bufferWrite, bufferWriteIndex, 0) == SOCKET_ERROR) {
					showError("send");
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
		return *(short*)&bufferRead[bufferReadIndex - 2];
	}

	int read4() {
		bufferReadIndex += 4;
		return *(int*)&bufferRead[bufferReadIndex - 4];
	}

	float read4f() {
		bufferReadIndex += 4;
		return *(float*)&bufferRead[bufferReadIndex - 4];
	}

	char* readStr() {
		char *text = (char*)&bufferRead[bufferReadIndex];
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
			bufferWrite[bufferWriteIndex++] = *((char*)&a + i);
		}
	}

	void write4(int a) {
		for (char i = 0; i < 4; i++) {
			bufferWrite[bufferWriteIndex++] = *((char*)&a + i);
		}
	}

	void write4f(float a) {
		for (char i = 0; i < 4; i++) {
			bufferWrite[bufferWriteIndex++] = *((char*)&a + i);
		}
	}

	void writeStr(string text) {
		for (uint i = 0, length = text.size(); i <= length; i++) {
			bufferWrite[bufferWriteIndex++] = (char)text[i];
		}
	}
}