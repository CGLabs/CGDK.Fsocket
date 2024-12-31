//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*                       Ver 10.0 / Release 2019.12.11                       *
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
//-----------------------------------------------------------------------------
//
// CGMessageMap
//
// 1. CGMessageMap이란...?
//    1) Message의 처리를 정의를 Map형태로 작성할 수 있도록 한 것이다.
//
// 2. CGMessageMap의 정의
//    CGCII Message Map을 사용하기 위해서는 아래와 같은 내용의 작성이 필요하다.
//		
//    1) 먼저 Message Map을 사용하고자하는 클래스의 정의에 아래와 같이 
//       'DECLARE_CGMESSAGE_MAP'을 추가해 준다.
//       일반적으로 이 것은 Class를 정의하는 헤더파일에 작성한다.
//
//       class foo
//       {
//          ....
//
//          DECLARE_CGMASSAGE_MAP
//          ....
//       };
//
//    2) CPP파일에 Message Map은 아래와 같이 작성한다.
//       Message Map 정의의 시작은 'BEGIN_CGMESAAGE_MAP(클래스명)' 키워드를
//       작성한다.
//       그리고 필요한 Message와 그 Message를 받았을 때 처리할 함수명과
//       함수의 파라메터를 아래와 같이 써준다.
//             ON_MESSAGE(메시지, 함수명, 파라메터형)
//       이때 파라메터는 반드시 sMESSAGE를 상속받은 형이어야 하고 메시지를 전송할 때
//       형과 일치해야 한다.
//       기본적인 예는 아래와 같다.
//
//       BEGIN_CGMESSAGE_MAP(foo)
//             CGMESSAGE_TYPE(MESSAGE_SEND_A, on_message_A, sMESSAGE);
//             CGMESSAGE_TYPE(MESSAGE_SEND_B, on_message_B, sMESSAGE);
//       END_CGMESSAGE_MAP
//
//    3) 이 때 on_message_A함수는 아래와 같이 작성한다.
//
//       int foo::on_message_A(sMESSAGE& _msg)
//       {
//           ...
//       }
//
//       *주의: BEGIN_MESSAGE_MAP은 반드시 CPP파일에 작성해야 하며 헤더파일(.h)에
//              작성해서는 안된다.
//    
//    4) 마지막으로 이 Message Map을 사용하기 위해서는 반드시 메시지를 받는 함수
//       즉 on_message()함수에 아래와 같은 키워드를 추가해 주어야 한다.
//
//       int foo::on_message(sMESSAGE& _msg)
//       {
//           EXECUTE_CGMESSAGE_MAP(_msg);
//
//           ...
//
//           return 0;
//       }
//
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// ...
//-----------------------------------------------------------------------------
#if defined(_MSC_VER)
	#pragma warning(disable:4065)
#endif


//-----------------------------------------------------------------------------
// message map
//-----------------------------------------------------------------------------
// 1) declare message map
#define	DECLARE_CGMESSAGE_MAP	CGDK::result_code __execute_message(sMESSAGE& _msg);

// 2) declare message map
#define	DEFINE_PROCESS_MESSAGE			virtual	CGDK::result_code process_message(sMESSAGE& _msg) override	{ EXECUTE_CGMESSAGE_MAP(_msg); return eRESULT::BYPASS;}
#define	DEFINE_PROCESS_MESSAGE_BLOCK	virtual	CGDK::result_code process_message(sMESSAGE& _msg) override	{ EXECUTE_CGMESSAGE_MAP(_msg); return eRESULT::DONE;}
#define	DEFINE_ON_MESSAGE				virtual	CGDK::result_code on_message(sMESSAGE& _msg) override		{ EXECUTE_CGMESSAGE_MAP(_msg); return eRESULT::BYPASS;}
#define	DEFINE_ON_MESSAGE_BLOCK			virtual	CGDK::result_code on_message(sMESSAGE& _msg) override		{ EXECUTE_CGMESSAGE_MAP(_msg); return eRESULT::DONE;}

// 3) message mask
const CGDK::message_t eMESSAGE_MASK = 0x00ffffff;

// 4) execute message function
#define EXECUTE_PARENT_CGMESSAGE_MAP(parent, msg)				\
{																\
	auto _mm_result = parent::on_message(_msg);					\
																\
	if (_mm_result != eRESULT::BYPASS)							\
		return result;											\
}

