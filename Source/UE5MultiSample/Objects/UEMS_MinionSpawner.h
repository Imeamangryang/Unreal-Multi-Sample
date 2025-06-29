// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UEMS_MinionSpawner.generated.h"

UCLASS()
class UE5MULTISAMPLE_API AUEMS_MinionSpawner : public AActor
{
	GENERATED_BODY()
	
	// 스폰될 영역
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> SpawnArea;

	// 타이머 핸들
	UPROPERTY()
	FTimerHandle SpawnTimerHandle;

public:	
	// Sets default values for this actor's properties
	AUEMS_MinionSpawner();

	// 스폰할 미니언 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<TSubclassOf<class AUEMS_Minion>> SpawnableMinions;

	// 스폰 리젠 주기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	float SpawnDelay = 10.0f;

	// 시작시 스폰할 미니언 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 NumMinionsAtStart = 5; 


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Spawn();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
