#include "UI/Base/CGSWidgetBase.h"

#include "Sessions/CGSSessionSubsystem.h"
#include "Lobby/CGSLobbySubsystem.h"

void UCGSWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();
}

UCGSSessionSubsystem*
UCGSWidgetBase::GetSessionSubsystem() const
{
    if (!GetGameInstance())
    {
        return nullptr;
    }

    return GetGameInstance()->
        GetSubsystem<UCGSSessionSubsystem>();
}

UCGSLobbySubsystem*
UCGSWidgetBase::GetLobbySubsystem() const
{
    if (!GetGameInstance())
    {
        return nullptr;
    }

    return GetGameInstance()->
        GetSubsystem<UCGSLobbySubsystem>();
}