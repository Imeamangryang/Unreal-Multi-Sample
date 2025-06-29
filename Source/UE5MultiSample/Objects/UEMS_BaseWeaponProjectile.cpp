// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/UEMS_BaseWeaponProjectile.h"
#include "UEMS_BaseWeaponProjectile.h"
#include "Characters/UEMS_Character.h"
#include "Characters/UEMS_CharacterStats.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AUEMS_BaseWeaponProjectile::AUEMS_BaseWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 콜리전 컴포넌트 생성
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SphereCollision->SetGenerateOverlapEvents(true); // 충돌 이벤트 활성화
	SphereCollision->SetSphereRadius(10.f); // 구체의 반지름 설정
	SphereCollision->BodyInstance.SetCollisionProfileName(TEXT("BlockAll")); // 충돌 프로파일 설정
	SphereCollision->OnComponentHit.AddDynamic(this, &AUEMS_BaseWeaponProjectile::OnHit); // 충돌 이벤트 바인딩

	RootComponent = SphereCollision; // 루트 컴포넌트 설정

	// 메쉬 컴포넌트 생성
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent); // 메쉬를 루트 컴포넌트에 부착
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly); // 물리적 충돌만 활성화
	Mesh->SetRelativeLocation(FVector(-40.f, 0.f, 0.f)); // 메쉬의 상대 위치 설정
	Mesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f)); // 메쉬의 상대 회전 설정
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/KayKit/DungeonElements/dagger_common.dagger_common"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object); // 메쉬 설정
	}

	// 발사체 이동 컴포넌트 생성
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = SphereCollision; // 업데이트할 컴포넌트 설정
	ProjectileMovement->ProjectileGravityScale = 0.f; // 중력 스케일 설정 (0이면 중력 영향 없음)
	ProjectileMovement->InitialSpeed = 3000.f; // 초기 속도 설정
	ProjectileMovement->MaxSpeed = 3000.f; // 최대 속도 설정
	ProjectileMovement->bRotationFollowsVelocity = true; // 회전 방향이 속도를 따르도록 설정
	ProjectileMovement->bShouldBounce = false; // 바운스 비활성화

	bReplicates = true; // 네트워크 복제 활성화
}

// Called when the game starts or when spawned
void AUEMS_BaseWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUEMS_BaseWeaponProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto ComputedDamage = Damage;
	if (const auto Character = Cast<AUEMS_Character>(GetInstigator()))
	{
		ComputedDamage *= Character->GetCharacterStats()->DamageMultiplier; // 캐릭터의 데미지 배율 적용
	}

	if (OtherActor && OtherActor != this) // 충돌한 액터가 유효하고 자신이 아닐 경우
	{
		// 데미지 적용
		const FDamageEvent DamageEvent(UDamageType::StaticClass());
		OtherActor->TakeDamage(ComputedDamage, DamageEvent, GetInstigatorController(), this);
		// 발사체 파괴
		Destroy();
	}

}

// Called every frame
void AUEMS_BaseWeaponProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

