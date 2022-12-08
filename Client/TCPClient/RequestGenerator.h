#pragma once
#include <string>
#include <vector>

class RequestGenerator
{

public:
	RequestGenerator() {};
	~RequestGenerator() {};

	std::string generatePostRequest();
	std::string generateReadRequest();

private:
	// vector with Topic IDs
	std::vector<std::string> topicIDs{ "Cars", "Aliens", "Skydiving", "Basketball", "Fencing", "Gaming", "Travel", "Science", "AI", "Bikes" };
};