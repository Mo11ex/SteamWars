#include "SWAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

USWAttributeSet::USWAttributeSet()
{
	InitHealth(100.f);
}

void USWAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USWAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USWAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USWAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USWAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void USWAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void USWAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
}

void USWAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USWAttributeSet, Health, OldHealth);
}

void USWAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USWAttributeSet, MaxHealth, OldMaxHealth);
}

void USWAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USWAttributeSet, Stamina, OldStamina);
}

void USWAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USWAttributeSet, MaxStamina, OldMaxStamina);
}