/** *
 * Copyright (C) 2021-2021
 *
 * Peter Li - All Rights Reserved
 * Unauthorized copying of this file, 
 * or any of the code content within the Embersong source directory
 * via any medium is strictly prohibited
 * Proprietary and confidential
 * 
 * Peter Li <Peterlimail47@gmail.com>
 * */

#include "GridMarker.h"

#include "Engine/TriggerVolume.h"
#include "Engine/EngineTypes.h"
#include "Engine/TargetPoint.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "EngineUtils.h"

#include "mathutils.h"

// Sets default values
AGridMarker::AGridMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	gridarea = CreateDefaultSubobject<UBoxComponent>(TEXT("GridArea"));
	gridarea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    gridarea->SetMobility(EComponentMobility::Movable);
    RootComponent = gridarea;

	ism = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("instanced_static_mesh"));
    auto rules = FAttachmentTransformRules::KeepRelativeTransform;

    ism->AttachToComponent(RootComponent, rules);
	ism->SetCastShadow(false);
	ism->SetMobility(EComponentMobility::Static);
	ism->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	ism->SetWorldScale3D({1.0f, 1.0f, 1.0f});
	ism->SetCanEverAffectNavigation(false);
	ism->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ism->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	ism->SetCollisionResponseToChannel(ECC_EngineTraceChannel3, ECollisionResponse::ECR_Block);
	ism->bNavigationRelevant = false;

	
	SetGridVisible(false);
}

void AGridMarker::rtInit()
{
	
	auto extents		= gridarea -> GetScaledBoxExtent();
	auto extents_upper	= extents;
	extents_upper.X		= -extents_upper.X;
	extents_upper.Y		= -extents_upper.Y;

	auto extents_lower	= extents_upper;
	extents_lower.Z		= -extents_lower.Z;

	FRotator rotation = this->GetActorRotation();
	SHOW_FVECTOR(extents_upper, "");

	FVector forward = FVector(1.0f, 0.0f, 0.0f);
	FVector right = FVector(0.0f, 1.0f, 0.0f);
	auto up = FVector(0.0f, 0.0f, 1.0f);

	auto rot_ext = rotation.RotateVector(extents_upper) + GetActorLocation();
	auto rot_ext_lower = rotation.RotateVector(extents_lower) + GetActorLocation();

	rot_fwd = rotation.RotateVector(forward);
	rot_up = rotation.RotateVector(up);
	rot_right = rotation.RotateVector(right);


	SHOW_FVECTOR(rot_ext, "");

	const auto gridsize = GRID_SIZE;
	x_max = extents.X / gridsize * 2;
	y_max = extents.Y / gridsize * 2;
    slowUpdateTick = 0 ;

	ClearNodes();

	for (int x = 0; x < x_max; x++)
	{
		for (int y = 0; y < y_max; y++)
		{
			FVector start_trace = 
                rot_fwd*x*gridsize + 
                rot_right*y*gridsize + 
                rot_ext + 
                rot_fwd * gridsize/2 + 
                rot_right*gridsize/2;

			FVector end_trace = 
                rot_fwd*x*gridsize + 
                rot_right*y*gridsize + 
                rot_ext_lower + 
                rot_fwd * gridsize/2 + 
                rot_right*gridsize/2;

			FHitResult trace_keepout;


            FVector start_offsets[4] = 
            { 
                (FVector(0.5f,   0.5f, 0.0f)*gridsize) + start_trace,
                (FVector(0.5f,  -0.5f, 0.0f)*gridsize) + start_trace,
                (FVector(-0.5f, -0.5f, 0.0f)*gridsize) + start_trace,
                (FVector(-0.5f,  0.5f, 0.0f)*gridsize) + start_trace
            };

            FVector end_offsets[4] = 
            { 
                (FVector(0.5f,   0.5f, 0.0f)*gridsize) + end_trace,
                (FVector(0.5f,  -0.5f, 0.0f)*gridsize) + end_trace,
                (FVector(-0.5f, -0.5f, 0.0f)*gridsize) + end_trace,
                (FVector(-0.5f,  0.5f, 0.0f)*gridsize) + end_trace
            };
            
            bool corners_filled = true;
			FHitResult trace;
            // trace 4 corners of the position
            for (int i = 0; i < 4; i++)
            {
			    GetWorld()->LineTraceSingleByChannel(trace, start_offsets[i], end_offsets[i], ECC_GameTraceChannel1); 
                if(!trace.bBlockingHit)
                {
                    corners_filled = false;
                    break;
                }
            }

			GetWorld()->LineTraceSingleByChannel(trace, start_trace, end_trace, ECC_GameTraceChannel1); 
			GetWorld()->LineTraceSingleByChannel(trace_keepout, start_trace, end_trace, ECC_GameTraceChannel2); 
			// ECC_GameTraceChannel1 is the gridmark channel for casting walkable grids
			// ECC_GameTraceChannel2 is the grid keepout channel for deleting
			if (corners_filled && !(trace_keepout.bBlockingHit)) {
                
                auto tf = FTransform();
                tf.SetLocation(trace.Location + rot_up * 1);
                tf.SetRotation(trace.Actor.Get()->GetActorTransform().GetRotation());
                
                AddNode(tf, x, y);
			}
		}
	}

    // lock the GridNodemap /TODO elements must never be inserted or deleted from the GridNodeMap 
    // ever past this point.
    //
    // TAtomics are deprecated.. should i use std:: atomics instead? idfk

    // Generate the hashmap for node searching
    for (auto iter = GridNodeMap.CreateIterator(); iter; ++iter)
    {
        FGridNode* this_node = &iter->Value;
        auto thispoint = this_node -> pos;
        GridOverlay.Add((thispoint.y << 16) + thispoint.x, this_node);
    }

    // Generate links for all grid maps
    for (auto iter = GridNodeMap.CreateIterator(); iter; ++iter)
    {
        FGridNode* this_node = &iter->Value;
        auto thispoint = this_node -> pos;
        generateLinks(this_node);
    }
    
    RescanCosts();
}

