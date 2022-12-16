# DiscussionForum
Y3 Concurrent &amp; Parallel Systems

Implement a C++ server for a simple discussion forum geared towards high traffic and short messages. The server allows multiple clients to connect concurrently over the network, and to post and read messages on multiple topics.

Server (solution & executable provided)
	This is the provided multithreaded server where I have changed the code in main.cpp to make it my own.
	I also added the request parser in this solution.
	
	To start the server, dubble click on the TCPMultiThreaded.exe file or simply run it in Visual Studio by opening up the TCPServerMultithreaded.sln.

Client (solution & executable provided)
	This is the provided client where I have changed the main.cpp file to make it my own. I have also created a 
	RequestGenerated cpp and header file where I implemented the generator class.

	To start the client, open up the command promt and cd to the folder where the TCPClient.exe file is located.
	Then , type in the following: TCPClient.exe 127.0.0.1 2 3 10 0.

		- 1 is the number of poster threads (configurable)
		- 2 is the number of reader threads (configurable)
		- 10 is the number of seconds a thread stays alive (not configurable)
		- 0 is the throttle (not configurable)

The IP address of the computer on which the tests were taken: 10.72.84.36
The tests were conducted with my server and the reference test harness, as well as my own test harness.
