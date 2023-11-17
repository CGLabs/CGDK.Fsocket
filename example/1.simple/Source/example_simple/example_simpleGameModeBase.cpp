// Copyright Epic Games, Inc. All Rights Reserved.


#include "example_simpleGameModeBase.h"

void Aexample_simpleGameModeBase::StartPlay()
{
	// 1) start...
	Super::StartPlay();

	// 2) socket_example를 신규로 생성합니다.(언제든 필요할 때 생성하시면 됩니다.)
	auto psocket = make_own<socket_example>();

	// 3) 접속할 주소를 설정한다.(127.0.0.1(local_host)의 20000 포트로 접속한다.)
	TSharedRef<FInternetAddr> addr_session_server = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool result;
	addr_session_server->SetIp(TEXT("127.0.0.1"), result);
	addr_session_server->SetPort(20000);

	// 4) 접속!
	psocket->start(addr_session_server);

	// 5) 생성한 socket을 entity manager에 설정(own_ptr<T>는 move로만 대입이 가능합니다.)
	this->m_psocket = std::move(psocket);
}

void Aexample_simpleGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 1) end play
	Super::EndPlay(EndPlayReason);

	// 2) Play를 종료하면 reset하면m_psocket이 nullptr로 reset합니다.(언제든 필요할 때 reset하시면 됩니다.)
	//    owp_ptr<>이므로 destroy가 호출되어 socket의 접속이 끊깁니다.
	this->m_psocket.reset();
}
