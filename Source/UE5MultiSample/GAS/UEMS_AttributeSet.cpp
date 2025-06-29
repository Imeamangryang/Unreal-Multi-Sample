// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/UEMS_AttributeSet.h"
#include "Net/UnrealNetwork.h"

UUEMS_AttributeSet::UUEMS_AttributeSet()
{
	InitWalkSpeed(200.f);
	InitSprintSpeed(400.f);
	InitDamage(1.f);
	InitLevelXP(10.f);
	InitStealth(1.f);
}

void UUEMS_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UUEMS_AttributeSet, WalkSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUEMS_AttributeSet, SprintSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUEMS_AttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUEMS_AttributeSet, LevelXP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUEMS_AttributeSet, Stealth, COND_None, REPNOTIFY_Always);
}

void UUEMS_AttributeSet::OnRep_WalkSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUEMS_AttributeSet, WalkSpeed, OldValue);
}

void UUEMS_AttributeSet::OnRep_SprintSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUEMS_AttributeSet, SprintSpeed, OldValue);
}

void UUEMS_AttributeSet::OnRep_Damage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUEMS_AttributeSet, Damage, OldValue);
}

void UUEMS_AttributeSet::OnRep_LevelXP(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUEMS_AttributeSet, LevelXP, OldValue);
}

void UUEMS_AttributeSet::OnRep_Stealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUEMS_AttributeSet, Stealth, OldValue);
}
