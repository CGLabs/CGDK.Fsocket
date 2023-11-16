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

namespace CGDK
{
//-----------------------------------------------------------------------------
/**

 @class		Nmessage_transmitter


*/
//-----------------------------------------------------------------------------
class Nmessage_transmitter : virtual public Imessage_transmitter
{
protected:
	virtual	bool				on_messageable_attaching(Imessageable* /*_pmessageable*/) { return true;}
	virtual	void				on_messageable_attached(Imessageable* /*_pmessageable*/) {}
	virtual	void				on_messageable_detaching(Imessageable* /*_pmessageable*/) {}
	virtual	void				on_messageable_detached(Imessageable* /*_pmessageable*/) {}

protected:
	virtual result_code			process_transmit_message(sMESSAGE& _msg) override;

	virtual bool				process_attach(Imessageable* _pmessageable) override { return this->process_attach_messageable_back(_pmessageable);}
	virtual bool				process_detach(Imessageable* _pmessageable) noexcept override;
	virtual int					process_reset_message_transmitter() noexcept override;
			bool				process_attach_messageable_back(Imessageable* _pmessageable);
private:
			std::mutex			m_lockable_array_messageable;
			std::array<object_ptr<Imessageable>, MAX_MEDIATOR> m_array_messageable;
			int					m_array_messageable_size = 0;
};

inline result_code Nmessage_transmitter::process_transmit_message(sMESSAGE& _msg)
{
	// declare)
	std::array<object_ptr<Imessageable>, MAX_MEDIATOR> array_messageable;
	int array_messageable_size;

	// 1) copy messageable list for temporary
	scoped_lock(this->m_lockable_array_messageable)
	{
		array_messageable_size = this->m_array_messageable_size;
		auto iter_src = this->m_array_messageable.begin();
		auto iter_src_end = iter_src + array_messageable_size;
		auto iter_dest = array_messageable.begin();

		// - on_messageable_detaching
		while(iter_src != iter_src_end)
		{
			*iter_dest = *iter_src;

			++iter_src;
			++iter_dest;
		}
	}

	// 2) call 'request_process_messagel' all of messageable
	for (auto iter = array_messageable.begin(), iter_end = iter + array_messageable_size; iter != iter_end; ++iter)
	{
		// - call 'request_process_messagel'
		const auto result = (*iter)->request_process_message(_msg);

		// check) return if processed
		RETURN_IF(result != eRESULT::BYPASS, result);
	}

	// return) 
	return result_code(eRESULT::BYPASS);
}

inline bool Nmessage_transmitter::process_detach(Imessageable* _pmessageable) noexcept
{
	// 1)  if nulll, remove all
	if (_pmessageable == nullptr)
	{
		return this->process_reset_message_transmitter() != 0;
	}

	scoped_lock(m_lockable_array_messageable)
	{
		// check)
		RETURN_IF(this->m_array_messageable_size == 0, false);
		
		for(auto iter = this->m_array_messageable.begin(), iter_end = iter + this->m_array_messageable_size; iter!=iter_end; ++iter)
		{
			if (*iter == _pmessageable)
			{
				// - call 'on_messageable_detaching'
				this->on_messageable_detaching(_pmessageable);

				if (iter != iter_end)
				{
					do
					{
						auto iter_pre = iter++;
						*iter_pre = std::move(*iter);
					} while (iter != iter_end);
				}
				else
				{
					(*iter).reset();
				}
				--this->m_array_messageable_size;
				break;
			}
		}
	}

	// 4) call 'on_messageable_detached'
	this->on_messageable_detached(_pmessageable);

	// 5) release
	this->release();

	// return) 
	return true;
}

inline int Nmessage_transmitter::process_reset_message_transmitter() noexcept
{
	// declare) 
	std::array<object_ptr<Imessageable>, MAX_MEDIATOR> array_messageable;
	int array_messageable_size = 0;

	scoped_lock(this->m_lockable_array_messageable)
	{
		// - Swap
		array_messageable = std::move(this->m_array_messageable);
		array_messageable_size = this->m_array_messageable_size;
		this->m_array_messageable_size = 0;

		// - on_messageable_detaching
		for (auto iter = array_messageable.begin(), iter_end = iter + array_messageable_size; iter != iter_end; ++iter)
		{
			this->on_messageable_detaching(*iter);
		}
	}

	for (auto iter = array_messageable.begin(), iter_end = iter + array_messageable_size; iter != iter_end; ++iter)
	{
		// - on_messageable_detached
		this->on_messageable_detached(*iter);

		// - ReleaseÇÑ´Ù.
		this->release();
	}

	// return)
	return array_messageable_size;
}

inline bool Nmessage_transmitter::process_attach_messageable_back(Imessageable* _pmessageable)
{
	// check) 
	RETURN_IF(_pmessageable == nullptr, false);

	scoped_lock(this->m_lockable_array_messageable)
	{
		// check) 
		CGASSERT_ERROR(this->m_array_messageable_size < MAX_MEDIATOR);

		// check)
		THROW_IF(this->m_array_messageable_size >= MAX_MEDIATOR, throwable::failure(eRESULT::FULL, 0, "messageable full"));

		// check) return if aleady exist
		{
			auto iter = this->m_array_messageable.begin();
			auto iter_end = this->m_array_messageable.begin() + this->m_array_messageable_size;

			while (iter != iter_end)
			{
				if (*iter == _pmessageable)
					return false;

				++iter;
			}
		}

		// 1) OnRegistering
		auto result = this->on_messageable_attaching(_pmessageable);

		// check) return if false
		RETURN_IF(result == false, false);

		// 2) add messageable
		this->m_array_messageable.at(this->m_array_messageable_size) = _pmessageable;
		++this->m_array_messageable_size;

		// 3) add reference count
		this->add_ref();
	}

	// 4) call 'on_messageable_attached'
	this->on_messageable_attached(_pmessageable);

	// return) 
	return true;
}


}