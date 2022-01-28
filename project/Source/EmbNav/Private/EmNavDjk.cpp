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

#include "EmNavDjk.h"
#include "GridMarker.h"

void FDjkCtx::SetStartKey(FGridNode* _startkey)
{
    FSearchedNode x = FSearchedNode(_startkey);
    startkey = _startkey;
    searched.Add(startkey, x);
}

void FDjkCtx::SetDestKey(FGridNode* _destkey)
{
    FSearchedNode x = FSearchedNode(_destkey);
    destkey = _destkey;
    searched.Add(destkey, x);
    SetExtentsMode(false);
}

FDjkCtx::FDjkCtx(FVector location, FGridPath* _path, UWorld* _world)
{
    path = _path;
    no_path = false;
    max_cost = FLT_MAX;
    world = _world;
    destkey = nullptr;
    startkey = nullptr;
    closest_key = nullptr;
    extents_mode = true;
    closest_dist = FLT_MAX;


    auto startnode = FSearchedNode(location, world);
    auto idx = searched.Add(startnode.node, startnode);

    startkey = startnode.node;
    if(!startkey)
    {
        SHOW_FVECTOR(location, "Context cannot find a start node");
        done = true;
        no_path = true;
        return ;
    }
    currkey = startkey;
    done = false;
    searchAtKey(startkey) -> fcost = 0;

    /*
    UE_LOG(LogTemp, Warning, TEXT("DjkCtx:: start at %d %d"), 
        startnode.node->pos.x,
        startnode.node->pos.y 
    );
    */
}

FDjkCtx::FDjkCtx():
    world(nullptr),
    destkey(nullptr),
    startkey(nullptr),
    currkey(nullptr),
    closest_key(nullptr),
    path(nullptr),
    closest_dist(0),
    max_cost(0),
    partial_mode(false),
    extents_mode(true),
    no_path(true),
    done(true),
    TargetDest(FVector(0.0f,0.0f,0.0f))
{
}

FGridSpan FDjkCtx::GetSpanSynchronously(float _max_cost)
{
    max_cost = _max_cost;
    destkey = 0;

    while(!isDone())
    {
        next();
    }
    
    FGridSpan gridspan;

    for(auto iter = searched.CreateIterator(); iter; ++iter)
    {
        FSpanNode spanode;
        spanode.location = iter.Value().node->location;
        spanode.cost = iter.Value().fcost;
        gridspan.nodes.Add(spanode);
    }

    return gridspan;
}


void FDjkCtx::setMaxCost(float _max_cost)
{
    max_cost = _max_cost;
}

FGridSpanMulti AGridMarker::GetMultiSpanOptimized(
        UObject* WorldContextObject, 
        FVector start, 
        float cost, 
        FGridSpanSearchParams params 
)
{
    UWorld* world = WorldContextObject->GetWorld();
    FDjkCtx navContext = FDjkCtx(start, nullptr, world);

    // DESPERATE_TRACE();
    return navContext.GetMultiSpanOptimized(cost, params);
}

float FDjkCtx::CalcDistCompare(FVector start, FVector end)
{
    return MAX(((start - end)*FVector(1.0, 1.0, 0.1)).Size(), 160);
}

void FDjkCtx::setDestination(FVector destination)
{
    auto destnode = FSearchedNode(destination, world);
    auto idx = searched.Add(destnode.node, destnode);
    destkey = destnode.node;
    SetExtentsMode(false);

    if (destkey)
    {
        TargetDest = destkey->location;
    }
    else {
        TargetDest = destination;
    }

    closest_key = startkey;
    if (startkey)
    {
		closest_dist = CalcDistCompare(startkey->location, TargetDest);
    }
    // UE_LOG(LogTemp, Warning, TEXT("FDjkCtx:: destination set at %p"), destnode.node);
	// SHOW_FVECTOR(dest->node->location, " destination location");
}

void FDjkCtx::SetExtentsMode(bool mode)
{
    extents_mode = mode;
}

