#include <iostream>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <string>
#include <regex>

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

	std::multimap<std::string, std::string> MessagesList; // create a map to store all the POST IDs in first. In second store all the messages.

	do {
		server->receiveData(data, 0);

		if (data.request != "" && data.request != "exit" && data.request != "EXIT")
		{
			//std::cout << "[" << socketIndex << "] Bytes received: " << data.request.size() << std::endl;
			//std::cout << "[" << socketIndex << "] Data received: " << data.request << std::endl;

			// code from the request parser below
			PostRequest post = PostRequest::parse(data.request);
			if (post.valid)
			{
				//std::cout << "Post request: " << post.toString() << std::endl;
				//std::cout << "Post topic: " << post.getTopicId() << std::endl;
				//std::cout << "Post message: " << post.getMessage() << std::endl;

				std::string topicID = post.getTopicId();
				std::string message = post.getMessage();

				MessagesList.emplace(topicID, message);

				int j = MessagesList.count(topicID) - 1;
				data.reply = std::to_string(j);

				server->sendReply(data);
				continue;
			}

			ReadRequest read = ReadRequest::parse(data.request);
			if (read.valid)
			{
				//std::cout << "Read request" << read.toString() << std::endl;
				//std::cout << "Read topic: " << read.getTopicId() << std::endl;
				//std::cout << "Read post id: " << read.getPostId() << std::endl;

				std::string topicID = read.getTopicId();
				int postId = read.getPostId();
				int i = -1; // so the first is 0.

				for (auto iterator = MessagesList.begin(); iterator != MessagesList.end(); iterator++) { // iterate through the multimap to find the right ID. Within the ID, find the right message
					if (iterator->first == topicID) {
						i++;
						if (i == postId) {
							data.reply = iterator->second;
							break;
						}
					}
				}
				server->sendReply(data);
				continue;
			}

			CountRequest count = CountRequest::parse(data.request);
			if (count.valid)
			{
				//std::cout << "Count request: " << count.toString() << std::endl;
				//std::cout << "Count topic: " << count.getTopicId() << std::endl;

				data.reply = std::to_string(MessagesList.count(count.getTopicId()));

				server->sendReply(data);
				continue;
			}

			ListRequest list = ListRequest::parse(data.request);
			if (list.valid)
			{
				//std::cout << "List request: " << list.toString() << std::endl;

				std::string listOfMessages;

				for (auto i = MessagesList.begin(); i != MessagesList.end(); i = MessagesList.upper_bound(i->first)) {
					listOfMessages = listOfMessages + i->first + "#";
				}
				listOfMessages = listOfMessages.substr(0, listOfMessages.size() - 1);
				data.reply = listOfMessages;

				server->sendReply(data);
				continue;
			}
			server->sendReply(data);

			ExitRequest exitReq = ExitRequest::parse(data.request);
			if (exitReq.valid)
			{
				std::cout << "Exit request: " << exitReq.toString() << std::endl;

				server->sendReply(data);
				continue;
			}

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