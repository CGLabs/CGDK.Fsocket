#pragma once
// ----------------------------------------------------------------------------
//
// Isocket
//
// ----------------------------------------------------------------------------
class Isocket :
			virtual public				net::socket::tcp_client<>
{
public:
	virtual void						prepare(AGameModeBase* _game_mode, Imessageable* _pmessageable) = 0;
};

