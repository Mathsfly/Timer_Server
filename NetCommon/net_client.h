#pragma once
#include "net_common.h"

namespace ts
{
	namespace net
	{
		template <typename T>
		class client_interface
		{
		public:
			client_interface()
			{}

			virtual ~client_interface()
			{
				// If the client is destroyed, always try and disconnect from server
				Disconnect();
			}

		public:
			// Connect to server with hostname/ip-address and port
			bool Connect(const std::string& host, const uint16_t port)
			{

			}

			// Disconnect from server
			void Disconnect()
			{

			}

			// Check if client is actually connected to a server
			bool IsConnected()
			{

			}


		protected:
			// asio context handles the data transfer...
			asio::io_context m_context;
			// ...but needs a thread of its own to execute its work commands
			std::thread thrContext;
			// The client has a single instance of a "connection" object, which handles data transfer
			std::unique_ptr<connection<T>> m_connection;

		private:
			// This is the thread safe queue of incoming messages from server
			tsqueue<owned_message<T>> m_qMessagesIn;
		};
	}
}