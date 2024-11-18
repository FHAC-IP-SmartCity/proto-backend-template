#pragma once

#include <Arduino.h>

class PipeLine {
    private:
        bool o;
    public:
        PipeLine(bool open): o{open} {
            PipeLine::init();
        }

        // Initializes the Pipe with the corresponding baud rate
        void init();

        // Sends a string into pipe
        void send(uint64_t id, const char* value, size_t size);

        // Sends an integer into pipe
        void send(uint64_t id, int64_t value);

        // Sends a double into pipe
        void send(uint64_t id, double value);

        // Print info to the proto or serial console.
        // If a connection is open, PipeLine::println()
        // will use the proto console
        void println(const char* value);

        // Check if proto is still listening
        // Takes longer than isOpen() due to RTT
        bool ping(uint8_t timeout = 50);

        // Checks internal state if the pipe is open
        // It could be, that the proto isn't listening anymore,
        // to recheck, use PipeLine::ping()
        bool isOpen();

        // Tries to open a proto session
        // Returns false if proto is not listening
        bool tryOpen(uint8_t timeout = 25);

        // Retries until successful to open a proto session
        void open();

        // Closes a proto session
        // The session can reopen with PipeLine::tryOpen()
        void close();
};

extern PipeLine pipeline;