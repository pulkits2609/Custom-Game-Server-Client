#include "Sessions/CGSSessionSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

//all of the above includes are classes being provided by dependencies : 
//http , jsonUtilities, json which were added to build.cs

void UCGSSessionSubsystem::Initialize(
    FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("CGS Session Subsystem Initialized"));
    Login("Pulkit", "password123"); //testing only for now
}

void UCGSSessionSubsystem::Deinitialize()
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("CGS Session Subsystem Deinitialized"));

    Super::Deinitialize();
}

void UCGSSessionSubsystem::Login(const FString& Username, const FString& Password) {
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    JsonObject->SetStringField(
        TEXT("username"), Username
    );
    JsonObject->SetStringField(
        TEXT("password"), Password
    );

    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);

    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    //now JsonString contains username and password

    //creating HTTP Request now 
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(TEXT("http://localhost:8080/session/login"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(
        TEXT("Content-Type"),
        TEXT("application/json")
    );

    Request->SetContentAsString(JsonString);
    //before processing request , we bind response to request !
    Request->OnProcessRequestComplete().BindLambda(
        [](FHttpRequestPtr Request,
            FHttpResponsePtr Response,
            bool bWasSuccessful)
        {
            if (!bWasSuccessful){
                UE_LOG(
                    LogTemp,
                    Error,
                    TEXT("Request Failed"));

                return;
            }

            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Response: %s"),
                *Response->GetContentAsString());
        });
        
    //now we process the request!
    Request->ProcessRequest();
}