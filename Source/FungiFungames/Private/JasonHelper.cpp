// Fill out your copyright notice in the Description page of Project Settings.


#include "JasonHelper.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "HAL/PlatformFileManager.h"

static TSharedPtr<FJsonObject> LoadJsonFile(const FString& FilePath)
{
    FString JsonString;

    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        return MakeShared<FJsonObject>();
    }

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        return JsonObject;
    }

    return MakeShared<FJsonObject>();
}

static bool SaveJsonFile(const FString& FilePath, TSharedPtr<FJsonObject> JsonObject)
{
    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);

    if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
    {
        return false;
    }

    FString Directory = FPaths::GetPath(FilePath);
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*Directory))
    {
        PlatformFile.CreateDirectoryTree(*Directory);
    }

    return FFileHelper::SaveStringToFile(JsonString, *FilePath);
}


bool UJasonHelper::ReadFloatFromJson(const FString& FilePath, const FString& Key, float& OutValue)
{
    TSharedPtr<FJsonObject> JsonObject = LoadJsonFile(FilePath);

    if (JsonObject->HasField(Key))
    {
        OutValue = (float)JsonObject->GetNumberField(Key);
        return true;
    }
    OutValue = 0.f;
    return false;
}

bool UJasonHelper::WriteFloatToJson(const FString& FilePath, const FString& Key, float Value)
{
    TSharedPtr<FJsonObject> JsonObject = LoadJsonFile(FilePath);

    JsonObject->SetNumberField(Key, Value);

    return SaveJsonFile(FilePath, JsonObject);
}

bool UJasonHelper::KeyExistsInJson(const FString& FilePath, const FString& Key)
{
    TSharedPtr<FJsonObject> JsonObject = LoadJsonFile(FilePath);
    return JsonObject->HasField(Key);
}

bool UJasonHelper::WriteIntToJson(const FString& FilePath, const FString& Key, int Value)
{
    TSharedPtr<FJsonObject> JsonObject = LoadJsonFile(FilePath);
    JsonObject->SetNumberField(Key, Value);
    return SaveJsonFile(FilePath, JsonObject);
}

bool UJasonHelper::ReadIntFromJson(const FString& FilePath, const FString& Key, int& OutValue)
{
    TSharedPtr<FJsonObject> JsonObject = LoadJsonFile(FilePath);
    if (JsonObject->HasField(Key))
    {
        OutValue = (int)JsonObject->GetNumberField(Key);
        return true;
    }
    OutValue = 0;
    return false;
}

bool UJasonHelper::WriteBoolToJson(const FString& FilePath, const FString& Key, bool Value)
{
    TSharedPtr<FJsonObject> JsonObject = LoadJsonFile(FilePath);
    JsonObject->SetBoolField(Key, Value);
    return SaveJsonFile(FilePath, JsonObject);
}

bool UJasonHelper::ReadBoolFromJson(const FString& FilePath, const FString& Key, bool& OutValue)
{
    TSharedPtr<FJsonObject> JsonObject = LoadJsonFile(FilePath);
    if (JsonObject->HasField(Key))
    {
        OutValue = JsonObject->GetBoolField(Key);
        return true;
    }
    OutValue = false;
    return false;
}

bool UJasonHelper::SortJasonKeysAlphabetically(const FString& FilePath)
{
    TSharedPtr<FJsonObject> JsonObject = LoadJsonFile(FilePath);

    if (!JsonObject.IsValid())
    {
        return false;
    }

    TArray<FString> Keys;
    JsonObject->Values.GetKeys(Keys);
    
    Keys.Sort();

    TSharedPtr<FJsonObject> SortedJsonObject = MakeShared<FJsonObject>();
    
    for (const FString& Key : Keys)
    {
        SortedJsonObject->SetField(Key, JsonObject->TryGetField(Key));
    }

    return SaveJsonFile(FilePath, SortedJsonObject);
}

FString UJasonHelper::GetBalancingFilePath()
{
    return FPaths::Combine(
        FPaths::ProjectContentDir(),
        TEXT("Balancing"),
        TEXT("BalancingData.json")
    );
}