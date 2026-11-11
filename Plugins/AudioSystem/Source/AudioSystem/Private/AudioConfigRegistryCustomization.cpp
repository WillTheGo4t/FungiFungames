#include "AudioConfigRegistryCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"

TSharedRef<IDetailCustomization> FAudioConfigRegistryCustomization::MakeInstance()
{
    return MakeShareable(new FAudioConfigRegistryCustomization());
}

void FAudioConfigRegistryCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    // Hier können wir das Standard-UI komplett ausblenden, wenn wir wollen
    // Wir holen uns später die Property "AudioRegistry" (deine TMap)
    // TSharedRef<IPropertyHandle> MapProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UAudioConfigRegistry, AudioRegistry));

    // Für den ersten Test lassen wir es leer, um zu prüfen, ob es fehlerfrei registriert wird.
}