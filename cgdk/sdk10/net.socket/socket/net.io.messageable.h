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
// net::io::messageable::Nbase
//
//-----------------------------------------------------------------------------
class net::io::messageable::Nbase : virtual public Imessageable
{
protected:
	// 1) on_message
	virtual result_code			on_message(sMESSAGE_NETWORK& /*_msg*/) {return 0;}

public:
	virtual	result_code			process_message(sMESSAGE& _msg) override;
};

inline result_code net::io::messageable::Nbase::process_message(sMESSAGE& _msg)
{
	// declare) 
	result_code	result = eRESULT::BYPASS;

	if (is_network_message(_msg.message))
	{
		// - call 'on_message' function
		result = this->on_message((sMESSAGE_NETWORK&)_msg);
	}

	// return)
	return result;
}


}