// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/UEMS_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include <EnhancedInputComponent.h>
#include "AbilitySystemComponent.h"
#include "GAS/UEMS_AttributeSet.h"
#include "Characters/UEMS_CharacterStats.h"
#include "Engine/DataTable.h"
#include "Objects/UEMS_Interactable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Objects/UEMS_WeaponProjectileComponent.h"

// Sets default values
AUEMS_Character::AUEMS_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 카메라 부착을 위한 스프링 암 컴포넌트 생성 및 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f; // 카메라와 캐릭터 사이의 거리
	CameraBoom->bUsePawnControlRotation = true; // 카메라가 캐릭터의 회전을 따르도록 설정

	// 카메라 컴포넌트 생성 및 설정
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom); // 카메라를 스프링 암에 부착
	FollowCamera->bUsePawnControlRotation = false; // 카메라가 캐릭터의 회전을 따르지 않도록 설정

	// PawnNoiseEmitterComponent 생성
	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComponent"));
	NoiseEmitter->NoiseLifetime = 0.01f; // 소음 지속 시간 설정

	/*
	*	캐릭터의 기본 설정
	*	캐릭터 회전 설정
	*	캡슐 콜리전 크기 설정
	*   캐릭터 메쉬	설정
	*	캐릭터무브먼트 설정
	*/ 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(35.0f, 85.0f); // 캡슐 콜리전 크기 설정

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f)); // 메쉬의 위치 조정
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f)); // 메쉬의 회전 조정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/Characters/Avatars/Avatar_BS/SK_Avatar_BS.SK_Avatar_BS"));
	if (SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object); // 메쉬 설정
	}

	GetCharacterMovement()->bOrientRotationToMovement = true; // 캐릭터가 이동 방향으로 회전하도록 설정
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // 회전 속도 설정
	GetCharacterMovement()->MaxWalkSpeed = 500.0f; // 걷기 속도 설정
	GetCharacterMovement()->MinAnalogWalkSpeed = 50.0f; // 최소 아날로그 걷기 속도 설정
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f; // 걷기 중 감속 설정

	// AbilitySystemComponent 생성
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent")); 
	AbilitySystemComponent->SetIsReplicated(true); 
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); 

	AttributeSet = CreateDefaultSubobject<UUEMS_AttributeSet>(TEXT("AttributeSet")); // AttributeSet 생성
	
	// Weapon 생성
	Weapon = CreateDefaultSubobject<UUEMS_WeaponProjectileComponent>(TEXT("Weapon")); // 무기 컴포넌트 생성
	Weapon->SetupAttachment(RootComponent); // 루트 컴포넌트에 부착
	Weapon->SetRelativeLocation(FVector(120.f, 70.f, 0.f)); // 무기 위치 설정
}

// Called when the game starts or when spawned
void AUEMS_Character::BeginPlay()
{
	Super::BeginPlay();

	// AbilitySystemComponent 초기화
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	// 캐릭터 스탯 업데이트

	// 입력 매핑 컨텍스트 추가
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}

		//PlayerController->ConsoleCommand(TEXT("ShowDebug abilitysystem"));
	}

	UpdateCharacterStats(1);
}

// Called every frame
void AUEMS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ROLE_Authority) return; // 서버에서만 실행
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this); // 자신을 무시하도록 설정

	auto SphereRadius = 50.0f;
	auto StartLocation = GetActorLocation() + GetActorForwardVector() * 150.0f; // 캐릭터 앞쪽으로 100 유닛 떨어진 위치
	auto EndLocation = StartLocation + GetActorForwardVector() * 500.0f; // 캐릭터 앞쪽으로 100 유닛 떨어진 위치

	auto IsHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		SphereRadius,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (IsHit && HitResult.GetActor()->GetClass()->ImplementsInterface(UUEMS_Interactable::StaticClass()))
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SphereRadius, 12, FColor::Red, false, 1.f); // 히트된 위치에 구체 그리기
		InteractableActor = HitResult.GetActor(); // 상호작용할 액터 설정
	}
	else
	{
		InteractableActor = nullptr; // 상호작용할 액터 초기화
	}
	
	// 스프린트 중 소음 발생
	if (GetCharacterMovement()->MaxWalkSpeed == GetCharacterStats()->SprintSpeed)
	{
		auto Noise = 1.f;

		if (GetCharacterStats() && GetCharacterStats()->StealthMultiplier)
		{
			Noise = Noise / GetCharacterStats()->StealthMultiplier;
		}
		NoiseEmitter->MakeNoise(this, Noise, GetActorLocation()); // 소음 발생
	}
}

