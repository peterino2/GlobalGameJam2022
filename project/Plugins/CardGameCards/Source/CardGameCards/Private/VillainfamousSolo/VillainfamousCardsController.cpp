// CREATED FOR A GAMEJAM


#include "VillainfamousSolo/VillainfamousCardsController.h"


// Sets default values
AVillainfamousCardsController::AVillainfamousCardsController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVillainfamousCardsController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVillainfamousCardsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVillainfamousCardsController::HandleMouseOverCards(float DeltaTime)
{
	
}

