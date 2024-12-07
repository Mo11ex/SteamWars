#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterComponents/AbilitySystem/Abilities/SWGameplayAbility.h"
#include "FireShoot.generated.h"

class UAbilityTask_SuccessFailEvent;

UCLASS()
class STEAMWARS_API UFireShoot : public USWGameplayAbility
{
	GENERATED_BODY()

public:
	UFireShoot();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UFUNCTION()
	void OnCancelled(FGameplayEventData Payload);

	UFUNCTION()
	void EventReceived(FGameplayEventData Payload);

	UPROPERTY()
	TSubclassOf<UGameplayEffect> ResponseGameplayEffect;

public:
	FGameplayTag SuccessTag;
	FGameplayTag FailedTag;

private:
	TWeakObjectPtr<UAbilityTask_SuccessFailEvent> Task;
	
};
