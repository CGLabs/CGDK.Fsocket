//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                          network socket classes                           *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*  This Program is programmed by Cho SangHyun. sangduck@cgcii.co.kr         *
//*  Best for Game Developement and Optimized for Game Developement.          *
//*                                                                           *
//*                (c) 2003. Cho Sanghyun. All right reserved.                *
//*                          http://www.CGCII.co.kr                           *
//*                                                                           *
//*****************************************************************************

#pragma once
//----------------------------------------------------------------------------
//
// definitions
//
//----------------------------------------------------------------------------
// 1) 
#define	CGDK_NET_SOCKET

// 2) 
#ifndef NDEBUG
	#define	_VALIDATE_SEND_MESSAGE
	#define	_VALIDATE_RECEIVE_MESSAGE
#endif

// 4) disable warning
#if defined(_MSC_VER)
	#pragma warning( disable : 4250)
#endif


//----------------------------------------------------------------------------
// prototypes
//----------------------------------------------------------------------------
namespace CGDK
{
	namespace net
	{
		class sockaddr;

		namespace io
		{
			class Isocket;
			class Iclosable;
			class Iaddress;
			class Iconnective;
			class Iconnectable;
			class Isendable;
			class Isend_request;
			class Isender;
			class Isender_datagram;
			class Isend_completor;
			class Ipacketable;

			namespace socket
			{
				class Ntcp;
				class Nudp;
			}

			namespace messageable
			{
				class Nbase;
				class base;
			}

			namespace sender
			{
				class Ndatagram;
				class Nstream;
				template <class,class> class Nheader;
				template <class> class Nstream_header;
			}

			namespace send_request
			{
				class Nbase;
			}

			namespace packetable
			{
				class Ndatagram;
				class Ndatagram_response;

				template <class> struct PRIMITIVE;
				template <class> class Nstream;
			}

			namespace statistics
			{
				struct TRAFFIC;
				struct TRAFFIC_INFO;
				class Nsocket;
			}

			class Nsocket_tcp;
			class Nsocket_tcp_client;
			class Nsocket_udp;
		}

		namespace socket
		{
			template <class, class> class tcp;
			template <class, class> class tcp_client;
			class udp;
		}
	}

	namespace executor
	{
		class socket;
	}
}

#include <vector>
#include <ostream>

//----------------------------------------------------------------------------
//
// head files
//
//----------------------------------------------------------------------------
// 1) UE4 socket header files
#include "Sockets.h"
#include "SocketSubSystem.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "IPAddress.h"

// 2) object system
#include "../sdk10/system.object.h"

// 3) definitions
#include "net.socket/socket/net.definitions.socket.protocol.h"
#include "net.socket/socket/net.definitions.socket.h"

// 4) ip
#include "net.socket/socket/net.sockaddr.h"

// 5) statistics & API
#include "net.socket/socket/net.statistics.h"

// 6) messages
#include "net.socket/socket/net.message.h"

// 7) interfaces
#include "net.socket/socket/net.io.Iaddress.h"
#include "net.socket/socket/net.io.Isocket.h"
#include "net.socket/socket/net.io.Iclosable.h"
#include "net.socket/socket/net.io.Iconnectable.h"
#include "net.socket/socket/net.io.Isendable.h"
#include "net.socket/socket/net.io.Isend_completor.h"
#include "net.socket/socket/net.io.Isend_request.h"
#include "net.socket/socket/net.io.Isender.h"
#include "net.socket/socket/net.io.Ipacketable.h"

// 8) socket executor
#include "net.socket/socket/executor.socket.h"


//----------------------------------------------------------------------------
// component Classes
//----------------------------------------------------------------------------
// 1) send requestor
#include "net.socket/socket/net.io.send_request.h"

// 2) sender
#include "net.socket/socket/net.io.sender.stream.h"
#include "net.socket/socket/net.io.sender.stream_header.h"
#include "net.socket/socket/net.io.sender.datagram.h"

// 3) packetable
#include "net.socket/socket/net.io.packetable.stream.h"
#include "net.socket/socket/net.io.packetable.datagram.h"

// 4) messageable
#include "net.socket/socket/net.io.messageable.h"

// 5) tcp/udp
#include "net.socket/socket/net.Nsocket_tcp.h"
#include "net.socket/socket/net.Nsocket_tcp_client.h"
#include "net.socket/socket/net.Nsocket_udp.h"


//----------------------------------------------------------------------------
// templates
//----------------------------------------------------------------------------
// 1) socket - tcp
#include "net.socket/socket/net.socket.tcp.h"
#include "net.socket/socket/net.socket.tcp_client.h"

// 2) socket - udp
#include "net.socket/socket/net.socket.udp.h"
