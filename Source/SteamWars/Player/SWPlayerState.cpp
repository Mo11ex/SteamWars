#include "SWPlayerState.h"
#include "AbilitySystem/SWAbilitySystemComponent.h"
#include "AbilitySystem/SWAttributeSet.h"

ASWPlayerState::ASWPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<USWAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<USWAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* ASWPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

USWAttributeSet* ASWPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
