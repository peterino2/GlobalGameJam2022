// CREATED FOR A GAMEJAM


#include "VillainfamousSolo/VillainfamousCard.h"

#include "CardGameCards.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "VillainfamousSolo/VillainfamousSoloBoard.h"
#include "VisualLogger/VisualLogger.h"


// Sets default values
AVillainfamousCard::AVillainfamousCard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cardType = FGameplayTag::RequestGameplayTag(TEXT("CardType.VillainAction.Combat"));
}

// Called when the game starts or when spawned
void AVillainfamousCard::BeginPlay()
{
	Super::BeginPlay();

	oldCardStrength = cardStrength;
}

void AVillainfamousCard::PlayCardFromHand()
{
	CreateServerProxyAndPlay(GetActorLocation());
	OnPlayedFromHand();
}

void AVillainfamousCard::OnMouseOver_Implementation()
{
	// Not Implemented, called from controller
}

void AVillainfamousCard::OnEndMouseOver_Implementation()
{
	// Not Implemented, called from controller
}

void AVillainfamousCard::OnClicked_Implementation()
{
	// Not Implemented, called from controller
}

void AVillainfamousCard::ProxyPlayToBoard()
{
	auto* boardActor = UGameplayStatics::GetActorOfClass(GetWorld(), AVillainfamousSoloBoard::StaticClass());
	
	if(auto* board = Cast<AVillainfamousSoloBoard>(boardActor))
	{
		// add proxy card to board and signal the board
	}
}

void AVillainfamousCard::CreateServerProxyAndPlay_Implementation(FVector_NetQuantize SpawnLocation)
{
	FTransform spawnTransform;
	FActorSpawnParameters spawnParams;
	
	auto* proxyCard = Cast<AVillainfamousCard>(
		GetWorld()->SpawnActor(GetClass(), &spawnTransform, spawnParams)
	);

	proxyCard->InitAsProxyFor(this);
	proxyCard->InitAsProxyForMulti(this);
	proxyCard->ProxyPlayToBoard();
	
#if ENABLE_VISUAL_LOG
	if(FVisualLogger::Get().IsRecording())
	{
		UE_VLOG(this, LogCardGame, VeryVerbose,
		
		TEXT("Card Name: %s type: %s strength: %f"),
			*cardName.ToString(),
			*cardType.ToString(),
			cardStrength
			
		);
	}
#endif
}

void AVillainfamousCard::InitAsProxyFor(AVillainfamousCard* Source)
{
	cardName = Source->cardName;
	cardStrength = Source->cardStrength;
	cardType = Source->cardType;
	customCardArt = Source->customCardArt;
	bHasCustomCardArt = Source->bHasCustomCardArt;
	bIsProxy = true;
}

void AVillainfamousCard::InitAsProxyForMulti_Implementation(AVillainfamousCard* Source)
{
	InitAsProxyFor(Source);
}

void AVillainfamousCard::OnRep_CardStrength()
{
#if ENABLE_VISUAL_LOG
	if(FVisualLogger::Get().IsRecording())
	{
		UE_VLOG(this, LogCardGame, VeryVerbose,
		TEXT("Card strength replicated: %f -> %f "),
			oldCardStrength,
			cardStrength
		);
	}
#endif
	ShowCardStrengthUpdate();
	oldCardStrength = cardStrength;
}

void AVillainfamousCard::ShowCardStrengthUpdate_Implementation()
{
}

void AVillainfamousCard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(AVillainfamousCard, cardStrength, COND_None, REPNOTIFY_OnChanged);
}

// Called every frame
void AVillainfamousCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

