#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <signal.h>

#include "server.hpp"

namespace SigHandler {
	std::function<void(int)> shutdownHandler;
	void signalHandler (int signum) { shutdownHandler(signum); }
}

int main()
{
	std::shared_ptr<Server> server(std::make_shared<Server>());
	SigHandler::shutdownHandler = [&] (int signum) {
		server->stop();
	};

	server->run();

	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = SigHandler::signalHandler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	server->join();
	std::cout << "Exit" << std::endl;
}
