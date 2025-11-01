#pragma once

#include <thread>
#include <atomic>
#include <chrono>
#include <memory>

#include "app/EcuCommon.hpp"
#include "app/ConcreteSubscriber.hpp"
#include "interface/MessageFactory.hpp"
#include "middleware/MessageBus.hpp"

// Concrete Implementation
// Publish Only
class RpmECU : public ECU {
public:
    RpmECU(MessageBus& bus) : ECU(bus), m_factory(std::make_unique<RpmMsgFactory>()) {}

protected:
    void run() override {
        while (m_running) {
            auto msg = m_factory->factoryMethod();
            m_messageBus.publish(msg);
            std::this_thread::sleep_for(m_sleepTime);
        }
    }

private:
    std::chrono::milliseconds      m_sleepTime{200 + (rand() % 501)};
    std::unique_ptr<RpmMsgFactory> m_factory;
};

// Concrete Implementation
// Publish Only
class TempECU : public ECU {
public:
    TempECU(MessageBus& bus) : ECU(bus), m_factory(std::make_unique<TempMsgFactory>()) {}

protected:
    void run() override {
        while (m_running) {
            auto msg = m_factory->factoryMethod();
            m_messageBus.publish(msg);
            std::this_thread::sleep_for(m_sleepTime);
        }
    }

private:
    std::chrono::milliseconds       m_sleepTime{500 + (rand() % 301)};
    std::unique_ptr<TempMsgFactory> m_factory;
};

// Concrete Implementation
// Subscribe Only
class DashboardECU : public ECU {
public:
    DashboardECU(MessageBus& bus) : ECU(bus) {
        m_subscriber = std::make_shared<DashboardSubscriber>();
        m_messageBus.subscribe("RPM", m_subscriber);
        m_messageBus.subscribe("TEMP", m_subscriber);
        m_messageBus.subscribe("STATUS", m_subscriber);
    }

protected:
    void run() override {
        // Performance: Now its polling, consider doing interrupt-based
        while (m_running) {
            // Infinite loop - do nothing for now
            if (m_subscriber->hasMessage()) {
                m_subscriber->print();
            }
        }
    }

private:
    std::shared_ptr<DashboardSubscriber> m_subscriber;
};

// Concrete Implementation
// Publish & Subscribe
class EngineControlECU : public ECU {
public:
    EngineControlECU(MessageBus& bus) : ECU(bus) {
        m_subscriber = std::make_shared<LambdaSubscriber>([this](const MessagePtr& msg) {
            if (msg->getTopic() == "RPM") {
                m_currentRpm = std::stoi(msg->getValue());
            }
        });

        m_messageBus.subscribe("RPM", m_subscriber);
        m_factory = std::make_unique<StatusMsgFactory>();
    }

protected:
    void run() override {
        while (m_running) {
            auto statusMsg = m_factory->factoryMethod();
            m_messageBus.publish(statusMsg);

            // Publish every 2 seconds
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

private:
    std::shared_ptr<Subscriber>       m_subscriber;
    std::unique_ptr<StatusMsgFactory> m_factory;
    int                               m_currentRpm = 0;
};