void FDjkCtx::next()
{
    // search for lowest value with lowest fcost
    float lowest_fcost = FLT_MAX;
    float lowest_dist = FLT_MAX;
    FGridNode* nextcurr = currkey;
    calcNeighbourCosts();
    if((!destkey) && (!extents_mode))
    {
        DESPERATE_TRACE();
        no_path = true;
        return;
    }

    //WMSG("Mode ok, going forward")
    for (auto it = searched.CreateIterator(); it; ++it)
    {
        // pair.Key;
        // pair.Value;
        if ( auto psearch = &it.Value() )
        {
            if(!(psearch->visited))
            {
                // Destination set clause
                // use heuristic as the selector.
                if(!extents_mode)
                {
                    // order of priority 
                    // distance, then fcost
                    if((psearch -> dist ) < lowest_dist)
                    {
                        lowest_fcost = FLT_MAX;
                        nextcurr = psearch->node;
                        lowest_dist = psearch->dist;
                        // UE_LOG(LogTemp, Warning, TEXT("lowest_dist = %f"), lowest_dist);
                    }
                    if((psearch->dist ) == lowest_dist && psearch->fcost < lowest_fcost)
                    {
                        lowest_fcost = psearch->fcost;
                        nextcurr = psearch->node;
                    }
                } 
                else 
                {
                    if( (psearch -> fcost < lowest_fcost))
                    {
                        lowest_fcost = psearch->fcost;
                        nextcurr = psearch->node;
                    }
                }
            }
        }
    }
    
    if (nextcurr) {
        if (nextcurr == currkey)
        {
            // UE_LOG(LogTemp, Warning, TEXT("No Path found"));
            //if(!getDestinationPath())
            //{
                no_path = true;
            //}
        }
        // extents mode clause
		currkey = nextcurr;
    }
}

bool FDjkCtx::GetPathInner(FGridNode* target_key)
{
    // start at dest, walk through -> shortest until we reach the start,
    // this path traces backwards from the destination to the start so it must be reverse iterated through.
    auto walker = searchAtKey(target_key);
    int32_t iterations = 1200;
    *path = FGridPath();
    path->total_cost = walker->fcost;
    bool error_found = false;
    while(walker != searchAtKey(startkey) &&( iterations > 0))
    {
        path->path.Add(FGridNode(*walker->node));
        if (walker -> shortestkey)
        {
            auto newwalker = searchAtKey(walker -> shortestkey);
            if(!newwalker)
            {
                WMSG("No Shortest key set for path");
                iterations = 0;
                error_found = true;
                break;
            }
            walker = newwalker;
        }
        else
        {
            //UE_LOG(LogTemp, Warning, TEXT("Grid entry has no shortestkey set"));
        }
        iterations--;
    }

    if (iterations <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Path found, Graph tree traversed, no backpath found"));
        return false;
    }
    else
    {
        return true;
    }
}

bool FDjkCtx::GetPartialPath()
{
    return GetPathInner(closest_key);
}

bool FDjkCtx::getDestinationPath()
{
    return GetPathInner(destkey);
}