// Called to bind functionality to input
void AUEMS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 입력 액션 바인딩
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUEMS_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUEMS_Character::Look);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AUEMS_Character::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AUEMS_Character::SprintStop);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &AUEMS_Character::Interaction);
	}
}

void AUEMS_Character::UpdateCharacterStats(int32 CharacterLevel)
{
	auto IsSprinting = false;
	if (GetCharacterStats())
	{
		IsSprinting = GetCharacterMovement()->MaxWalkSpeed == GetCharacterStats()->WalkSpeed;
	}

	if (CharacterStatsDataTable)
	{
		TArray<FUEMS_CharacterStats*> CharacterStatsRows;
		CharacterStatsDataTable->GetAllRows<FUEMS_CharacterStats>(TEXT("CharacterStats"), CharacterStatsRows);

		if (CharacterStatsRows.Num() > 0)
		{
			const auto NewCharacterLevel = FMath::Clamp(CharacterLevel, 1, CharacterStatsRows.Num());
			CharacterStats = CharacterStatsRows[NewCharacterLevel - 1];

			GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->WalkSpeed; // 걷기 속도 설정
			
			// 캐릭터가 레벨업을 해도 Sprint가 끊기지 않도록 설정 
			if (IsSprinting)
			{
				SprintStart_Server();
			}

		}
	}

}

void AUEMS_Character::Move(const FInputActionValue& Value)
{
	// 이동 입력 처리
	FVector2D MovementInput = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// 카메라 방향에 따라 이동 방향 계산
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, ControlRotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementInput.Y);
		AddMovementInput(RightDirection, MovementInput.X);
	}
}

void AUEMS_Character::Look(const FInputActionValue& Value)
{
	// 시점 조정 입력 처리
	FVector2D LookInput = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerPitchInput(LookInput.Y);
		AddControllerYawInput(LookInput.X);
	}
}

void AUEMS_Character::SprintStart(const FInputActionValue& Value)
{
	SprintStart_Server(); // 서버에서 스프린트 시작
}

void AUEMS_Character::SprintStop(const FInputActionValue& Value)
{
	SprintEnd_Server(); // 서버에서 스프린트 종료
}

void AUEMS_Character::Interaction(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, TEXT("Interaction!")); // 상호작용 메시지 출력
	Interact_Server(); // 서버에서 상호작용 처리
}

void AUEMS_Character::SprintStart_Client_Implementation()
{
	if (GetCharacterStats())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->SprintSpeed; // 스프린트 속도 설정
	}
}

void AUEMS_Character::SprintEnd_Client_Implementation()
{
	if (GetCharacterStats())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->WalkSpeed; // 걷기 속도 설정
	}
}

// 서버에서 스프린트 시작
void AUEMS_Character::SprintStart_Server_Implementation()
{
	SprintStart_Client(); // 클라이언트에서 스프린트 시작
}

// 서버에서 스프린트 종료
void AUEMS_Character::SprintEnd_Server_Implementation()
{
	SprintEnd_Client(); // 클라이언트에서 스프린트 종료
}

void AUEMS_Character::Interact_Server_Implementation()
{
	if (InteractableActor)
	{
		IUEMS_Interactable::Execute_Interact(InteractableActor, this);
	}
}



