#pragma once

#include "CoreMinimal.h"
#include "SWBaseCharacter.h"
#include "SWFPSCharacter.generated.h"

UCLASS()
class STEAMWARS_API ASWFPSCharacter : public ASWBaseCharacter
{
	GENERATED_BODY()

public:
	ASWFPSCharacter();

	// GAS Server only
	virtual void PossessedBy(AController* NewController) override;
	// GAS Client only
	virtual void OnRep_PlayerState() override;
	
protected:
	virtual void BeginPlay() override;

	/*-------------------Components-------------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | First person")
	class USkeletalMeshComponent* FirstPersonMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputDataAsset* InputActions;
	/*------------------------------------------------*/
	
public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/*---------------Movement---------------*/
	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;
	virtual void StartCrouch() override;
	virtual void StopCrouch() override;
	virtual void Jump() override;
	virtual void StopJumping() override;
	/*---------------------------------------*/

	void InitAbilitySystemComponent();

	const FName SocketFPCamera = FName("CameraSocket");
};
