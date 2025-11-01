#include "app/ConcreteSubscriber.hpp"
#include "interface/Message.hpp"
#include "interface/MessageFactory.hpp"
#include "middleware/MessageBus.hpp"

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
};
