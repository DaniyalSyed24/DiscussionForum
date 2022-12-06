#include <iostream>

#include "TCPClient.h"

#define DEFAULT_PORT 12345

int main(int argc, char **argv)
{
	// Validate the parameters
	if (argc != 6) { // we have 5 parameters -> IP Address, Poster threads, Reader threads, Time and Throttle
		printf("usage: %s server-name|IP-address|Poster Threads|Reader Threads|Time|Throttle\n", argv[0]);
		return 1;
	}

	TCPClient client(argv[1], DEFAULT_PORT);
	std::string request;

	client.OpenConnection();

	do {
		request = "";
		std::cout << "Enter string to send to server or \"exit\" (without quotes to terminate): ";
		std::getline(std::cin, request);

		std::cout << "String sent: " << request << std::endl;
		std::cout << "Bytes sent: " << request.size() << std::endl;

		std::string reply = client.send(request);

		std::cout << "String returned: " << reply << std::endl;
		std::cout << "Bytes received: " << reply.size() << std::endl;
	} while (request != "exit" && request != "EXIT");

	client.CloseConnection();

	return 0;
}

// IMPORTED THREADPOOL CODE

#include <vector>
#include <chrono>
#include <string>

#include "ThreadPool.h"

int threadTask(int i)
{
	std::string str = "hello " + std::to_string(i) + "\n";
	std::cout << str;
	std::this_thread::sleep_for(std::chrono::seconds(3));
	str = "world " + std::to_string(i) + "\n";
	std::cout << str;
	return i * i;
}

int main()
{

	ThreadPool pool(4);
	std::vector< std::future<int> > results;

	for (int i = 0; i < 8; ++i) {
		results.emplace_back(
			pool.enqueue(threadTask, i)
		);
	}

	for (auto&& result : results)
	{
		std::string str = "result: " + std::to_string(result.get()) + "\n";
		std::cout << str;
	}

	std::cout << std::endl;

	return 0;
}
//////