#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UEMS_CharacterStats.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct UE5MULTISAMPLE_API FUEMS_CharacterStats : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NextLevelExp = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StealthMultiplier = 1.0f;
};
