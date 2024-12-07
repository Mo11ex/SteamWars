#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItem.h"
#include "RangeWeaponItem.generated.h"

UCLASS(Blueprintable)
class STEAMWARS_API ARangeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	ARangeWeaponItem();

	void FireShoot();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWeaponBarrelComponent* WeaponBarrel;

private:
	const FName SocketWeaponMuzzle = FName("Barrel");
};
