// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UEMS_WeaponProjectileComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5MULTISAMPLE_API UUEMS_WeaponProjectileComponent : public USceneComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUEMS_BaseWeaponProjectile> ProjectileClass;

	// 입력 시스템을 따로 사용하여 캐릭터에 의존하지 않도록 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* WeaponMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* ThrowAction;

public:	
	// Sets default values for this component's properties
	UUEMS_WeaponProjectileComponent();

	// 투사체 Setter 함수
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SetProjectileClass(TSubclassOf<class AUEMS_BaseWeaponProjectile> NewProjectileClass);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Throw();

	UFUNCTION(Server, Reliable)
	void Throw_Server();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
