// CREATED FOR A GAMEJAM

#pragma once

#include "CoreMinimal.h"
#include "EmbTdCamera.h"
#include "NetworkedCard.h"
#include "UObject/Object.h"
#include "CCH_PlayerPawn.generated.h"

USTRUCT(BlueprintType)
struct FCardInitInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ANetworkedCard> cardClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int cardCount;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isScheme;
};

/**
 * 
 */
UCLASS()
class CARDGAMECARDS_API ACCH_PlayerPawn : public AEmbTdCamera
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACCH_PlayerPawn(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// All card and Hand Control effects are handled here
	
	bool isInitialized = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<ANetworkedCard*> cardsInHand;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<ANetworkedCard*> cardsInDeck;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<ANetworkedCard*> cardsInSchemeDeck;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<ANetworkedCard*> spentCards;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<ANetworkedCard*> spentSchemeCards;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AddCardToDeckServer(ANetworkedCard* card, bool scheme);
	
	UFUNCTION(NetMulticast, Reliable)
	void AddCardToDeckMulti(ANetworkedCard* card, bool scheme);
	
	UFUNCTION()
	void AddCardToDeckLocal(ANetworkedCard* card, bool scheme);

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	AActor* PlayerActionDeck;
	
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	AActor* PlayerSchemeDeck;

	// Requests the server for what's the next card in my deck.
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryDrawCard(bool scheme);
	
	UFUNCTION(BlueprintCallable)
	void DrawCard(bool scheme);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AddCardToHandServer(ANetworkedCard* card);
	
	UFUNCTION(NetMulticast, Reliable)
	void AddCardToHandMulti(ANetworkedCard* card);
	
	UFUNCTION(NetMulticast, Reliable)
	void AddCardToHandLocal(ANetworkedCard* card);

	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintCallable)
	void SetupDeck(TArray<FCardInitInfo> initalCards);
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryShuffleDeck(bool shuffleSchemeDeck);

	UFUNCTION(NetMulticast, Reliable)
	void ShuffleDeckMulti(const TArray<ANetworkedCard*>& newDeckList, bool shuffleSchemeDeck);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
