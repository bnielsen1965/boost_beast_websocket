
#ifndef WSSERVER
#define WSSERVER

#define DEFAULT_THREADS 4

#include <boost/beast/core.hpp>
#include <boost/thread.hpp>
#include <vector>

#include "sessionlist.hpp"

class Server : public std::enable_shared_from_this<Server> {
	public:
		Server (int threads = DEFAULT_THREADS);
		~Server ();
		void run ();
		void stop ();
		void join ();

	private:
		void errorCodeCheck (boost::beast::error_code ec);
		void doAccept ();
		void onAccept (boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);

		int threadCount;
		boost::asio::io_context ioc;
		boost::asio::ip::tcp::acceptor acceptor;
		boost::thread_group threadGroup;
		SessionList sessionList;
};

#endif
