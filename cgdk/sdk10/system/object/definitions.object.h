//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                              object classes                               *
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
// eOBJECT_STATE
//
//    1) initialize	eOBJECT_STATE::NONE    ->eOBJECT_STATE::INITIALIZING_PENDING ->eOBJECT_STATE::STOPPED
//    2) start		eOBJECT_STATE::STOPPED ->eOBJECT_STATE::START_PENDING        ->eOBJECT_STATE::RUNNING
//    3) stop		eOBJECT_STATE::RUNNING ->eOBJECT_STATE::STOP_PENDING		 ->eOBJECT_STATE::STOPPED
//    4) Destory	eOBJECT_STATE::STOPPED ->eOBJECT_STATE::DESTROYING_PENDING   ->eOBJECT_STATE::NONE
//    5) pause		eOBJECT_STATE::RUNNING ->eOBJECT_STATE::PAUSE_PENDING		 ->eOBJECT_STATE::PAUSED
//    6) continue	eOBJECT_STATE::PAUSED  ->eOBJECT_STATE::CONTINUE_PENDING	 ->eOBJECT_STATE::RUNNING
//
//-----------------------------------------------------------------------------
enum class eOBJECT_STATE : int32_t
{
	NONE = 0,
	STOPPED = 1,	// SERVICE_STOPPED
	READY = 1,
	START_PENDING = 2,	// SERVICE_START_PENDING
	REQUESTING = 2,
	STOP_PENDING = 3,	// SERVICE_STOP_PENDING
	CLOSE_PENDING = 3,
	RUNNING = 4,	// SERVICE_RUNNING
	PROCESSED = 4,
	CONTINUE_PENDING = 5,	// SERVICE_CONTINUE_PENDING
	PAUSE_PENDING = 6,	// SERVICE_PAUSE_PENDING
	PAUSED = 7,	// SERVICE_PAUSED
	INITIALIZING_PENDING = 8,
	DESTROYING_PENDING = 9,
	UPDATE = 10,
	COMMIT = 11,
	NOT_CONNECT = 12,
	ESTABLISHED = 13,
	DEFAULT = 0xff
};

inline eOBJECT_STATE operator & (eOBJECT_STATE _lhs, eOBJECT_STATE _rhs) { return static_cast<eOBJECT_STATE>(static_cast<uint32_t>(_lhs) & static_cast<uint32_t>(_rhs)); }
inline eOBJECT_STATE operator | (eOBJECT_STATE _lhs, eOBJECT_STATE _rhs) { return static_cast<eOBJECT_STATE>(static_cast<uint32_t>(_lhs) | static_cast<uint32_t>(_rhs)); }
inline eOBJECT_STATE operator & (eOBJECT_STATE _lhs, uint32_t _rhs) { return static_cast<eOBJECT_STATE>(static_cast<uint32_t>(_lhs) && _rhs); }
inline eOBJECT_STATE operator | (eOBJECT_STATE _lhs, uint32_t _rhs) { return static_cast<eOBJECT_STATE>(static_cast<uint32_t>(_lhs) || _rhs); }


//-----------------------------------------------------------------------------
//
// eRESULT
//
//-----------------------------------------------------------------------------
enum class  eRESULT : int
{
	// - Default
	NONE									 = 0x00000000,	//            = SUCCESS
	BYPASS									 = 0x00000000,	//            = SUCCESS

	// - Success
	SUCCESS									 = 0x00000000,	// 0x0******* Success
	DONE									 = 0x00000001,	//            Success - Done
	PENDED									 = 0x00000201,	//            Success - setting and Execution Pended
	SUCCESS_CREATE							 = 0x00000202,	//            Success - on creation new account
	SUCCESS_RESTORE							 = 0x00000203,	//            Success - on Restore
	PENDING									 = 0x00000204,	//            Pending
	TRANSFER								 = 0x00000205,	//            Transfer
	_SUCCESS_USER							 = 0x00010000,	// 0x0001**** for user definition

