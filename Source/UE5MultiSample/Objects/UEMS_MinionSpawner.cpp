// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/UEMS_MinionSpawner.h"
#include "UEMS_MinionSpawner.h"
#include "Characters/UEMS_Minion.h"
#include "Components/BoxComponent.h"


// Sets default values
AUEMS_MinionSpawner::AUEMS_MinionSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea")); // 스폰 영역 생성
	SpawnArea->SetupAttachment(RootComponent); // 루트 컴포넌트에 부착
	SpawnArea->SetBoxExtent(FVector(1000.f, 1000.f, 100.f)); // 스폰 영역 크기 설정
}

// Called when the game starts or when spawned
void AUEMS_MinionSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (SpawnableMinions.IsEmpty()) return; // 스폰할 미니언 클래스가 없으면 종료

	if (GetLocalRole() != ROLE_Authority) return; // 서버에서만 실행

	for (int32 i = 0; i < NumMinionsAtStart; ++i) // 시작 시 스폰할 미니언 수만큼 반복
	{
		Spawn(); // 스폰 함수 호출
	}

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AUEMS_MinionSpawner::Spawn, SpawnDelay, true, SpawnDelay); // 타이머 설정
}

void AUEMS_MinionSpawner::Spawn()
{
	FActorSpawnParameters SpawnParams; // 스폰 파라미터 설정
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding; // 다른 오브젝트와 충돌을 피하면서 스폰

	auto Minion = SpawnableMinions[FMath::RandRange(0, SpawnableMinions.Num() - 1)]; // 랜덤으로 스폰할 미니언 클래스 선택

	const auto Rotation = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f); // 랜덤 회전값 설정
	const auto Location = SpawnArea->GetComponentLocation() + FVector(
		FMath::RandRange(-SpawnArea->GetScaledBoxExtent().X, SpawnArea->GetScaledBoxExtent().X),
		FMath::RandRange(-SpawnArea->GetScaledBoxExtent().Y, SpawnArea->GetScaledBoxExtent().Y),
		0.f); // 스폰 위치 설정

	GetWorld()->SpawnActor<AUEMS_Minion>(Minion, Location, Rotation, SpawnParams); // 미니언 스폰
}

// Called every frame
void AUEMS_MinionSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

