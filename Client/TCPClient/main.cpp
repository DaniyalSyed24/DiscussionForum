#include <iostream>
#include <vector>
#include <thread>
#include <ostream>
#include <chrono>
#include <iostream>
#include <string>
#include <map>

#include "TCPClient.h"
#include "RequestGenerator.h"
#include <list>

#define DEFAULT_PORT 12345


void randomFunction() {
	std::cout << "I just got executed!" << "\n";
}

std::string serverIP = "127.0.0.1";

std::multimap<int, int> totalMessagesPerPostThread;
std::multimap<int, int> totalMessagesPerReadThread;

void PostRequest(int threadId)
{
	//std::cout << "Thread " << threadId << " sent: " << std::endl;
	// open TCP connection
	TCPClient client(serverIP, DEFAULT_PORT);
	client.OpenConnection();

	int itr = 0;

	// generate POST requests for 1 second 10 times.
	auto startMain = std::chrono::high_resolution_clock::now();	// start timer of 10 seconds
	while (true)
	{
		RequestGenerator postGenerator;
		std::list<int> totalMessagesPerSecond;

		auto start = std::chrono::high_resolution_clock::now();	// start timer of 1 second
		while (true) // generate random strings for 1s
		{
			std::string randomString = postGenerator.generatePostRequest();
			client.send(randomString);

			totalMessagesPerPostThread.emplace(threadId, 1);
			totalMessagesPerSecond.push_back(1);	


			auto end = std::chrono::high_resolution_clock::now(); // Check if 1s has elapsed, if yes -> break
			std::chrono::duration<double, std::milli> elapsed = end - start;
			if (elapsed.count() >= 1000)
			{
				break;
			}
		}

		//std::cout << "Second " << itr << ": " << totalMessagesPerSecond.size() << "\n";
		itr++;

		auto endMain = std::chrono::high_resolution_clock::now(); // Check if 10s has elapsed, if yes -> break
		std::chrono::duration<double, std::milli> elapsedMain = endMain - startMain;
		if (elapsedMain.count() >= 10000)
		{
			break;
		}
	}
	client.CloseConnection();
}

void ReadRequest(int threadId)
{
	//std::cout << "Thread " << threadId << " sent: " << std::endl;
	// open TCP connection
	TCPClient client(serverIP, DEFAULT_PORT);
	client.OpenConnection();

	int itr = 0;

	// generate READ requests for 1 second 10 times.
	auto startMain = std::chrono::high_resolution_clock::now();	// start timer of 10 seconds
	while (true)
	{
		RequestGenerator readGenerator;
		std::list<int> totalMessagesPerSecond;

		auto start = std::chrono::high_resolution_clock::now();	// start timer of 1 second
		while (true) // generate random strings for 1s
		{
			std::string randomString = readGenerator.generateReadRequest();
			client.send(randomString);

			totalMessagesPerReadThread.emplace(threadId, 1);
			totalMessagesPerSecond.push_back(1);


			auto end = std::chrono::high_resolution_clock::now(); // Check if 1s has elapsed, if yes -> break
			std::chrono::duration<double, std::milli> elapsed = end - start;
			if (elapsed.count() >= 1000)
			{
				break;
			}
		}

		//std::cout << "Second " << itr << ": " << totalMessagesPerSecond.size() << "\n";
		itr++;

		auto endMain = std::chrono::high_resolution_clock::now(); // Check if 10s has elapsed, if yes -> break
		std::chrono::duration<double, std::milli> elapsedMain = endMain - startMain;
		if (elapsedMain.count() >= 10000)
		{
			break;
		}
	}
	client.CloseConnection();
}

int main(int argc, char** argv, int posters, int readers, int time, int throttle) // added parameters
{
	if (argc != 6) { // validate the parameters
		printf("usage: %s server-name|IP-address|Poster Threads|Reader Threads|Time|Throttle\n", argv[0]);
		return 1;
	}

	int numberOfPosterThreads = std::stoi(argv[2]);
	int numberOfReaderThreads = std::stoi(argv[3]);
	int totalSeconds = std::stoi(argv[4]);

	std::string request;

	//create vectors of threads
	std::vector< std::thread > posterThreadsVector;
	std::vector< std::thread > readerThreadsVector;

	//generate m poster threads and push them in the poster thread vector
	for (int i = 0; i < numberOfPosterThreads; i++)
	{
		posterThreadsVector.push_back(std::thread(PostRequest, i));
	}

	//generate n reader threads and push them in the reader thread vector
	for (int i = 0; i < numberOfReaderThreads; i++) {
		readerThreadsVector.push_back(std::thread(ReadRequest, i));
	}

	// Wait for all threads to finish
	for (auto& thread : posterThreadsVector) {
		thread.join();
	}
	for (auto& thread : readerThreadsVector) {
		thread.join();
	}

	// Print out the number of messages sent by each thread
	for (int i = 0; i < numberOfPosterThreads; i++){
		std::cout << "The average requests of POST Thread " << i << " is: " << totalMessagesPerPostThread.count(i)/10 << "\n";
	}
	// Print out the number of messages sent by each thread
	for (int i = 0; i < numberOfReaderThreads; i++){
		std::cout << "The average requests of READ Thread " << i << " is: " << totalMessagesPerReadThread.count(i) / 10 << "\n";
	}
	std::cout << "\n";

	int totalPosterRequests = 0;
	for (int i = 0; i < numberOfPosterThreads; i++){
		totalPosterRequests = totalPosterRequests + totalMessagesPerPostThread.count(i);
	}
	int averagePostReq = totalPosterRequests / numberOfPosterThreads;

	std::cout << "Total poster requests: " << totalPosterRequests << ".\n";
	std::cout << "Average requests per poster Thread: " << averagePostReq << ".\n";

	int totalReaderRequests = 0;
	for (int i = 0; i < numberOfReaderThreads; i++){
		totalReaderRequests = totalReaderRequests + totalMessagesPerReadThread.count(i);
	}
	int averageReadReq = totalReaderRequests / numberOfReaderThreads;

	std::cout << "Total reader requests: " << totalReaderRequests << ".\n";
	std::cout << "Average requests per reader Thread: " << averageReadReq << ".\n";

	int totalRequests = totalPosterRequests + totalReaderRequests;
	int averageReqPerThread = (averagePostReq + averageReadReq) / 2;

	std::cout << "Total requests: " << totalRequests << ".\n";
	std::cout << "Average resquests per thread: " << averageReqPerThread << ".\n";
	std::cout << "Average requests per thread per second:" << "TBD" << ".\n";


	do {
		/*
		request = "";
		std::cout << "Enter string to send to server or \"exit\" (without quotes to terminate): ";
		std::getline(std::cin, request);

		std::cout << "String sent: " << request << std::endl;
		std::cout << "Bytes sent: " << request.size() << std::endl;

		std::string reply = client.send(request);

		std::cout << "String returned: " << reply << std::endl;
		std::cout << "Bytes received: " << reply.size() << std::endl;
		*/

	} while (request != "exit" && request != "EXIT");

	return 0;
}
