// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/UEMS_BasePickup.h"
#include "UEMS_BasePickup.h"
#include "Characters/UEMS_Character.h"
#include "Components/SphereComponent.h"


// Sets default values
AUEMS_BasePickup::AUEMS_BasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 콜리전 컴포넌트 생성 및 설정
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = SphereCollision;
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetSphereRadius(200.0f);

	// 메시 컴포넌트 생성 및 설정
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true; // 네트워크 복제 활성화

}

// Called when the game starts or when spawned
void AUEMS_BasePickup::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AUEMS_BasePickup::OnBeginOverlap);
}

void AUEMS_BasePickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	if (const auto Character = Cast<AUEMS_Character>(OtherActor))
	{
		Pickup(Character);
	}
}

void AUEMS_BasePickup::Pickup_Implementation(AUEMS_Character* OwningCharacter)
{
	SetOwner(OwningCharacter);
}

// Called every frame
void AUEMS_BasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

