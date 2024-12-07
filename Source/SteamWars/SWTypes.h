#pragma once

#define ECC_Bullet ECC_GameTraceChannel1

UENUM(BlueprintType)
enum class ESWAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Shoot
};

const FName SocketFPCamera = FName("CameraSocket");