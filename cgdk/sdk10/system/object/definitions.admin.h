//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*                       Ver 10.0 / Release 2019.12.11                       *
//*                                                                           *
//*                           Server.Admin Classes                            *
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
namespace server_admin
{
	//-----------------------------------------------------------------------------
	/**

	@class		iid_t (instance id)
	@brief
	@todo
	@ref		sSETTING_INFO, sSERVICE_INSTANCE_INFO
	@details	\n

	*/
	//-----------------------------------------------------------------------------
	// 1) instance id (매번 객체화될 때마다 할당되는 id이다. (0:비객체화, >0:객체화))
	typedef uint64_t iid_t;


	//-----------------------------------------------------------------------------
	/**

	@class		sRANK
	@brief		설정 정보의 기본 내용
	@todo
	@ref		sHOST_SETTING_INFO, sSERVICE_INSTANCE_INFO, SERVICE_SETTING
	@details	\n

	*/
	//-----------------------------------------------------------------------------
	// 1) instance의 타입
	namespace eINSTANCE_TYPE
	{
		constexpr static int NONE = 0;
		constexpr static int HOST = 1;
		constexpr static int SERVICE = 2;
		constexpr static int ADMIN_SERVER = 3;
		constexpr static int ADMON_CIENT = 4;
	};

	// 2) revision
	struct sREVISION
	{
		ENABLE_STRUCT_SERIALIZABLE

			std::string			url;						// - URL
		int64_t				revision{ 0 };				// - version
		chrono::time::utc::time_point time;				// - time
	};

	// 3) rank(역할)
	struct sRANK
	{
		// 1) group/field
		int32_t				group{ 0 };
		int32_t				field{ 0 };

		// 2) role/Index
		int32_t				role{ 0 };
		int32_t				index{ 0 };

		// function)
		bool				empty() const noexcept { return this->group == 0 && this->field == 0 && this->role == 0 && this->index == 0; }
		bool				exist() const noexcept { return !this->empty(); }
		void				clear() noexcept;

		std::string			to_string() const { return NFMT::format("{}.{}.{}.{}"sv, group, field, role, index); }
	};
	inline bool operator == (const sRANK& _lhs, const sRANK& _rhs) noexcept { return _lhs.group == _rhs.group && _lhs.field == _rhs.field && _lhs.role == _rhs.role && _lhs.index == _rhs.index; }
	inline bool operator != (const sRANK& _lhs, const sRANK& _rhs) noexcept { return !(_lhs == _rhs); }
	inline bool operator >  (const sRANK& _lhs, const sRANK& _rhs) noexcept { return (_lhs.group > _rhs.group) ? true : (_lhs.group < _rhs.group) ? false : (_lhs.field > _rhs.field) ? true : (_lhs.field < _rhs.field) ? false : (_lhs.role > _rhs.role) ? true : (_lhs.role < _rhs.role) ? false : (_lhs.index > _rhs.index); }
	inline bool operator <= (const sRANK& _lhs, const sRANK& _rhs) noexcept { return !(_lhs > _rhs); }
	inline bool operator <  (const sRANK& _lhs, const sRANK& _rhs) noexcept { return  (_rhs > _lhs); }
	inline bool operator >= (const sRANK& _lhs, const sRANK& _rhs) noexcept { return !(_lhs > _rhs); }
}
}