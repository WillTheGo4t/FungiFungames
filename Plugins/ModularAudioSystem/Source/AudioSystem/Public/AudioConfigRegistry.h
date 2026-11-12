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

    // Volume multiplier for this specific sound. 
    // Multiplies with the base Volume of the whole Tag.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VolumeMultiplier = 1.0f;

    // Pitch multiplier for this specific sound
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PitchMultiplier = 1.0f;

    // Defines how this sound is loaded into memory.
    // Inherited (Default): Uses project settings.
    // RetainOnLoad: For short, frequently repeating sounds like footsteps or UI clicks.
    // PrimeOnLoad: For background music or ambiance that plays shortly after loading.
    // LoadOnDemand: For rare or very long audio files to save memory.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESoundWaveLoadingBehavior LoadingBehavior = ESoundWaveLoadingBehavior::Inherited;
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

    // The category of this sound (e.g., UI, SFX, Music).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    ESoundClassEnum Class = ESoundClassEnum::SFX;

    // List of sound variations. One will be picked randomly when played.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TArray<FGameSoundWaveEntry> SoundWaves;

    // Base volume for the entire Tag.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float Volume = 1.0f;

    // Minimum pitch range applied randomly when spawning the sound.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float PitchMin = 0.8f;

    // Maximum pitch range applied randomly when spawning the sound.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float PitchMax = 1.2f;

    // If true, the sound repeats continuously until stopped.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    bool bLooping = false;

    // Defines the audio Type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    EAudioPlaybackType PlaybackType = EAudioPlaybackType::Play2D;

    // If true, the sound continues playing during level transitions. (Only for 2D Sounds!)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (EditCondition = "PlaybackType == EAudioPlaybackType::Play2D"))
    bool bPersistAcrossLevel = false;

    // Defines how the sound volume drops off over distance.
    // Only used for 3D sounds (At Location / Attached).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spacialization", meta = (EditCondition = "PlaybackType != EAudioPlaybackType::Play2D"))
    USoundAttenuation* Attenuation = nullptr;

    // Defines rules for how many instances of this sound can play simultaneously.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limits")
    USoundConcurrency* Concurrency = nullptr;

    // Advanced settings for fading and exclusive playback.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Music Settings")
    FGameSoundMusicSettings MusicSettings;
};


UCLASS(BlueprintType)
class AUDIOSYSTEM_API UAudioConfigRegistry : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Registry")
    TMap<FGameplayTag, FGameSoundData> AudioRegistry;
#if WITH_EDITOR
    virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
};