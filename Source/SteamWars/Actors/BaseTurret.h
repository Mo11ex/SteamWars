#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTurret.generated.h"

UCLASS()
class STEAMWARS_API ABaseTurret : public AActor
{
	GENERATED_BODY()

public:
	ABaseTurret();

protected:
	virtual void BeginPlay() override;

public:
	//Нужно указывать VisibleAnywhere и BlueprintReadOnly, тк нам не нужно его изменять его из BP, а тем более изменять.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Basement;

	//Нужно указывать VisibleAnywhere и BlueprintReadOnly, тк нам не нужно его изменять его из BP, а тем более изменять.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") 
	UStaticMeshComponent* Gun;

	//Здесь всё хорошо, тк эти параметры нужно будет изменять и с карты и с BP.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	double SphereRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	double MaxRotationAngle = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	double LookRadius = 500;

private:
	FTimerHandle TimerHandle;
	
	TArray<FHitResult> TempHits;
	
	FVector StartLocation;
	FVector EndLocation;
	FRotator TempRotation;
	double CurrentYaw;
	double 	RotationDirection;
	bool Captured;
	FCollisionQueryParams CollisionParams;
	AActor* PlayerActor;

	void TurretUpdate();
	void LookAtPlayer(AActor* Actor);
	void RotateTurret(FRotator Rotator);
};
