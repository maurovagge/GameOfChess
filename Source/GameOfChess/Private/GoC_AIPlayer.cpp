#include "GoC_AIPlayer.h"
#include <limits>
#include "GameManager.h"
#include "EngineUtils.h"
#include "Board.h"



// Max Depth for Minimax search
constexpr int32 MaxDepth = 3;

AGoC_AIPlayer::AGoC_AIPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Team = ETeam::Black;
}

// Called when the game starts or when spawned
void AGoC_AIPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGoC_AIPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Minimax algorithm (to be called recursively to find best move)
int32 AGoC_AIPlayer::MiniMax(VirtualBoard board, int32 Depth, ETeam team)
{
	AGameManager* GameManager = GetGameManager();
	// Check if we have reached the maximum depth or if a game-over condition has been reached
	if (Depth == 0) 
	{
		// Evaluate the current board state and return the corresponding score
		return Evaluate(board, ETeam::Black);
	}

	int32 bestScore = 0;
	if (team == ETeam::Black)
	{
		bestScore = std::numeric_limits<int32>::min(); // Initialize the best score to a very low value 
	}
	else
	{
		bestScore = std::numeric_limits<int32>::max(); // Initialize the best score to a very high value
	}

	// Get all possible moves for the AI
	TArray<VirtualBoardMove> AllMoves = ABoard::GlobalBoardManager->GetAllMoves(board, team);
	for (VirtualBoardMove& move : AllMoves)
	{
		//simulate the move
		VirtualBoard boardAfterSimulatedMove = ABoard::GlobalBoardManager->Move(board, move);

		//go ahead with minimax by checking opponent team moves
		if (team == ETeam::Black)
		{
			team = ETeam::White;
		}
		else
		{
			team = ETeam::Black;
		}
		// Recursively call the minimax function to evaluate the next move
		int32 score = MiniMax(boardAfterSimulatedMove, Depth - 1, team);

		// Update the best score
		if (team == ETeam::Black)
		{
			bestScore = FMath::Max(bestScore, score);
		}
		else
		{
			bestScore = FMath::Min(bestScore, score);
		}
	}
	return bestScore; // Return the best score found
}



//Evaluation function for minimax algo 
//return a score based on available pieces on the board for each team
float AGoC_AIPlayer::Evaluate(VirtualBoard board, ETeam playerTeam)
{
	// Scores for each piece type
	const float PawnScore = 1.0f;
	const float RookScore = 5.0f;
	const float KnightScore = 3.0f;
	const float BishopScore = 3.0f;
	const float QueenScore = 9.0f;
	float checkScore = 0.0f;

	// Counters for each piece
	int32 WhitePawnCount = 0;
	int32 BlackPawnCount = 0;
	int32 WhiteRookCount = 0;
	int32 BlackRookCount = 0;
	int32 WhiteKnightCount = 0;
	int32 BlackKnightCount = 0;
	int32 WhiteBishopCount = 0;
	int32 BlackBishopCount = 0;
	int32 WhiteQueenCount = 0;
	int32 BlackQueenCount = 0;

	// Calculate score for each piece type
	for (int32 row = 0; row < NUMROWS; row++)
	{
		for (int32 col = 0; col < NUMCOLS; col++)
		{
			VirtualBoardEntry entry = board.GetBoardEntry(row, col);
			if (entry.Team == ETeam::White)
			{
				switch (entry.PieceType)
				{
				case EPieceType::Pawn:
					WhitePawnCount++;
					break;
				case EPieceType::Rook:
					WhiteRookCount++;
					break;
				case EPieceType::Knight:
					WhiteKnightCount++;
					break;
				case EPieceType::Bishop:
					WhiteBishopCount++;
					break;
				case EPieceType::Queen:
					WhiteQueenCount++;
					break;
				default:
					break;
				}
			}
			else if (entry.Team == ETeam::Black)
			{
				switch (entry.PieceType)
				{
				case EPieceType::Pawn:
					BlackPawnCount++;
					break;
				case EPieceType::Rook:
					BlackRookCount++;
					break;
				case EPieceType::Knight:
					BlackKnightCount++;
					break;
				case EPieceType::Bishop:
					BlackBishopCount++;
					break;
				case EPieceType::Queen:
					BlackQueenCount++;
					break;
				default:
					break;
				}
			}
		}
	}

	// give a big score if opponent is in check
	ETeam checkTeam = (playerTeam == ETeam::White) ? ETeam::Black : ETeam::White;
	if (ABoard::GlobalBoardManager->IsInCheck(board, checkTeam))
	{
		checkScore = 90.0f;
	}


	// Calculate total score based on the difference of pieces
	float score = 0.0f;
	if (playerTeam == ETeam::White)
	{
		score = (PawnScore * (WhitePawnCount - BlackPawnCount)) +
			(RookScore * (WhiteRookCount - BlackRookCount)) +
			(KnightScore * (WhiteKnightCount - BlackKnightCount)) +
			(BishopScore * (WhiteBishopCount - BlackBishopCount)) +
			(QueenScore * (WhiteQueenCount - BlackQueenCount)) +
			checkScore;
	}
	else if (playerTeam == ETeam::Black)
	{
		score = (PawnScore * (BlackPawnCount - WhitePawnCount)) +
			(RookScore * (BlackRookCount - WhiteRookCount)) +
			(KnightScore * (BlackKnightCount - WhiteKnightCount)) +
			(BishopScore * (BlackBishopCount - WhiteBishopCount)) +
			(QueenScore * (BlackQueenCount - WhiteQueenCount)) +
			checkScore;
	}

	return score;
}

