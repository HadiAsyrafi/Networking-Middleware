#pragma once

#include <memory>
#include <iostream>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>

#include "interface/Message.hpp"
#include "interface/PublisherSubscriber.hpp"

// Concrete Subscriber
class DashboardSubscriber : public Subscriber {
public:
    // Queue first - fast & non-blocking
    void onMessage(const MessagePtr& msg) override {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_messageQueue.push(msg);
    }

    bool hasMessage() const
    {
        return !m_messageQueue.empty();
    }

    void print()
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        if (!m_messageQueue.empty()) {
            auto msg = m_messageQueue.front();
            m_messageQueue.pop();
            std::cout << "Dashboard [" << msg->getTopic()
                << "]  \t:" << msg->getValue() << std::endl;
        }
    }

private:
    std::queue<MessagePtr> m_messageQueue;
    std::mutex             m_queueMutex;
    //std::condition_variable m_cv;     // Explore its use for blocking wait
};

// Concrete Subscriber
// Generic wrapper that allows creating subscribers without creating a dedicated class
class LambdaSubscriber : public Subscriber {
public:
    LambdaSubscriber(std::function<void(const MessagePtr&)> callback) : m_callback(callback) {}
    
    void onMessage(const MessagePtr& msg) override {
        m_callback(msg);
    }

private:
    std::function<void(const MessagePtr&)> m_callback;
};
