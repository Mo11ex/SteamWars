#pragma once

#include "CoreMinimal.h"
#include "SWTypes.h"
#include "GameFramework/GameMode.h"
#include "SWGameMode.generated.h"


class AEnemyBaseCharacter;
class AEnemySpawner;


USTRUCT(BlueprintType)
struct FWaveSpawnData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemyBaseCharacter> EnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpawnersID SpawnerID;
};

USTRUCT(BlueprintType)
struct FWaveDataFor : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWaveSpawnData> SpawnInfos;
};

USTRUCT(BlueprintType)
struct FWaveData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<TSubclassOf<AEnemyBaseCharacter>, int> EnemyPool;
};

UCLASS()
class STEAMWARS_API ASWGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	TArray<AActor*> GetSpawners() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawners")
	TSubclassOf<AEnemyBaseCharacter> EnemyChar;

	void StartWave();
	void EndWave();
	void EnterTransition();
	void BuildEnemyPool();
	int GetPoolSize() const;
	void OnEnemyDefeated();
	
	FORCEINLINE FTimerHandle GetTimerHandle() const { return WaveTimer; }
	
	TSubclassOf<AEnemyBaseCharacter> GetEnemyFromPool();
	
	void NewWave();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int WaveNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ESpawnersID, AEnemySpawner*> SpawnersMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float TransitionWaveTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int EnemyMaxCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int EnemyRemaining = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TMap<TSubclassOf<AEnemyBaseCharacter>, int> EnemyPool;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave")
	UDataTable* WaveDataTable;
	
	virtual void BeginPlay() override;

private:
	FTimerHandle WaveTimer;

	void SpawnEnemy();

	void InitializeSpawners();
	ESpawnersID GetSpawnerIDForEnemy(TSubclassOf<AEnemyBaseCharacter> Enemy);
};
