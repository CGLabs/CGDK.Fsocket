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
		template <typename TTYPE, typename TCON = std::vector<object_ptr<TTYPE>>> class Nattachable;
		class Idestroyable;
	}

	class Imessage_transmitter;
}

//----------------------------------------------------------------------------
// head files
//----------------------------------------------------------------------------
// 1) CGDK base header files
#include "../sdk10/common.h"

// 2) chrono
#include "system/object/chrono.h"
#include "system/object/chrono_tick.h"
#include "system/object/chrono_time.h"
#include "system/object/chrono_time_client.h"

// 3) object definitions
#include "system/object/definitions.object.h"
#include "system/object/definitions.result_code.h"

// 4) traits
#include "system/object/object_traits.h"

// 5) disposable & reference counting
#include "system/object/Idisposable.h"
#include "system/object/Ireferenceable.h"
#include "system/object/object_ptr.h"

// 6) referenceable 
#include "system/object/referenceable.Nno_act.h"

// 7) object allocation
#include "system/object/obj.h"
#include "system/object/make_object.h"
#include "system/object/make_object_raw.h"
#include "system/object/make_own.h"
#include "system/object/throwable.h"

// 8) buffer
#include "../buffer"
#include "system/object/Imemory.h"
#include "../shared_buffer"
#include "../buffers/_buffer_unreal.h"
#include "system/object/make_shared_buffer.h"

// 9) etc
#include "system/object/object.Iattachable.h"
#include "system/object/Imessageable.h"
#include "system/object/Imessage_transmitter.h"
#include "system/object/Nmessage_transmitter.h"
#include "system/object/Idestroyable.h"
#include "system/object/own_ptr.h"

// 10) definitions for buffer
#include "system/object/definitions.buffer.h"
