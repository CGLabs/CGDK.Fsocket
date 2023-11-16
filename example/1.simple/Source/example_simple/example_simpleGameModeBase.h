// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "socket_example.h"
#include "example_simpleGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class EXAMPLE_SIMPLE_API Aexample_simpleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void				StartPlay() override;
	virtual void				EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	own_ptr<socket_example> m_psocket;
};

