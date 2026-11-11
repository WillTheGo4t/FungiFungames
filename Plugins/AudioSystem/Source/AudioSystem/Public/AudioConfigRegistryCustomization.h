#pragma once

#if WITH_EDITOR
#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "IPropertyTypeCustomization.h"
#include "Input/Reply.h" 

class FAudioConfigRegistryCustomization : public IDetailCustomization
{
public:
    static TSharedRef<IDetailCustomization> MakeInstance();
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
private:
    FReply OnStopAllClicked();
};

class FGameSoundDataCustomization : public IPropertyTypeCustomization
{
public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();
    virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, class FDetailWidgetRow& HeaderRow, class IPropertyTypeCustomizationUtils& CustomizationUtils) override;
    virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, class IDetailChildrenBuilder& ChildBuilder, class IPropertyTypeCustomizationUtils& CustomizationUtils) override;
private:
    FReply OnPlayClicked(TSharedRef<IPropertyHandle> StructPropertyHandle);
    FReply OnStopClicked(TSharedRef<IPropertyHandle> StructPropertyHandle);
};
#endif