#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CGSSessionSubsystem.generated.h"

UCLASS()
class CUSTOM_GAME_SERVER_CLIENT_API UCGSSessionSubsystem
    : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    void Login(const FString& Username,const FString& Password);

private:

    FString SessionID;
    FString SessionToken;
};