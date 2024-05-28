#include "websocket.h"

Websocket::Websocket()
    : resolver_(io_context_), ws_(io_context_) {
    // Resolve the WebSocket endpoint
    auto const results = resolver_.resolve(host_, port_);

    // Connect to the WebSocket server
    asio::connect(ws_.next_layer(), results.begin(), results.end());

    // Perform the WebSocket handshake
    ws_.handshake(host_, "/");
}

Websocket::~Websocket() {
    // Close the WebSocket connection
    ws_.close(websocket::close_code::normal);
    // Run the IO context to process the close operation
    io_context_.run();
}

json Websocket::getData() {
    // Read a message into our buffer
    beast::flat_buffer buffer;
    ws_.read(buffer);

    // The data is in the buffer
    std::string message = beast::buffers_to_string(buffer.data());

    // Parse the JSON data
    json data = json::parse(message);

    return data;
}