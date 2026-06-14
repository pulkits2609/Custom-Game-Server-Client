#pragma once

#include "CoreMinimal.h"
#include "UI/Base/CGSWidgetBase.h"

#include "Lobby/CGSLobbyTypes.h"

#include "CGSCreateLobbyWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class CUSTOM_GAME_SERVER_CLIENT_API UCGSCreateLobbyWidget
    : public UCGSWidgetBase
{
    GENERATED_BODY()

protected:

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

public:

    UFUNCTION(BlueprintCallable, Category="CGS|Lobby")
    void CreateLobby(
        const FString& LobbyName,
        int32 MaxPlayers);

protected:

    UFUNCTION()
    virtual void HandleLobbyCreated(
        const FCGSLobbyActionResponse& Response);

    UFUNCTION()
    virtual void HandleLobbyCreateFailed(
        const FString& Error);
};