#define	EXECUTE_CGMESSAGE_MAP(msg)								\
{																\
	auto xcg_result = __execute_message(msg);					\
																\
	if(static_cast<uint64_t>(xcg_result) != static_cast<uint64_t>(eRESULT::BYPASS)) \
		return xcg_result;										\
}

// 5) execute message function
#define	EXECUTE_CLASS_CGMESSAGE_MAP(class_type, msg)			\
{																\
	auto xcg_result = class_type::__execute_message(msg);		\
																\
	if(static_cast<uint64_t>(xcg_result) != static_cast<uint64_t>(eRESULT::BYPASS))	\
		return xcg_result;										\
}

// 6) begin mssage map head
#define	CGMESSAGE_MAP_HEAD										\
	{															\
		result_code _mm_result;									\
		switch(static_cast<CGDK::message_t>(_msg.message))		\
		{														\
		default:												\
				{												\
					_mm_result = eRESULT::BYPASS;				\
					break;

#define	CGMESSAGE_MAP_HEAD_MASK(mask)							\
	{															\
		result_code _mm_result;									\
		switch(static_cast<CGDK::message_t>(_msg.message) & mask)\
		{														\
		default:												\
				{												\
					_mm_result = eRESULT::BYPASS;				\
					break;


// head) execute message function
#define	BEGIN_CGMESSAGE_MAP(class_name)							\
	CGDK::result_code class_name::__execute_message(sMESSAGE& _msg)\
	CGMESSAGE_MAP_HEAD

#define	BEGIN_CGMESSAGE_MAP_MASK(class_name)					\
	CGDK::result_code class_name::__execute_message(sMESSAGE& _msg)\
	CGMESSAGE_MAP_HEAD_MASK(eMESSAGE_MASK)


#define	BEGIN_CGMESSAGE_MAP_TEMPLATE(class_name, type_a, type_b)		\
	CGDK::result_code class_name<type_a, type_b>::__execute_message(sMESSAGE& _msg) \
	CGMESSAGE_MAP_HEAD

#define	BEGIN_CGMESSAGE_MAP_TEMPLATE3(class_name, type_a, type_b, type_c)		\
	CGDK::CGDK::result_code class_name<type_a, type_b, type_c>::__execute_message(sMESSAGE& _msg) \
	CGMESSAGE_MAP_HEAD

#define	BEGIN_CGMESSAGE_MAP_TEMPLATE4(class_name, type_a, type_b, type_c, type_d) \
	CGDK::result_code class_name<type_a, type_b, type_c, type_d>::__execute_message(sMESSAGE& _msg) \
	CGMESSAGE_MAP_HEAD

#define	BEGIN_CGMESSAGE_MAP_TEMPLATE5(class_name, type_a, type_b, type_c, type_d, type_e) \
	CGDK::result_code class_name<type_a, type_b, type_c, type_d, type_e>::__execute_message(sMESSAGE& _msg) \
	CGMESSAGE_MAP_HEAD


#define	BEGIN_CGMESSAGE_MAP_MASK_TEMPLATE(class_name, type_a, type_b)	\
	CGDK::result_code class_name<type_a, type_b>::__execute_message(sMESSAGE& _msg) \
	CGMESSAGE_MAP_HEAD_MASK(eMESSAGE_MASK)

#define	BEGIN_CGMESSAGE_MAP_MASK_TEMPLATE3(class_name, type_a, type_b, type_c)	\
	CGDK::result_code class_name<type_a, type_b, type_c>::__execute_message(sMESSAGE& _msg)	\
	CGMESSAGE_MAP_HEAD_MASK(eMESSAGE_MASK)

#define	BEGIN_CGMESSAGE_MAP_MASK_TEMPLATE4(class_name, type_a, type_b, type_c, type_d)	\
	CGDK::result_code class_name<type_a, type_b, type_c, type_d>::__execute_message(sMESSAGE& _msg)	\
	CGMESSAGE_MAP_HEAD_MASK(eMESSAGE_MASK)

#define	BEGIN_CGMESSAGE_MAP_MASK_TEMPLATE5(class_name, type_a, type_b, type_c, type_d, type_e)	\
	CGDK::result_code class_name<type_a, type_b, type_c, type_d, type_e>::__execute_message(sMESSAGE& _msg)	\
	CGMESSAGE_MAP_HEAD_MASK(eMESSAGE_MASK)


#define	CGMESSAGE_TYPE(msg_type, function, type)				\
				}												\
		case	static_cast<CGDK::message_t>(msg_type):			\
				{												\
					_mm_result = function((type&)_msg);			\
					break;

#define	CGMESSAGE_TYPE_SUB(msg_type, msg_def)					\
				}												\
		case	static_cast<CGDK::message_t>(msg_type):			\
				{												\
					switch(static_cast<CGDK::message_t>(msg_def))\
					{											\
					default:									\
							{									\
								_mm_result = eRESULT::BYPASS;	\
								break;

// body) execute Message Function
#define	ON_CGMESSAGE(message, function, type)					\
						}										\
				case	static_cast<CGDK::message_t>(message):	\
						{										\
							_mm_result = function((type&)_msg);	\
							break;

#define	ON_CGMETHOD_RETURN(message, function, return_type)		\
						}										\
				case	static_cast<CGDK::message_t>(message):	\
						{										\
							auto temp_buffer = alloc_shared_buffer(sizeof(return_type));\
							temp_buffer.append<return_type>(function());\
							((sMESSAGE_BUFFER&)_msg).buf_message = temp_buffer;\
							_mm_result = eRESULT::DONE;			\
							break;

#define	ON_CGMETHOD(message, function)							\
						}										\
				case	static_cast<CGDK::message_t>(message):	\
						{										\
							function();							\
							_mm_result = eRESULT::DONE;			\
							break;

#define	END_SUB_CGMESSAGE_MAP									\
						}										\
				};												\
				break;

// tail) execute Message Function
#define	END_CGMESSAGE_MAP										\
				};												\
		};														\
		return _mm_result;										\
	}

// nested message map
#define	BEGIN_NESTED_CGMESSAGE_MAP								\
	CGMESSAGE_MAP_HEAD

#define	END_NESTED_CGMESSAGE_MAP								\
				};												\
		};														\
		if(_mm_result != eRESULT::BYPASS)						\
			return _mm_result;									\
	}

