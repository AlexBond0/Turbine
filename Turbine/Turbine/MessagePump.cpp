#include "MessagePump.h"




// ==================================================================
// MessageNode


MessageNode::MessageNode() {

	// add this to the MessagePump notify list
	MessagePump::AddReceiver(this->GetNotifyFunc());
}

// Wizardry to make onNotify calleable from the MessagePump
std::function<void(Message)> MessageNode::GetNotifyFunc()
{
	auto messageListener = [=](Message message) -> void {

		this->OnNotify(message);
	};

	return messageListener;
}

void MessageNode::Send(Message message)
{
	MessagePump::SendMessage(message);
}



// ==================================================================
// MessagePump

std::vector<std::function<void(Message)>> MessagePump::_receivers;
std::queue<Message> MessagePump::_messages;

// Add a new receiver to the message pump system
void MessagePump::AddReceiver(std::function<void(Message)> messageReceiver) {

	_receivers.push_back(messageReceiver);
}

// queue a message into the pump
void MessagePump::SendMessage(Message message) {

	_messages.push(message);
}

// release all messages to receivers
void MessagePump::Notify() {

	// run through all messages
	while (!_messages.empty()) {

		// cool std::function stuff to call the onMessage function
		for (auto iter = _receivers.begin(); iter != _receivers.end(); iter++) {

			(*iter)(_messages.front());
		}

		// remove message from pump
		_messages.pop();
	}
}