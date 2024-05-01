// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoC_Player.generated.h"

UCLASS()
class GAMEOFCHESS_API AGoC_Player : public APawn
{
    GENERATED_BODY()

public:
    // Constructor
    AGoC_Player();

    // Set the player's name
    void SetPlayerName( FString& NewName);

    // Get the player's name
    FString GetPlayerName() ;

    // Set the player's score
    void SetPlayerScore(int32 NewScore);

    // Get the player's score
    int32 GetPlayerScore() ;

protected:
    // The name of the player
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
    FString PlayerName;

    // The score of the player
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
    int32 PlayerScore;
};
