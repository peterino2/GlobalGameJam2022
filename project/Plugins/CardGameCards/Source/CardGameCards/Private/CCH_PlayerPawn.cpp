// CREATED FOR A GAMEJAM


#include "CCH_PlayerPawn.h"

#include "Kismet/KismetArrayLibrary.h"

ACCH_PlayerPawn::ACCH_PlayerPawn(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void ACCH_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ACCH_PlayerPawn::AddCardToDeckServer_Implementation(ANetworkedCard* card)
{
	AddCardToDeckLocal(card);
	AddCardToDeckMulti(card);
}

void ACCH_PlayerPawn::AddCardToDeckMulti_Implementation(ANetworkedCard* card)
{
	AddCardToDeckLocal(card);
}

void ACCH_PlayerPawn::AddCardToDeckLocal(ANetworkedCard* card)
{
	cardsInDeck.AddUnique(card);
	card->OnAddedToDeck();
}

void ACCH_PlayerPawn::TryDrawCard_Implementation()
{
	if(cardsInDeck.Num() > 0)
	{
		ANetworkedCard* nextCard = cardsInDeck.Top();
		cardsInDeck.Remove(nextCard);
		AddCardToHandServer(nextCard);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(
			TEXT("Uh Oh we're out of cards")
		));
	}
}

void ACCH_PlayerPawn::AddCardToHandServer_Implementation(ANetworkedCard* card)
{
	if(!card) return;
	AddCardToHandLocal(card);
	AddCardToHandMulti(card);
}

void ACCH_PlayerPawn::AddCardToHandMulti_Implementation(ANetworkedCard* card)
{
	if(!card) return;
	AddCardToHandLocal(card);
}

void ACCH_PlayerPawn::AddCardToHandLocal_Implementation(ANetworkedCard* card)
{
	if(!card) return;
	cardsInHand.AddUnique(card);
	card->OnAddedToHand();
}

void ACCH_PlayerPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ACCH_PlayerPawn::SetupDeck(TArray<FCardInitInfo> InitialCards)
{
	if(GetLocalRole() != ROLE_Authority) {return;}

	TArray<ANetworkedCard* > cardList;

	for(auto cardInfo : InitialCards)
	{
		auto* cdo = cardInfo.cardClass.GetDefaultObject();
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(
			TEXT("%s count: %d"),
			*cdo->cardName.ToString(),
			cardInfo.cardCount
		));
		// use this form for When we're playing cards to the board.
		// auto* newCard = Cast<ANetworkedCard>(GetWorld()->SpawnActor(cardInfo.cardClass.Get(), &spawnTransform, spawn_params));
		for(int i = 0; i < cardInfo.cardCount; i++)
		{
			FTransform spawnTransform;
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			auto* newCard = Cast<ANetworkedCard>(GetWorld()->SpawnActor(cardInfo.cardClass.Get(), &spawnTransform, spawnParams));
			//ANetworkedCard* newCard = NewObject<ANetworkedCard>(this, cardInfo.cardClass);
			cardList.Add(newCard);
		}
	}
	
	// shuffle the deck
	int32 LastIndex = cardList.Num() - 1;
	for (int32 i = 0; i <= LastIndex; ++i)
	{
		int32 Index = FMath::RandRange(i, LastIndex);
		if (i != Index)
		{
			cardList.Swap(i, Index);
		}
	}

	for(auto* card : cardList)
	{
		AddCardToDeckServer(card);
	}
}

void ACCH_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
