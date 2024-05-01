#pragma once

#include "Piece.h"
#include "CoreMinimal.h"
#include "GoC_Player.h" // Include the base player class
#include "GoC_AIPlayer.generated.h"

class AGameManager;

UCLASS()
class GAMEOFCHESS_API AGoC_AIPlayer : public AGoC_Player
{
    GENERATED_BODY()

public:
    // Constructor
    AGoC_AIPlayer();

    // Function to set the AI player team
    void SetTeam(ETeam NewTeam) { Team = NewTeam; }

    // Function to get the AI player team
    ETeam GetTeam()  { return Team; }

    // Function to evaluate the board
    float Evaluate( VirtualBoard board, ETeam PlayerTeam);

    // Function to make the AIPlayer's move
    void MakeAIMove( ABoard* Board);

    // Function to make the AIPlayer's move
    void MakeAIMoveMinimax( ABoard* Board);


    
    // Function to get a GameManager istance
    AGameManager* GetGameManager() ;

protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Minimax algorithm
    int32 MiniMax(VirtualBoard Board, int32 depth, ETeam team);

    // Alpha-beta pruning
    int32 AlphaBetaPruning(int32 depth, int32 alpha, int32 beta, bool maximizingPlayer);

    // AI player team
    UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "AIPlayer")
    ETeam Team;

};

