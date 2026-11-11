#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

// Diese Klasse überschreibt das Standard-UI deines Data Assets
class FAudioConfigRegistryCustomization : public IDetailCustomization
{
public:
    // Nötig für die Registrierung in der Engine
    static TSharedRef<IDetailCustomization> MakeInstance();

    // Die Hauptfunktion, in der wir später die UI-Elemente (Buttons, Sortierung) zeichnen
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};
