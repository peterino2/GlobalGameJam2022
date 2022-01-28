/** *
 * Copyright (C) 2020-2021
 *
 * Peter Li - All Rights Reserved
 * Unauthorized copying of this file, 
 * or any of the code content within the Embersong source directory
 * via any medium is strictly prohibited
 * Proprietary and confidential
 * 
 * Peter Li <Peterlimail47@gmail.com>
 * */

#include "EmNavTypes.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "GridMarker.h"

uint32 FGridPoint::hash() const
{
    FGridPoint hash_point = FGridPoint{x, y};
    memset(&hash_point, 0, sizeof(FGridPoint));

    return FCrc::MemCrc32(&hash_point, sizeof(FGridPoint));
}

uint32 GetTypeHash(const FGridPoint& in)
{
    const uint32 Hash = FCrc::MemCrc32(&in, sizeof(in));
    UE_LOG(LogTemp, Verbose, TEXT("Creating hash entry for TMAP with value of %d Gridpoint.x =%d Gridpoint.y =%d"), Hash,
            in.x, in.y);
    return Hash;
}

FSearchedNode::FSearchedNode(FGridNode* grid_node)
{
    fcost = FLT_MAX;
    dist = FLT_MAX;
    node = grid_node;
    shortestkey = nullptr;
    visited = false;
}

FSearchedNode::FSearchedNode()
{
    node = nullptr;
    dist = FLT_MAX;
    fcost = FLT_MAX;
    shortestkey = nullptr;
    visited = false;
}

FSearchedNode::FSearchedNode(FVector location, UWorld* world)
{
    node = FGridNode::GetNearestNode(location, world);
    fcost = FLT_MAX;
    dist = FLT_MAX;
    shortestkey = nullptr;
    visited = false;
}

FGridNode* FGridNode::GetNearestNode(FVector location, UWorld* world)
{
    FGridNode* rv = GetNearestNodeAtPoint(location, world);
    float closest_dist = FLT_MAX;
    AGridMarker* closest_marker = nullptr;
    int32_t closest_index = 0;

    bool found = false;

    if(!rv)
    {
        for (TActorIterator<AGridMarker> ActorItr(world); ActorItr; ++ActorItr)
        {
            float radius = 3500;
            AGridMarker* marker = *ActorItr;
            auto ism = marker->get_ism();
	        // virtual TArray<int32> GetInstancesOverlappingSphere(const FVector& Center, float Radius, bool bSphereInWorldSpace=true) const;
            TArray<int32> instances = ism->GetInstancesOverlappingSphere( location, 3500);
            
            for(int i = 0; i < instances.Num(); i++)
            {
                FTransform trans;

                ism->GetInstanceTransform(instances[i], trans, true);
                float dist = ((trans.GetLocation() - location) * FVector(1.0, 1.0, 0.1)).Size();
                if(dist < closest_dist)
                {
                    found = true;
                    closest_dist = dist;
                    closest_marker = marker;
                    closest_index = instances[i];
                }
            }
        }
    }
    if(found)
    {
        rv = closest_marker->getGridNodeAtIndex(closest_index);
    }
    return rv;
}

FGridNode* FGridNode::GetNearestNodeAtPoint(FVector location, UWorld* world)
{
    FGridNode* rv = nullptr;
    FHitResult hit;
    FVector start_trace = location + FVector(0.0, 0.0f, 600.0f);
    FVector end_trace = location + FVector(0.0, 0.0f, -600.0f);

    world->LineTraceSingleByChannel(hit, start_trace, end_trace, ECC_Visibility);

    if(hit.bBlockingHit)
    {
        if(auto marker = Cast<AGridMarker>(hit.Actor.Get()))
        {
            if(auto ism = Cast<UInstancedStaticMeshComponent>(hit.Component))
            {
	            rv = marker->getGridNodeAtIndex(hit.Item);
            }
        }
    }
    

    return rv;
}

FGridPath::FGridPath()
{
    total_cost = 0;
    is_complete = false;
    path = TArray<FGridNode>();
}

