#pragma once

#include "CoreMinimal.h"
#include "UI/Base/CGSWidgetBase.h"
#include "Sessions/Delegates/CGSSessionDelegates.h"

#include "CGSLoginWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class CUSTOM_GAME_SERVER_CLIENT_API UCGSLoginWidget
    : public UCGSWidgetBase
{
    GENERATED_BODY()

protected:

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

public:

    UFUNCTION(BlueprintCallable, Category="CGS|Login")
    void Login(
        const FString& Username,
        const FString& Password);

protected:

    UFUNCTION()
    virtual void HandleLoginSuccess(
        const FCGSLoginResponse& Response);

    UFUNCTION()
    virtual void HandleLoginFailed(
        const FString& Error);
};