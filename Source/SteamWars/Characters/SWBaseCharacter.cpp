#include "SWBaseCharacter.h"
#include "../AbilitySystem/SWAbilitySystemComponent.h"

ASWBaseCharacter::ASWBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* ASWBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

USWAttributeSet* ASWBaseCharacter::GetAttributeSet() const
{
	return AttributeSet.Get();
}

void ASWBaseCharacter::GiveDefaultAbilities()
{
	check(AbilitySystemComponent);
	if(!HasAuthority()) return;

	for(TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities)
	{
		const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

void ASWBaseCharacter::InitDefaultAttributes() const
{
	if (!AbilitySystemComponent || !DefaultAttributeEffect) return;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1.f, EffectContext);

	if(SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}


