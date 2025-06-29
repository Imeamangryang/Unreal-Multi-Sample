// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "UEMS_AttributeSet.generated.h"

/**
 * 
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class UE5MULTISAMPLE_API UUEMS_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UUEMS_AttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_WalkSpeed)
	FGameplayAttributeData WalkSpeed;
	ATTRIBUTE_ACCESSORS(UUEMS_AttributeSet, WalkSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_SprintSpeed)
	FGameplayAttributeData SprintSpeed;
	ATTRIBUTE_ACCESSORS(UUEMS_AttributeSet, SprintSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UUEMS_AttributeSet, Damage)
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_LevelXP)
	FGameplayAttributeData LevelXP;
	ATTRIBUTE_ACCESSORS(UUEMS_AttributeSet, LevelXP)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Stealth)
	FGameplayAttributeData Stealth;
	ATTRIBUTE_ACCESSORS(UUEMS_AttributeSet, Stealth)

protected:
	UFUNCTION()
	void OnRep_WalkSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_SprintSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_LevelXP(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Stealth(const FGameplayAttributeData& OldValue);
	

};
