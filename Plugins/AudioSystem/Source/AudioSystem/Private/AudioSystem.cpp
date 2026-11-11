#include "AudioSystem.h"

#if WITH_EDITOR
#include "PropertyEditorModule.h"
#include "AudioConfigRegistryCustomization.h"
#endif

#define LOCTEXT_NAMESPACE "FAudioSystemModule"

void FAudioSystemModule::StartupModule()
{
#if WITH_EDITOR
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

    PropertyModule.RegisterCustomClassLayout(
        "AudioConfigRegistry", 
        FOnGetDetailCustomizationInstance::CreateStatic(&FAudioConfigRegistryCustomization::MakeInstance)
    );
#endif
}

void FAudioSystemModule::ShutdownModule()
{
#if WITH_EDITOR

    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.UnregisterCustomClassLayout("AudioConfigRegistry");
    }
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAudioSystemModule, AudioSystem)