#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "SWBaseCharacter.generated.h"

class USWGameplayAbility;
class USWCharacterEquipmentComponent;
class UGameplayAbility;
class UGameplayEffect;
class USWAttributeSet;
struct FInputActionValue;

UCLASS()
class STEAMWARS_API ASWBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASWBaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual USWAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Character")
	virtual bool IsAlive() const;
	
	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "GAS|Character")
	virtual void FinishDying();
	
	const USWCharacterEquipmentComponent* GetEquipmentComponent() const;

	/*---------------------------------Attributes------------------------------------*/
	UFUNCTION(BlueprintCallable, Category = "GAS|Character|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Character|Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Character|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Character|Attributes")
	float GetMaxStamina() const;	
	/*-------------------------------------------------------------------------------*/

protected:
	void GiveDefaultAbilities();
	void InitDefaultAttributes() const;

	virtual void SetHealth(float Health);
	virtual void SetStamina(float Stamina);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS|Animation")
	UAnimMontage* DeathMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Components")
	USWCharacterEquipmentComponent* CharacterEquipmentComponent;
	
	UPROPERTY()
	TObjectPtr<class USWAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<USWAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TArray<TSubclassOf<USWGameplayAbility>> DefaultAbilities;

	UPROPERTY()
	uint8 bAbilitiesInitialized:1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;
};
