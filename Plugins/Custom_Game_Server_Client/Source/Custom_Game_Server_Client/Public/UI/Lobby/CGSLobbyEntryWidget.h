#pragma once

#include "CoreMinimal.h"
#include "UI/Base/CGSWidgetBase.h"
#include "Lobby/CGSLobbyTypes.h"

#include "CGSLobbyEntryWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class CUSTOM_GAME_SERVER_CLIENT_API UCGSLobbyEntryWidget
    : public UCGSWidgetBase
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    void SetLobbyInfo(
        const FCGSLobbyInfo& InLobbyInfo);

    UFUNCTION(BlueprintCallable)
    void JoinCurrentLobby();

    UFUNCTION(BlueprintPure)
    const FCGSLobbyInfo& GetLobbyInfo() const;

    UFUNCTION(BlueprintCallable, Category="CGS|Lobby")
    void SetTargetUsername(
        const FString& InUsername);

    UFUNCTION(BlueprintCallable, Category="CGS|Lobby")
    void KickPlayer();

protected:

    UPROPERTY(BlueprintReadOnly)
    FCGSLobbyInfo LobbyInfo;

    UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
    FString TargetUsername;
};