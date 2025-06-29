// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UEMS_Minion.generated.h"

UCLASS()
class UE5MULTISAMPLE_API AUEMS_Minion : public ACharacter
{
	GENERATED_BODY()

	// 주변 캐릭터를 감지하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPawnSensingComponent> PawnSense;

	// 캐릭터를 감지하기 위한 콜리전 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> Collision;

	// 이동할 위치
	UPROPERTY()
	FVector PatrolLocation;

	// AI가 주변 미니언들에게 알림을 전파하기 위한 반경
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float AlertRadius = 6000.0f;

	// 미니언의 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health = 5.0f;

	// 미니언이 쓰러졌을 때 스폰될 아이템
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUEMS_BasePickup> SpawnedPickup;

public:
	// Sets default values for this character's properties
	AUEMS_Minion();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float PatrolSpeed = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float ChaseSpeed = 350.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float PatrolRadius = 50000.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 소음 감지 시 호출되는 함수
	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	// 피격 시 호출되는 함수
	UFUNCTION()
	void OnDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void PostInitializeComponents() override;

	// 새로운 순찰 위치를 설정
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetNextPatrolLocation();

	// 캐릭터를 추적하도록 설정
	UFUNCTION(BlueprintCallable, Category = "AI")
	void Chase(APawn* TargetPawn);

	// 폰을 발견했을 때 호출되는 함수
	UFUNCTION()
	void OnPawnDetected(APawn* DetectedPawn);

	// 플레이어가 잡혔는 지 확인하는 함수
	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void GoToLocation(const FVector& TargetLocation);

	FORCEINLINE UPawnSensingComponent* GetPawnSensingComponent() const { return PawnSense; }
	FORCEINLINE USphereComponent* GetCollisionComponent() const { return Collision; }


};
