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

namespace CGDK
{
// ----------------------------------------------------------------------------
//  settings
// ----------------------------------------------------------------------------
// 1) for tcp receiveable
#define	DEFAULT_TCP_MESSAGE_BUFFER_SIZE					(          1 * 1024)
#define	DEFAULT_TCP_MESSAGE_BUFFER_SIZE_MAX				(         64 * 1024)
#define	DEFAULT_MAX_BYTES_OF_MESSAGE_RECEIVE_QUEUE		(  16 * 1024 * 1024)

// 2) for tcp sending queue
#define	DEFAULT_MAX_DEPTH_OF_MESSAGE_SEND_QUEUE			(              1024)
#define	DEFAULT_MAX_BYTES_OF_MESSAGE_SEND_QUEUE			(  16 * 1024 * 1024)


// ----------------------------------------------------------------------------
// Basic
// ----------------------------------------------------------------------------
// 1) CG Network socket�� ���¸� ��Ÿ��
enum class eSOCKET_STATE : int32_t
{
	CLOSED				 = 0,	//  0 : ������ ����� ����
	BINDED				 = 1,	//  1 : Binded
	SYN					 = 2,	//  2 : �������� ���� ����
	SEND_DISCONNECTED	 = 3,	//  3 : (Active) �������� �������
	FIN_WAIT_1			 = 4,	//  4 : (Active) �������� �������
	FIN_WAIT_2			 = 5,	//  5 : (Active) �������� �������
	TIME_WAIT			 = 6,	//  6 : (Active) �������� �������
	CLOSE_WAIT			 = 7,	//  7 : (Passive) �������� �������
	LAST_ACK			 = 8,	//  8 : (Passive) �������� �������
	ESTABLISHED			 = 9,	//  9 : ���Ӹ� �� ����
	LISTEN				 = 10,	// 10 : listen����
	CERTIFIED			 = 11,	// 11 : ������ �ǰ� Client������ ���� ����.
	LOGINED				 = 12,	// 12 : ID�� Password�� �ְ� Log-In�� �� ����.
	MAX
};

// 2) acceptor
#define	NO_START_ACCEPT						(-1)

const uint64_t	DISCONNECT_REASON_NODISCONNECTEX			 = 0x1000000000000000;	// Mask

const uint64_t	DISCONNECT_REASON_NONE						 = 0x0000000000000000;	// �ϻ����� ó�� ������ ���
const uint64_t	DISCONNECT_REASON_FAIL						 = 0x0000000100000000;	// ������ ���� ���� ������ ���(�ۼ��� �� ����, ���濡 ���� �������� ����)
const uint64_t	DISCONNECT_REASON_ACTIVE					 = 0x0000000200000000;	// ���� ���Ḧ ��û�� ���� ���(�����Ǿ� ���� ������ Passive)
const uint64_t	DISCONNECT_REASON_ABORTIVE					 = 0x0000000400000000;	// ���� ���������� ���(�����Ǿ� ���� ������ Graceful)
const uint64_t	DISCONNECT_REASON_EXCEPTION					 = 0x0000000800000000;	// ���ܷ� ���� ���� ���
const uint64_t	DISCONNECT_REASON_MASK						 = 0x0fffffff00000000;	// Mask
const uint64_t	DISCONNECT_REASON_MASK_USER					 = 0x00000000ffffffff;	// User ����

const uint64_t	DISCONNECT_REASON_OVERFLOW_MAX_MESSAGES		 = 0x0000000000000001;	// send�� Message ���� Overflow�� ������ �����ߴ�.
const uint64_t	DISCONNECT_REASON_OVERFLOW_MAX_BYTES		 = 0x0000000000000002;	// send�� Bytes ���� Overflow�� ������ �����ߴ�.
const uint64_t	DISCONNECT_REASON_MESSAGE_LENGTH_TOO_LONG	 = 0x0000000000000003;	// Message�� ���̰� �ʹ� ���
const uint64_t	DISCONNECT_REASON_MESSAGE_LENGTH_TOO_SHORT	 = 0x0000000000000004;	// Message�� ���̰� �ʹ� ª��
const uint64_t	DISCONNECT_REASON_MESSAGE_UNHANDLED_EXCEPTION= 0x0000000000000005;	// Messageó�� �� �ڵ鸵 ���� ���� Exception
const uint64_t	DISCONNECT_REASON_BUFFER_IS_NULL			 = 0x0000000000000006;	// shared_buffer�� buf���� nullptr�̴�.
const uint64_t	DISCONNECT_REASON_INVALID_STATE				 = 0x0000000000000007;	// ó�� ������ ���� �� ���� �����̴�.
const uint64_t	DISCONNECT_REASON_INVALID_SOCKET_HANDLE		 = 0x0000000000000008;	// socket handle�� INVALID_SOCKET_HANDLE�̴�.
const uint64_t	DISCONNECT_REASON_FAIL_TO_SOCKET_PREPARE	 = 0x0000000000000009;	// socket�� prepare�ܰ迡�� �����ߴ�.
const uint64_t	DISCONNECT_REASON_FAIL_TO_SOCKET_ACCEPT		 = 0x000000000000000a;	// socket�� Accept �������������ߴ�.
const uint64_t	DISCONNECT_REASON_FAIL_TO_SOCKET_CONNECT	 = 0x000000000000000b;	// socket�� connect �ܰ迡�� �����ߴ�.
const uint64_t	DISCONNECT_REASON_KEEP_ALIVE_TIME_OUT		 = 0x000000000000000c;	// socket�� prepare�ܰ迡�� �����ߴ�.
const uint64_t	DISCONNECT_REASON_DISABLED					 = 0x000000000000000e;	// acceptor�� connector�� disabled�� �����̴�.
const uint64_t	DISCONNECT_REASON_CONNECTION_FULL			 = 0x000000000000000f;	// �ִ����Ӽ��� �ʰ��� ��������Ǿ��.
const uint64_t	DISCONNECT_REASON_EXCEPTION_OCCURED			 = 0x0000000000000010;	// ó���� ���ܰ� �߻��� ������ ����Ǿ���.
const uint64_t	DISCONNECT_REASON_OPEN_SOCKET_LIMITS		 = 0x0000000000000011;	// socket�� �ִ� ���� ���� �ʰ��� ������ ����Ǿ���.

const uint64_t	DISCONNECT_REASON_SOCKET_CERTIFY_FAIL		 = 0x0000000010000001;	// socekt certify ����

const uint64_t	DISCONNECT_REASON_SUCCESS					 = 0;
const uint64_t	DISCONNECT_REASON_GRACEFUL					 = 0;
const uint64_t	DISCONNECT_REASON_FAIL_TIMEOUT				 = (DISCONNECT_REASON_FAIL | DISCONNECT_REASON_ACTIVE);
const uint64_t	DISCONNECT_REASON_FAIL_ABORTIVE				 = (DISCONNECT_REASON_FAIL | DISCONNECT_REASON_ABORTIVE | DISCONNECT_REASON_ACTIVE);
const uint64_t	DISCONNECT_REASON_FAIL_EXCEPTION			 = (DISCONNECT_REASON_FAIL | DISCONNECT_REASON_EXCEPTION);


// ----------------------------------------------------------------------------
// Defaults
// ----------------------------------------------------------------------------
namespace net
{
namespace io
{
	namespace socket
	{
		namespace tcp
		{
			using Ndefault = net::io::socket::Ntcp;
		}
	}

