#include <iostream>
#include <ts_net.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
	ServerTimeMessage,
	DisconnectAll
};



class CustomServer : public ts::net::server_interface<CustomMsgTypes>
{
public:
	CustomServer(uint16_t nPort) : ts::net::server_interface<CustomMsgTypes>(nPort)
	{

	}

protected:
	virtual bool OnClientConnect(std::shared_ptr<ts::net::connection<CustomMsgTypes>> client)
	{
		std::cout << "New Client Connect\n";
		ts::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);
		return true;
	}

	// Called when a client appears to have disconnected
	virtual void OnClientDisconnect(std::shared_ptr<ts::net::connection<CustomMsgTypes>> client)
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	// Called when a message arrives
	virtual void OnMessage(std::shared_ptr<ts::net::connection<CustomMsgTypes>> client, ts::net::message<CustomMsgTypes>& msg)
	{
		switch (msg.header.id)
		{
		case CustomMsgTypes::ServerPing:
		{
			std::cout << "[" << client->GetID() << "]: hey\n";

			// Simply bounce message back to client
			client->Send(msg);
		}
		break;

		case CustomMsgTypes::MessageAll:
		{
			std::cout << "[" << client->GetID() << "]: Message All\n";
			// Construct a new message and send it to all clients
			ts::net::message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::ServerMessage;
			msg << client->GetID();
			MessageAllClients(msg, client);

		}
		break;
		}
	}

	virtual void BroadCashTimer()
	{
		using boost::posix_time::ptime;
		using boost::posix_time::second_clock;
		using boost::gregorian::day_clock;
		ptime todayUtc(day_clock::universal_day(), second_clock::universal_time().time_of_day());
		ts::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerTimeMessage;
		msg << todayUtc;
		MessageAllClients(msg);
	}

	virtual void DisconnectAll()
	{
		ts::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::DisconnectAll;
		MessageAllClients(msg);
	}
};

int main()
{
	CustomServer server(60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}

	return 0;
}