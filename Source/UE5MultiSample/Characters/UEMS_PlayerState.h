// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UEMS_PlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnXPChanged, int32, NewXP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterLevelChanged, int32, NewLevel);

/**
 * 
 */
UCLASS()
class UE5MULTISAMPLE_API AUEMS_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnXPChanged OnXPChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCharacterLevelChanged OnCharacterLevelChanged;

	UFUNCTION(BlueprintCallable, Category = "EXP")
	void AddXP(int32 Value);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EXP", ReplicatedUsing = "OnRep_XP")
	int32 XP = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EXP", ReplicatedUsing = "OnRep_CharacterLevel")
	int32 CharacterLevel = 1;
	
	UFUNCTION()
	void OnRep_XP(int32 OldValue) const;

	UFUNCTION()
	void OnRep_CharacterLevel(int32 OldValue) const;
};
