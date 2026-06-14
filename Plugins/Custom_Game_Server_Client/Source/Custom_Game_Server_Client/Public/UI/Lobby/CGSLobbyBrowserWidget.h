#pragma once

#include "CoreMinimal.h"
#include "UI/Base/CGSWidgetBase.h"
#include "Lobby/CGSLobbyTypes.h"

#include "CGSLobbyBrowserWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class CUSTOM_GAME_SERVER_CLIENT_API UCGSLobbyBrowserWidget
    : public UCGSWidgetBase
{
    GENERATED_BODY()

protected:

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

public:

    UFUNCTION(BlueprintCallable)
    void RefreshLobbyList();

    UFUNCTION(BlueprintCallable)
    void JoinLobby(const FString& LobbyID);

protected:

    UFUNCTION()
    virtual void HandleLobbyListReceived(
        TArray<FCGSLobbyInfo> Lobbies);

    UFUNCTION()
    virtual void HandleLobbyListFailed(
        const FString& Error);

    UFUNCTION()
    virtual void HandleLobbyJoined(
        const FCGSLobbyActionResponse& Response);

    UFUNCTION()
    virtual void HandleLobbyJoinFailed(
        const FString& Error);
};