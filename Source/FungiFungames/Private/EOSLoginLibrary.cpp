#include "EOSLoginLibrary.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Engine/Engine.h"

void UEOSLoginLibrary::ForceDeviceIDLogin()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
		if (Identity.IsValid())
		{
			Identity->AddOnLoginCompleteDelegate_Handle(0, FOnLoginCompleteDelegate::CreateLambda([](int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
				{
					if (bWasSuccessful)
					{
						if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, TEXT("EOS SERVER: LOGIN ERFOLGREICH! Device ID aktiv! Du kannst hosten."));
					}
					else
					{
						if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, FString::Printf(TEXT("EOS FEHLER: %s"), *Error));
					}
				}));

			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("C++: AutoLogin (New Login Flow) gestartet..."));

			Identity->AutoLogin(0);
		}
	}
}