#include "UI/Login/CGSLoginWidget.h"

#include "Sessions/CGSSessionSubsystem.h"

void UCGSLoginWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (UCGSSessionSubsystem* Session =
        GetSessionSubsystem())
    {
        Session->OnLoginSuccess.AddDynamic(
            this,
            &UCGSLoginWidget::HandleLoginSuccess);

        Session->OnLoginFailed.AddDynamic(
            this,
            &UCGSLoginWidget::HandleLoginFailed);
    }
}

void UCGSLoginWidget::NativeDestruct()
{
    if (UCGSSessionSubsystem* Session =
        GetSessionSubsystem())
    {
        Session->OnLoginSuccess.RemoveDynamic(
            this,
            &UCGSLoginWidget::HandleLoginSuccess);

        Session->OnLoginFailed.RemoveDynamic(
            this,
            &UCGSLoginWidget::HandleLoginFailed);
    }

    Super::NativeDestruct();
}

void UCGSLoginWidget::Login(
    const FString& Username,
    const FString& Password)
{
    if (UCGSSessionSubsystem* Session =
        GetSessionSubsystem())
    {
        Session->Login(
            Username,
            Password);
    }
}

void UCGSLoginWidget::HandleLoginSuccess(
    const FCGSLoginResponse& Response)
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Login Widget Success"));
}

void UCGSLoginWidget::HandleLoginFailed(
    const FString& Error)
{
    UE_LOG(
        LogTemp,
        Error,
        TEXT("Login Widget Failed: %s"),
        *Error);
}