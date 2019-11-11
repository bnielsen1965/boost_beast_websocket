
#include "session.hpp"

#include <iostream>
#include <boost/asio/dispatch.hpp>

Session::Session (boost::asio::ip::tcp::socket&& socket) : ws(std::move(socket)) {
	this->connected = false;
	this->connecting = true;
	this->ready = false;
}

Session::~Session () {
	std::cout << "Session destroyed" << std::endl;
}

void Session::run () {
	boost::asio::dispatch(ws.get_executor(), boost::beast::bind_front_handler(&Session::onRun, shared_from_this()));
}

void Session::onRun () {
	ws.set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));
	ws.set_option(boost::beast::websocket::stream_base::decorator(
		[] (boost::beast::websocket::response_type& res) {
			res.set(boost::beast::http::field::server, "example boost beast websocket server");
		}
	));
	ws.async_accept(boost::beast::bind_front_handler(&Session::onAccept, shared_from_this()));
}

void Session::onAccept (boost::beast::error_code ec) {
	this->connected = true;
	this->connecting = false;
	this->write("{ \"status\": \"connected\" }");
	doRead();
}

void Session::doRead () {
	ws.async_read(buffer, boost::beast::bind_front_handler(&Session::onRead, shared_from_this()));
}

void Session::onRead (boost::beast::error_code ec, std::size_t bytesTransferred) {
	this->ready = true; // TODO set via message handler
	boost::ignore_unused(bytesTransferred);
	if (ec == boost::beast::websocket::error::closed) {
		std::cout << "websocket closed" << std::endl;
		close();
		return;
	}
	if (ec == boost::asio::error::eof) {
		std::cout << "websocket eof" << std::endl;
		close();
		return;
	}
	if (ec == boost::asio::error::connection_reset) {
		std::cout << "websocket connection reset" << std::endl;
		close();
		return;
	}
	if (ec) {
		std::cout << "websocket error, " << ec.message() << std::endl;
		close();
		return;
	}
	std::cout << "websocket message: " << boost::beast::buffers_to_string(buffer.data()) << std::endl;
	write(boost::beast::buffers_to_string(buffer.data()));
	buffer.consume(buffer.size());
	doRead();
}

void Session::write (std::string msg) {
	auto msgp = std::make_shared<std::string>(msg);
	ws.async_write(boost::asio::buffer(*msgp), boost::beast::bind_front_handler(
		[msgp, self = shared_from_this()] (boost::beast::error_code ec, std::size_t bytesTransferred) {
			self->onWrite(ec,bytesTransferred);
		}
	));
}

void Session::onWrite (boost::beast::error_code ec, std::size_t bytesTransferred) {
	boost::ignore_unused(bytesTransferred);
	std::cout << "websocket wrote " << bytesTransferred << " bytes" << std::endl;
}

void Session::close () {
	boost::beast::error_code ec;
	ws.close(boost::beast::websocket::close_code::normal, ec);
	if (ec) {
		std::cout << "websocket close error: " << ec.message() << std::endl;
	}
	this->connected = false;
}

bool Session::isConnected () {
	return this->connected;
}

bool Session::isConnecting () {
	return this->connected;
}

bool Session::isReady () {
	return this->ready;
}
