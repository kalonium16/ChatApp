#include <memory>

enum class RequestTypes : uint32_t
{
	ServerAccept,
	IsServerUp,
	GetOtherClients,
	MessageToClient,
	IsClientOnline

};