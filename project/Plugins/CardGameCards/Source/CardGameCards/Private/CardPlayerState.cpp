// CREATED FOR A GAMEJAM


#include "CardPlayerState.h"


// Sets default values
ACardPlayerState::ACardPlayerState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACardPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACardPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

