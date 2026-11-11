#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h" 
#include "MudularAudioSystem.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AUDIOSYSTEM_API UMudularAudioSystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "Audio|Editor")
    void EditorPlaySound(FGameplayTag SoundTag);

    UFUNCTION(BlueprintImplementableEvent, Category = "Audio|Editor")
    void EditorStopSound(FGameplayTag SoundTag);
};