void AGridMarker::RescanCosts()
{
    // Generate costs
    for (auto iter = GridNodeMap.CreateIterator(); iter; ++iter)
    {
        FGridNode* this_node = &iter->Value;
        ScanCostAtNode(this_node, 5 /* halfiter = */);
    }
}

void AGridMarker::generateLinks(FGridNode* this_node)
{
    auto thispoint = this_node -> pos;
	const FGridPoint sdirs[] = { 
		{ thispoint.x    , thispoint.y - 1 }, // UP
		{ thispoint.x - 1, thispoint.y - 1 }, // UP_LEFT
		{ thispoint.x - 1, thispoint.y     }, // LEFT
		{ thispoint.x - 1, thispoint.y + 1 }, // DOWN_LEFT

		{ thispoint.x    , thispoint.y + 1 }, // DOWN
		{ thispoint.x + 1, thispoint.y + 1 }, // DOWNRIGHT
		{ thispoint.x + 1, thispoint.y     }, // RIGHT
		{ thispoint.x + 1, thispoint.y - 1 }  // UPRIGHT
	};

    for(int dir = 0; dir < sizeof(sdirs)/sizeof(sdirs[0]); dir++)
    {
        auto* search = getGridNodeAtPoint(sdirs[dir]);
        if(search)
        {
            int rdir = (dir + 4) % PATH_COUNT;
            this_node->path_nodes[dir] = search; //assign to 
            this_node->path_costs[dir] = FLT_MAX; //assign to 
            this_node->isValid = true;
        }
    }
}

void AGridMarker::ScanCostAtNode(FGridNode* this_node, float base_cost, bool halfiter)
{
    if(this_node -> root != this)
    {
        UE_LOG(LogTemp, Warning, TEXT("this_node does not point to 'this'"));
    }

    auto thispoint = this_node->pos;

	for(int dir = 0; dir < PATH_COUNT; dir++)
	{
        bool isCorner = dir % 2 == 0;
        bool can_walk = false;
        auto* search = this_node->path_nodes[dir];
        //auto* search = this_node->GetNi(dir, nullptr);
		if (search)
        {
            FVector start_trace = this_node->location + FVector(0.0f, 0.0f, 75.0f);
            FVector end_trace = search->location + FVector(0.0f, 0.0f, 75.0f);
            FHitResult hit1;

            float radius = 20.0f;
            
	        GetWorld()->SweepSingleByChannel(hit1, start_trace, end_trace,
                FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(radius));

			can_walk = (!hit1.bBlockingHit);

            int rdir = (dir + 4) % PATH_COUNT;

            float cost = isCorner ? base_cost : base_cost * 1.4;
            cost = can_walk ? cost: 1000;

            this_node->path_costs[dir] = cost; 
            search->path_costs[rdir] = cost; 
		}
	}
}

