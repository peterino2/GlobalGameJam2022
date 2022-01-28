#include "EmbTdCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AEmbTdCharacter::AEmbTdCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 92000.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->bCanWalkOffLedges = false;
	GetCharacterMovement()->SetNetAddressable(); // Make DSO components net addressable
	GetCharacterMovement()->SetIsReplicated(true); // Enable replication by default;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AEmbTdCharacter::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);
}

void AEmbTdCharacter::BeginPlay() 
{
    Super::BeginPlay();
}
