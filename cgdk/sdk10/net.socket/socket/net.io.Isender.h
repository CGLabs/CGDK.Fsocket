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
// net::io::Isender
//
//-----------------------------------------------------------------------------
class net::io::Isender : virtual public Ireferenceable
{
public:
			bool				send( const shared_buffer& _buffer, uint64_t _option = 0) { return process_send(_buffer, _option); }
			bool				send( shared_buffer&& _buffer, uint64_t _option = 0) { return send(_buffer, _option); }

	template <std::size_t ICOUNT>
			bool				send( const shared_buffer (&_array_buffer)[ICOUNT], uint64_t _option = 0);
	template <class T>
	typename std::enable_if<is_send_container_buffer<T>::value, bool>::type
								send( const T& _list, uint64_t _option = 0);
	template <class T>
	typename std::enable_if<is_send_container<T>::value, bool>::type
								send( const T& _list, uint64_t _option = 0);

	template <class T>
			Isender&			operator<<(const T& _rhs) { send(_rhs); return *this;}

protected:
	virtual	bool				process_send( const shared_buffer& _buffer, uint64_t _option) PURE;
};

template <std::size_t ICOUNT>
bool net::io::Isender::send( const shared_buffer(&_array_buffer)[ICOUNT], uint64_t _option)
{
	for (std::size_t i = 0; i < ICOUNT; ++i)
	{
		send(_array_buffer[i], _option);
	}

	// return) 
	return	true;
}

template <class T>
typename std::enable_if<is_send_container_buffer<T>::value, bool>::type
net::io::Isender::send( const T& _list, uint64_t _option)
{
	for (const auto& iter : _list)
	{
		send(iter, _option);
	}

	// return) 
	return	true;
}

class net::io::Isender_datagram : virtual public Ireferenceable
{
public:
			bool				send_to( const shared_buffer& _buffer, const FInternetAddr& _address, uint64_t _option = 0) { return process_send(_buffer, _address, _option);}
			bool				send_to( const shared_buffer& _buffer, FInternetAddr&& _address, uint64_t _option = 0) { return process_send(_buffer, _address, _option); }

	template <std::size_t ICOUNT>
			bool				send_to( const shared_buffer (&_array_buffer)[ICOUNT], const FInternetAddr& _address, uint64_t _option = 0);
	template <std::size_t ICOUNT>
			bool				send_to( const shared_buffer (&_array_buffer)[ICOUNT], FInternetAddr&& _address, uint64_t _option = 0) { return send_to(_array_buffer, _address, _option); }
	template <class T, std::size_t ICOUNT>
	typename std::enable_if<!std::is_base_of_v<shared_buffer, T>, bool>::type
								send_to( const T(&_array)[ICOUNT], const FInternetAddr& _address, uint64_t _option = 0);
	template <class T>
	typename std::enable_if<is_send_container_buffer<T>::value, bool>::type
								send_to( const T& _list, const FInternetAddr& _address, uint64_t _option = 0);
	template <class T>
	typename std::enable_if<is_send_container<T>::value, bool>::type
								send_to( const T& _list, FInternetAddr&& _address, uint64_t _option = 0) { return send_to(_list, _address, _option); }

protected:
	virtual	bool				process_send(const shared_buffer& _buffer, const FInternetAddr& _address, uint64_t _option) PURE;
};

template <std::size_t ICOUNT>
bool net::io::Isender_datagram::send_to( const shared_buffer (&_array_buffer)[ICOUNT], const FInternetAddr& _address, uint64_t _option)
{
	// 1) get size
	auto size_buffer = get_size_of(_array_buffer);

	// check) 
	check(size_buffer != 0);

	// check) 
	RETURN_IF(size_buffer == 0, false);

	// 2) alloc shared_buffer
	auto buffer_temp = alloc_shared_buffer(size_buffer);

	// 3) append shared_buffer
	for (std::size_t i = 0; i < ICOUNT; ++i)
	{
		// 2) 붙인다.
		buffer_temp += _array_buffer[i];
	}

	// 4) send
	return	process_send(buffer_temp, _address, _option);
}

template <class T>
typename std::enable_if<is_send_container_buffer<T>::value, bool>::type
net::io::Isender_datagram::send_to( const T& _list_buffer, const FInternetAddr& _address, uint64_t _option)
{
	// 1) get size
	auto size_buffer = get_size_of(_list_buffer);

	// check) 
	check(size_buffer != 0);

	// check) 
	RETURN_IF(size_buffer == 0, false);

	// 2) alloc shared_buffer
	auto buffer_temp = alloc_shared_buffer(size_buffer);

	// 3) append shared_buffer
	for (const auto& iter : _list_buffer)
	{
		// 2) 붙인다.
		buffer_temp += iter;
	}

	// 4) send
	return	process_send(buffer_temp, _address, _option);
}


}