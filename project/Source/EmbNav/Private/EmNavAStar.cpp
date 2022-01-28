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

#include "EmNavAStar.h"
#include "mathutils.h"

FAStarNode::FAStarNode():
    node(nullptr),
    shortestkey(nullptr),
    fcost(0.0),
    hcost(0.0),
    gcost(0.0),
    visited(false),
    DistToDest(0.0f),
    MoveCostForShortest(0.0f)
{}

FAStarNode::FAStarNode(FVector dest, FGridNode* _node)
{
    node = _node;
    shortestkey = nullptr;
    MoveCostForShortest = 0;

    fcost = FLT_MAX;
    hcost = FLT_MAX;
    gcost = FLT_MAX;

    visited = false;

    if (node)
    {
		DistToDest = (dest - node->location ).Size();
        gcost = DistToDest/20.0f; // Scale it down such that 100 units ~= 5
    }
}

FAStarNode::FAStarNode(FVector dest, FVector location, UWorld* world)
{
    node = FGridNode::GetNearestNode(location, world);
    shortestkey = nullptr;

    fcost = FLT_MAX;
    hcost = FLT_MAX;
    gcost = FLT_MAX;
    MoveCostForShortest = 0;

    visited = false;

    if (node)
    {
		DistToDest = (dest - node->location ).Size();
        gcost = DistToDest/20.0f; // Scale it down such that 100 units ~= 5
    }
}

void FAStarCtx::CalculateNeighbourCosts()
{
    auto curr = searchAtKey(currkey);

    if (!curr)
    {
        WMSG("!!Invalid key detected. not in search");
        no_path = true;
        return;
    }

    for (int dir = 0; dir < PATH_COUNT; dir++)
    {
        if (auto* pnext = curr->GetNextGridNode(dir, startkey))
        {
            bool should_be_added = true;
            // Apply some filters from the context
            if (pnext == startkey)
            {
                should_be_added = false;
            }
            auto* pNextSearch = searched.Find(pnext);
            if (true)
            {
                if (!pNextSearch)
                {
					searched.Add(pnext, FAStarNode(Dest, pnext));
                    pNextSearch = searchAtKey(pnext);
					// it's not actually in the node so we create a new node
					curr = searchAtKey(currkey);
                }

				if (pNextSearch)
				{
					float pNextHCost = curr->GetCost(dir) + curr->hcost;
					/* if there are any movement modifiers this would be a good place to put them */
					float pNextFCost = pNextHCost + pNextSearch->gcost;
					if (pNextFCost < pNextSearch->fcost)
					{
						pNextSearch->fcost = pNextFCost;
						pNextSearch->hcost = pNextHCost;
                        pNextSearch->shortestkey = currkey;
					}
				}
            }
        }
    }

    curr->visited = true;

    // hcost should be determined now apply acceptance radius clause
    if (curr->DistToDest < acceptable_radius)
    {
        if(curr->hcost < closest_hcost)
        {
            closest_key = curr->node;
            closest_dist = curr->DistToDest;
            closest_hcost = curr->hcost;
        }
    }
    else 
    {
        if(curr->DistToDest < closest_dist)
        {
            closest_key = curr->node;
            closest_dist = curr->DistToDest;
            closest_hcost = curr->hcost;
        }
    }
}


FAStarCtx::FAStarCtx(FVector start, FVector dest, UWorld* _world)
{
    searched.Empty();
    Dest = dest;
    world = _world;
    auto startnode = FAStarNode(dest, start, world);
    auto destnode = FAStarNode(dest, dest, world);
    closest_hcost = FLT_MAX;
    closest_dist = FLT_MAX;
    closest_key = nullptr;
    initialized = true;
    startkey = startnode.node;
    startnode.fcost = startnode.gcost;
    startnode.hcost = 0;
    destkey = destnode.node;
    if(destkey)
    {
        Dest = destkey->location;
        partial_mode = false;
        // searched.Add(destkey, destnode);
    }
    else {
        acceptable_radius = 3500;
        partial_mode = true;
    }
    no_path = false;

    if (startkey)
    {
        searched.Add(startkey, startnode);
        currkey = searched.Find(startkey)->node;
    }
    else {
        no_path = true;
    }
}

FGridPathVectors FAStarCtx::GetDestinationPath(bool optimize)
{
    FGridPathVectors rv;
    auto ipath = GetDestinationPathInner();
    rv.total_cost = ipath.total_cost;

    if(optimize)
    {
        ipath.ParsePathPoints(&rv.path);
    }
    else{
        ipath.AllPathPoints(&rv.path);
    }

    return rv;
}

