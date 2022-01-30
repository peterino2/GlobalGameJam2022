// CREATED FOR A GAMEJAM

#pragma once

#include "CoreMinimal.h"
#include "NetworkedCardsController.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "NetworkedCard.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCardEventDelegate);

UCLASS()
class CARDGAMECARDS_API ANetworkedCard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANetworkedCard();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool onField;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool inDeck;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool inHand;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated, ReplicatedUsing=OnRep_isFaceUp)
	bool isFaceUp;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated)
	bool isScheme = false;

	UFUNCTION()
	void OnRep_isFaceUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerController* OwnerPlayer;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText cardName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText cardDescription;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText flavourText;
	
	UFUNCTION()
	void RegisterCardEvents();
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryFlipCardServer(APlayerController* player, bool force=false);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnFlipCard();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnPlayedFromHand();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnAddedToHand();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnAddedToDeck();

	UFUNCTION(BlueprintNativeEvent)
	void OnPlay();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

