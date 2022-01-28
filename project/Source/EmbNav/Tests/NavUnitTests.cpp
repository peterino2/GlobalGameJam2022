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

#define IS_TEST_CPP

#include "GridMarker.h"
#include "EmNavDjk.h"
#include "EmNavAStar.h"
#include "EmNavTestHelpers.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FHelloWorldTest, 
    "Embersong.Core.UnitHelloWorld", 
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter 
)

bool FHelloWorldTest::RunTest(const FString& Parameters)
{
    // Make the test pass by returning true, or fail by returning false.
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMultiExtentsTest, 
    "Embersong.Nav.Djk Span Extents", 
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter 
)

void quick_testgraph(FGridNodeGraph& graph, int id, int expected_geometries)
{
    FDjkCtx ctx;
    ctx.SetStartKey(graph.GetAtPos(0, 0));
    FGridSpanSearchParams params;

    FGridSpanMulti multispan = ctx.GetMultiSpanOptimized(FLT_MAX, params);

    WARN_ASSERT(
        multispan.spans.Num() == expected_geometries, 
        "Multispan span did not draw exactly %d geometry, drawn %d instead?", 
        expected_geometries,
        multispan.spans.Num() 
    );
}

bool FMultiExtentsTest::RunTest(const FString& Parameters)
{
    /*Construct an array of FGridNodes with proper association*/
    FGridNodeGraph graph[] = { 
        FGridNodeGraph(1, 1),
        FGridNodeGraph(2, 2),
        FGridNodeGraph(500, 500),
        FGridNodeGraph(500, 1),
        FGridNodeGraph(1, 500),
        FGridNodeGraph(2, 32)
    };

    int expected_geometries[] = {1, 1, 1, 1, 1, 1};

    FDjkCtx ctx; // alright lads, lets do this

    FGridNodeGraph& nodegraph = graph[0];

    ctx.SetStartKey(graph[0].GetAtPos(0, 0));

    FGridSpanSearchParams params;
    FGridSpanMulti multispan = ctx.GetMultiSpanOptimized(FLT_MAX, params);

    WARN_ASSERT(multispan.spans.Num() == 1, "Multispan span did not draw exactly 1 geometry, drawn %d instead?" , multispan.spans.Num());

    WARN_ASSERT(multispan.unique_nodecount == 8, "Multispan should have drawn exactly 8 ");

    for(int i = 0; i < multispan.spans.Num(); i++)
    {
        int span_nodecount = multispan.spans[i].nodes.Num();
        FVector start_loc = multispan.spans[i].nodes[0].location;
        FVector end_loc = multispan.spans[0].nodes[span_nodecount - 1].location;

        // WMSG("span#%d, span_nodecount=%d", i, span_nodecount);
        WARN_ASSERT((end_loc - start_loc).Size() < 10.0f, "first and last node of span #%d not closed", i);
    }

    for(int i = 0; i < GET_ARRAY_SIZE(graph); i ++)
    {
        quick_testgraph(graph[i], i, expected_geometries[i]);
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAStarSimple, 
    "Embersong.Nav.AStarSimple", 
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter 
)

bool FAStarSimple::RunTest(const FString& Parameters)
{
    FAStarCtx ctx;

    return true;
}
