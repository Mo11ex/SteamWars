#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "SWBaseCharacter.generated.h"

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

	virtual void Move(const FInputActionValue& Value) {};
	virtual void Look(const FInputActionValue& Value) {};
	virtual void StartCrouch() {};
	virtual void StopCrouch() {};
	
protected:
	void GiveDefaultAbilities();
	void InitDefaultAttributes() const;

	UPROPERTY()
	TObjectPtr<class USWAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<USWAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;
};
