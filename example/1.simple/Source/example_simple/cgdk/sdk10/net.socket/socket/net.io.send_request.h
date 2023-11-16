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
// net::io::send_request::Nbase
//
//-----------------------------------------------------------------------------
class net::io::send_request::Nbase :
// Inherited classes)
	virtual public				net::io::Isendable,
	virtual public				net::io::Isend_request
{
// publics)
public:
	// 1) send할 때 부르는 함수.
	virtual	bool				request_send( net::io::Isend_completor* _psend_completor, const buffer_view* _array_buffer, std::size_t _count_buffer, Ireferenceable* _powner, std::size_t _count_message, const FInternetAddr& _paddress) override
								{
									return	process_sendable(_psend_completor, _array_buffer, _count_buffer, _powner, _count_message, _paddress);
								}
};


}