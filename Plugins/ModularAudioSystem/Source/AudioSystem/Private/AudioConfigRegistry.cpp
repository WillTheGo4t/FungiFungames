#include "AudioConfigRegistry.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

#if WITH_EDITOR
void UAudioConfigRegistry::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
    Super::PostEditChangeChainProperty(PropertyChangedEvent);
    if (PropertyChangedEvent.ChangeType != EPropertyChangeType::ValueSet)
    {
        return;
    }

    if (PropertyChangedEvent.Property != nullptr)
    {
        const FName PropertyName = PropertyChangedEvent.Property->GetFName();

        if (PropertyName == GET_MEMBER_NAME_CHECKED(FGameSoundWaveEntry, LoadingBehavior))
        {
            for (auto& Pair : AudioRegistry)
            {
                for (FGameSoundWaveEntry& Entry : Pair.Value.SoundWaves)
                {
                    if (!Entry.Wave.IsNull())
                    {
                        if (USoundWave* Wave = Entry.Wave.LoadSynchronous())
                        {
                            if (Wave->LoadingBehavior != Entry.LoadingBehavior)
                            {
                                Wave->LoadingBehavior = Entry.LoadingBehavior;
                                Wave->MarkPackageDirty();
                                UE_LOG(LogTemp, Warning, TEXT("Updated Loading Behavior for: %s"), *Wave->GetName());
                            }
                        }
                    }
                }
            }
        }
    }
}
#endif