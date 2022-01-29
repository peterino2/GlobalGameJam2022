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
	TArray<ANetworkedCard*> spentCards;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AddCardToDeckServer(ANetworkedCard* card);
	
	UFUNCTION(NetMulticast, Reliable)
	void AddCardToDeckMulti(ANetworkedCard* card);
	
	UFUNCTION()
	void AddCardToDeckLocal(ANetworkedCard* card);

	// Requests the server for what's the next card in my deck.
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryDrawCard();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AddCardToHandServer(ANetworkedCard* card);
	
	UFUNCTION(NetMulticast, Reliable)
	void AddCardToHandMulti(ANetworkedCard* card);
	
	UFUNCTION(NetMulticast, Reliable)
	void AddCardToHandLocal(ANetworkedCard* card);

	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintCallable)
	void SetupDeck(TArray<FCardInitInfo> initalCards);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
