#include "GameManager.h"
#include "GoC_PlayerController.h"
#include "Board.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Public/TimerManager.h"


AGameManager::AGameManager()
{
	PlayerControllerClass = AGoC_PlayerController::StaticClass();

	// Set default game state and result
	CurrentGameState = EGameState::GameState_Default;
	CurrentGameResult = EGameResult::GameResult_None;

	AIPlayer = nullptr;
	HumanPlayer = nullptr;
	CurrentPlayer = HumanPlayer;

	bCanMove = false;

	PrimaryActorTick.bCanEverTick = true;

	bGameEnded = true;
}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	StartGame();
}


//Set instance of AI player actor
void AGameManager::SetAIPlayerInstance(AGoC_AIPlayer* AIPlayerInstance)
{
	AIPlayer = AIPlayerInstance;
}

//Get instance of AI player actor
AGoC_AIPlayer* AGameManager::GetAIPlayerInstance() const
{
	return AIPlayer;
}

//Set instance of human player actor
void AGameManager::SetHumanPlayerInstance(AGoC_HumanPlayer* HumanPlayerInstance)
{
	HumanPlayer = HumanPlayerInstance;
}


//Get instance of human player actor
AGoC_HumanPlayer* AGameManager::GetHumanPlayerInstance() const
{
	return HumanPlayer;
}


//Return if is the turn of a given player
bool AGameManager::IsMyTurn(AGoC_Player* Player)
{
	if (CurrentPlayer == Player)
	{
		return true;
	}
	return false;
}




//Restart the game
void AGameManager::RestartGame()
{
	ABoard* Board = ABoard::GlobalBoardInstance;
	Board->ResetBoard();
	SetupPieces();
	GeneralMessage = FString::Printf(TEXT("Click on a piece"), AIScore);
	bGameEnded = false;
}

//Reset players score
void AGameManager::ResetScore()
{
	AIScore = 0;
	HumanScore = 0;
}


//Move backward along at the playback
void AGameManager::HistPlayBackward()
{
	if (CurrentPlayer == HumanPlayer)
	{
		VirtualBoard vb = ABoard::GlobalBoardManager->PlayBack();
		ABoard::GlobalBoardInstance->SpawnPieces(vb);
	}
}

//Move forward along at the playback
void AGameManager::HistPlayForward()
{
	if (CurrentPlayer == HumanPlayer)
	{
		VirtualBoard vb = ABoard::GlobalBoardManager->PlayForward();
		ABoard::GlobalBoardInstance->SpawnPieces(vb);
	}
}


//Move at the beginning of playback
void AGameManager::HistPlayStart()
{
	if (CurrentPlayer == HumanPlayer)
	{
		VirtualBoard vb = ABoard::GlobalBoardManager->PlayStart();
		ABoard::GlobalBoardInstance->SpawnPieces(vb);
	}
}

//Move to the end of playback
void AGameManager::HistPlayEnd()
{
	if (CurrentPlayer == HumanPlayer)
	{
		VirtualBoard vb = ABoard::GlobalBoardManager->PlayEnd();
		ABoard::GlobalBoardInstance->SpawnPieces(vb);
	}
}

void AGameManager::Exit()
{
}

void AGameManager::SetRandomOpponent()
{
	bMinimaxOpponent = false;
}

void AGameManager::SetMinimaxOpponent()
{
	bMinimaxOpponent = true;
}

void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Return if human player can move
bool AGameManager::CanMove()
{
	return bCanMove;
}

//Check all win conditions
bool AGameManager::CheckWinConditions()
{
	// Retrieve the AI player instance from the GameManager
	AGoC_AIPlayer* AIPlayerInstance = GetAIPlayerInstance();

	// Retrieve the human player instance from the GameManager
	AGoC_HumanPlayer* HumanPlayerInstance = GetHumanPlayerInstance();

	// Check if either player is in checkmate
	if (AIPlayer)
	{
		if (ABoard::GlobalBoardManager->IsCheckMate(ABoard::GlobalBoardManager->GetCurrentBoard(), ETeam::Black))
		{
			// AI player is in checkmate, so human player wins
			HumanScore++;
			GeneralMessage = _T("CheckMate! WHITE WINS!");

			EndGame(HumanPlayer);
			return true;
		}
	}
	if (HumanPlayer)
	{
		if (ABoard::GlobalBoardManager->IsCheckMate(ABoard::GlobalBoardManager->GetCurrentBoard(), ETeam::White))
		{
			// Human player is in checkmate, so AI player wins
			AIScore++;

			GeneralMessage = _T("CheckMate! BLACK WINS!");

			EndGame(AIPlayer);
			return true;
		}
	}

	return false;
}

//Game finished
void AGameManager::EndGame(AGoC_Player* WinningPlayer)
{
	ABoard::GlobalBoardInstance->HighlightTiles(ABoard::GlobalBoardInstance->GetAllTiles(), true, 0);
}

//Game start here
void AGameManager::StartGame()
{

	bGameEnded = false;

	//Setup player actors
	HumanPlayer = GetWorld()->SpawnActor<AGoC_HumanPlayer>(FVector(), FRotator());
	AIPlayer = GetWorld()->SpawnActor<AGoC_AIPlayer>(FVector(), FRotator());

	CurrentPlayer = HumanPlayer;

	//Setup and spawn board
	if (BoardClass != nullptr)
	{
		ABoard::GlobalBoardInstance = GetWorld()->SpawnActor<ABoard>(BoardClass);
		ABoard::GlobalBoardInstance->SetupBoard();
		
		// Setup pieces and camera
		SetupPieces();
		SpawnFixedCamera();

		bCanMove = true;
		GeneralMessage = FString::Printf(TEXT("Click on a piece"), AIScore);

	}
}

