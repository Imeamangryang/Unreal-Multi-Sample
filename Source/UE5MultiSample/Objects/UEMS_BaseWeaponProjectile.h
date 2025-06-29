// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UEMS_BaseWeaponProjectile.generated.h"

UCLASS()
class UE5MULTISAMPLE_API AUEMS_BaseWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
	// 콜리전 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereCollision;

	// 발사체 이동 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	// 메쉬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	// 발사체의 데미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float Damage = 1.f;


public:	
	// Sets default values for this actor's properties
	AUEMS_BaseWeaponProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 발사체가 목표에 충돌했을 때 호출되는 함수
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 컴포넌트에 대한 Getter 함수
	FORCEINLINE USphereComponent* GetSphereCollision() const { return SphereCollision; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
	
	
};
