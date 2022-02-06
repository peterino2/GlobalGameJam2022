// CREATED FOR A GAMEJAM

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NetworkedCard.h"
#include "GameFramework/Actor.h"
#include "VillainfamousCard.generated.h"

// new actor class containing all base data for implementing the solo deckbuilding card
UCLASS()
class CARDGAMECARDS_API AVillainfamousCard : public ANetworkedCard
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVillainfamousCard();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, ReplicatedUsing=OnRep_CardStrength)
	float cardStrength;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsProxy=false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasCustomCardArt;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* customCardArt;
	
	float oldCardStrength;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag cardType;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void PlayCardFromHand(); // calls OnPlayedFromHan

	UFUNCTION(BlueprintNativeEvent)
	void OnMouseOver();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnEndMouseOver();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnClicked();

protected:
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void CreateServerProxyAndPlay(FVector_NetQuantize SpawnLocation); // This creates a networked proxy of this card.

	UFUNCTION()
	void ProxyPlayToBoard(); // automatically called on proxies

	// Affectively performs a deep copy for all
	UFUNCTION()
	void InitAsProxyFor(AVillainfamousCard* Source); 
	
	UFUNCTION(NetMulticast, Reliable)
	void InitAsProxyForMulti(AVillainfamousCard* Source);

	UFUNCTION()
	void OnRep_CardStrength();

	UFUNCTION(BlueprintNativeEvent)
	void ShowCardStrengthUpdate();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
