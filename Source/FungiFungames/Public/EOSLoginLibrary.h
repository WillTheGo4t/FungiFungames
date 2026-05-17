#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EOSLoginLibrary.generated.h"

UCLASS()
class FUNGIFUNGAMES_API UEOSLoginLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Unser eigener, fehlerfreier Login-Knoten
	UFUNCTION(BlueprintCallable, Category = "EOS Custom")
	static void ForceDeviceIDLogin();
};