void FDjkCtx::calcNeighbourCosts()
{
    // walk iteration from current to neighbours
    // if path_costs == -1 or path_nodes is nullptr then that path is not traversable
    // and that node shall not be added to searched

    // start with curr, search radially
    auto curr = searchAtKey(currkey);
    //UE_LOG(LogTemp, Warning, TEXT(" Calculating neighbour costs for %p\n"), curr);
    for(int dir = 0; dir < PATH_COUNT; dir++)
    {
        if(auto* pnext = curr->GetNextGridNode(dir, searchAtKey(startkey)->node))
        {

            auto* pNextSearch = searched.Find(pnext);

            if( pNextSearch )
            {
                float cost_to_here_from_direction = pNextSearch->fcost + curr->GetCost(dir);

                if((cost_to_here_from_direction <= max_cost))
                {

                    if (( cost_to_here_from_direction < curr->fcost))
                    {
                        curr -> fcost = cost_to_here_from_direction;
                        curr -> shortestkey = pnext;
                        curr -> dist = FLT_MAX;
                        if(!extents_mode)
                        {
                            if(destkey)
                            {
                                curr -> dist = CalcDistCompare(curr->node->location, TargetDest);
                            }
                        }
                    }
                }
            }

            float cost_to_node_from_here = curr->fcost + curr->GetCost(dir);
            if(cost_to_node_from_here <= max_cost)
            {
                if( !pNextSearch )
                {
                    auto newnode = FSearchedNode(pnext);
                    searched.Add(pnext, newnode);
                    pNextSearch = searched.Find(pnext);
                    curr = searchAtKey(currkey); 
                }

                if ( cost_to_node_from_here < pNextSearch->fcost)
                {
                    /*UE_LOG(LogTemp, Warning, TEXT("shortestkey set fcost=%f pos=%d,%d shortest=%d,%d"), 
                        cost_to_node_from_here,
                        pNextSearch->node->pos.x,
                        pNextSearch->node->pos.y,
                        curr->node->pos.x,
                        curr->node->pos.y
                    )                    */
                    pNextSearch -> fcost = cost_to_node_from_here;
                    pNextSearch -> shortestkey = currkey;
                    if(!extents_mode)
                    {
                        if(destkey)
                        {
                            pNextSearch -> dist = CalcDistCompare(pNextSearch->node->location, TargetDest);
                        }
                    }
                }
            }
        }
    }

    // How fucking high was I?

    curr = searchAtKey(currkey);

    if(! closest_key )
    {
        closest_key = curr->node;
        closest_dist = curr->dist;
    }


    if((curr->dist < closest_dist))
    {
        closest_key = curr->node;
        closest_dist = curr->dist;
    }
    else if((curr->dist == closest_dist))
    {
		auto closest = searchAtKey(closest_key);
        if (closest)
        {
            if (curr->fcost < closest->fcost)
            {
				closest_key = curr->node;
				closest_dist = curr->dist;
            }

        }
    }

    searchAtKey(currkey) -> visited = true;
}


#define TURN(dir) dir = (dir + 1) % PATH_COUNT // UP LEFT DOWN RIGHT
#define RTURN(dir) dir = dir - 2 < 0 ? (dir - 2) + 8 : dir - 2    // UP RIGHT DOWN LEFT
#define RTURN1(dir) dir = dir - 1 < 0 ? 7 : dir - 1    // UP RIGHT DOWN LEFT
#define ISDIAGONAL(dir) !((dir % 2) == 0)