void AGridMarker::OnConstruction(const FTransform& Transform)
{
    // rtInit();
}

TArray<FVector> AGridMarker::GetSpanLocations(FGridSpan span)
{
    TArray<FVector> rv;
    for(int i = 0; i < span.nodes.Num(); i++)
    {
        rv.Add(span.nodes[i].location);
    }
    return rv;
}

FGridSpan AGridMarker::GetSpanExtendsOptimized(UObject* WorldContextObject, FVector start, float cost/*Parameters*/)
{
    UWorld* world = WorldContextObject->GetWorld();
    FDjkCtx navContext = FDjkCtx(start, nullptr, world);
    return navContext.GetSpanExtendsOptimized(cost);
}

FGridSpan AGridMarker::GetSpanExtends(UObject* WorldContextObject, FVector start, float cost)
{
    UWorld* world = WorldContextObject->GetWorld();
    FDjkCtx navContext = FDjkCtx(start, nullptr, world);
    return navContext.GetSpanExtends(cost);
}

FGridSpan AGridMarker::GetSpanSynchronously(UObject* WorldContextObject, FVector start, float cost)
{
    UWorld* world = WorldContextObject->GetWorld();
    FDjkCtx navContext = FDjkCtx(start, nullptr, world);
    return navContext.GetSpanSynchronously(cost);
}
void AGridMarker::SetGridVisible(bool visible)
{
	if (auto mesh = get_ism() )
	{
        if(!visible)
        {
            mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            mesh->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	        mesh->SetCollisionResponseToChannel(ECC_EngineTraceChannel3, ECollisionResponse::ECR_Block);
        }
        else
        {
            mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            mesh->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	        mesh->SetCollisionResponseToChannel(ECC_EngineTraceChannel3, ECollisionResponse::ECR_Block);
        }
		mesh->SetVisibility(visible, true);
	}
}

void AGridMarker::ClearNodes()
{
	if (auto mesh = get_ism())
	{
		mesh->ClearInstances();
        GridOverlay.Empty();
        GridNodeMap.Empty();
	}
}
// Called when the game starts or when spawned
void AGridMarker::BeginPlay()
{
	Super::BeginPlay();
    rtInit();
} 
// Called every frame
void AGridMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    slowUpdateTick += DeltaTime;
    if(slowUpdateTick > 0.100)
    {
        // do slowUpdate 
        slowUpdateTick = 0;
    }
}

int AGridMarker::AddNode(const FTransform& ft, int32_t x, int32_t y) {
	if (auto mesh = get_ism())
	{
		auto ism_index = ism->AddInstance(ft);
        FGridNode gridnode;
        auto point = FGridPoint{x, y};
        
        gridnode.ism_index = ism_index;
        gridnode.pos.x = x;
        gridnode.pos.y = y;
        for(int i = 0; i < (PATH_COUNT); i++)
        {
            gridnode.path_nodes[i] = nullptr;
            gridnode.isValid = false;
            gridnode.path_costs[i] = FLT_MAX;
        }

        gridnode.location = ft.GetLocation();
        gridnode.root = this;
        
        GridNodeMap.Add(ism_index, gridnode);
		return ism_index;
	}
	else 
	{
		return -1;
	}
}

void NodeInfo_t::initFromNearest(FVector location, UWorld* world)
{
    UInstancedStaticMeshComponent* closest_ism = nullptr;
    int32_t closest_index = 0;
    AGridMarker* closest_marker = nullptr;

    FHitResult hit;
    FVector start_trace = location + FVector(0.0, 0.0f, 600.0f);
    FVector end_trace = location + FVector(0.0, 0.0f, -600.0f);

    world->LineTraceSingleByChannel(hit, start_trace, end_trace, ECC_Visibility);

    if(hit.bBlockingHit)
    {
        if(auto _marker = Cast<AGridMarker>(hit.Actor.Get()))
        {
            if(auto _ism = Cast<UInstancedStaticMeshComponent>(hit.Component))
            {
                index = hit.Item;
                root = _marker;
                ism = _ism;
            }
        }
    }
}

NodeInfo_t::NodeInfo_t(FVector location, UWorld* world)
{
    initFromNearest(location, world);
}

