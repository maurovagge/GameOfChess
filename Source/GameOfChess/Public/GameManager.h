#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Piece.h"
#include "Piece_Bishop.h"
#include "Piece_Rook.h"
#include "Piece_Knight.h"
#include "Piece_Queen.h"
#include "Piece_King.h"
#include "Piece_Pawn.h"
#include "GoC_HumanPlayer.h"
#include "GoC_AIPlayer.h"
#include "VirtualBoardManager.h"
#include "GameManager.generated.h"


 // Enum to define the current game state.
 
UENUM(BlueprintType)
enum class EGameState : uint8
{
    GameState_Default UMETA(DisplayName = "Default"),
    GameState_PlayerTurn UMETA(DisplayName = "PlayerTurn"),
    GameState_AITurn UMETA(DisplayName = "AITurn"),
    GameState_GameOver UMETA(DisplayName = "GameOver")
};


 // Enum to define the result of the game.
 
UENUM(BlueprintType)
enum class EGameResult : uint8
{
    GameResult_None UMETA(DisplayName = "None"),
    GameResult_PlayerWin UMETA(DisplayName = "PlayerWin"),
    GameResult_AIWin UMETA(DisplayName = "AIWin"),
    GameResult_Draw UMETA(DisplayName = "Draw")
};

class AGoC_AIPlayer;





USTRUCT(BlueprintType)
struct FTileCoordinates
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess")
    int32 RowIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess")
    int32 ColumnIndex;

    FTileCoordinates() : RowIndex(0), ColumnIndex(0) {}
    FTileCoordinates(int32 Row, int32 Column) : RowIndex(Row), ColumnIndex(Column) {}
};


 // AGameManager handles game logic and state.
 
UCLASS()
class GAMEOFCHESS_API AGameManager : public AGameModeBase
{
    GENERATED_BODY()

public:
    // Constructor 
    AGameManager();

    // Called when the game starts or when spawned 
    virtual void BeginPlay() override;

    // Called every frame
    void Tick(float DeltaTime);

    // Function to start or restart the game 
    UFUNCTION(BlueprintCallable)
    void StartGame();

    // Function to handle the end of a player's turn
    UFUNCTION(BlueprintCallable, Category = "Game")
    void EndPlayerTurn();

    // Function to set the AI player instance
    void SetAIPlayerInstance(AGoC_AIPlayer* AIPlayerInstance);

    // Function to get the AI player instance
    AGoC_AIPlayer* GetAIPlayerInstance() const;

    // Function to set the human player instance
    void SetHumanPlayerInstance(AGoC_HumanPlayer* HumanPlayerInstance);

    // Function to get the human player instance
    AGoC_HumanPlayer* GetHumanPlayerInstance() const;

    // Function to check if the game is over
    bool GameOver(ABoard* Board, ETeam PlayerTeam);

    // Funtion to end the game
    void EndGame(AGoC_Player* WinningPlayer);

    // Function to handle player input
    void HandlePlayerInput();

    // Function to handle pawn promotion
    void HandlePawnPromotion(APiece* PromotedPawn);

    // Function to set up pieces at the beginning pof the game
    void SetupPieces();

    bool CanMove();

    // score value for human player
    UPROPERTY(EditAnywhere)
    FString ScoreHumanPlayer = "White score: 0";

    // score value for AI player
    UPROPERTY(EditAnywhere)
    FString ScoreAiPlayer = "Black score: 0";


    UPROPERTY(EditAnywhere)
    FString CurrentTurnMessage = "Human (White)";

    UPROPERTY(EditAnywhere)
    FString CurrentMoveMessage = "";

    UPROPERTY(EditAnywhere)
    FString GeneralMessage = "";

    // get the score for human player
    UFUNCTION(BlueprintCallable)
    FString GetScoreHumanPlayerMessage();

    // get the score for AI player
    UFUNCTION(BlueprintCallable)
    FString GetScoreAiPlayerMessage();

    // get the current turn message
    UFUNCTION(BlueprintCallable)
    FString GetTurnMessage();

    // get the current move message
    UFUNCTION(BlueprintCallable)
    FString GetMoveMessage();

    // get the current move message
    UFUNCTION(BlueprintCallable)
    FString GetGeneralMessage();

    UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
    TSubclassOf<ABoard> BoardClass;

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
    TSubclassOf<APlayerCameraManager> FixedCameraClass;

    // The default pawn class used by players
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setup")
    TSubclassOf<AGoC_Player> DefaultPlayerClass;

    // reference to a Board object
    UPROPERTY(VisibleAnywhere)    
    ABoard* NewBoard;

    // Function to spawn cameras
    void SpawnPlayerCameras();

    // Declaration of the PlayerCameraClass variable 
    UPROPERTY(EditDefaultsOnly, Category = "Player Camera")
    TSubclassOf<ACameraActor> PlayerCameraClass;

    // Function the spawn the camera
    void SpawnFixedCamera();

    //Function to update game state 
    void UpdateGameState();

    // Function to check the current turn owner
    bool IsMyTurn(AGoC_Player* Player);


    // Funtion to choose the "random level" for AI
    UFUNCTION(BlueprintCallable)
    void SetRandomOpponent();

    // Funtion to choose the "minimax level" for AI
    UFUNCTION(BlueprintCallable)
    void SetMinimaxOpponent();

    // Funtion to restart the game
    UFUNCTION(BlueprintCallable)
    void RestartGame();

    // Funtion to choose the reset the score of players
    UFUNCTION(BlueprintCallable)
    void ResetScore();

    UFUNCTION(BlueprintCallable)
    void HistPlayBackward();

    UFUNCTION(BlueprintCallable)
    void HistPlayForward();

    UFUNCTION(BlueprintCallable)
    void HistPlayStart();

    UFUNCTION(BlueprintCallable)
    void HistPlayEnd();

    // Funtion to terminate the game and exit
    UFUNCTION(BlueprintCallable)
    void Exit();

    UFUNCTION(BlueprintCallable)
    void UndoLastMove();


    bool CanPlay();

protected:
    // Current game state 
    UPROPERTY(BlueprintReadOnly)
    EGameState CurrentGameState;

    // Current game result 
    UPROPERTY(BlueprintReadOnly)
    EGameResult CurrentGameResult;

    // Function to check win conditions
    bool CheckWinConditions();

    // AI player instance
    AGoC_AIPlayer* AIPlayer;

    // Human player instance
    AGoC_HumanPlayer* HumanPlayer;

    // Current player on turn
    AGoC_Player* CurrentPlayer;

    bool bMinimaxOpponent;
    bool bCanMove;
    int32 AIScore;
    int32 HumanScore;

    bool bGameEnded;

    VirtualBoardManager VBoardManager;

};