//Initialize board manager and spawn pieces over the board
void AGameManager::SetupPieces()
{
	if (!ABoard::GlobalBoardInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("GameManager: Board instance is not valid."));
		return;
	}
	VirtualBoard vb = ABoard::GlobalBoardManager->Init();

	ABoard::GlobalBoardInstance->SpawnPieces(vb);
	return;
}


//Spawn camera on playboard
void AGameManager::SpawnFixedCamera()
{
	// Check if the camera class is valid
	if (!FixedCameraClass)
	{
		return;
	}

	// Get the world
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Spawn the fixed camera above the game board
	FVector CameraLocation = FVector(350.0f, 350.0f, 1100.0f);
	FRotator CameraRotation = FRotator(-90.0f, 0.0f, -90.0f);
	APlayerCameraManager* FixedCamera = World->SpawnActor<APlayerCameraManager>(FixedCameraClass, CameraLocation, CameraRotation);

	// Set the fixed camera as the view target for the player controller
	if (FixedCamera)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
		if (PlayerController)
		{
			PlayerController->SetViewTarget(FixedCamera);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn fixed camera."));
	}
}


//Function to check board conditions after a move, and switch turn to opponent player
void AGameManager::EndPlayerTurn()
{
	GeneralMessage = _T("");

	//Check for any pawn to be promoted to queen
	VirtualBoard vb = ABoard::GlobalBoardManager->GetCurrentBoard();
	if (ABoard::GlobalBoardManager->CheckForPawnToPromote(vb))
	{
		ABoard::GlobalBoardManager->SetCurrentBoard(vb);
		ABoard::GlobalBoardInstance->SpawnPieces(vb);
	}

	//Check for check condition over black team
	if (ABoard::GlobalBoardManager->IsInCheck(ABoard::GlobalBoardManager->GetCurrentBoard(), ETeam::Black))
	{
		//set king tile in "check" color
		ABoard::GlobalBoardInstance->SetKingUnderCheck(ETeam::Black, true);
		GeneralMessage = _T("Black is under check!");
	}

	//Check for check condition over white team
	if (ABoard::GlobalBoardManager->IsInCheck(ABoard::GlobalBoardManager->GetCurrentBoard(), ETeam::White))
	{
		//set king tile in "check" color
		ABoard::GlobalBoardInstance->SetKingUnderCheck(ETeam::White, true);
		GeneralMessage = _T("White is under check!");
	}

	//Check all final conditions (someone won)
	if (CheckWinConditions())
	{
		return;
	}

	// Switch the current player
	if (CurrentPlayer == HumanPlayer)
	{
		CurrentPlayer = AIPlayer;
		CurrentTurnMessage = "AI (BLACK)";
	}
	else if (CurrentPlayer == AIPlayer)
	{
		CurrentPlayer = HumanPlayer;
		CurrentTurnMessage = "HUMAN (WHITE)";
	}

	//Launch AI move with a delay of 2 secs, just to make the game more "real"
	if (CurrentPlayer == AIPlayer)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{				
			//launch proper AI algorithm
			if (bMinimaxOpponent)
			{
				//minimax AI
				AIPlayer->MakeAIMoveMinimax(ABoard::GlobalBoardInstance);
			}
			else
			{
				//random AI
				AIPlayer->MakeAIMove(ABoard::GlobalBoardInstance);
			}
			EndPlayerTurn();
		}, bMinimaxOpponent ? 1 : 2, false);
	}
}

//Get score message to be displayed on screen
FString AGameManager::GetScoreHumanPlayerMessage()
{
	return FString::Printf(TEXT("White score: %d"), HumanScore);
}

//Get score message to be displayed on screen
FString AGameManager::GetScoreAiPlayerMessage()
{
	return FString::Printf(TEXT("Black score: %d"), AIScore);

}

//Get turn message to be displayed on screen
FString AGameManager::GetTurnMessage()
{
	return CurrentTurnMessage;
}

//Get last move message to be displayed on screen
FString AGameManager::GetMoveMessage()
{
	return ABoard::GlobalBoardManager->GetCurrentMove();
}

//Get general messages to be displayed on screen
FString AGameManager::GetGeneralMessage()
{
	if (ABoard::GlobalBoardManager->IsInPlayback())
	{
		return FString::Printf(TEXT("Move forbidden"), AIScore);
	}
	return GeneralMessage;
}

//Undo last move round 
void AGameManager::UndoLastMove()
{
	if (CurrentPlayer == HumanPlayer)
	{
		ABoard::GlobalBoardManager->UndoLastMove();
		ABoard::GlobalBoardInstance->SpawnPieces(ABoard::GlobalBoardManager->GetCurrentBoard());
	}
}

//Check if game conditions allow human player to move
bool AGameManager::CanPlay()
{
	if (bGameEnded)
	{
		return false;
	}
	if (ABoard::GlobalBoardManager->IsInPlayback())
	{
		return false;
	}

	return true;
}