// CREATED FOR A GAMEJAM


#include "VillainfamousSolo/VillainfamousEnemyCard.h"


// Sets default values
AVillainfamousEnemyCard::AVillainfamousEnemyCard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


void AVillainfamousEnemyCard::TakeDamage(AActor* source, int amount, FGameplayTag DamageType)
{
	OnDamageTakenBP(source, amount, DamageType);
}

void AVillainfamousEnemyCard::OnDamageTakenBP_Implementation(AActor* source, int amount, FGameplayTag DamageType)
{
}

// Called when the game starts or when spawned
void AVillainfamousEnemyCard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVillainfamousEnemyCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

