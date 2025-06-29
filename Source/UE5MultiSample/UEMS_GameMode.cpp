// Fill out your copyright notice in the Description page of Project Settings.


#include "UEMS_GameMode.h"
#include "UEMS_GameState.h"
#include "Characters/UEMS_PlayerController.h"
#include "Characters/UEMS_PlayerState.h"
#include "Characters/UEMS_Character.h"
#include "Characters/UEMS_Minion.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"



AUEMS_GameMode::AUEMS_GameMode()
{
	// GameState 할당
	GameStateClass = AUEMS_GameState::StaticClass();

	// PlayerState 할당
	PlayerStateClass = AUEMS_PlayerState::StaticClass();

	// PlayerController 할당
	PlayerControllerClass = AUEMS_PlayerController::StaticClass();

	// DefaultPawnClass 할당
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/BP_Character"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AUEMS_GameMode::AlertMinions(AActor* AlertInstigator, const FVector& Location, float Radius)
{
	TArray<AActor*> Minions;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUEMS_Minion::StaticClass(), Minions);

	for (const auto Minion : Minions)
	{
		if (AlertInstigator == Minion) continue; // 자신은 제외

		if (const auto Distance = FVector::Distance(AlertInstigator->GetActorLocation(), Minion->GetActorLocation()); Distance < Radius) // 반경 내에 있는지 확인
		{
			if (const auto MinionCharacter = Cast<AUEMS_Minion>(Minion))
			{
				MinionCharacter->GoToLocation(Location); // 위치로 이동
			}
		}
	}
}
