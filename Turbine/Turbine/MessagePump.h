#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

// A message container
struct Message {

	std::string tag;
	json message;
};

// The sender and receiver of messages
class MessageNode {

public:

	MessageNode();
	~MessageNode() {};

protected:

	std::function<void(Message)> GetNotifyFunc();
	void Send(Message message);

	// function to ovveride to handle the message
	virtual void OnNotify(Message message) {};

};

// The Pump which messages are passed through. only one of these pumps exists
class MessagePump {

public: 

	// Shouldn't be cloneable
	MessagePump(MessagePump& other) = delete;

	// Shouldn't be assignable
	void operator=(const MessagePump &) = delete;

	// static MessagePump *GetInstance(const std::string& value);

	static void AddReceiver(std::function<void(Message)> messageReceiver);
	static void SendMessage(Message message);
	static void Notify();

private:

	// Constructor not accesible from outside
	MessagePump() {};

	static std::vector<std::function<void(Message)>> _receivers;
	static std::queue<Message> _messages;
};