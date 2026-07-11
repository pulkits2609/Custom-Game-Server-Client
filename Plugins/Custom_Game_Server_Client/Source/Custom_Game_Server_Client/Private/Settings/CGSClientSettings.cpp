#include "Settings/CGSClientSettings.h"

UCGSClientSettings::UCGSClientSettings()
{
    BaseUrl = TEXT("http://localhost:8080");

    RealtimeHost = TEXT("127.0.0.1");
    RealtimePort = 8081;
}

FName UCGSClientSettings::GetCategoryName() const
{
	return TEXT("Custom Game Server");
}

FText UCGSClientSettings::GetSectionText() const
{
	return FText::FromString(
		TEXT("Custom Game Server Client"));
}