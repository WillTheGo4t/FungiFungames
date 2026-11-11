#include "AudioConfigRegistryCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/Input/SButton.h"
#include "Editor/UnrealEdEngine.h"
#include "UnrealEdGlobals.h"
#include "GameplayTagsManager.h"
#include "MudularAudioSystem.h" 
#include "AudioConfigRegistry.h"

static FString QueuedTagToPlay = "";
static FDelegateHandle PostPIEStartedHandle;

void TriggerPlayOnSubsystem(FString TagName)
{
    if (GEditor && GEditor->GetPIEWorldContext())
    {
        if (UWorld* PIEWorld = GEditor->GetPIEWorldContext()->World())
        {
            UMudularAudioSystem* TargetSubsystem = nullptr;

            for (TObjectIterator<UMudularAudioSystem> It; It; ++It)
            {
                if (It->GetWorld() == PIEWorld && It->GetClass() != UMudularAudioSystem::StaticClass())
                {
                    TargetSubsystem = *It;
                    break;
                }
            }

            if (!TargetSubsystem) TargetSubsystem = PIEWorld->GetSubsystem<UMudularAudioSystem>();

            if (TargetSubsystem)
            {
                UE_LOG(LogTemp, Warning, TEXT("Audio Subsystem gefunden! Sende Play-Befehl an Blueprint für: %s"), *TagName);
                FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(*TagName));
                TargetSubsystem->EditorPlaySound(Tag);
            }
        }
    }
}

void TriggerStopOnSubsystem(FString TagName)
{
    if (GEditor && GEditor->GetPIEWorldContext())
    {
        if (UWorld* PIEWorld = GEditor->GetPIEWorldContext()->World())
        {
            UMudularAudioSystem* TargetSubsystem = nullptr;

            for (TObjectIterator<UMudularAudioSystem> It; It; ++It)
            {
                if (It->GetWorld() == PIEWorld && It->GetClass() != UMudularAudioSystem::StaticClass())
                {
                    TargetSubsystem = *It;
                    break;
                }
            }

            if (!TargetSubsystem) TargetSubsystem = PIEWorld->GetSubsystem<UMudularAudioSystem>();

            if (TargetSubsystem)
            {
                UE_LOG(LogTemp, Warning, TEXT("Audio Subsystem gefunden! Sende Stop-Befehl an Blueprint für: %s"), *TagName);
                FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(*TagName));
                TargetSubsystem->EditorStopSound(Tag);
            }
        }
    }
}

FString GetTagNameFromProperty(TSharedRef<IPropertyHandle> StructPropertyHandle)
{
    FString TagName = "None";
    TArray<UObject*> OuterObjects;
    StructPropertyHandle->GetOuterObjects(OuterObjects);

    if (OuterObjects.Num() > 0 && OuterObjects[0])
    {
        if (UAudioConfigRegistry* Registry = Cast<UAudioConfigRegistry>(OuterObjects[0]))
        {
            TArray<void*> RawData;
            StructPropertyHandle->AccessRawData(RawData);

            if (RawData.Num() > 0 && RawData[0])
            {
                FGameSoundData* TargetData = static_cast<FGameSoundData*>(RawData[0]);

                for (auto& Pair : Registry->AudioRegistry)
                {
                    if (&Pair.Value == TargetData)
                    {
                        TagName = Pair.Key.GetTagName().ToString();
                        break;
                    }
                }
            }
        }
    }
    return TagName;
}

TSharedRef<IDetailCustomization> FAudioConfigRegistryCustomization::MakeInstance()
{
    return MakeShareable(new FAudioConfigRegistryCustomization());
}

void FAudioConfigRegistryCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Audio System", FText::FromString("Audio System"), ECategoryPriority::Important);

    Category.AddCustomRow(FText::FromString("Master Stop"))
        .WholeRowContent()
        [
            SNew(SButton)
                .Text(FText::FromString("STOP AUDIO SIMULATION (ALL SOUNDS)"))
                .HAlign(HAlign_Center)
                .OnClicked(this, &FAudioConfigRegistryCustomization::OnStopAllClicked)
        ];
}

TSharedRef<IPropertyTypeCustomization> FGameSoundDataCustomization::MakeInstance()
{
    return MakeShareable(new FGameSoundDataCustomization());
}

void FGameSoundDataCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    HeaderRow
        .NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(0, 0, 5, 0)
                [
                    SNew(SButton)
                        .Text(FText::FromString("Play"))
                        .OnClicked(this, &FGameSoundDataCustomization::OnPlayClicked, PropertyHandle)
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(SButton)
                        .Text(FText::FromString("Stop"))
                        .OnClicked(this, &FGameSoundDataCustomization::OnStopClicked, PropertyHandle)
                ]
        ];
}

void FGameSoundDataCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    uint32 NumChildren;
    PropertyHandle->GetNumChildren(NumChildren);
    for (uint32 i = 0; i < NumChildren; ++i)
    {
        ChildBuilder.AddProperty(PropertyHandle->GetChildHandle(i).ToSharedRef());
    }
}

FReply FAudioConfigRegistryCustomization::OnStopAllClicked()
{
    if (GEditor->IsPlaySessionInProgress())
    {
        GEditor->RequestEndPlayMap();
    }
    return FReply::Handled();
}

FReply FGameSoundDataCustomization::OnPlayClicked(TSharedRef<IPropertyHandle> StructPropertyHandle)
{
    FString TagName = GetTagNameFromProperty(StructPropertyHandle);

    if (TagName == "None" || TagName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("FEHLER: Konnte den Gameplay Tag nicht auslesen! (Tag war None)"));
        return FReply::Handled();
    }

    if (GEditor->IsPlaySessionInProgress())
    {
        TriggerPlayOnSubsystem(TagName);
    }
    else
    {
        QueuedTagToPlay = TagName;

        if (!PostPIEStartedHandle.IsValid())
        {
            PostPIEStartedHandle = FEditorDelegates::PostPIEStarted.AddLambda([](bool bIsSimulating)
                {
                    TriggerPlayOnSubsystem(QueuedTagToPlay);
                    QueuedTagToPlay = "";
                    FEditorDelegates::PostPIEStarted.Remove(PostPIEStartedHandle);
                    PostPIEStartedHandle.Reset();
                });
        }

        FRequestPlaySessionParams SessionParams;
        SessionParams.WorldType = EPlaySessionWorldType::SimulateInEditor;
        GEditor->RequestPlaySession(SessionParams);
    }

    return FReply::Handled();
}

FReply FGameSoundDataCustomization::OnStopClicked(TSharedRef<IPropertyHandle> StructPropertyHandle)
{
    FString TagName = GetTagNameFromProperty(StructPropertyHandle);

    if (GEditor->IsPlaySessionInProgress())
    {
        TriggerStopOnSubsystem(TagName);
    }
    return FReply::Handled();
}