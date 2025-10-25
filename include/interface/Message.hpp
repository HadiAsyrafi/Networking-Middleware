#pragma once
#include <string>
#include <memory>

class Message {
public:
    explicit Message(std::string type) : type_(std::move(type)) {}
    virtual ~Message() = default;

    std::string type() const { return type_; }

private:
    std::string type_;
};

using MessagePtr = std::shared_ptr<Message>;