	namespace sender
	{
		namespace tcp
		{
			template <class HEADER_T = uint32_t> using Ndefault = net::io::sender::Nstream_header<HEADER_T>;
		}
		namespace udp
		{
			using Ndefault = net::io::sender::Ndatagram;
		}
	}

	namespace packetable
	{
		namespace tcp
		{
			template <class HEADER_T = uint32_t> using Ndefault = net::io::packetable::Nstream<HEADER_T>;
		}
		namespace udp
		{
			using Ndefault = net::io::packetable::Ndatagram;
		}
	}

	namespace messageable
	{
		namespace tcp
		{
			using Ndefault = net::io::messageable::Nbase;
		}
		namespace udp
		{
			using Ndefault = net::io::messageable::Nbase;
		}
	}
}
}

// ----------------------------------------------------------------------------
// SFINAE) 
// ----------------------------------------------------------------------------
// shared_buffer linear container<T>)
template<class T>					struct is_send_container_buffer : public std::false_type {};
template<class A>					struct is_send_container_buffer<std::vector<shared_buffer, A>> : public std::true_type {};
template<class A>					struct is_send_container_buffer<std::list<shared_buffer, A>> : public std::true_type {};
template<class A>					struct is_send_container_buffer<std::deque<shared_buffer, A>> : public std::true_type {};
template<std::size_t A>				struct is_send_container_buffer<std::array<shared_buffer, A>> : public std::true_type {};
template<class A>					struct is_send_container_buffer<circular_list<shared_buffer, A>> : public std::true_type {};
template<std::size_t A>				struct is_send_container_buffer<static_vector<shared_buffer, A>> : public std::true_type {};

// linear container<T>)
template<class T>					struct is_send_container : public std::false_type {};
template<class A, class B>			struct is_send_container<std::vector<A, B>> : public std::true_type {};
template<class A, class B>			struct is_send_container<std::list<A, B>> : public std::true_type {};
template<class A, class B>			struct is_send_container<std::deque<A, B>> : public std::true_type {};
template<class A, std::size_t B>	struct is_send_container<std::array<A, B>> : public std::true_type {};
template<class A, class B>			struct is_send_container<circular_list<A, B>> : public std::true_type {};
template<class A, std::size_t B>	struct is_send_container<static_vector<A, B>> : public std::true_type {};


}