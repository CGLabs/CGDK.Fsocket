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
	virtual void				on_connect(net::io::Iconnective* _pconnective) override; // ���ӵǾ��� ��
	virtual void				on_fail_connect(net::io::Iconnective* _pconnective, uint64_t _disconnect_reason) override; // ���ӿ� �������� ��
	virtual	void				on_disconnect(uint64_t _reason) override; // ������ ����Ǿ��� ��
	virtual result_code			on_message (sMESSAGE_NETWORK& _msg) override; // �޽����� ���۵Ǿ� ���� ��
};
