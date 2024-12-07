#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SWAbilitySystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STEAMWARS_API USWAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	USWAbilitySystemComponent();

protected:
	virtual void BeginPlay() override;
};
