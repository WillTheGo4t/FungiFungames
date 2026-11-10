#include "AudioConfigRegistry.h"
#include "Kismet/GameplayStatics.h"
#include "Editor.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"

#if WITH_EDITOR
void UAudioConfigRegistry::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
    Super::PostEditChangeChainProperty(PropertyChangedEvent);

    // Prüfen, ob sich eine Variable geändert hat
    if (PropertyChangedEvent.Property != nullptr)
    {
        const FName PropertyName = PropertyChangedEvent.Property->GetFName();

        // Hat der User "LoadingBehavior" im Editor umgestellt?
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FGameSoundWaveEntry, LoadingBehavior))
        {
            // Schleife durch die Map (Simpel gehalten für dieses Beispiel)
            for (auto& Pair : AudioRegistry)
            {
                for (FGameSoundWaveEntry& Entry : Pair.Value.SoundWaves)
                {
                    // Lade die Audio-Datei in den RAM
                    if (USoundWave* Wave = Entry.Wave.LoadSynchronous())
                    {
                        // Wenn der Wert abweicht, setze ihn und speichere die Datei!
                        if (Wave->LoadingBehavior != Entry.LoadingBehavior)
                        {
                            Wave->LoadingBehavior = Entry.LoadingBehavior;
                            Wave->MarkPackageDirty(); // Sagt der Engine: "Datei wurde geändert"
                            UE_LOG(LogTemp, Warning, TEXT("Updated Loading Behavior for: %s"), *Wave->GetName());
                        }
                    }
                }
            }
        }
    }
}

void UAudioConfigRegistry::PlayPreview()
{
    // Stoppe alten Sound falls er läuft
    StopPreview();

    if (!AudioRegistry.Contains(TagToPreview))
    {
        UE_LOG(LogTemp, Warning, TEXT("Tag not found in Registry for Preview."));
        return;
    }

    FGameSoundData Data = AudioRegistry[TagToPreview];

    if (Data.SoundWaves.Num() > 0)
    {
        // Wähle das erste Wave-Entry für das Preview
        FGameSoundWaveEntry Entry = Data.SoundWaves[0];

        if (USoundWave* Wave = Entry.Wave.LoadSynchronous())
        {
            // Erzeuge im Editor einen Sound
            PreviewAudioComponent = UGameplayStatics::SpawnSound2D(
                GEditor->GetEditorWorldContext().World(),
                Wave,
                Data.Volume * Entry.VolumeMultiplier,
                Data.PitchMin * Entry.PitchMultiplier
            );
        }
    }
}

void UAudioConfigRegistry::StopPreview()
{
    if (PreviewAudioComponent && PreviewAudioComponent->IsPlaying())
    {
        PreviewAudioComponent->Stop();
    }
}
#endif