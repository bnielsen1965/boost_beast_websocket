
#ifndef WSSERVER_SESSION
#define WSSERVER_SESSION

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

class Session : public std::enable_shared_from_this<Session> {
	public:
		Session (boost::asio::ip::tcp::socket&& socket);
		~Session ();
		void run ();
		void onRun ();
		void onAccept (boost::beast::error_code ec);
		void doRead ();
		void onRead (boost::beast::error_code ec, std::size_t bytesTransferred);
		void write (std::string msg);
		void onWrite (boost::beast::error_code ec, std::size_t bytesTransferred);
		void close ();

	private:
		boost::beast::websocket::stream<boost::beast::tcp_stream> ws;
		boost::beast::flat_buffer buffer;
};

#endif