FGridSpan FDjkCtx::GetSpanExtendsInner(FGridNode* start_edge, int start_dir, TMap<uint32, int>* keycheck)
{
    int dir = start_dir;

    const float gridsize = GRID_SIZE;
    const float halfgrid = gridsize/2 + 10;

    bool first = true;
    bool should_break = false;
    FGridNode* walker = start_edge;
    int iterations = 1200;

    bool test_mode = walker->root > 0;

    FGridSpan gridspan;

	//UE_LOG(LogTemp, Warning, TEXT("start_dir %d "), start_dir);
    do
    {
        FVector up = FVector(GRID_SIZE, 0.0f, 0.0f);
        FVector left = FVector(0.0f, GRID_SIZE, 0.0f);

        if (walker->root)
        {
            up = -walker->root->rot_right;
            left = -walker->root->rot_fwd;
        }

        FVector edge_vectors[] = {
            (up)*halfgrid, // up
            (up + left) * halfgrid, // upleft
            (left)*halfgrid,    // left
            (-up + left) * halfgrid,    // downleft
            (-up) * halfgrid, // down
            (-up + -left) * halfgrid, // downright
            (-left) * halfgrid, // right
            (up + -left) * halfgrid // upright
        };

        int maxturns = PATH_COUNT;
        while ((!(searchAtKey(walker->GetNext(dir, startkey)))) && maxturns)
        {

            int ism_index = walker->ism_index;
            FTransform trans;
            FVector center = 
                walker -> pos.x * 100 * FVector(1.0f, 0.0f, 0.0f) + 
                walker->pos.y*100 *FVector(0.0f, 1.0f, 0.0f);
            FRotator rot = FRotator(0.0f,0.0f,0.0f);

            if (walker->root)
            {
                if (auto* ism = walker->root->get_ism())
                {
                    ism->GetInstanceTransform(ism_index, trans, true);
                    center = trans.GetLocation();
                    rot = trans.GetRotation().Rotator();
                }
            }
            if (keycheck)
            {
                uint32 hash = FGridSpanKey(walker, dir).hash();
                keycheck->Add(hash, 1);
                /*
                UE_LOG(LogTemp, Warning, TEXT("inserted a thing, pos = %d %d dir= %d hash =%u"),
                        walker->pos.x,
                        walker->pos.y,
                        dir,
                        hash);
                        */
            }
            FSpanNode spanode;
            spanode.location = center + rot.RotateVector(edge_vectors[dir]);
            spanode.cost = 0;
            gridspan.nodes.Add(spanode);

            auto last_dir = dir;
            TURN(dir);
            maxturns--;
            //UE_LOG(LogTemp, Warning, TEXT("Applying turn dir =%d -> %d"), last_dir, dir);
        }

        if(first)
        {
            start_edge = walker;
            start_dir = dir;
	        // UE_LOG(LogTemp, Warning, TEXT("first_edge found at %d %d dir = %d"), start_edge->pos.x, start_edge->pos.y, start_dir);
        }

        if(ISDIAGONAL(dir))
        {
            FGridNode* cross = walker->GetNext((dir + 1) % 8, startkey);
            if(cross)
            {
                int testdir = dir;
                RTURN(testdir);
				FVector center = 
					cross -> pos.x * 100 * FVector(1.0f, 0.0f, 0.0f) + 
					cross->pos.y*100 *FVector(0.0f, 1.0f, 0.0f);
				FRotator rot = FRotator(0.0f,0.0f,0.0f);
                // UE_LOG(LogTemp, Warning, TEXT("Checking cross = %d %d %d %d"), cross->pos.x, cross->pos.y, dir, testdir);
                if(!(searchAtKey(cross->GetNext(testdir, startkey))))
                {
                    int ism_index = cross->ism_index;
                    FTransform trans;
                    if(cross->root)
                    {
                        if(auto* ism = cross->root->get_ism())
                        {
                            ism->GetInstanceTransform(ism_index, trans, true);

                            center = trans.GetLocation();
                            rot = trans.GetRotation().Rotator();

                        }
                    }
					if(keycheck)
					{
						uint32 hash = FGridSpanKey(walker, dir).hash();
						//if(!keycheck->Contains(hash))
						//{
							keycheck->Add(hash, 1);
						//  UE_LOG(LogTemp, Warning, TEXT("inserted a thing, pos = %d %d dir= %d hash =%u"), 
						//          walker->pos.x,
						//          walker->pos.y,
						//          dir,
						//          hash);
						//}
					}
					FSpanNode spanode;
					spanode.location = center + rot.RotateVector(edge_vectors[testdir]);
					spanode.cost = 0;
					gridspan.nodes.Add(spanode);
                }
            }
            else{
                // UE_LOG(LogTemp, Warning, TEXT("Cross is invalid?"));
                // 
            }
        }
		should_break = ((walker == start_edge) && (start_dir == dir) && (!first)) || !(maxturns);
        walker = walker->GetNext(dir, startkey);

	    // UE_LOG(LogTemp, Warning, TEXT("walking to next edge at %d %d dir = %d"), walker->pos.x, walker->pos.y, dir);

        if(ISDIAGONAL(dir))
        {
            RTURN1(dir);
        }

		RTURN(dir);

        iterations--;
        first = false;
    }while((!should_break) && iterations);


    if (!iterations)
    {
        UE_LOG(LogTemp, Error, TEXT("Max Iterations exceeded for edge finding"));
        FGridSpan newgridspan;
        return newgridspan;
    }

    // FSpanNode newspannode = new_vectors[0];

    // new_vectors.Add(newspannode);
    // if(span->nodes.Num() > 1)
    // {
    //     newspannode = new_vectors[1];
    //     new_vectors.Add(newspannode);
    // }
    int lastid = gridspan.nodes.Num();

    if(lastid)
    {
        // WMSG("Adding closing node");
        FSpanNode newnode = gridspan.nodes[0];
        gridspan.nodes.Add(newnode); // close the loop
    }

    return gridspan;
}   

