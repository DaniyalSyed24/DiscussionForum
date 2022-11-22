#include <iostream>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>

#include <string>

#include "config.h"
#include "TCPServer.h"
#include "TCPClient.h"
#include "RequestParser.h"

#define DEFAULT_PORT 12345

bool terminateServer = false;

void serverThreadFunction(TCPServer* server, ReceivedSocketData && data);

int main()
{
	TCPServer server(DEFAULT_PORT);

	ReceivedSocketData receivedData;

	std::vector<std::thread> serverThreads;

	std::cout << "Starting server. Send \"exit\" (without quotes) to terminate." << std::endl;

	while (!terminateServer)
	{
		receivedData = server.accept();

		if (!terminateServer)
		{
			serverThreads.emplace_back(serverThreadFunction, &server, receivedData);
		}
	}

	for (auto& th : serverThreads)
		th.join();

	std::cout << "Server terminated." << std::endl;

	return 0;
}

void serverThreadFunction(TCPServer* server, ReceivedSocketData && data)
{
	unsigned int socketIndex = (unsigned int) data.ClientSocket;

	do {
		server->receiveData(data, 0);

		if (data.request != "" && data.request != "exit" && data.request != "EXIT")
		{
			std::cout << "[" << socketIndex << "] Bytes received: " << data.request.size() << std::endl;
			std::cout << "[" << socketIndex << "] Data received: " << data.request << std::endl;

			// remove the reverse string reply and replace with the request parser.

			/* data.reply = data.request;
			std::reverse(data.reply.begin(), data.reply.end());

			server->sendReply(data); */
	

			// code from the request parser below//

			PostRequest post = PostRequest::parse(data.request);
			if (post.valid)
			{
				std::cout << "Post request: " << post.toString() << std::endl;
				std::cout << "Post topic: " << post.getTopicId() << std::endl;
				std::cout << "Post message: " << post.getMessage() << std::endl;
				continue;
			}
			server->sendReply(data);

			ReadRequest read = ReadRequest::parse(data.request);
			if (read.valid)
			{
				std::cout << "Read request" << read.toString() << std::endl;
				std::cout << "Read topic: " << read.getTopicId() << std::endl;
				std::cout << "Read post id: " << read.getPostId() << std::endl;
				continue;
			}
			server->sendReply(data);

			std::cout << "Unknown request: " << data.request << std::endl;
			std::cout << std::endl;

		}
		else if (data.request == "exit" || data.request == "EXIT")
		{
			std::cout << "[" << socketIndex << "] Data received: " << data.request << std::endl;
			std::cout << "[" << socketIndex << "] Exiting... Bye bye!" << std::endl;

			data.reply = data.request;
			server->sendReply(data);
		}
	} while (data.request != "exit" && data.request != "EXIT" && !terminateServer);

	if (!terminateServer && (data.request == "exit" || data.request == "EXIT"))
	{
		terminateServer = true;

		TCPClient tempClient(std::string("127.0.0.1"), DEFAULT_PORT);
		tempClient.OpenConnection();
		tempClient.CloseConnection();
	}

	server->closeClientSocket(data);
}