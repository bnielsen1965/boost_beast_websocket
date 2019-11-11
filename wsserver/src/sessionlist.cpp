
#include "sessionlist.hpp"
#include <iostream>

SessionList::SessionList () {}

SessionList::~SessionList () {}

void SessionList::createSession (boost::asio::ip::tcp::socket&& socket) {
	std::stringstream msg;
	msg << "{ \"message\": \"Client connected.\" }";
	messageToSessions(msg.str());

	this->sessionCleanUp();
	std::shared_ptr<Session> session = std::make_shared<Session>(std::move(socket));
	session->run();
	sessionList.push_back(session);
}

void SessionList::messageToSessions (std::string msg) {
	std::vector<std::shared_ptr<Session>>::iterator session;
	for (session = sessionList.begin(); session != sessionList.end(); ++session) {
		if ((*session)->isConnected() && (*session)->isReady()) {
			(*session)->write(msg);
		}
	}
}

void SessionList::sessionCleanUp () {
	int count = 0;
	std::vector<std::shared_ptr<Session>>::iterator session;
	for (session = sessionList.begin(); session != sessionList.end();) {// ++session) {
		if ((*session)->isConnected() || (*session)->isConnecting()) {
		++session;
		}
		else {
			session = sessionList.erase(session);
			++count;
		}
	}
	if (count) {
		std::stringstream msg;
		msg << "{ \"message\": \"Cleaned up " << count << " sessions.\" }";
		this->messageToSessions(msg.str());
	}
}
