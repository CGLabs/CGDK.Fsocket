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
// net::io::Iaddress
//
//-----------------------------------------------------------------------------
class net::io::Iaddress : 
	virtual public				Ireferenceable
{
public:
			TSharedPtr<FInternetAddr> get_socket_address() const noexcept { return process_get_socket_address();}
			TSharedPtr<FInternetAddr> get_peer_address() const noexcept { return process_get_peer_address();}

protected:
	virtual	TSharedPtr<FInternetAddr> process_get_socket_address() const noexcept PURE;
	virtual	TSharedPtr<FInternetAddr> process_get_peer_address() const noexcept PURE;
};


}
