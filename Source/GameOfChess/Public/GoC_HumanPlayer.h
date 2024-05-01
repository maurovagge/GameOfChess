#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Piece.h"
#include "Camera/CameraComponent.h"
#include "GoC_Player.h" // Include the base player class
#include "GoC_HumanPlayer.generated.h"

class AGameManager;

UCLASS()
class GAMEOFCHESS_API AGoC_HumanPlayer : public AGoC_Player
{
    GENERATED_BODY()

public:
    // Constructor
    AGoC_HumanPlayer();

    // Function called when the left mouse button is clicked
    virtual bool LeftMouseButtonClicked();

    // Function called when the right mouse button is clicked
    virtual void RightMouseButtonClicked();

    // Function to highlight possible moves for the selected piece
    void HighlightValidMoves(APiece* SelectedPiece);

    // Function to move the selected piece to the clicked tile
    bool MoveSelectedPiece(ATile* ClickedTile);

    // Function to deselect the currently selected piece
    void DeselectPiece();

    // Function to clear all the highlighted tiles
    void ClearTileHighlights();

    // camera component attacched to player pawn
    UCameraComponent* Camera;

    // Function to set the human player team
    void SetTeam(ETeam NewTeam) { Team = NewTeam; }

    // Function to get the human player team
    ETeam GetTeam() const { return Team; }

    // Function to get a GameManager istance
    AGameManager* GetGameManager() const;

protected:
    // Currently selected piece
    APiece* SelectedPiece;

    int32 selectedRow;
    int32 selectedCol;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Human player team
    UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "AIPlayer")
    ETeam Team;

    // Whether a piece is currently selected
    bool bIsPieceSelected;

    // Array of highlighted tiles for possible moves
    TArray<ATile*> HighlightedTiles;
    
    // Variable to check if human has moved
    bool bHasMoved;
};