//Move function for AI (MINIMAX)
void AGoC_AIPlayer::MakeAIMoveMinimax(ABoard* Board)
{
	AGameManager* GameManager = GetGameManager();
	if (GameManager->IsMyTurn(this))
	{
		int32 depth = MaxDepth;

		// Get all possible moves for the AI player
		TArray<VirtualBoardMove> AllMoves = ABoard::GlobalBoardManager->GetAllMoves(ABoard::GlobalBoardManager->GetCurrentBoard(), ETeam::Black);
		if (AllMoves.Num())
		{
			// Initialize variables to keep track of the best move and its score
			VirtualBoardMove bestMove;
			int32 bestScore = std::numeric_limits<int32>::min();

			//Iterate through all possible moves
			for (VirtualBoardMove& move : AllMoves)
			{
				//remove moves that would result in check condition
				if (ABoard::GlobalBoardManager->TestMoveForCheck(ABoard::GlobalBoardManager->GetCurrentBoard(), move, ETeam::Black))
				{
					continue;
				}
				//	Make the move
				VirtualBoard boardAfterSimulatedMove = ABoard::GlobalBoardManager->Move(ABoard::GlobalBoardManager->GetCurrentBoard(), move);

				// Evaluate the board after making the move
				int32 score = MiniMax(boardAfterSimulatedMove, depth, ETeam::White);

				// Check if the score of this move is better than the current best score
				if (score > bestScore)
				{
					bestScore = score;
					bestMove = move;
				}
			}
			// Make the best move
			VirtualBoard resultingBoard = ABoard::GlobalBoardManager->Move(ABoard::GlobalBoardManager->GetCurrentBoard(), bestMove);
			ABoard::GlobalBoardManager->SetCurrentBoard(resultingBoard);
			ABoard::GlobalBoardInstance->SpawnPieces(ABoard::GlobalBoardManager->GetCurrentBoard());
		}
	}
}

//Move function for AI (RANDOM)
void AGoC_AIPlayer::MakeAIMove(ABoard* Board)
{

	AGameManager* GameManager = GetGameManager();
	if (GameManager->IsMyTurn(this))
	{
		TArray<VirtualBoardMove> moves = ABoard::GlobalBoardManager->GetAllMoves(ABoard::GlobalBoardManager->GetCurrentBoard(), ETeam::Black);

		if (moves.IsEmpty())
		{
			//no more moves: game over 
			return;
		}
		int32 rand = 0;

		while (true)
		{
			if (moves.Num() > 0)
			{
				rand = FMath::Rand() % moves.Num();

				//remove moves that would result in check condition
				if (ABoard::GlobalBoardManager->TestMoveForCheck(ABoard::GlobalBoardManager->GetCurrentBoard(), moves[rand], ETeam::Black))
				{
					moves.RemoveAt(rand);
				}
				else
				{
					//move 
					VirtualBoard vb = ABoard::GlobalBoardManager->Move(ABoard::GlobalBoardManager->GetCurrentBoard(), moves[rand]);
					ABoard::GlobalBoardManager->SetCurrentBoard(vb);
					ABoard::GlobalBoardInstance->HighlightTiles(moves[rand], true, 1);
					ABoard::GlobalBoardInstance->SpawnPieces(ABoard::GlobalBoardManager->GetCurrentBoard());

					break;
				}
			}
			else
			{
				//no more moves: game over 
				return;
			}
		}
	}
}

//Get Game manager actor
AGameManager* AGoC_AIPlayer::GetGameManager()
{
	// Get the world
	UWorld* World = GetWorld();

	if (!World)
	{
		return nullptr;
	}

	// Iterate through all game managers in the world
	for (TActorIterator<AGameManager> It(World); It; ++It)
	{
		AGameManager* GameManager = *It;
		if (GameManager)
		{
			// Return the first found game manager
			return GameManager;
		}
	}

	// Print error message if game manager is not found
	return nullptr;
}