// attribute용 Message Map
#define	CGMESSAGE_TYPE_ATTRIBUTE								CGMESSAGE_TYPE_SUB(static_cast<CGDK::message_t>(CGMESSAGE::ATTRIBUTE), ((const sMESSAGE_BUFFER&)_msg).data()fer.front<CGDK::message_t>(4) & CGMESSAGE::SYSTEM::MASK)


//-----------------------------------------------------------------------------
// sub-message type
//-----------------------------------------------------------------------------
// 1) buffer message
#define	CGMESSAGE_TYPE_SUB_BUFFER								CGMESSAGE_TYPE_SUB(static_cast<CGDK::message_t>(CGMESSAGE::BUFFER), ((const sMESSAGE_BUFFER&)_msg).data()fer.front<CGDK::message_t>(4) & CGMESSAGE::SYSTEM::MASK)
#define	CGMESSAGE_TYPE_SUB_BUFFER_MESSAGE(msg)					CGMESSAGE_TYPE_SUB(msg, ((const sMESSAGE_BUFFER&)_msg).data()fer.front<CGDK::message_t>(4) & CGMESSAGE::SYSTEM::MASK)

// 2) declare message map
#define	DECLARE_CGMESSAGE_MAP_SUB(name)	CGDK::result_code __execute_message##name(sMESSAGE& _msg);

// 3) execute message function
#define	EXECUTE_CGMESSAGE_MAP_SUB(name, msg)					\
{																\
	auto xcg_result = __execute_message##name(msg);				\
																\
	if(xcg_result != eRESSULT::BYPASS)							\
		return xcg_result;										\
}

// head) execute message function
#define	BEGIN_CGMESSAGE_MAP_SUB(class_name, name)				\
	CGDK::result_code class_name::__execute_message##name(sMESSAGE& _msg) \
	{															\
		switch(_msg.message)									\
		{														\
		default:												\
				{												\
					return eRESSULT::BYPASS;

// head) execute message function
#define	BEGIN_CGMESSAGE_MAP_SUB_MASK(class_name, name)			\
	CGDK::result_code class_name::__execute_message##name(sMESSAGE& _msg) \
	{															\
		switch(static_cast<CGDK::message_t>(_msg.message) & eMESSAGE_MASK)	\
		{														\
		default:												\
				{												\
					return eRESSULT::BYPASS;
