#include "SWAbilitySystemComponent.h"

USWAbilitySystemComponent::USWAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void USWAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}
