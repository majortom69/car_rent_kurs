
#ifndef WEBSOCKET_CLASS_H
#define WEBSOCKET_CLASS_H


#include <iostream>
#include <string>

#include <boost/beast/core/detail/base64.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

#include <nlohmann/json.hpp>
using json = nlohmann::json;



class Websocket {
public:
    Websocket();
    ~Websocket();
    json getData();

    void sendData(const json& data) {
        // Send JSON data to the server
        ws_.write(asio::buffer(data.dump()));
    }

private:
    asio::io_context io_context_;
    tcp::resolver resolver_;
    websocket::stream<tcp::socket> ws_;
    const std::string host_ = "localhost";
    const std::string port_ = "8080";
};

#endif