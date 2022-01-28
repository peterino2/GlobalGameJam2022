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

#include "EmbNav.h"
#include "Modules/ModuleManager.h"

#include "Log.h"

/******************************************************************************
* Function:         virtual void FEmbNav::StartupModule
* Description:      
* Where:
* Return:           
* Error:            
*****************************************************************************/
void FEmbNav::StartupModule(){
    UE_LOG(LogEmbNav, Log, TEXT("EmbNav module starting up"));
}

/******************************************************************************
* Function:         
* Description:      
* Where:
* Return:           
* Error:            
*****************************************************************************/
void FEmbNav::ShutdownModule(){
    UE_LOG(LogEmbNav, Log, TEXT("EmbNav module shutting down"));
}

 
IMPLEMENT_PRIMARY_GAME_MODULE(FEmbNav, EmbNav, "EmbNav" );
