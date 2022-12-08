#include "RequestGenerator.h"


int randomNumberGenerator() {
	return rand() % 10;
}

std::string randomMessagesGenerator() {
	return std::to_string(rand());
}


std::string RequestGenerator::generatePostRequest()
{
	std::string request = "POST@"; //add request
	std::string requestTopic = request + topicIDs[randomNumberGenerator()] + "#"; //add topic
	std::string requestTopicMessage = requestTopic + randomMessagesGenerator(); //add message

	return requestTopicMessage;
}

std::string RequestGenerator::generateReadRequest()
{
	std::string request = "READ@"; // add request
	std::string requestTopic = request + topicIDs[randomNumberGenerator()] + "#";
	std::string requestTopicMessage = requestTopic + std::to_string(randomNumberGenerator());

	return requestTopicMessage;
}
