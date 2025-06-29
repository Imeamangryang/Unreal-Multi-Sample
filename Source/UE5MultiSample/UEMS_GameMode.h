// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UEMS_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class UE5MULTISAMPLE_API AUEMS_GameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AUEMS_GameMode();
	
	// 각 AI 액터들과 통신을 위한 함수
	UFUNCTION(BlueprintCallable, Category = "Minion")
	void AlertMinions(AActor* AlertInstigator, const FVector& Location, float Radius);
	
};
