#pragma once

#include <string>
#include <memory>

#include <interface/Message.hpp>

/**
 * The MessageFactory class declares the factory method that is supposed to return an
 * object of a Message class. The MessageFactory's subclasses usually provide the
 * implementation of this method.
 */

class MessageFactory {
    /**
      * Note that the MessageFactory may also provide some default implementation of the
      * factory method.
      */
public:
    virtual ~MessageFactory(){};
    virtual MessagePtr factoryMethod() const = 0;

    /**
      * Also note that, despite its name, the MessageFactory's primary responsibility is
      * not creating products. Usually, it contains some core business logic that
      * relies on Message objects, returned by the factory method. Subclasses can
      * indirectly change that business logic by overriding the factory method and
      * returning a different type of product from it.
      */

    std::string getTopic() const {
        // Call the factory method to create a Message object.
        MessagePtr product = this->factoryMethod();

        // Now, use the product.
        std::string result = "MessageFactory: The same creator's code has just worked with " + product->getTopic();

        return result;
    }
};

/**
 * Concrete MessageFactorys override the factory method in order to change the
 * resulting product's type.
 */
class RpmMsgFactory : public MessageFactory {
public:
    MessagePtr factoryMethod() const override {
        int rpm = 1000 + rand() % 5000;
        return std::make_shared<RpmMessage>(rpm);
    }
};

class TempMsgFactory : public MessageFactory {
public:
    MessagePtr factoryMethod() const override {
        std::string status = "Engine Running";
        return std::make_shared<TempMessage>(status);
    }
};
