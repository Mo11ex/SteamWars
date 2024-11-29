#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "SWPlayerState.generated.h"

class USWAbilitySystemComponent;
class USWAttributeSet;

UCLASS()
class STEAMWARS_API ASWPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ASWPlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual USWAttributeSet* GetAttributeSet() const;

protected:
	UPROPERTY()
	TObjectPtr<USWAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<USWAttributeSet> AttributeSet;
};
