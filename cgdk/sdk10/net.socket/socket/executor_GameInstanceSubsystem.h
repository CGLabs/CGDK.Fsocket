#pragma once

#include "CoreMinimal.h"
#include "../../net.socket.h"
#include "Subsystems/WorldSubsystem.h"
#include "executor_GameInstanceSubsystem.generated.h"

UCLASS()
class Uexecutor_GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }
	virtual void Initialize(FSubsystemCollectionBase& Collection) override { CGDK::executor::socket::initialize_instance(); }
	virtual void Deinitialize() override { CGDK::executor::socket::destory_executor(); }

	///** Overridden to check global network context */
	//ENGINE_API virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;

	//virtual void Tick(float DeltaTime) override { CGDK::executor::socket::run_executor(); }
};
