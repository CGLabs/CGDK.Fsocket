// Copyright Epic Games, Inc. All Rights Reserved.


#include "example_simpleGameModeBase.h"

void Aexample_simpleGameModeBase::StartPlay()
{
	// 1) start...
	Super::StartPlay();

	// 2) socket_example�� �űԷ� �����մϴ�.(������ �ʿ��� �� �����Ͻø� �˴ϴ�.)
	auto psocket = make_own<socket_example>();

	// 3) ������ �ּҸ� �����Ѵ�.(127.0.0.1(local_host)�� 20000 ��Ʈ�� �����Ѵ�.)
	TSharedRef<FInternetAddr> addr_session_server = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool result;
	addr_session_server->SetIp(TEXT("127.0.0.1"), result);
	addr_session_server->SetPort(20000);

	// 4) ����!
	psocket->start(addr_session_server);

	// 5) ������ socket�� entity manager�� ����(own_ptr<T>�� move�θ� ������ �����մϴ�.)
	this->m_psocket = std::move(psocket);
}

void Aexample_simpleGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 1) end play
	Super::EndPlay(EndPlayReason);

	// 2) Play�� �����ϸ� reset�ϸ�m_psocket�� nullptr�� reset�մϴ�.(������ �ʿ��� �� reset�Ͻø� �˴ϴ�.)
	//    owp_ptr<>�̹Ƿ� destroy�� ȣ��Ǿ� socket�� ������ ����ϴ�.
	this->m_psocket.reset();
}
