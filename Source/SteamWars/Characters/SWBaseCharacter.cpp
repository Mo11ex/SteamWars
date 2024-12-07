#include "SWBaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/CharacterComponents/SWCharacterEquipmentComponent.h"
#include "Components/CharacterComponents/AbilitySystem/SWAbilitySystemComponent.h"
#include "Components/CharacterComponents/AbilitySystem/AttributeSet/SWAttributeSet.h"
#include "Components/CharacterComponents/AbilitySystem/Abilities/SWGameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"

ASWBaseCharacter::ASWBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bAbilitiesInitialized = false;
	
	CharacterEquipmentComponent = CreateDefaultSubobject<USWCharacterEquipmentComponent>(TEXT("CharacterEquipment"));
}

UAbilitySystemComponent* ASWBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

USWAttributeSet* ASWBaseCharacter::GetAttributeSet() const
{
	return AttributeSet.Get();
}

bool ASWBaseCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void ASWBaseCharacter::Die()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	//OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities();

		//FGameplayTagContainer EffectTagsToRemove;
		//EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		//int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		//AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}
}

void ASWBaseCharacter::FinishDying()
{
	Destroy();
}

const USWCharacterEquipmentComponent* ASWBaseCharacter::GetEquipmentComponent() const
{
	return CharacterEquipmentComponent;
}

float ASWBaseCharacter::GetHealth() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetHealth();
	}
	return 0.0f;
}

float ASWBaseCharacter::GetMaxHealth() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float ASWBaseCharacter::GetStamina() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetStamina();
	}
	return 0.0f;
}

float ASWBaseCharacter::GetMaxStamina() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetMaxStamina();
	}
	return 0.0f;
}

void ASWBaseCharacter::GiveDefaultAbilities()
{
	check(AbilitySystemComponent);
	if(!HasAuthority()) return;
	
	if(!bAbilitiesInitialized)
	{
		for(TSubclassOf<USWGameplayAbility> AbilityClass : DefaultAbilities)
		{
			const FGameplayAbilitySpec AbilitySpec(
				AbilityClass, 1,
				static_cast<int32>(AbilityClass.GetDefaultObject()->AbilityInputID), this);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}

	bAbilitiesInitialized = true;
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

void ASWBaseCharacter::SetHealth(float Health)
{
	if(AttributeSet)
	{
		AttributeSet->SetHealth(Health);
	}
}

void ASWBaseCharacter::SetStamina(float Stamina)
{
	if(AttributeSet)
	{
		AttributeSet->SetStamina(Stamina);
	}
}


