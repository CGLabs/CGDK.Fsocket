//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                              Factory Classes                              *
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
//-----------------------------------------------------------------------------
//
// Definitions for buffer
//
//-----------------------------------------------------------------------------
namespace CGDK
{

struct sMESSAGE_BUFFER : public sMESSAGE
{
public:
			sMESSAGE_BUFFER() noexcept : sMESSAGE() {}
			sMESSAGE_BUFFER(uint32_t _message) noexcept : sMESSAGE(_message) {}
			template<class TSOURCE>
			sMESSAGE_BUFFER(uint32_t _message, TSOURCE _psource) noexcept : sMESSAGE(_message, _psource) {}
			template<class TSOURCE>
			sMESSAGE_BUFFER(uint32_t _message, TSOURCE _psource, const shared_buffer& _buffer) noexcept : sMESSAGE(_message, _psource), buf_message(_buffer) {}
			sMESSAGE_BUFFER(uint32_t _message, const shared_buffer& _buffer) noexcept : sMESSAGE(_message), buf_message(_buffer) {}
			sMESSAGE_BUFFER(const shared_buffer& _buffer) noexcept : sMESSAGE(eMESSAGE::SYSTEM::BUFFER), buf_message(_buffer) {}

public:
			shared_buffer		buf_message;
};


}