
#include "server.hpp"

#include <iostream>
#include <memory>
#include <boost/asio/strand.hpp>

#include "session.hpp"

Server::Server (int threads) : ioc(threads), acceptor(ioc) {
	threadCount = threads;
	boost::beast::error_code ec;
	auto const address = boost::asio::ip::make_address("0.0.0.0");
	boost::asio::ip::tcp::endpoint endpoint(address, 8080);
	acceptor.open(endpoint.protocol(), ec);
	errorCodeCheck(ec);
	acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);
	errorCodeCheck(ec);
	acceptor.bind(endpoint, ec);
	errorCodeCheck(ec);
	acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
	errorCodeCheck(ec);
}

Server::~Server () {
	std::cout << "Server Server" << std::endl;
}

void Server::run () {
	doAccept();
	for(int x = 0; x < threadCount; ++x) {
		threadGroup.create_thread(
			[this, x] {
				std::cout << "Thread start " << x << std::endl;
				ioc.run();
				std::cout << "Thread end " << x << std::endl;
			}
		);
	}
}

void Server::stop () {
	ioc.stop();
}

void Server::join () {
	threadGroup.join_all();
}

void Server::doAccept () {
	acceptor.async_accept(boost::asio::make_strand(ioc), boost::beast::bind_front_handler(&Server::onAccept, shared_from_this()));
}

void Server::onAccept (boost::beast::error_code ec, boost::asio::ip::tcp::socket socket) {
	std::string remoteAddress = socket.remote_endpoint().address().to_string();
	std::cout << "Connection from " << remoteAddress << std::endl;
	errorCodeCheck(ec); // need to do something else so we don't exit
	std::make_shared<Session>(std::move(socket))->run();
	doAccept();
}

void Server::errorCodeCheck (boost::beast::error_code ec) {
	if (ec)	{
		std::cerr << ec.message() << std::endl;
		exit(1);
	}
}
