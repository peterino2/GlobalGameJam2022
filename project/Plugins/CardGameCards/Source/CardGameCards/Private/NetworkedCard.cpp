// CREATED FOR A GAMEJAM


#include "NetworkedCard.h"

#include "NetworkedCardsController.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ANetworkedCard::ANetworkedCard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANetworkedCard::BeginPlay()
{
	Super::BeginPlay();
}

void ANetworkedCard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(ANetworkedCard, inHand, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ANetworkedCard, isFaceUp, COND_None, REPNOTIFY_OnChanged);
}

void ANetworkedCard::OnRep_isFaceUp()
{
	OnFlipCard();
}

void ANetworkedCard::RegisterCardEvents()
{
}

void ANetworkedCard::TryFlipCardServer_Implementation(APlayerController* player, bool force)
{
	if(!force) // then validate
	{
		if(player != OwnerPlayer)
		{
			return;
		}
	}
	isFaceUp = !isFaceUp;
	OnFlipCard();
}

void ANetworkedCard::OnFlipCard_Implementation()
{
}

// Called every frame
void ANetworkedCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

