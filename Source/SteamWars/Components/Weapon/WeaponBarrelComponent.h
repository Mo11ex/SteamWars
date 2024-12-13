#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SceneComponent.h"
#include "WeaponBarrelComponent.generated.h"


class UGameplayEffect;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STEAMWARS_API UWeaponBarrelComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UWeaponBarrelComponent();

	void Shot(FVector ShotStart, FVector ShotDirection);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Barrel Attributes")
	float FiringRange = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Barrel Attributes")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> ResponseGameplayEffect;
};
