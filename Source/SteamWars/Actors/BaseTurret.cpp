#include "BaseTurret.h"

#include "Characters/SWFPSCharacter.h"


ABaseTurret::ABaseTurret()
{
	PrimaryActorTick.bCanEverTick = false;

	Basement = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Basement"));
	Basement->SetRelativeLocation(FVector(0, 0, 0));
	
	Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	Gun->SetupAttachment(Basement);
	Gun->SetRelativeLocation(FVector(0, 0, 0));
	
	RotationDirection = 1;
	CurrentYaw = 0.0f;
	Captured = false;
	TempRotation = this->GetActorRotation();
}

void ABaseTurret::BeginPlay()
{
	Super::BeginPlay();

	CollisionParams.AddIgnoredActor(this);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABaseTurret::TurretUpdate, 0.05f, true);
}

void ABaseTurret::TurretUpdate()
{
	StartLocation = this->GetActorLocation();

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SphereRadius);
	TArray<FHitResult> HitResults;
	
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation,
	FQuat::Identity, ECC_Pawn, CollisionShape, CollisionParams);
 
	if (bHit && !Captured)
	{
		for (const FHitResult& Hit : HitResults)
		{
			DrawDebugSphere(GetWorld(), Hit.Location, SphereRadius, 1, FColor::Red,
		false, 0.1f);
			
			AActor* HitActor = Hit.GetActor();
			
			if (HitActor->IsA(ASWFPSCharacter::StaticClass()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitActor->GetName());
				PlayerActor = HitActor;
				Captured = true;
				break;
			}
			
			CurrentYaw += TempRotation.Yaw + RotationDirection * MaxRotationAngle * 0.1f;

			if (CurrentYaw >= MaxRotationAngle) {
				CurrentYaw = MaxRotationAngle;
				RotationDirection *= -1;
			}
			else if (CurrentYaw <= -MaxRotationAngle) {
				CurrentYaw = -MaxRotationAngle;
				RotationDirection *= -1;
			}

			EndLocation.X = LookRadius * FMath::Cos(FMath::DegreesToRadians(CurrentYaw));
			EndLocation.Y = LookRadius * FMath::Sin(FMath::DegreesToRadians(CurrentYaw));
			EndLocation.Z = Gun->GetRelativeLocation().Z;
			RotateTurret(FRotator(0, CurrentYaw, 0));
		}
	}
	
	if (Captured) LookAtPlayer(PlayerActor);
	DrawDebugSphere(GetWorld(), StartLocation, SphereRadius, 10, FColor::Red, false, 0.1f);
}

void ABaseTurret::LookAtPlayer(AActor* Actor)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, Actor->GetActorLocation(), ECC_Pawn, CollisionParams);
	
	AActor* HitActor = HitResult.GetActor();

	if (HitActor->IsA(ASWFPSCharacter::StaticClass()))
	{
		ASWFPSCharacter* Character = Cast<ASWFPSCharacter>(Actor);
		if (!Character) return;
		FVector PlayerLocation = Character->GetActorLocation();
		FVector DirectionToPlayer = (PlayerLocation - GetActorLocation()).GetSafeNormal();
		FRotator NewRotation = DirectionToPlayer.Rotation();
		
		RotateTurret(NewRotation);
	}
	else 
	{
		CollisionParams.AddIgnoredActor(this); 
		Captured = false;
	}
}

void ABaseTurret::RotateTurret(FRotator Rotator)
{	
	FRotator GunRotation = Gun->GetRelativeRotation();
	FRotator TargetBaseRotation = FRotator(GunRotation.Pitch, Rotator.Yaw, GunRotation.Roll);
	GunRotation = FMath::RInterpTo(GunRotation, TargetBaseRotation, 0.05f, 1);
	Gun->SetRelativeRotation(GunRotation);

	if (Captured) {
		FRotator TargetGunRotation = FRotator(Rotator.Pitch, GunRotation.Yaw, GunRotation.Roll);
		GunRotation = FMath::RInterpTo(GunRotation, TargetGunRotation, 0.05f, 1);
		Gun->SetRelativeRotation(GunRotation);
	}
}