// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/UEMS_WeaponProjectileComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Objects/UEMS_BaseWeaponProjectile.h"
#include "Characters/UEMS_Character.h"

// Sets default values for this component's properties
UUEMS_WeaponProjectileComponent::UUEMS_WeaponProjectileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	ProjectileClass = AUEMS_BaseWeaponProjectile::StaticClass(); // 기본 발사체 클래스 설정
}

void UUEMS_WeaponProjectileComponent::SetProjectileClass(TSubclassOf<class AUEMS_BaseWeaponProjectile> NewProjectileClass)
{
	ProjectileClass = NewProjectileClass; // 발사체 클래스 설정
}


// Called when the game starts
void UUEMS_WeaponProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())) // 캐릭터의 컨트롤러를 가져옴.
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) // 로컬 플레이어의 서브시스템을 가져옴.
		{
			Subsystem->AddMappingContext(WeaponMappingContext, 1); // 입력 매핑 컨텍스트를 추가함.
		}

		if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) // 컨트롤러의 입력 컴포넌트를 가져옴.
		{
			InputComponent->BindAction(ThrowAction, ETriggerEvent::Started, this, &UUEMS_WeaponProjectileComponent::Throw); // 던지기 액션을 바인딩함.
		}
	}
	
}

void UUEMS_WeaponProjectileComponent::Throw()
{
	Throw_Server();
}

void UUEMS_WeaponProjectileComponent::Throw_Server_Implementation()
{
	if (ProjectileClass)
	{
		const auto Character = Cast<AUEMS_Character>(GetOwner());
		const auto ProjectileSpawnLocation = GetComponentLocation();
		const auto ProjectileSpawnRotation = GetComponentRotation();
		auto ProjectileSpawnParams = FActorSpawnParameters();
		ProjectileSpawnParams.Owner = GetOwner();
		ProjectileSpawnParams.Instigator = Character;

		// 발사체를 생성함.
		GetWorld()->SpawnActor<AUEMS_BaseWeaponProjectile>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotation, ProjectileSpawnParams);
	}
}


// Called every frame
void UUEMS_WeaponProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

