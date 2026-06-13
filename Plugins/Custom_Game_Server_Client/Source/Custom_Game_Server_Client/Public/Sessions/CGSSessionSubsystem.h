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

    // Session APIs
	void Login(const FString& Username, const FString& Password);
	void Logout();
	void FetchSession();

    //Session State
    const FString& GetSessionID() const;
    const FString& GetSessionToken() const;

    bool IsLoggedIn() const;

private:

    FString SessionID;
    FString SessionToken;
};