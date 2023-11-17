#pragma once
// ----------------------------------------------------------------------------
//
// socket_example
//
// ----------------------------------------------------------------------------
#include "cgdk/sdk10/net.socket.h"
using namespace CGDK;

class socket_example: virtual public net::socket::tcp_client<>
{
protected:
	virtual void				on_connect(net::io::Iconnective* _pconnective) override; // 접속되었을 때
	virtual void				on_fail_connect(net::io::Iconnective* _pconnective, uint64_t _disconnect_reason) override; // 접속에 실패했을 때
	virtual	void				on_disconnect(uint64_t _reason) override; // 접속이 종료되었을 때
	virtual result_code			on_message (sMESSAGE_NETWORK& _msg) override; // 메시지가 전송되어 왔을 때
};
