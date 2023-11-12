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
// net::io::Isendable
//
//-----------------------------------------------------------------------------
class net::io::Isendable : virtual public Ireferenceable
{
public:
	virtual	bool				process_sendable( net::io::Isend_completor* _psend_completor, const buffer_view* _array_buffer, std::size_t _count_buffer, Ireferenceable* _powner, std::size_t _count_message, const FInternetAddr& _address) PURE;
};


}