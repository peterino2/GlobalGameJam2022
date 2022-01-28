// CREATED FOR A GAMEJAM


#include "NetworkedCardsController.h"

#include "NetworkedCard.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ANetworkedCardsController::ANetworkedCardsController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bShowMouseCursor = true;
}

// Called when the game starts or when spawned
void ANetworkedCardsController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANetworkedCardsController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ANetworkedCardsController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("AltSelect", IE_Pressed, this, &ANetworkedCardsController::OnAltSelectPressed);
	InputComponent->BindAction("AltSelect", IE_Released, this, &ANetworkedCardsController::OnAltSelectReleased);

	InputComponent->BindAction("Select", IE_Released, this, &ANetworkedCardsController::OnSelectReleased);
	InputComponent->BindAction("Select", IE_Pressed, this, &ANetworkedCardsController::OnSelectPressed);

	InputComponent->BindAction("Escape", IE_Released, this, &ANetworkedCardsController::OnEscapePressed);

	InputComponent->BindAxis("MoveForward", this, &ANetworkedCardsController::OnMoveForwardAxis);
	InputComponent->BindAxis("MoveRight", this, &ANetworkedCardsController::OnMoveRightAxis);
	
	InputComponent->BindAction("ZoomInWheel", IE_Pressed, this, &ANetworkedCardsController::OnZoomInWheel); 
	InputComponent->BindAction("ZoomOutWheel", IE_Pressed, this, &ANetworkedCardsController::OnZoomOutWheel); 

	InputComponent->BindAction("RotateControl", IE_Released, this, &ANetworkedCardsController::OnRotateControlReleased); 
	InputComponent->BindAction("RotateControl", IE_Pressed, this, &ANetworkedCardsController::OnRotateControlPressed); 

	InputComponent->BindAxis("RotateRightAxis", this, &ANetworkedCardsController::OnRotateRightAxis); 
}

void ANetworkedCardsController::SetupCardPlayerCameraPosition_Implementation(int playerNumber)
{
}

void ANetworkedCardsController::SetPlayerNumberMulti_Implementation(int newPlayerNumber)
{
	PlayerNumber = newPlayerNumber;
	if(UGameplayStatics::GetPlayerController(GetWorld(), 0) == this)
	{
		 SetupCardPlayerCameraPosition(newPlayerNumber);
	}
}

void ANetworkedCardsController::SetPlayerNumberServer_Implementation(int newPlayerNumber)
{
	PlayerNumber = newPlayerNumber;
	SetPlayerNumberMulti(newPlayerNumber);
}

void ANetworkedCardsController::FlipCard_Implementation(AActor* target, bool force)
{
	if(auto* card = Cast<ANetworkedCard>(target) )
	{
		card->TryFlipCardServer(this, force);
	}
}

void ANetworkedCardsController::OnAltSelectPressed_Implementation()
{
}

void ANetworkedCardsController::OnAltSelectReleased_Implementation()
{
}

void ANetworkedCardsController::OnEscapePressed_Implementation()
{
}

void ANetworkedCardsController::OnMoveForwardAxis_Implementation(float axis)
{
}

void ANetworkedCardsController::OnMoveRightAxis_Implementation(float axis)
{
}

void ANetworkedCardsController::OnZoomInWheel_Implementation()
{
}

void ANetworkedCardsController::OnZoomOutWheel_Implementation()
{
}

void ANetworkedCardsController::OnRotateControlReleased_Implementation()
{
}

void ANetworkedCardsController::OnRotateControlPressed_Implementation()
{
}

void ANetworkedCardsController::OnRotateRightAxis_Implementation(float axis)
{
}

void ANetworkedCardsController::OnSelectPressed_Implementation()
{
}

void ANetworkedCardsController::OnSelectReleased_Implementation()
{
}


// Called every frame
void ANetworkedCardsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
