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

//-----------------------------------------------------------------------------
// cgdk
//-----------------------------------------------------------------------------
namespace CGDK
{
	struct sVERSION
	{
		ENABLE_STRUCT_SERIALIZABLE

		uint32_t			build{ 0 };
		uint16_t			minor{ 0 };
		uint16_t			major{ 0 };

		void clear() noexcept
		{
			this->build = 0;
			this->minor = 0;
			this->major = 0;
		}
	};

	struct sVERSION_EX : public sVERSION
	{
		ENABLE_STRUCT_SERIALIZABLE

		std::string			str_version;
		chrono::time::utc::time_point build_time;

		void clear() noexcept
		{
			sVERSION::clear();
			str_version.clear();
			build_time = (chrono::time::utc::time_point::min)();
		}
	};

	struct sDEVICE_INFO
	{
		ENABLE_STRUCT_SERIALIZABLE

		std::string			device_id; // android:IMEI, ios:UDID
		std::string			model;
		std::string			hardware_serial;
		std::string			sim_id;
		std::string			sim_serial;
		std::string			finger_print;
		std::string			aaid;

		void clear() noexcept
		{
			this->device_id.clear();

			this->model.clear();
			this->hardware_serial.clear();
			this->sim_id.clear();
			this->sim_serial.clear();
			this->aaid.clear();
		}
	};

	struct sOS_INFO
	{
		ENABLE_STRUCT_SERIALIZABLE

		eOS_TYPE			type {eOS_TYPE::NOT_DEFINED};
		std::string			name;
		sVERSION_EX			version;
		std::string			product_name;
		eLANGUAGE			language{ eLANGUAGE::NOT_DEFINED };

		void clear() noexcept
		{
			this->type = eOS_TYPE::NOT_DEFINED;
			this->name.clear();
			this->version.clear();
			this->product_name.clear();
			this->language = eLANGUAGE::NOT_DEFINED;
		}
	};

	enum class eSERVER_SYSTEM_TYPE : uint32_t
	{
		UNKNOWN				 = 0,
		ADMINSERVER			 = 1,
		ADMINCLIENT			 = 2,
		HOSTAGENT			 = 3,
		WEBSERVER			 = 4,
		SERVER				 = 5,
		RDBMS				 = 6,
		NOSQL				 = 7,
	};

	enum class ePROCESSOR_TYPE
	{
		NOT_DEFINED			 = 0,
		x86_64				 = 1,
		ANDROID				 = 2
	};

	namespace SYSTEM
	{
		struct sPROCESSOR
		{
			ePROCESSOR_TYPE	type{ ePROCESSOR_TYPE::NOT_DEFINED };
			uint32_t		level{ 0 };
			uint32_t		revision{ 0 };
			int				number_of_processor{ 0 };
		};

		struct sMEMORY
		{
			uint64_t		bytes_physical_usage{ 0 };
			uint64_t		bytes_physical_total{ 0 };
		};

		struct sNETWORK
		{
			uint64_t		dummy{ 0 };
		};

		struct sINFO
		{
			sPROCESSOR		processor;
			sMEMORY			memory;
			sNETWORK		network;
		};

		struct sSTATUS
		{
			uint64_t		processor_usage{ 0 }; // %x1000 (1.0%->1000, 100%->100000)
			uint64_t		thread_count{ 0 };
			uint64_t		process_count{ 0 };
			uint64_t		handle_count{ 0 };

			uint64_t		bytes_memory_usage{ 0 };
			uint64_t		bytes_traffic{ 0 };

			chrono::time::utc::time_point time_update;		// - 데이터가 작성된 시간
		};
	}

	namespace PROCESS
	{
		struct sINFO
		{
			uint64_t		pid;
		};
	
		struct sSTATUS
		{
			uint64_t		thread_coun{ 0 };
			uint64_t		handle_count{ 0 };
			uint64_t		usage_process{ 0 };			// - process 사용량
			uint64_t		bytes_memory_usage{ 0 };
			uint64_t		size_traffic{ 0 };
		};
	};
}