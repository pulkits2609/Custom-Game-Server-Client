#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGSWidgetBase.generated.h"

class UCGSSessionSubsystem;
class UCGSLobbySubsystem;

UCLASS(Abstract)
class CUSTOM_GAME_SERVER_CLIENT_API UCGSWidgetBase
    : public UUserWidget
{
    GENERATED_BODY()

protected:

    virtual void NativeConstruct() override;

    UCGSSessionSubsystem* GetSessionSubsystem() const;
    UCGSLobbySubsystem* GetLobbySubsystem() const;
};