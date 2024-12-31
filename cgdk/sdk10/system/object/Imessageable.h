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
//                                                                            
// Imessageable
//                                                                            
//-----------------------------------------------------------------------------
class Imessageable
{
// constructor/destructor)
public:
			Imessageable() noexcept : m_enable_messageable(true) {}
			Imessageable(bool _enable) noexcept : m_enable_messageable(_enable) {}
	virtual	~Imessageable() noexcept {}

// public)
public:
	//! @brief 메시지 처리를 요청한다. @param _msg 처리 요청하는 메시지 @return true !0 처리됨 @return 0 처리하지 않음
			result_code			request_process_message(sMESSAGE& _msg)	
			{
				if(this->is_messageable_disabled())
					return eRESULT::FAIL_DISABLED;

				return this->process_message(_msg);
			}
																		  
	// 2) enable Status													  
			void				enable_messageable() noexcept			{ this->m_enable_messageable = true;}
			void				disable_messageable() noexcept			{ this->m_enable_messageable = false;}
			bool				is_messageable_enabled() const noexcept	{ return this->m_enable_messageable;}
			bool				is_messageable_disabled() const noexcept{ return !this->is_messageable_enabled();}

// framework)
protected:
	//! @brief 메시지 처리를 정의한다. @param _msg 처리 요청하는 메시지 @return true !0 처리됨 @return 0 처리되지 않음
	virtual	result_code			process_message(sMESSAGE& _msg) PURE;

// implementation)
private:
	// 1) Flag
			bool				m_enable_messageable;
};


}