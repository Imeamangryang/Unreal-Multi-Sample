// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/UEMS_PlayerState.h"
#include "AbilitySystemInterface.h"
#include "UEMS_Character.generated.h"

UCLASS()
class UE5MULTISAMPLE_API AUEMS_Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUEMS_Character();

private:
	// 카메라 부착을 위한 스프링 암 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	// 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// 입력 매핑 컨텍스트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultInputMappingContext;

	/*
	*	입력 액션 모음
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction; // 이동 액션

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction; // 시점 조정 액션

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction; // 스프린트 액션

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractionAction; // 상호작용 액션

	/*
	* GAS 컴포넌트  
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<const class UUEMS_AttributeSet> AttributeSet;

	// 캐릭터 스탯 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* CharacterStatsDataTable;

	struct FUEMS_CharacterStats* CharacterStats; // 캐릭터 스탯 구조체

	UPROPERTY()
	AActor* InteractableActor; // 상호작용할 액터

	// 소음 발생기 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stealth", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPawnNoiseEmitterComponent> NoiseEmitter;

	// 무기 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUEMS_WeaponProjectileComponent> Weapon;

	Tptr

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const struct FInputActionValue& Value); // 이동 함수
	void Look(const FInputActionValue& Value); // 시점 조정 함수
	void SprintStart(const FInputActionValue& Value); // 스프린트 시작 함수
	void SprintStop(const FInputActionValue& Value); // 스프린트 정지 함수
	void Interaction(const FInputActionValue& Value); // 상호작용 함수

	UFUNCTION(Server, Reliable)
	void SprintStart_Server();

	UFUNCTION(Server, Reliable)
	void SprintEnd_Server();

	UFUNCTION(NetMulticast, Reliable)
	void SprintStart_Client();

	UFUNCTION(NetMulticast, Reliable)
	void SprintEnd_Client();


	UFUNCTION(Server, Reliable)
	void Interact_Server();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IAbilitySystemInterface 구현
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	// 캐릭터 스탯 업데이트 함수
	void UpdateCharacterStats(int32 CharacterLevel);

	// FORCEINLINE 매크로는 코드를 인라인으로 처리. 함소 호출을 줄여 성능을 향상시킴.
	// 카메라의 Gettter함수
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// 캐릭터 스탯 Getter
	FORCEINLINE FUEMS_CharacterStats* GetCharacterStats() const { return CharacterStats; } 

	// 무기 Getter
	FORCEINLINE UUEMS_WeaponProjectileComponent* GetWeapon() const { return Weapon; }

};
