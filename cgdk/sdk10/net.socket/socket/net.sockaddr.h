//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*                       Ver 10.0 / Release 2019.12.11                       *
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

namespace CGDK
{
using u_char = unsigned char;
using u_short = unsigned short;
using u_long = unsigned long;
using u_llong = uint64_t;
using ADDRESS_FAMILY = u_short;
using SCOPE_ID = unsigned long;

struct in6_addr 
{
	union 
	{
		u_char  Byte[16];
		u_short Word[8];
		u_long	Dword[4];
		u_llong	Qword[2];
	} u;
};

struct in_addr 
{
	union 
	{
		struct 
		{
			u_char s_b1;
			u_char s_b2;
			u_char s_b3;
			u_char s_b4;
		} S_un_b;

		struct 
		{
			u_short s_w1;
			u_short s_w2;
		} S_un_w;

		u_long S_addr;
		u_char S_bytes[4];
	} S_un;
};

struct sockaddr_in 
{
	short          sin_family;
	u_short        sin_port;
	struct in_addr sin_addr;
	char           sin_zero[8];
};

struct sockaddr_in6
{
	ADDRESS_FAMILY sin6_family; // AF_INET6.
	u_short sin6_port;           // Transport level port number.
	u_long  sin6_flowinfo;       // IPv6 flow information.
	in6_addr sin6_addr;         // IPv6 address.
	union {
		u_long sin6_scope_id;     // Set of interfaces for a scope.
		SCOPE_ID sin6_scope_struct;
	};
};

constexpr u_short constexpr_htons(u_short _value) noexcept { return (_value << 8) | (_value >> 8); }

class net::sockaddr
{
public:
	constexpr sockaddr() noexcept : bytes_{ 0 } {}
	sockaddr(const TSharedPtr<FInternetAddr>& _sockaddr)
	{
		uint32_t addr;
		_sockaddr->GetIp(addr);
		sockaddr_v4.sin_addr.S_un.S_addr = addr;
		this->port(_sockaddr->GetPort());
		sockaddr_v4.sin_family = 2; // AF_INET
		length = sizeof(sockaddr_in);
	}

public:
			constexpr bool		empty() const noexcept								{ return length == 0;}
			constexpr bool		exist() const noexcept								{ return length != 0;}
			void				clear() noexcept									{ sockaddr_v6.sin6_family = 0; length = 0; }
			constexpr bool		is_v4() const noexcept								{ return sockaddr_v6.sin6_family == 2;} // AF_INET
			constexpr bool		is_v6() const noexcept								{ return sockaddr_v6.sin6_family == 23;} // AF_INET6
			constexpr bool		is_unspecified() const noexcept						{ return sockaddr_v6.sin6_family == 0;}
			constexpr bool		is_bind_any() const noexcept						{ return is_v4() ? sockaddr_v4.sin_addr.S_un.S_addr==0 : (is_v6() ? (sockaddr_v6.sin6_addr.u.Qword[0] == 0 && sockaddr_v6.sin6_addr.u.Qword[1] == 0) : false);}
			//constexpr bool		is_loopback() const noexcept						{ return is_v4() ? sockaddr_v4.address_.is_loopback() : (is_v6() ? sockaddr_v6.address_.is_loopback() : false);}
			//constexpr bool		is_multicast() const noexcept						{ return is_v4() ? sockaddr_v4.address_.is_multicast() : (is_v6() ? sockaddr_v6.address_.is_multicast() : false);}

			//constexpr bool		is_private() const noexcept							{ return is_v4() ? sockaddr_v4.address_.is_private() : (is_v6() ? sockaddr_v6.address_.is_private() : false);}
			//constexpr bool		is_public() const noexcept							{ return !is_private();}

			constexpr auto		family() const noexcept								{ return sockaddr_v6.sin6_family;}
			constexpr auto		port() const noexcept								{ return constexpr_htons(sockaddr_v6.sin6_port);}
			void				port(unsigned short _port) noexcept					{ sockaddr_v6.sin6_port = constexpr_htons(_port);}
			constexpr auto		size() const noexcept								{ return length;}
			TSharedPtr<FInternetAddr> to_FInternetAddr() noexcept					
			{
				TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(); 
				addr->SetIp(sockaddr_v4.sin_addr.S_un.S_addr);
				addr->SetPort(constexpr_htons(sockaddr_v6.sin6_port));
				return addr;
			}

			sockaddr& operator = (const TSharedRef<FInternetAddr>& _sockaddr)
			{
				uint32_t addr;
				_sockaddr->GetIp(addr);
				sockaddr_v4.sin_addr.S_un.S_addr = addr;
				this->port(_sockaddr->GetPort());
				sockaddr_v4.sin_family = 2; // AF_INET
				length = sizeof(sockaddr_in);
				return *this;
			}

public:
		union
		{
			sockaddr_in			sockaddr_v4;
			sockaddr_in6		sockaddr_v6;
			std::array<unsigned char, sizeof(sockaddr_in6)> bytes_;
		};
		int		length{ 0 };
};

// ----------------------------------------------------------------------------
// Structures
// ----------------------------------------------------------------------------
struct SEND
{
	SEND(net::io::Isender* _psender) { m_psender = _psender;}
	SEND(net::io::Isender& _psender) { m_psender = &_psender;}

	net::io::Isender*	m_psender;
};

struct SEND_TO
{
	SEND_TO(net::io::Isender_datagram* _psender, const net::sockaddr& _address) :m_psender_datagram(_psender), m_sockaddr(_address) {}
	SEND_TO(net::io::Isender_datagram& _psender, const net::sockaddr& _address) :m_psender_datagram(&_psender), m_sockaddr(_address) {}

	net::io::Isender_datagram*	m_psender_datagram;
	net::sockaddr				m_sockaddr;
};


}


