// CREATED FOR A GAMEJAM


#include "VillainfamousSolo/VillainfamousSoloBoard.h"


// Sets default values
AVillainfamousSoloBoard::AVillainfamousSoloBoard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AVillainfamousSoloBoard::ConstructBoardInEditor()
{
}

void AVillainfamousSoloBoard::ResolveCard(AVillainfamousCard* Card)
{
}

void AVillainfamousSoloBoard::FinishResolvingCard(AVillainfamousCard* Card)
{
}

void AVillainfamousSoloBoard::AddPlayerCard(AVillainfamousCard* Card)
{
}

void AVillainfamousSoloBoard::LoadEncounter(TArray<AVillainfamousCard*> Cards)
{
}

// Called when the game starts or when spawned
void AVillainfamousSoloBoard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVillainfamousSoloBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

