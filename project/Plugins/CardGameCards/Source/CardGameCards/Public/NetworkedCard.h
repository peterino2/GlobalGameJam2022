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

	// Sets default values for this actor's properties
	ANetworkedCard();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated)
	bool inHand;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated, ReplicatedUsing=OnRep_isFaceUp)
	bool isFaceUp;

	UFUNCTION()
	void OnRep_isFaceUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerController* OwnerPlayer;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FText cardName;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FText cardDescription;

	UFUNCTION()
	void RegisterCardEvents();
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryFlipCardServer(APlayerController* player, bool force=false);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnFlipCard();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
