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
//-----------------------------------------------------------------------------
//
// net::io::Iconnectable
//
//-----------------------------------------------------------------------------
class net::io::Iconnectable : 
// Inherited classes)
	virtual	public				net::io::Isocket,
	virtual public				net::io::Iclosable,
	virtual public				net::io::Iaddress,
	virtual public				net::io::statistics::Nsocket
{
// constructor/destructor) 
public:
			Iconnectable() noexcept {}
	virtual ~Iconnectable() noexcept {}

// publics) 
public:
	// 1) disconnect를 수행할 때 부르는 함수이다.(Graceful)
	virtual	bool				disconnect(uint64_t _disconnect_reason = DISCONNECT_REASON_NONE) noexcept PURE;

// frameworks) 
public:
	// 3) CompleteConnect
	virtual	bool				process_complete_connect (net::io::Iconnective* _pconnective, uintptr_t _return) PURE;
	virtual	bool				process_complete_disconnect () PURE;
};


}