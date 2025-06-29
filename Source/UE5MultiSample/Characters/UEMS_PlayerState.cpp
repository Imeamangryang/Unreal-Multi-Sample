// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/UEMS_PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GAS/UEMS_AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Characters/UEMS_Character.h"
#include "Characters/UEMS_CharacterStats.h"

void AUEMS_PlayerState::AddXP(const int32 Value)
{
	XP += Value;
	OnXPChanged.Broadcast(XP);

	if (const auto Character = Cast<AUEMS_Character>(GetPawn()))
	{
		if (Character->GetCharacterStats()->NextLevelExp < XP)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("Level Up!"));

			CharacterLevel++;
			Character->UpdateCharacterStats(CharacterLevel);
			OnCharacterLevelChanged.Broadcast(CharacterLevel);
		}
	}
}

// Replication되어 변경된 XP 값이 클라이언트에 전파될 때 호출되는 함수
void AUEMS_PlayerState::OnRep_XP(int32 OldValue) const
{
	OnXPChanged.Broadcast(XP);
}

void AUEMS_PlayerState::OnRep_CharacterLevel(int32 OldValue) const
{
	OnCharacterLevelChanged.Broadcast(CharacterLevel);
}

/**
 * Called to get the list of replicated properties
 * @param OutLifetimeProps - List of replicated properties
 */
void AUEMS_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AUEMS_PlayerState, XP, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AUEMS_PlayerState, CharacterLevel, COND_OwnerOnly);
}