	// - Fail = result of trying
	FAIL									 = 0x03000000,	// 0x3******* Fail
	FAIL_DISABLED							 = 0x03000001,
	TIME_OUT								 = 0x03000012,
	ALEADY_EXIST							 = 0x03000003,
	EXPIRED									 = 0x03000014,
	NOT_EXIST								 = 0x03000005,
	NOT_READY								 = 0x03000006,
	BANNED_MEMBER							 = 0x03000017,
	MEMBER_FULL								 = 0x03000018,
	INVALID_MEMBER							 = 0x03000019,
	INVALID_PASSWORD						 = 0x0300001a,
	INVALID_PLATFORM						 = 0x0300001b,
	ALEADY_EXIST_NICKNAME					 = 0x0300001c,
	ALEADY_DONE								 = 0x0300001d,
	API_FAILURE								 = 0x0300001e,
	FAIL_TO_ACCEPT_SOCKET					 = 0x0300001f,
	FAIL_TO_ATTACHING_TO_EXECUTOR			 = 0x03000020,
	CONNECTION_FAILURE						 = 0x03000021,
	FAIL_TO_CREATE							 = 0x03000022,
	FAIL_TO_OPEN							 = 0x03000023,
	FAIL_TO_OPERATION						 = 0x03000024,
	FAIL_TO_CONNECT							 = 0x03000025,
	FAIL_NO_WEBSOCKET_KEY					 = 0x03010026,
	INVALID_VALUE							 = 0x03010027,
	INVALID_SERVER							 = 0x03010028,
	INVALID_REQUEST							 = 0x03010029,
	BUSY									 = 0x0301002a,
	EXCEED_RETRY_LIMIT						 = 0x0301002b,
	NO_MEMBER_INFO							 = 0x0301002c,
	RETRY_OUT								 = 0x0301002d,
	FULL									 = 0x0301002e,
	_FAIL_USER								 = 0x030f0000,	// 0x030f**** for user definition

	// - Cancel = canceling
	CANCEL									 = 0x03100000,	// 0x2******* Cancel
	_CANCEL_USER							 = 0x031f0000,	// 0x031f**** for user definition

	// - Error of process
	ERROR_									 = 0x03200000,	// 0x3******* Error
	LOGIC_ERROR								 = 0x03200001,
	LENGTH_ERROR							 = 0x03200002,
	DOMAIN_ERROR							 = 0x03200003,
	OUT_OF_RANGE							 = 0x03200004,
	RUNTIME_ERROR							 = 0x03200005,
	OVERFLOW_ERROR							 = 0x03200006,
	UNDERFLOW_ERROR							 = 0x03200007,
	RANGE_ERROR								 = 0x03200008,
	BAD_ALLOC								 = 0x03200009,
	BAD_FUNCTION_CALL						 = 0x0320000a,
	CONCURRENT_LOCK_ERROR					 = 0x0320000b,
	ACCESS_DENIED							 = 0x0320000c,
	NO_INITIALIZED							 = 0x0320000d,
	INVALID_ARGUMENT						 = 0x0320000e,
	NOT_ALLOWED								 = 0x03200010,
	INVALID_PARAMETER_HANDLE				 = 0x03200011,
	INVALID_MESSAGE_LENGTH					 = 0x03200012,
	INVALID_MESSAGE_BUFFER_IS_NULL			 = 0x03200013,
	INVALID_MESSAGE_TOO_SHORT_LENGTH		 = 0x03200014,
	INVALID_MESSAGE_TOO_LONG_LENGTH			 = 0x03200015,
	INVALID_STATE							 = 0x03200016,
	INVALID_HANDLE							 = 0x03200017,
	_ERROR_USER								 = 0x032f0000,	// 0x032f**** for user definition

	// - Exception of process
	EXCEPTION								 = 0x03400000,	// 0x4*** Exception
	BAD_EXCEPTION							 = 0x03400001,
	FORCE_CLOSE								 = 0x03400002,
	NOT_SUPPORT								 = 0x03400003,
	INVALID_CALL							 = 0x03400004,
	INVALID_LICENSE							 = 0x03400005,
	ABNORMAL_STATE							 = 0x03400006,
	BUFFER_INVALID							 = 0x034d0007,
	BUFFER_INVALID_STRING					 = 0x034d0008,
	UNEXPECTED_EXCEPTION					 = 0x034d0000,
	_EXCEPTION_USER							 = 0x034f0000,	// 0x034f**** for user definition

	PROCESS									 = 0x03500000,	// 0x5*** Process
	REQUEST									 = 0x03500001,
	COMPLETE								 = 0x03500002,
	RESET									 = 0x03500003,
	RESTORE									 = 0x03500004,
	_PROCESS_USER							 = 0x035f0000,	// 0x035f**** for user definition

	_USER									 = 0x03600000,

	CGCII_EXPIRED							 = 0x7e000000,
	NOT_DEFINED								 = 0x7f000000	// 0xff** Not Defined
};

inline eRESULT operator & (eRESULT _lhs, eRESULT _rhs)	{ return static_cast<eRESULT>(static_cast<uint32_t>(_lhs) & static_cast<uint32_t>(_rhs));}
inline eRESULT operator | (eRESULT _lhs, eRESULT _rhs)	{ return static_cast<eRESULT>(static_cast<uint32_t>(_lhs) | static_cast<uint32_t>(_rhs));}
inline eRESULT operator & (eRESULT _lhs, uint32_t _rhs)	{ return static_cast<eRESULT>(static_cast<uint32_t>(_lhs) && _rhs);}
inline eRESULT operator | (eRESULT _lhs, uint32_t _rhs)	{ return static_cast<eRESULT>(static_cast<uint32_t>(_lhs) || _rhs);}

namespace std
{
	template <>
	struct is_error_code_enum<eRESULT> : true_type {};
}

