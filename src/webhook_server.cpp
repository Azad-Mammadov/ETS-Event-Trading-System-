#include "webhook_server.hpp" // Declares the start_webhook_server function and related functionality
#include "httplib.h"          // Lightweight HTTP server library for handling HTTP requests and responses
#include "event.hpp"          // Defines the Event class used to represent market or trading events
#include "event_queue.hpp"    // Provides the EventQueue class for managing and processing events

#include <iostream>           // For standard input/output operations (e.g., std::cout, std::cerr)
#include <nlohmann/json.hpp>  // JSON library for parsing and handling JSON data

using json = nlohmann::json;  // Alias for easier usage of the JSON library

void start_webhook_server(const std::string& secret) {
    httplib::Server svr;

    svr.Post("/webhook", [&](const httplib::Request& req, httplib::Response& res) {
        auto received_secret = req.get_header_value("X-Finnhub-Secret");

        if (received_secret != secret) {
            std::cerr << "Unauthorized webhook request.\n";
            res.status = 403;
            return;
        }

        try {
            json payload = json::parse(req.body);

            // You need to inspect payload format depending on what webhook you subscribe to
            if (payload.contains("symbol") && payload.contains("price")) {
                Event e;
                e.symbol = payload["symbol"];
                e.price = payload["price"];
                e.timestamp = std::to_string(std::time(nullptr));
                EventQueue::get_instance().push(e);

                std::cout << "Webhook received: " << e.symbol << " @ " << e.price << "\n";
            }

            res.status = 200;
        } catch (...) {
            std::cerr << "Failed to parse webhook payload\n";
            res.status = 400;
        }
    });

    std::cout << "[Webhook Server] Listening on port 8080...\n";
    svr.listen("0.0.0.0", 8080);
}

