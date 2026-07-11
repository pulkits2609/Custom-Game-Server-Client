#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Sessions/Delegates/CGSSessionDelegates.h"
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
    void Login(const FString& InUsername, const FString& InPassword);
	void Logout();
	void FetchSession();

    //Session State
    const FString& GetSessionID() const;
    const FString& GetSessionToken() const;
    const FString& GetUsername() const;
    const FString& GetPlayerName() const;

    bool IsLoggedIn() const;

    UPROPERTY(BlueprintAssignable, Category="CGS|Session")
    FOnLoginSuccess OnLoginSuccess;

    UPROPERTY(BlueprintAssignable, Category="CGS|Session")
    FOnLoginFailed OnLoginFailed;

    UPROPERTY(BlueprintAssignable, Category="CGS|Session")
    FOnLogoutSuccess OnLogoutSuccess;

    UPROPERTY(BlueprintAssignable, Category="CGS|Session")
    FOnSessionFetched OnSessionFetched;

    UPROPERTY(BlueprintAssignable, Category="CGS|Session")
    FOnSessionFetchFailed OnSessionFetchFailed;

private:

    FString SessionID;
    FString SessionToken;
    FString Username;
    FString PlayerName;

};