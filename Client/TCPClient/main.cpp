#include <iostream>
#include <vector>
#include <thread>
#include <ostream>
#include <chrono>
#include <iostream>

#include <iostream>
#include <chrono>

#include "TCPClient.h"
#include <future>

#define DEFAULT_PORT 12345



//void randomFunction() {
//	cout << "I just got executed!" << "\n";
//}


// generate a random number (0-9) to use as an index for getting TopicIDs.
int randomNumberGenerator() {
	return rand() % 10;
}

std::string randomMessagesGenerator() {
	return std::to_string(rand());
}

int main(int argc, char **argv, int posters, int readers, int time, int throttle) // added parameters
{
	// Validate the parameters
	if (argc != 6) {
		printf("usage: %s server-name|IP-address|Poster Threads|Reader Threads|Time|Throttle\n", argv[0]);
		return 1;
	}

	TCPClient client(argv[1], DEFAULT_PORT);
	std::string request;

	client.OpenConnection();


	int numberOfPosterThreads = std::stoi(argv[2]);
	int numberOfReaderThreads = std::stoi(argv[3]);
	int totalSeconds = std::stoi(argv[4]);

	//create vectors of threads
	std::vector< std::thread > posterThreadsVector;
	std::vector< std::thread > readerThreadsVector;


	//generate m poster threads and push them in the poster thread vector
	for (int i = 0; i < numberOfPosterThreads; i++) {
		posterThreadsVector.push_back(std::thread());
	}

	//generate n reader threads and push them in the reader thread vector
	for (int i = 0; i < numberOfReaderThreads; i++) {
		readerThreadsVector.push_back(std::thread());
	}

	// TESTING
	//int test = posterThreadsVector.size();
	//int test2 = readerThreadsVector.size();

	//std::cout << "number of posters threads " << test << "\n";
	//std::cout << "number of reader threads: " << test2;


	//vector with Topic IDs
	std::vector<std::string> topicIDs{ "Cars", "Aliens", "Skydiving", "Basketball", "Fencing", "Gaming", "Travel", "Science", "AI", "Bikes" };

	/* ---------- POST REQUESTS BELOW ---------- */

	//vector to store all random messages
	std::vector<std::string> randomPostMessages;

	auto finish = std::chrono::system_clock::now() + std::chrono::seconds{ 1 };
	do {

		std::string request = "POST@"; //add request
		std::string requestTopic = request + topicIDs[randomNumberGenerator()] + "#"; //add topic
		std::string requestTopicMessage = requestTopic + randomMessagesGenerator(); //add message

		std::cout << requestTopicMessage << "\n";

		auto st = std::chrono::high_resolution_clock::now();

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
			);

	} while (std::chrono::system_clock::now() < finish);



	/* ---------- READ REQUESTS BELOW ---------- */



	//do {
	//	/*
	//	request = "";
	//	std::cout << "Enter string to send to server or \"exit\" (without quotes to terminate): ";
	//	std::getline(std::cin, request);

	//	std::cout << "String sent: " << request << std::endl;
	//	std::cout << "Bytes sent: " << request.size() << std::endl;

	//	std::string reply = client.send(request);

	//	std::cout << "String returned: " << reply << std::endl;
	//	std::cout << "Bytes received: " << reply.size() << std::endl;
	//	*/

	//} while (request != "exit" && request != "EXIT");

	client.CloseConnection();

	return 0;
}