NodeInfo_t::NodeInfo_t(const FHitResult& hit)
{
	ism = nullptr;
	root = nullptr;
	if (auto actor = hit.GetActor())
	{
        initFromNearest(hit.Location, actor->GetWorld());
	}
}


FGridPath AGridMarker::PathToIsmByLocationBP(UWorld* WorldContextObject, FVector start, FVector location, float max_cost/*Parameters*/)
{
    return PathToIsmByLocation(start, location, WorldContextObject, max_cost);
}

FGridPath AGridMarker::PathToIsmByLocation(FVector start, FVector location, UWorld* world, float max_cost)
{
    FGridPath path = FGridPath();

    FDjkCtx navContext = FDjkCtx(start, &path, world);
    navContext.setDestination(location);
    navContext.setMaxCost(max_cost);

    while(!navContext.isDone())
    {
        navContext.next();
    }

    if(navContext.searchAtKey(navContext.destkey)->visited)
    {
        if(!navContext.getDestinationPath())
        {
            return FGridPath();
        }
        
    }
    else {
        path = FGridPath();
        if(!navContext.GetPartialPath())
        {
            return FGridPath();
        }
    }

    return path;
}

bool NodeInfo_t::GetTransform(FTransform & trans)
{
	if (ism) {
		return ism->GetInstanceTransform(index, trans, true);
	}
	return false;
}

FVector NodeInfo_t::GetLocation()
{
	FVector location;
	FTransform trans;
	GetTransform(trans);
	location = trans.GetLocation();
	return location;
}

FGridNode* AGridMarker::getGridNodeAtIndex(int32_t key) 
{
    auto rv =  GridNodeMap.Find(key);
    return rv;
}

FGridNode* AGridMarker::getGridNodeAtPoint(FGridPoint point) 
{
    if (GridOverlay.Contains((point.y << 16) + point.x))
    {
		return *GridOverlay.Find((point.y << 16) + point.x);
    }
    else {

        // UE_LOG(LogTemp, Warning, TEXT("No overlay data found for x = %d y = %d"), point.x, point.y);
        return nullptr;
    }
}

void FGridPath::AllPathPoints(TArray<FVector>* opath)
{
    opath->Empty();
    if (!path.Num())
    {
        return;
    }

    for(int i = path.Num() - 1; i; i--)
    {
        opath -> Add(path[i].location);
    }

    opath -> Add(path[0].location);

}

void AGridMarker::SendGridNodesUpdate(UWorld* world)
{

}

FGridPathVectors AGridMarker::GetVectorsToIsm(
        FVector start, 
        FVector location, 
        UWorld* WorldContextObject, 
        float max_cost, 
        bool optimized 
){

    auto ipath = AGridMarker::PathToIsmByLocation(start, location, WorldContextObject, FLT_MAX);
    FGridPathVectors rv;
    if(!optimized)
    {
        ipath.AllPathPoints(&rv.path);
    }
    else {
        ipath.ParsePathPoints(&rv.path);
    }
    rv.total_cost = ipath.total_cost;
    rv.is_complete = ipath.is_complete;
    return rv;

}

void FGridPath::ParsePathPoints(TArray<FVector>* opath)
{
    int lastdx = 0;
    int lastdy = 0;
    opath->Empty();
    if (!path.Num())
    {
        return;
    }
    FGridNode thisnode =  path[path.Num() - 1];

    for(int i = path.Num() - 1; i; i--)
    {
        // lastdx lastdy already set
        // get dx,dy
        // diff dx,dy
        // add if diff 
        int nextnodeid  =  i - 1 >=  0 ? i - 1  : 0;
        FGridNode nextnode = path[nextnodeid];
        
        int dx = nextnode.pos.x - thisnode.pos.x;
        int dy = nextnode.pos.y - thisnode.pos.y;

        /*
        UE_LOG(LogTemp, Warning, TEXT("dx=%d dy=%d lastdx=%d lastdy=%d thisnode.pos.x=%d thisnode.pos.y=%d nextnode.pos.x=%d nextnode.pos.py=%d"), 
            dx, dy,
            lastdx, lastdy,
            thisnode.pos.x, thisnode.pos.y,
            nextnode.pos.x, nextnode.pos.y
        );
        */
        if((dx != lastdx) || (dy != lastdy))
        {
            opath->Add(thisnode.location);
        }
        
        // iterate next
               
        lastdx = dx;
        lastdy = dy;
        thisnode = nextnode;
    }

    opath->Add(thisnode.location);
}
