
#ifndef WSSERVER_SESSIONLIST
#define WSSERVER_SESSIONLIST

#include <vector>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include "session.hpp"

class SessionList : public std::enable_shared_from_this<SessionList> {
	public:
		SessionList ();
		~SessionList ();
		void createSession (boost::asio::ip::tcp::socket&& socket);
		void messageToSessions (std::string msg);

	private:
		std::vector<std::shared_ptr<Session>> sessionList;
		void sessionCleanUp ();
};

#endif
