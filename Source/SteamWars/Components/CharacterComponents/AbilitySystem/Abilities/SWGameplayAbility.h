#pragma once

#include "CoreMinimal.h"
#include "SWTypes.h"
#include "Abilities/GameplayAbility.h"
#include "SWGameplayAbility.generated.h"

enum class ESWAbilityInputID : uint8;

UCLASS()
class STEAMWARS_API USWGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	USWGameplayAbility();

	//Abilities will activate when input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	ESWAbilityInputID AbilityInputID {ESWAbilityInputID::None};
};
