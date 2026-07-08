// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EditorUtilityFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FUNGIFUNGAMES_API UEditorUtilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Editor Utilities")
	static bool IsEditorBuild();
	
};
