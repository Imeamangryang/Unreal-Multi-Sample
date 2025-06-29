#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UEMS_Interactable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UUEMS_Interactable : public UInterface
{
	GENERATED_BODY()
};

class UE5MULTISAMPLE_API IUEMS_Interactable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction", meta = (DisplayName = "Interact"))
	void Interact(class AUEMS_Character* CharacterInstigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction", meta = (DisplayName = "Can Interact"))
	bool CanInteract(AUEMS_Character* CharacterInstigator) const;
};