// Fill out your copyright notice in the Description page of Project Settings.


#include "KiilEmAll.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AKiilEmAll::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController != nullptr)
    {
       EndGame(false);
    }
    //For loop over all AI charcs. in the lvl
    for (AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
    {
        // isDEAD?
        if (!AIController->IsDead())
        {
            return;
        }
           
    }    
    //end game
    EndGame(true);        
}

void AKiilEmAll::EndGame(bool bIsPlayerWinner)
{
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}

