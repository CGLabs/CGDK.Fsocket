//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                                  Common                                   *
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
//-----------------------------------------------------------------------------
// definitions)
//-----------------------------------------------------------------------------
// 1) ...
using message_t = uint32_t;


//-----------------------------------------------------------------------------
// CODE
//-----------------------------------------------------------------------------
namespace CODE_TYPE
{
	constexpr message_t		HEAD_0		 = 0x00000000;		// Reserved for System
	constexpr message_t		HEAD_1		 = 0x00100000;		// Reserved for Application

	constexpr message_t		SUB_0		 = 0x00000000;
	constexpr message_t		SUB_1		 = 0x00010000;
	constexpr message_t		SUB_2		 = 0x00020000;
	constexpr message_t		SUB_3		 = 0x00030000;
	constexpr message_t		SUB_4		 = 0x00040000;
	constexpr message_t		SUB_5		 = 0x00050000;
	constexpr message_t		SUB_6		 = 0x00060000;
	constexpr message_t		SUB_7		 = 0x00070000;
	constexpr message_t		SUB_8		 = 0x00080000;
	constexpr message_t		SUB_9		 = 0x00090000;
	constexpr message_t		SUB_10		 = 0x000a0000;
	constexpr message_t		SUB_A		 = SUB_10;

	constexpr message_t		WINDOWS		 = (HEAD_0 | SUB_0);
	constexpr message_t		SYSTEM		 = (HEAD_0 | SUB_1);
	constexpr message_t		NETWORK		 = (HEAD_0 | SUB_2);
	constexpr message_t		SERVER		 = (HEAD_0 | SUB_3);
	constexpr message_t		SCRIPT		 = (HEAD_0 | SUB_4);
	constexpr message_t		RENDER		 = (HEAD_0 | SUB_5);
	constexpr message_t		UPDATE		 = (HEAD_0 | SUB_6);
	constexpr message_t		UI			 = (HEAD_0 | SUB_7);
	constexpr message_t		INIT		 = (HEAD_0 | SUB_8);
	constexpr message_t		INPUT		 = (HEAD_0 | SUB_9);   // Input용 메세지   [11/10/2016 BSH]
	constexpr message_t		USER		 = (HEAD_1 | SUB_A);

	constexpr message_t		TAIL_0		 = 0x00000000;
	constexpr message_t		TAIL_1		 = 0x00001000;
	constexpr message_t		TAIL_2		 = 0x00002000;
	constexpr message_t		TAIL_3		 = 0x00003000;
	constexpr message_t		TAIL_4		 = 0x00004000;
	constexpr message_t		TAIL_5		 = 0x00005000;
	constexpr message_t		TAIL_6		 = 0x00006000;
	constexpr message_t		TAIL_7		 = 0x00007000;
	constexpr message_t		TAIL_8		 = 0x00008000;
	constexpr message_t		TAIL_9		 = 0x00009000;
	constexpr message_t		TAIL_10		 = 0x0000a000;
	constexpr message_t		TAIL_A		 = TAIL_10;

	constexpr message_t		TAIL_SUB_0	 = 0x00000000;
	constexpr message_t		TAIL_SUB_1	 = 0x00000100;
	constexpr message_t		TAIL_SUB_2	 = 0x00000200;
	constexpr message_t		TAIL_SUB_3	 = 0x00000300;
	constexpr message_t		TAIL_SUB_4	 = 0x00000400;
	constexpr message_t		TAIL_SUB_5	 = 0x00000500;
	constexpr message_t		TAIL_SUB_6	 = 0x00000600;
	constexpr message_t		TAIL_SUB_7	 = 0x00000700;
	constexpr message_t		TAIL_SUB_8	 = 0x00000800;
	constexpr message_t		TAIL_SUB_9	 = 0x00000900;
	constexpr message_t		TAIL_SUB_10	 = 0x00000a00;
	constexpr message_t		TAIL_SUB_A	 = 0x00000a00;
}

// 2) ...
namespace eMESSAGE
{
	namespace SYSTEM
	{
		// 1) Message
		constexpr message_t	FLAG_RELIABLE			 = 0x80000000;
		constexpr message_t	MASK					 = 0x0fffffff;
		constexpr message_t	BUFFER					 = (0x00000000 | 0x00010000 | 0x00005000); // CODE_TYPE::SYSTEM(CODE_TYPE::HEAD_0 | CODE_TYPE::SUB_1) | CODE_TYPE::TAIL_5

