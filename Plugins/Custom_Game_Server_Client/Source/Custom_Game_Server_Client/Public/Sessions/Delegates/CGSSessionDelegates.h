#pragma once

#include "CoreMinimal.h"
#include "CGSSessionDelegates.generated.h"

USTRUCT(BlueprintType)
struct FCGSLoginResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString SessionID;

    UPROPERTY(BlueprintReadOnly)
    FString Token;
};

USTRUCT(BlueprintType)
struct FCGSSessionInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString SessionID;

    UPROPERTY(BlueprintReadOnly)
    FString Username;

    UPROPERTY(BlueprintReadOnly)
    FString PlayerName;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLoginSuccess,
    const FCGSLoginResponse&,
    Response);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLoginFailed,
    const FString&,
    Error);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
    FOnLogoutSuccess);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnSessionFetched,
    const FCGSSessionInfo&,
    Response);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnSessionFetchFailed,
    const FString&,
    Error);