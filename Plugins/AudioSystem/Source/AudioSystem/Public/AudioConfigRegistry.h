#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundAttenuation.h"
#include "Sound/SoundConcurrency.h"
#include "Sound/SoundClass.h"
#include "AudioConfigRegistry.generated.h"



UENUM(BlueprintType)
enum class ESoundClassEnum : uint8
{
    UI UMETA(DisplayName = "UI"),
    SFX UMETA(DisplayName = "SFX"),
    Music UMETA(DisplayName = "Music")
};

UENUM(BlueprintType)
enum class EAudioPlaybackType : uint8
{
    Play2D UMETA(DisplayName = "Play 2D"),
    SpawnAtLocation UMETA(DisplayName = "Spawn At Location"),
    SpawnAttached UMETA(DisplayName = "Spawn Attached")
};

USTRUCT(BlueprintType)
struct FGameSoundWaveEntry
{
    GENERATED_BODY()

    // The raw audio file to play.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<USoundWave> Wave;

    // Individual volume adjustment for this specific wave.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VolumeMultiplier = 1.0f;

    // Individual pitch adjustment for this specific wave.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PitchMultiplier = 1.0f;

    // Automatically applied to the actual file when changed in the editor.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESoundWaveLoadingBehavior LoadingBehavior = ESoundWaveLoadingBehavior::RetainOnLoad;
};

// 3. Nested Struct: Music Settings
USTRUCT(BlueprintType)
struct FGameSoundMusicSettings
{
    GENERATED_BODY()

    // If true, smoothly fades out all currently playing sounds of the same class.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bExclusiveToSoundClass = false;

    // How long it takes for this new sound to fade in. (Only active if Exclusive is true!)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bExclusiveToSoundClass"))
    float FadeInTime = 1.0f;

    // How long it takes for the replaced sound to fade out.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bExclusiveToSoundClass"))
    float FadeOutTime = 2.0f;

    // Seconds before the fade-out finishes that this new sound should begin.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bExclusiveToSoundClass"))
    float CrossfadeOverlapTime = 0.5f;

    // Should the sound continue processing when volume drops to 0?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bVirtualizeWhenSilent = true;
};

USTRUCT(BlueprintType)
struct FGameSoundData
{
    GENERATED_BODY()

    // Nutzt jetzt das Enum statt der festen Klasse!
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    ESoundClassEnum Class = ESoundClassEnum::SFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TArray<FGameSoundWaveEntry> SoundWaves;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float Volume = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float PitchMin = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float PitchMax = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    bool bLooping = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    EAudioPlaybackType PlaybackType = EAudioPlaybackType::Play2D;

    // Der Bool für den Levelwechsel
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    bool bPersistAcrossLevel = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spacialization", meta = (EditCondition = "PlaybackType != EAudioPlaybackType::Play2D"))
    USoundAttenuation* Attenuation = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limits")
    USoundConcurrency* Concurrency = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Music Settings")
    FGameSoundMusicSettings MusicSettings;
};

// 5. Das Data Asset selbst
UCLASS(BlueprintType)
class AUDIOSYSTEM_API UAudioConfigRegistry : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Registry")
    TMap<FGameplayTag, FGameSoundData> AudioRegistry;

#if WITH_EDITORONLY_DATA
    // 6. Preview Funktion: Welchen Tag wollen wir gerade testen?
    UPROPERTY(EditAnywhere, Transient, Category = "Preview Tools")
    FGameplayTag TagToPreview;

    UPROPERTY(Transient)
    UAudioComponent* PreviewAudioComponent;
#endif

#if WITH_EDITOR
    // Diese Funktionen zaubern dir die Buttons in den Editor!
    UFUNCTION(CallInEditor, Category = "Preview Tools")
    void PlayPreview();

    UFUNCTION(CallInEditor, Category = "Preview Tools")
    void StopPreview();

    // Diese Engine-Funktion fängt jede Änderung im Data Asset ab
    virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
};