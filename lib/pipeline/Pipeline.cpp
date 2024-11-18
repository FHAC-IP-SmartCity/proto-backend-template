#include "Pipeline.h"
#include <Arduino.h>
#include <CBOR.h>
#include <CBOR_streams.h>
#include <time.h>

namespace cbor = ::qindesign::cbor;

uint8_t bytes[512]{0};
cbor::BytesPrint bp{bytes, sizeof(bytes)};

void PipeLine::init() {
    Serial.begin(115200);
}

void PipeLine::send(uint64_t id, const char* value, size_t size) {
    cbor::Writer cbor{bp};
    bp.reset();
    cbor.beginMap(2);
    const char* idText = "id";
    cbor.beginText(2);
    cbor.writeBytes(reinterpret_cast<const uint8_t*>(idText), 2);
    cbor.writeUnsignedInt(id);
    const char* valueText = "value";
    cbor.beginText(5);
    cbor.writeBytes(reinterpret_cast<const uint8_t*>(valueText), 5);
    cbor.beginText(size);
    cbor.writeBytes(reinterpret_cast<const uint8_t*>(value), size);
    Serial.write(bytes, bp.getIndex());
}

void PipeLine::send(uint64_t id, int64_t value) {
    cbor::Writer cbor{bp};
    bp.reset();
    cbor.beginMap(2);
    const char* idText = "id";
    cbor.beginText(2);
    cbor.writeBytes(reinterpret_cast<const uint8_t*>(idText), 2);
    cbor.writeUnsignedInt(id);
    const char* valueText = "value";
    cbor.beginText(5);
    cbor.writeBytes(reinterpret_cast<const uint8_t*>(valueText), 5);
    cbor.writeInt(value);
    Serial.write(bytes, bp.getIndex());
}

void PipeLine::send(uint64_t id, double value) {
    cbor::Writer cbor{bp};
    bp.reset();
    cbor.beginMap(2);
    const char* idText = "id";
    cbor.beginText(2);
    cbor.writeBytes(reinterpret_cast<const uint8_t*>(idText), 2);
    cbor.writeUnsignedInt(id);
    const char* valueText = "value";
    cbor.beginText(5);
    cbor.writeBytes(reinterpret_cast<const uint8_t*>(valueText), 5);
    cbor.writeDouble(value);
    Serial.write(bytes, bp.getIndex());
}

void PipeLine::println(const char* value) {
    if (PipeLine::isOpen()) {
        PipeLine::send(0, value, strlen(value));
    } else {
        Serial.println(value);
    }
}

bool PipeLine::ping(uint8_t timeout) {
    if (!this->isOpen()) {
        return false;
    }

    this->send(2, int64_t(0));

    unsigned long start = millis();
    while (Serial.available() <= 0 && millis() < start + timeout) {}

    if (Serial.available() <= 0) {
        return false;
    }

    uint8_t status = Serial.read();

    return status == 1;
}

bool PipeLine::isOpen() {
    return this->o;
}

void PipeLine::close() {
    if (this->o) {
        send(1, int64_t(1));
        this->o = false;
    }
}

bool PipeLine::tryOpen(uint8_t timeout) {
    if (this->isOpen()) {
        return true;
    }

    Serial.write("\0open\0", 6);

    unsigned long start = millis();
    while (Serial.available() <= 0 && millis() < start + timeout) {}

    if (Serial.available() <= 0) {
        return false;
    }

    uint8_t status = Serial.read();

    if (status == 1) {
        this->o = true;
    }

    return this->isOpen();
}

void PipeLine::open() {
    if (!this->o) {
        while(!this->tryOpen()) {
            delay(200);
        }
    }
}

PipeLine pipeline = PipeLine(false);