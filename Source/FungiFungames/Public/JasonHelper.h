// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JasonHelper.generated.h"

UCLASS()
class FUNGIFUNGAMES_API UJasonHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintCallable, Category = "MISC")
	static bool ReadFloatFromJson(const FString& FilePath, const FString& Key, float& OutValue);


	UFUNCTION(BlueprintCallable, Category = "MISC")
	static bool WriteFloatToJson(const FString& FilePath, const FString& Key, float Value);


	UFUNCTION(BlueprintCallable, Category = "MISC")
	static bool WriteIntToJson(const FString& FilePath, const FString& Key, int Value);


	UFUNCTION(BlueprintCallable, Category = "MISC")
	static bool ReadIntFromJson(const FString& FilePath, const FString& Key, int& OutValue);


	UFUNCTION(BlueprintCallable, Category = "MISC")
	static bool WriteBoolToJson(const FString& FilePath, const FString& Key, bool Value);


	UFUNCTION(BlueprintCallable, Category = "MISC")
	static bool ReadBoolFromJson(const FString& FilePath, const FString& Key, bool& OutValue);


	UFUNCTION(BlueprintCallable, Category = "MISC")
	static bool KeyExistsInJson(const FString& FilePath, const FString& Key);


	UFUNCTION(BlueprintPure, Category = "MISC")
	static FString GetBalancingFilePath();

	UFUNCTION(BlueprintCallable, Category = "MISC")
	static bool SortJasonKeysAlphabetically(const FString& FilePath);
};
