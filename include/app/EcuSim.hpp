#pragma once

#include <thread>
#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>

#include <interface/MessageFactory.hpp>
#include "middleware/MessageBus.hpp"
#include "interface/Message.hpp"

class ECU {
protected:
    MessageBus&       m_messageBus;
    std::atomic<bool> m_running{false};
    std::thread       m_thread;

public:
    ECU(MessageBus& bus) : m_messageBus(bus) {}

    virtual ~ECU() {
        stop();
    }

    void start() {
        m_running = true;

        // Note: &ECU::run is a pointer to the virtual "member function pointer"
        // Note: "this" provides the object whose vtable to look at
        m_thread  = std::thread(&ECU::run, this);

        // Note: Functionally similar to:
        // m_thread = std::thread([this]() { this->run(); });
    }

    void stop() {
        m_running = false;
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

protected:
    virtual void run() = 0;
    virtual MessagePtr createMessage() = 0;
};

// Concrete Implementation
class RpmECU : public ECU {
public:
    RpmECU(MessageBus& bus) : ECU(bus), m_factory(std::make_unique<RpmMsgFactory>()) {}

protected:
    void run() override {
        while (m_running) {
            auto message = createMessage();
            m_messageBus.publish(message);
            std::this_thread::sleep_for(m_sleepTime);
        }
    }

    MessagePtr createMessage() override {
        return m_factory->factoryMethod();
    }

private:
    std::chrono::milliseconds      m_sleepTime{200 + (rand() % 501)};
    std::unique_ptr<RpmMsgFactory> m_factory;
};

// Concrete Implementation
class TempECU : public ECU {
public:
    TempECU(MessageBus& bus) : ECU(bus), m_factory(std::make_unique<TempMsgFactory>()) {}

protected:
    void run() override {
        while (m_running) {
            auto message = createMessage();
            m_messageBus.publish(message);
            std::this_thread::sleep_for(m_sleepTime);
        }
    }

    MessagePtr createMessage() override {
        return m_factory->factoryMethod();
    }

private:
    std::chrono::milliseconds       m_sleepTime{500 + (rand() % 301)};
    std::unique_ptr<TempMsgFactory> m_factory;
};

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

// Concrete Implementation
class DashboardECU : public ECU {
public:
    DashboardECU(MessageBus& bus) : ECU(bus) {}

    void subscribeTopic(const std::string& string) {
        m_messageBus.subscribe(string, m_subscriber);
    }

protected:
    void run() override {
        while (m_running) {
            // Infinite loop - do nothing for now
            if (m_subscriber->hasMessage()) {
                m_subscriber->print();
            }
        }
    }

    MessagePtr createMessage() override {
        // Not used by DashboardECU - just implementing interface
        return nullptr;
    }

private:
    std::shared_ptr<DashboardSubscriber> m_subscriber = std::make_shared<DashboardSubscriber>();
};