FGridPath FAStarCtx::GetDestinationPathInner()
{
    FGridPath rv;
    FAStarNode* walker = nullptr;
	rv.is_complete = false;
    if (destkey)
    {
		if (auto d = searchAtKey(destkey))
		{
            if (d->visited)
            {
				walker = searchAtKey(destkey);
				rv.is_complete = true;
            }
		}
    }

    if (no_path)
    {
        if (closest_key)
        {
            // UE_LOG(LogTemp, Warning, TEXT("destination not reached but partial destination found at"), );

            auto x = searchAtKey(closest_key);

            // UE_LOG(LogTemp, Warning, TEXT("Distance=%f fcost=%f hcost=%f gcost=%f"), (closest_key->location - Dest).Size(), x->fcost, x->hcost, x->gcost);
            walker = searchAtKey(closest_key);
        }
    }

    bool error_found = true;
    int iterations = 1200;

    if (walker)
    {
		rv.total_cost = walker->hcost;
        while (walker->shortestkey && (iterations > 0) && walker != searchAtKey(startkey))
        {
			rv.path.Add(*walker->node);
            auto newwalker = searchAtKey(walker->shortestkey);
            if (!newwalker)
            {
                WMSG("Shortestkey is set but not in search tree??");
                iterations = 0;
                error_found = true;
                break;
            }
            walker = newwalker;
        }
		iterations--;
    }

    return rv;
}

void FAStarCtx::next()
{
    if (no_path)
    {
        WMSG("next called with no path");
        return;
    }
    
    float lowest_fcost = FLT_MAX-1;
    float lowest_gcost = FLT_MAX;
    FGridNode* nextcurr = nullptr;

    bool path_found = false;


    if (currkey)
    {
        //1. Calculate all costs for the current node

		CalculateNeighbourCosts();

		// 2. walk through all nonvisited nodes and look for the lowest fcost
		for (auto it = searched.CreateIterator(); it; ++it)
		{
			if (auto psearch = &it.Value())
			{
				if (!(psearch->visited))
				{
					if ((psearch->fcost < lowest_fcost))
					{
						lowest_fcost = psearch->fcost;
						nextcurr = psearch->node;
						lowest_gcost = psearch->gcost;
					}
					else if ((psearch->fcost == lowest_fcost) && (psearch->gcost < lowest_gcost))
					{
						lowest_gcost = psearch->gcost;
						nextcurr = psearch->node;
					}
				}
			}
		}
        if ((currkey == nextcurr) && currkey != destkey) 
        {
            no_path = true;
        }
        auto x = searchAtKey(currkey);
        // UE_LOG(LogTemp, Warning, TEXT("Distance=%f fcost=%f hcost=%f gcost=%f"), (currkey->location - Dest).Size(), x->fcost, x->hcost, x->gcost);
        currkey = nextcurr;
    }
    else {
        no_path = true;
        return;
    }
}

FGridPathVectors UEmNavLibHelper::PathToLocationSynchronousOptimized( UWorld* WorldContextObject,FVector start, FVector dest, float radius)
{
    auto ctx = StartPathFindingTask(WorldContextObject, start, dest, radius);
    
    int max_iterations = 1500;
    while ((!ctx.isDone()) && max_iterations)
    {
        ctx.next();
        max_iterations--;
    }
    if (max_iterations < 0)
    {
        WMSG("maximum number of iterations reached");
    }
    return ctx.GetDestinationPath(true);
}

FGridPathVectors UEmNavLibHelper::PathToLocationSynchronous( UWorld* WorldContextObject,FVector start, FVector dest, float radius)
{
    auto ctx = StartPathFindingTask(WorldContextObject, start, dest, radius);
    
    int max_iterations = 1500;
    while ((!ctx.isDone()) && max_iterations)
    {
        ctx.next();
        max_iterations--;
    }
    if (max_iterations < 0)
    {
        WMSG("maximum number of iterations reached");
    }
    return PathFindingTask_GetBestPath(ctx);
}

FAStarCtx UEmNavLibHelper::StartPathFindingTask( UWorld* WorldContextObject, FVector start, FVector dest, float radius)
{
    FAStarCtx ctx(start, dest, WorldContextObject);
    ctx.SetRadius(radius);
    return ctx;
}

bool UEmNavLibHelper::PathFindingTaskNext(FAStarCtx& ctx)
{
    if (!ctx.isDone())
    {
        ctx.next();
    }
    return ctx.isDone();
}

FGridPathVectors UEmNavLibHelper::PathFindingTask_GetBestPath(FAStarCtx& ctx)
{
    auto rv = ctx.GetDestinationPath();

    return rv;
}
