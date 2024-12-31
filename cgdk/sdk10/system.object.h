//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                              object Classes                               *
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
// CGDK10.system.object
//
//-----------------------------------------------------------------------------
#define	CGDK_SYSTEM_OBJECT


//----------------------------------------------------------------------------
// proto...
//----------------------------------------------------------------------------
namespace CGDK
{
	class Imemory;

	namespace object
	{
		template <class TTYPE> class Iattachable;
		template <typename TTYPE, typename TCON = std::vector<TSharedPtr<TTYPE>>> class Nattachable;
	}

	class Imessage_transmitter;
}

#define _UNREAL

//----------------------------------------------------------------------------
// head files
//----------------------------------------------------------------------------
// 1) CGDK base header files
#include "../sdk10/common.h"

// 2) chrono
#include "system/object/chrono_tick.h"

// 3) object definitions
#include "system/object/definitions.object.h"
#include "system/object/definitions.result_code.h"

// 4) traits
#include "system/object/object_traits.h"

// 7) object allocation
#include "system/object/throwable.h"

// 8) buffer
#include "../buffer"
#include "../shared_buffer"
#include "../buffers/_buffer_unreal.h"
#include "common/definitions.system.h"
#include "common/definitions.cgdk.h"

// 9) etc
#include "system/object/Imessageable.h"
#include "system/object/Nmessage_transmitter.h"

// 10) definitions for buffer
#include "system/object/definitions.buffer.h"

// 11) admin
#include "system/object/definitions.admin.h"

