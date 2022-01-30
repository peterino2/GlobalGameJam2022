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

void ACCH_PlayerPawn::AddCardToDeckServer_Implementation(ANetworkedCard* card, bool scheme)
{
	AddCardToDeckLocal(card, scheme);
	AddCardToDeckMulti(card, scheme);
}

void ACCH_PlayerPawn::AddCardToDeckMulti_Implementation(ANetworkedCard* card, bool scheme)
{
	AddCardToDeckLocal(card, scheme);
}

void ACCH_PlayerPawn::AddCardToDeckLocal(ANetworkedCard* card, bool scheme)
{
	auto& deck = scheme ? cardsInSchemeDeck : cardsInDeck;
	deck.AddUnique(card);
	card->inDeck = true;
	card->inHand = false;
	card->onField = false;
	card->isFaceUp = false;
	card->OnAddedToDeck();
}

void ACCH_PlayerPawn::DrawCard(bool scheme)
{
	auto& deck = scheme? cardsInSchemeDeck: cardsInDeck;
	if(deck.Num() > 0 && !HasAuthority())
	{
		ANetworkedCard* nextCard = deck.Top();
		deck.Remove(nextCard);
	}
	TryDrawCard(scheme);
}

void ACCH_PlayerPawn::TryPlayCard_Implementation(ANetworkedCard* card)
{
	PlayCardLocal(card);
	PlayCardMulti(card);
}

void ACCH_PlayerPawn::PlayCardMulti_Implementation(ANetworkedCard* card)
{
	PlayCardLocal(card);
}

void ACCH_PlayerPawn::PlayCardLocal(ANetworkedCard* card)
{
	cardsInHand.Remove(card);

	card->inHand = false;
	card->onField = true;
	card->inDeck = false;
	if(card->isScheme)
	{
		spentSchemeCards.AddUnique(card);
	}
	else
	{
		spentCards.AddUnique(card);
	}
	
	card->OnPlayedFromHand();
	OnCardPlay(card);
}

void ACCH_PlayerPawn::OnCardPlay_Implementation(ANetworkedCard* card)
{
}

void ACCH_PlayerPawn::TryDrawCard_Implementation(bool scheme)
{
	auto& deck = scheme? cardsInSchemeDeck: cardsInDeck;
	if(deck.Num() > 0)
	{
		ANetworkedCard* nextCard = deck.Top();
		deck.Remove(nextCard);
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
	OnDraw();
}

void ACCH_PlayerPawn::AddCardToHandLocal_Implementation(ANetworkedCard* card)
{
	if(!card) return;
	cardsInHand.AddUnique(card);
	card->inDeck = false;
	card->inHand = true;
	card->onField = false;
	card->isFaceUp = false;
	card->OnAddedToHand();
}

void ACCH_PlayerPawn::OnDraw_Implementation()
{
}

void ACCH_PlayerPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ACCH_PlayerPawn::SetupDeck(TArray<FCardInitInfo> InitialCards)
{
	if(GetLocalRole() != ROLE_Authority) {return;}

	TArray<ANetworkedCard* > cardList;
	TArray<ANetworkedCard* > schemeCardList;

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
			newCard->isScheme = cardInfo.isScheme;
			// ANetworkedCard* newCard = NewObject<ANetworkedCard>(this, cardInfo.cardClass);
			// auto& deck = cardInfo.isScheme ? cardsInSchemeDeck : cardsInDeck;
			AddCardToDeckServer(newCard, cardInfo.isScheme);
		}
	}
}

void ACCH_PlayerPawn::TryShuffleDeck_Implementation(bool shuffleSchemeDeck)
{
	// shuffle the deck
	auto& deck = shuffleSchemeDeck ? cardsInSchemeDeck : cardsInDeck;
	
	int32 LastIndex = deck.Num() - 1;
	for (int32 i = 0; i <= LastIndex; ++i)
	{
		int32 Index = FMath::RandRange(i, LastIndex);
		if (i != Index)
		{
			 deck.Swap(i, Index);
		}
	}
	
	ShuffleDeckMulti(deck, shuffleSchemeDeck);
}

void ACCH_PlayerPawn::ShuffleDeckMulti_Implementation(const TArray<ANetworkedCard*>& newDeckList, bool shuffleSchemeDeck)
{
	auto& deck = (shuffleSchemeDeck ? cardsInSchemeDeck : cardsInDeck);
	if(deck.Num() != newDeckList.Num())
	{
	}
	deck.Empty();

	for(auto* card: newDeckList)
	{
		deck.AddUnique(card);
	}
}

void ACCH_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