		// 2) Message for network message
		constexpr message_t	NETWORK					 = BUFFER | 0x02000000;	// 0x02015000
		constexpr message_t	NETWORK_MASK			 = 0x0ffff000;
	}

	namespace NETWORK
	{
		constexpr message_t	COMMON = (CODE_TYPE::NETWORK | CODE_TYPE::TAIL_0);

		namespace SOCKET	{ constexpr message_t	BASE = (CODE_TYPE::NETWORK | CODE_TYPE::TAIL_1);}
		namespace P2P		{ constexpr message_t	BASE = (CODE_TYPE::NETWORK | CODE_TYPE::TAIL_2);}
		namespace UPDATE	{ constexpr message_t	BASE = (CODE_TYPE::NETWORK | CODE_TYPE::TAIL_3);}
		namespace SECURITY	{ constexpr message_t	BASE = (CODE_TYPE::NETWORK | CODE_TYPE::TAIL_4);}
		namespace ROOM		{ constexpr message_t	BASE = (CODE_TYPE::NETWORK | CODE_TYPE::TAIL_6);}
		namespace USER		{ constexpr message_t	BASE = (CODE_TYPE::NETWORK | CODE_TYPE::TAIL_7);}

		constexpr message_t	REQUEST_CLIENT_INFO		 = COMMON + 0;
		constexpr message_t	SEND_CLIENT_INFO		 = COMMON + 1;
		constexpr message_t	REQUEST_SERVER_INFO		 = COMMON + 2;
		constexpr message_t	SEND_SERVER_INFO		 = COMMON + 3;

		constexpr message_t	ACK						 = COMMON + 8;
		constexpr message_t	REQUEST_ACK				 = COMMON + 9;
		constexpr message_t	PING_REQ				 = COMMON + 10;
		constexpr message_t	PING_ACK				 = COMMON + 11;
		constexpr message_t	REQUEST_LOGIN			 = COMMON + 12;
		constexpr message_t	ACCEPT_LOGIN			 = COMMON + 13;
		constexpr message_t	REJECT_LOGIN			 = COMMON + 14;
		constexpr message_t	REQUEST_CERTIFY			 = COMMON + 15;
		constexpr message_t	RESPONSE_CERTIFY		 = COMMON + 16;

		namespace ROOM
		{
			constexpr message_t	COMMON = (ROOM::BASE | CODE_TYPE::TAIL_SUB_0);

			namespace CHANNEL { constexpr message_t	BASE = (ROOM::BASE | CODE_TYPE::TAIL_SUB_1); }
		}
	}

	namespace SERVER
	{
		constexpr message_t	COMMON = (CODE_TYPE::SERVER | CODE_TYPE::TAIL_0);

		namespace TIME_EVENT { constexpr message_t	BASE = (CODE_TYPE::SERVER | CODE_TYPE::TAIL_1);}
		namespace QUERY		 { constexpr message_t	BASE = (CODE_TYPE::SERVER | CODE_TYPE::TAIL_2);}
		namespace DOSS		 { constexpr message_t	BASE = (CODE_TYPE::SERVER | CODE_TYPE::TAIL_3);}
		namespace ADMIN		 { constexpr message_t	BASE = (CODE_TYPE::SERVER | CODE_TYPE::TAIL_4);}
	}

}

//-----------------------------------------------------------------------------
// basic message
//-----------------------------------------------------------------------------
struct sMESSAGE
{
public:
			sMESSAGE() noexcept : message(0), source() 	{}
			sMESSAGE(message_t _message) noexcept : message(_message), source()	{}
			template <class TSOURCE>
			sMESSAGE(message_t _message, const TSOURCE& _source) noexcept : message(_message), source(reinterpret_cast<uint64_t>(_source))	{}
			sMESSAGE(const sMESSAGE& _copy) noexcept : message(_copy.message), source(_copy.source) {}
public:
			template <class TSOURCE>
			TSOURCE				get_source() const { return reinterpret_cast<TSOURCE>(source); }
			bool				has_source() const noexcept { return source != 0;}
			template <class TSOURCE>										  
			void				set_source(const TSOURCE& _source) noexcept { this->source = reinterpret_cast<uint64_t>(const_cast<TSOURCE>(_source));}
public:
			message_t			message;
protected:
			uint64_t			source;
};


}