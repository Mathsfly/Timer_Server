#include <iostream>
#include <ts_net.h>


enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
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