#include "socket_example.h"
#include "definitions_message.h"


//shared_buffer message_USER_LOGIN_REQUEST(REQUEST_ID _request_id, ACCOUNT_INDEX _account_index, AREA_INDEX _area_index)
//{
//	// 1) alloc buffer
//	auto buf_temp = alloc_shared_buffer(8 + get_size_of(_request_id) + get_size_of(_account_index) + get_size_of(_area_index));
//	
//	// 2) writee message
//	buf_temp.append<uint32_t>();
//	buf_temp.append<uint32_t>(eMESSAGE::USER_LOGIN_REQUEST);
//	buf_temp.append<REQUEST_ID>(_request_id);
//	buf_temp.append<>(_account_index);
//	buf_temp.append<AREA_INDEX>(_area_index);
//		
//	// 3) write message length
//	buf_temp.front<uint32_t>() = buf_temp.size<uint32_t>();
//	
//	// return) 
//	return buf_temp;
//}

void socket_example::on_connect(net::io::Iconnective* /*_pconnective*/)
{
	UE_LOG(LogTemp, Log, TEXT("connected"));
}

void socket_example::on_fail_connect(net::io::Iconnective* /*_pconnective*/, uint64_t /*_disconnect_reason*/)
{
	UE_LOG(LogTemp, Log, TEXT("fail to connect"));
}

void socket_example::on_disconnect(uint64_t _reason)
{
	UE_LOG(LogTemp, Log, TEXT("disconnected"));
}

result_code socket_example::on_message(sMESSAGE_NETWORK& _msg)
{
	// - 전송되어온 메시지는 _msg.buf_message에 존재한다.
	//   4Byte Offset을 준 buffer를 읽어 내기 위해 복사해 온다.
	auto buf_recv = _msg.buf_message + offset(4);

	// - 제일 앞 4Byte의 값이 message_type이다.
	auto message_type = buf_recv.front<int>();

	// - 메시지 종류에 따른 처리를 수행한다.
	switch (message_type)
	{
	case	MESSAGE_TYPE::ENTER:
			UE_LOG(LogTemp, Display, TEXT("Member Entered"));
			break;

	case	MESSAGE_TYPE::LEAVE:
			UE_LOG(LogTemp, Display, TEXT("Member Leaved"));
			break;

	case	MESSAGE_TYPE::CHATTING:
			UE_LOG(LogTemp, Display, TEXT("Chatting Message"));
			break;

	default:
			break;
	}

	// return) 메시지를 완료 했으면 eRESULT::DONE
	//         완료하지 못했으면 eRESULT::BYPASS(0)를 리턴합니다.(강제사항은 아님)
	return eRESULT::BYPASS;
}
