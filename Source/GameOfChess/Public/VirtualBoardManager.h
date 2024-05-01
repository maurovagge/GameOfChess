#pragma once
#include "CoreMinimal.h"
#include "VirtualBoard.h"

#define MAXMOVE 7
#define NUMROWS 8
#define NUMCOLS 8


//Class with all the logic to manage moves and board history
class VirtualBoardManager
{

public:

	// Constructor
	VirtualBoardManager();

	//Init board manager object
	VirtualBoard Init();

	//set current board
	void SetCurrentBoard(VirtualBoard board);

	//get current board
	VirtualBoard GetCurrentBoard();

	//undo last move
	void UndoLastMove();

	//get move description of selectefd board
	FString GetCurrentMove();

	//move back along the board history
	VirtualBoard PlayBack();

	//move forward along the board history
	VirtualBoard PlayForward();

	//move at the beginning of the board history
	VirtualBoard PlayStart();

	//move at the end of the board history
	VirtualBoard PlayEnd();

	//get if thie board manager is in playback mode
	bool IsInPlayback();

	//init a new board
	VirtualBoard InitBoard();

	//virtual move pieces on a given board
	VirtualBoard Move(VirtualBoard board, int32 startRow, int32  startCol, int32 endRow, int32  endCol);

	//virtual move pieces on a given board
	VirtualBoard Move(VirtualBoard board, VirtualBoardMove move);

	//virtual move pieces on a given board
	VirtualBoard Move(VirtualBoard board, BoardPosition startPos, BoardPosition endPos);

	//check if a move is valid
	bool IsValidMove(VirtualBoard board, int32 startRow, int32  startCol, int32 endRow, int32  endCol);
	
	//get all valid moves froma given position
	TArray<VirtualBoardMove> GetValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol);

	//get all moves of a team
	TArray<VirtualBoardMove> GetAllMoves(VirtualBoard board, ETeam team);

	//Check "check" condition for a team
	bool IsInCheck(VirtualBoard board, ETeam team);

	//Check "checkmate" condition for a team
	bool IsCheckMate(VirtualBoard board, ETeam team);

	//Check "stalemate" condition for a team
	bool IsStaleMate(VirtualBoard board, ETeam team);

	//test if a move would result in a check condition
	bool TestMoveForCheck(VirtualBoard board, VirtualBoardMove move, ETeam team);

	//check if a pawn has to be promoted to queen (and do it)
	bool CheckForPawnToPromote(VirtualBoard& board);


protected:

	//History of all the boards/moves
	TArray<class VirtualBoard> BoardHistory;

	//current (active) board
	VirtualBoard CurrentBoard;

	//check if position is within the board
	bool IsValidPosition(int32 row, int32 col);

	//get valid moves for bishop
	TArray<VirtualBoardMove>  GetBishopValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol);

	//get valid moves for rook
	TArray<VirtualBoardMove>  GetRookValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol);

	//get valid moves for knight
	TArray<VirtualBoardMove>  GetKnightValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol);

	//get valid moves for pawn
	TArray<VirtualBoardMove>  GetPawnValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol);

	//get valid moves for queen
	TArray<VirtualBoardMove>  GetQueenValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol);

	//get valid moves for king
	TArray<VirtualBoardMove>  GetKingValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol);

	//get row col letter notation 
	FString ConverRowColToNotation(int32 row, int32 col);

	//get notation movename
	FString GetMoveName(EPieceType pieceType, int32 row, int32 col, bool isCapture, bool isCheck);

	//get letter code for a piece
	FString PieceTypeToString(EPieceType pt);

	//index of curren navigating board along the board history
	int32 PlaybackIndex;

};

