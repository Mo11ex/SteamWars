#include "SWFPSCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SWTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterComponents/SWCharacterEquipmentComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputData/InputDataAsset.h"
#include "Components/CharacterComponents/AbilitySystem/SWAbilitySystemComponent.h"
#include "Player/SWPlayerState.h"


ASWFPSCharacter::ASWFPSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;

	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	FirstPersonMeshComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -86.f));
	FirstPersonMeshComponent->CastShadow = false;
	FirstPersonMeshComponent->bCastDynamicShadow = false;
	FirstPersonMeshComponent->SetOnlyOwnerSee(true);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMeshComponent, SocketFPCamera);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->bCastHiddenShadow = true;

	GetCharacterMovement()->bOrientRotationToMovement = 0;
}

void ASWFPSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystemComponent();
	GiveDefaultAbilities();
	InitDefaultAttributes();
}

void ASWFPSCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilitySystemComponent();
	InitDefaultAttributes();

	//InitBinds();
	
	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
	
	SetHealth(GetMaxHealth());
	SetStamina(GetMaxStamina());
}

void ASWFPSCharacter::InitBinds() const
{
	if(AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			FTopLevelAssetPath(FName("SWTypes"), FName("ESWAbilityInputID")),
			static_cast<int32>(ESWAbilityInputID::Confirm),
			static_cast<int32>(ESWAbilityInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void ASWFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASWFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASWFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InputActions->JumpAction, ETriggerEvent::Triggered, this, &ASWFPSCharacter::Jump);
		EnhancedInputComponent->BindAction(InputActions->JumpAction, ETriggerEvent::Completed, this, &ASWFPSCharacter::StopJumping);

		EnhancedInputComponent->BindAction(InputActions->MoveAction, ETriggerEvent::Triggered, this, &ASWFPSCharacter::Move);

		EnhancedInputComponent->BindAction(InputActions->LookAction, ETriggerEvent::Triggered, this, &ASWFPSCharacter::Look);

		EnhancedInputComponent->BindAction(InputActions->CrouchAction, ETriggerEvent::Triggered, this, &ASWFPSCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(InputActions->CrouchAction, ETriggerEvent::Completed, this, &ASWFPSCharacter::StopCrouch);

		EnhancedInputComponent->BindAction(InputActions->Shoot, ETriggerEvent::Triggered, this, &ASWFPSCharacter::FairShootHandle);
	}

	//InitBinds();
}

void ASWFPSCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ASWFPSCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void ASWFPSCharacter::StartCrouch()
{
	Crouch();
}

void ASWFPSCharacter::StopCrouch()
{
	UnCrouch();
}

void ASWFPSCharacter::Jump()
{
	Super::Jump();
}

void ASWFPSCharacter::StopJumping()
{
	Super::StopJumping();
}

void ASWFPSCharacter::FairShootHandle()
{
	SetLocalInputToASC(true, ESWAbilityInputID::Shoot);
}

void ASWFPSCharacter::FairShoot()
{
	CharacterEquipmentComponent->FireShoot();
}

void ASWFPSCharacter::InitAbilitySystemComponent()
{
	ASWPlayerState* PS = GetPlayerState<ASWPlayerState>();
	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<USWAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		AttributeSet = PS->GetAttributeSet();
	}
}

void ASWFPSCharacter::SetLocalInputToASC(bool bIsPressed, const ESWAbilityInputID AbilityInputID)
{
	if(!AbilitySystemComponent) return;

	if(bIsPressed)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(AbilityInputID));
	}
	else
	{
		AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(AbilityInputID));
	}
}

