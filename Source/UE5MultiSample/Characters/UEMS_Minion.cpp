// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/UEMS_Minion.h"
#include "UEMS_Minion.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/UEMS_Character.h"
#include "UEMS_GameMode.h"
#include "Objects/UEMS_BasePickup.h"

// Sets default values
AUEMS_Minion::AUEMS_Minion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // AI가 스폰될 때 자동으로 AIController를 소유하도록 설정
	AIControllerClass = AAIController::StaticClass(); // AIController 클래스 설정

	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	PawnSense->SensingInterval = 0.8f; // 감지 간격 설정
	PawnSense->SetPeripheralVisionAngle(45.0f); // 주변 시야 각도 설정
	PawnSense->SightRadius = 1500.0f; // 시야 반경 설정
	PawnSense->HearingThreshold = 400.0f; // 청각 감지 임계값 설정
	PawnSense->LOSHearingThreshold = 400.0f; // 시야 청각 감지 임계값 설정

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetSphereRadius(100.0f); // 구형 콜리전 반경 설정
	Collision->SetupAttachment(RootComponent); // 루트 컴포넌트에 부착

	GetCapsuleComponent()->InitCapsuleSize(60.f, 96.f);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true); // 오버랩 이벤트 활성화

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -91.f)); // 메쉬 위치 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/KayKit/Skeletons/skeleton_minion"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object); // 메쉬 설정
	}

	GetCharacterMovement()->bOrientRotationToMovement = true; // 이동 방향에 따라 회전하도록 설정
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f); // 회전 속도 설정
	GetCharacterMovement()->MaxWalkSpeed = 200.f; // 최대 걷기 속도 설정
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f; // 최소 아날로그 걷기 속도 설정
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; // 걷기 중 감속 설정

	// 스폰될 아이템 클래스 설정
	static ConstructorHelpers::FClassFinder<AUEMS_BasePickup> SpawnedPickupAsset(TEXT("/Game/Blueprints/BP_GoldCoinPickup.BP_GoldCoinPickup"));
	if (SpawnedPickupAsset.Succeeded())
	{
		SpawnedPickup = SpawnedPickupAsset.Class; // 스폰될 아이템 클래스 설정
	}
}

// Called when the game starts or when spawned
void AUEMS_Minion::BeginPlay()
{
	Super::BeginPlay();
	
	// 정찰 행동 시작
	SetNextPatrolLocation();
}

void AUEMS_Minion::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Noise Detected")); // 소음 감지 메시지 출력
	GoToLocation(Location); // 소음 발생 위치로 이동
}

void AUEMS_Minion::OnDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Health -= Damage; // 피해량만큼 체력 감소

	if (Health > 0.f) return; // 체력이 0보다 크면 종료

	// 미니언이 죽었을 때
	if (SpawnedPickup) // 스폰될 아이템이 설정되어 있으면
	{
		GetWorld()->SpawnActor<AUEMS_BasePickup>(SpawnedPickup, GetActorLocation(), GetActorRotation()); // 아이템 스폰
	}
	Destroy(); // 미니언 파괴
}

// Called every frame
void AUEMS_Minion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ROLE_Authority) return; // 서버에서만 실행

	if (GetCharacterMovement()->MaxWalkSpeed == ChaseSpeed) return; // 현재 속도가 추적 속도이면 종료

	if ((GetActorLocation() - PatrolLocation).Size() < 500.f) // 현재 위치와 목표 위치의 거리가 500 이하이면
	{
		SetNextPatrolLocation(); // 새로운 순찰 위치 설정
	}
}

// Called to bind functionality to input
void AUEMS_Minion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUEMS_Minion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (GetLocalRole() != ROLE_Authority) return; // 서버에서만 실행

	OnActorBeginOverlap.AddDynamic(this, &AUEMS_Minion::OnBeginOverlap); // 오버랩 이벤트 바인딩
	GetPawnSensingComponent()->OnSeePawn.AddDynamic(this, &AUEMS_Minion::OnPawnDetected); // 폰 감지 이벤트 바인딩
	GetPawnSensingComponent()->OnHearNoise.AddDynamic(this, &AUEMS_Minion::OnHearNoise); // 소음 감지 이벤트 바인딩

	OnTakeAnyDamage.AddDynamic(this, &AUEMS_Minion::OnDamage); // 피해 이벤트 바인딩
}

void AUEMS_Minion::SetNextPatrolLocation()
{
	if (GetLocalRole() != ROLE_Authority) return; // 서버에서만 실행

	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed; // 이동 속도 설정

	const auto LocationFound = UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, GetActorLocation(), PatrolLocation, PatrolRadius);// 랜덤 위치 찾기
	if (LocationFound)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PatrolLocation); // AIController를 사용하여 이동
	}
	
}

void AUEMS_Minion::Chase(APawn* TargetPawn)
{
	if (GetLocalRole() != ROLE_Authority) return; // 서버에서만 실행

	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed; // 이동 속도 설정
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), TargetPawn); // AIController를 사용하여 이동
	
	DrawDebugSphere(GetWorld(), TargetPawn->GetActorLocation(), 25.f, 12, FColor::Red, false, 10.f, 0, 2.f); // 타겟 위치에 구체 그리기

	// 레벨 내 다른 Minion들에게 알림.
	if (const auto GameMode = Cast<AUEMS_GameMode>(GetWorld()->GetAuthGameMode())) // 게임 모드 가져오기
	{
		GameMode->AlertMinions(this, TargetPawn->GetActorLocation(), AlertRadius); // 다른 미니언들에게 알림
	}
}

void AUEMS_Minion::OnPawnDetected(APawn* DetectedPawn)
{
	if (!DetectedPawn->IsA<AUEMS_Character>()) return; // 감지된 폰이 없으면 종료

	if (GetCharacterMovement()->MaxWalkSpeed != ChaseSpeed) // 현재 속도가 추적 속도가 아니면
	{
		Chase(DetectedPawn); // 감지된 폰을 추적
	}
}

void AUEMS_Minion::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor->IsA<AUEMS_Character>()) return; // 감지된 폰이 없으면 종료
}

void AUEMS_Minion::GoToLocation(const FVector& TargetLocation)
{
	PatrolLocation = TargetLocation; // 목표 위치 설정
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PatrolLocation); // AIController를 사용하여 이동
}

