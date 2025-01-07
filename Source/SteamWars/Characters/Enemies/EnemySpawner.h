#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

enum class ESpawnersID : uint8;
class AEnemyBaseCharacter;

UCLASS()
class STEAMWARS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* SphereComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<AEnemyBaseCharacter>> EnemyQueue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESpawnersID SpawnerID;
	
public:
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemy();

	void AddEnemyToQueue(const TSubclassOf<AEnemyBaseCharacter>& EnemyClass);

	UFUNCTION()
	void HandleEnemyDeath();

	ESpawnersID GetSpawnID() const;

	void DebugEnum(ESpawnersID SpawnType);

private:
	FTimerHandle TimerHandle;
};
