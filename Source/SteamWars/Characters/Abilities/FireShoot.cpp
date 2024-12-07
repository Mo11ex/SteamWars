#include "FireShoot.h"

#include "Characters/SWFPSCharacter.h"
#include "Components/CharacterComponents/AbilitySystem/AbilityTasks/AbilityTask_SuccessFailEvent.h"

UFireShoot::UFireShoot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTag ShootTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Hit"));
	AbilityTags.AddTag(ShootTag);
	ActivationOwnedTags.AddTag(ShootTag);

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon")));
}

void UFireShoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority)
	{
		ASWFPSCharacter* Hero = Cast<ASWFPSCharacter>(GetAvatarActorFromActorInfo());
		if (!Hero)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		Hero->FairShoot();

		Task = UAbilityTask_SuccessFailEvent::WaitSuccessFailEvent(this, SuccessTag, FailedTag);

		Task->SuccessEventReceived.AddDynamic(this, &UFireShoot::EventReceived);
		Task->FailEventReceived.AddDynamic(this, &UFireShoot::OnCancelled);

		Task->ReadyForActivation();

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UFireShoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if(Task.IsValid())
	{
		Task->ExternalCancel();
	}
}

void UFireShoot::OnCancelled(FGameplayEventData Payload)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UFireShoot::EventReceived(FGameplayEventData Payload)
{
	//Add GameplayEffect
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