void FDjkCtx::OptimizeSpanExtends(FGridSpan* span)
{

    TArray<FSpanNode> new_vectors;


    if(!span->nodes.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("invalid vector data for optimzation skipping this step"));
        return;
    }
    
    float tolerance = 10.0f;
    FVector last_pos = span->nodes[0].location;
    FVector last_dir; //extends.nodes[1].location - extends.nodes[0].location;
    bool first = true;

    for(int i = 0; i < span->nodes.Num(); i++)
    {
        FVector current_dir;
        if(first)
        {
            first = false;
        }

        current_dir = (span->nodes[i].location - last_pos);
        if ((current_dir - last_dir).Size() > tolerance)
        {
            if(i > 0)
            {
                new_vectors.Add(span->nodes[i-1]);
            }
        }
        last_pos = span->nodes[i].location;
        last_dir = current_dir;
    }

    if(span->nodes.Num())
    {
        FSpanNode newspannode = new_vectors[0];
        new_vectors.Add(newspannode);
    }
    span->nodes = new_vectors;
}

FGridSpan FDjkCtx::GetSpanExtends(float _max_cost) // deprecated
{
    max_cost = _max_cost;
    destkey = 0;

    while(!isDone())
    {
        next();
    }

    TArray<FGridNode*> edges;

    // 1. walk from from start until you hit a node that isn't searched
    FGridNode* start_edge = nullptr;
    FGridNode* walker = nullptr;
    
    int dir = UP;

    int iterations = 1200;
    auto start = searchAtKey(startkey)->node;

    walker = searchAtKey(startkey)->node;
    while(searchAtKey(walker->GetNext(dir, start)) && iterations)
    {
        walker = walker->GetNext(dir, start);
        iterations--;
    }

    while(!(searchAtKey(walker->GetNext(dir, start))))
    {
        auto last_dir = dir;
        TURN(dir);
        // UE_LOG(LogTemp, Warning, TEXT("Applying turn dir =%d -> %d"), last_dir, dir);
    }

    if (!iterations)
    {
        // UE_LOG(LogTemp, Error, TEXT("Max iterations exceeding for initial walking"));
        FGridSpan gridspan;
        return gridspan;
    }

    iterations = 1200;
    start_edge = walker;

    return GetSpanExtendsInner(start_edge, dir, nullptr);
}

FGridSpanMulti FDjkCtx::GetMultiSpanOptimized(float _max_cost, FGridSpanSearchParams params)
{
    // DESPERATE_TRACE();
    max_cost = _max_cost;
    currkey = startkey;
    destkey = 0;


    while(!isDone())
    {
        next();
    }

    // DESPERATE_TRACE();

    FGridSpanMulti multispan;
    multispan.spans.Empty();

    TMap<uint32, int> keycheck;
    keycheck.Empty();
    const int sdirs[] = {UP, LEFT, DOWN, RIGHT};

    if (!startkey)
    {
        return multispan;
    }

    for(auto iter = searched.CreateIterator(); iter; ++iter)
    {
        for(int i = 0; i < 4; i++)
        {
            // check this direction
            FGridNode* walker = iter.Value().node;
            FGridNode* walkernext = walker -> GetNext(sdirs[i], startkey);

            if(!(searchAtKey(walkernext)))
            {
                uint32 hash = FGridSpanKey(walker, sdirs[i]).hash();
                if (!keycheck.Contains(hash))
                {
                    // UE_LOG(LogTemp, Display, TEXT("edge found at %d %d  dir = %d"), walker->pos.x, walker->pos.y, i);
                    //
                    int dir = sdirs[i];
                    int max_rotations = 4;
                    while((!(searchAtKey(walker->GetNext(dir, startkey)))) && max_rotations)
                    {
                        max_rotations--;
                        TURN(dir);
                    }

                    FGridSpan newspan = GetSpanExtendsInner(walker, dir, &keycheck);
                    OptimizeSpanExtends(&newspan);
                    multispan.spans.Add(newspan);
                }

            }
        }
    }
    // DESPERATE_TRACE();

    UE_LOG(LogTemp, Display, TEXT("number of splines found = %d; key nodes = %d"), multispan.spans.Num(), keycheck.Num());

    multispan.unique_nodecount = keycheck.Num();

    return multispan;
}

FGridSpan FDjkCtx::GetSpanExtendsOptimized(float _max_cost)
{
    // DESPERATE_TRACE();
    FGridSpan extends = GetSpanExtends(_max_cost);
    OptimizeSpanExtends(&extends);
    return extends;
}

