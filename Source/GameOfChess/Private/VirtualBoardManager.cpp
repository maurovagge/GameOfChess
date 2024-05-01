#include "VirtualBoardManager.h"


VirtualBoardManager::VirtualBoardManager()
{
	PlaybackIndex = -1;
}


VirtualBoard VirtualBoardManager::Init()
{
	BoardHistory.Empty();
	PlaybackIndex = -1;
	VirtualBoard vb = InitBoard();
	SetCurrentBoard(vb);
	return vb;
}

void VirtualBoardManager::SetCurrentBoard(VirtualBoard board)
{
	CurrentBoard = board;
	BoardHistory.Add(CurrentBoard);
	PlaybackIndex++;
}

void VirtualBoardManager::UndoLastMove()
{
	//reset playback condition
	PlayEnd();
	if (BoardHistory.Num() > 2)
	{
		BoardHistory.RemoveAt(BoardHistory.Num() - 1);
		BoardHistory.RemoveAt(BoardHistory.Num() - 1);
		CurrentBoard = BoardHistory[BoardHistory.Num() - 1];
		PlaybackIndex -= 2;
	}
}

FString VirtualBoardManager::GetCurrentMove()
{
	//format string of current move
	if (BoardHistory.Num())
	{
		if (PlaybackIndex > 0)
		{
			return FString::Printf(TEXT("%d.%s  [%d/%d]"), PlaybackIndex, *(BoardHistory[PlaybackIndex].GetBoardMoveNotation()), PlaybackIndex, BoardHistory.Num() - 1);
		}
	}
	return _T("");
}


VirtualBoard VirtualBoardManager::GetCurrentBoard()
{
	return CurrentBoard;
}


VirtualBoard VirtualBoardManager::PlayBack()
{
	if (PlaybackIndex > 0)
	{
		PlaybackIndex--;
	}
	return BoardHistory[PlaybackIndex];
}

VirtualBoard VirtualBoardManager::PlayForward()
{
	if (PlaybackIndex < BoardHistory.Num() - 1)
	{
		PlaybackIndex++;
	}
	return BoardHistory[PlaybackIndex];
}


VirtualBoard VirtualBoardManager::PlayStart()
{
	if (PlaybackIndex >= 0)
	{
		PlaybackIndex = 0;
	}
	return BoardHistory[PlaybackIndex];
}

VirtualBoard VirtualBoardManager::PlayEnd()
{
	if (PlaybackIndex >= 0)
	{
		PlaybackIndex = BoardHistory.Num() - 1;
	}
	return BoardHistory[PlaybackIndex];
}

bool VirtualBoardManager::IsInPlayback()
{
	return (PlaybackIndex < BoardHistory.Num() - 1);
}

VirtualBoard VirtualBoardManager::InitBoard()
{
	//setup board with all pieces
	VirtualBoard board;
	board.SetEntry(0, 0, VirtualBoardEntry(EPieceType::Rook, ETeam::Black));
	board.SetEntry(0, 1, VirtualBoardEntry(EPieceType::Knight, ETeam::Black));
	board.SetEntry(0, 2, VirtualBoardEntry(EPieceType::Bishop, ETeam::Black));
	board.SetEntry(0, 3, VirtualBoardEntry(EPieceType::Queen, ETeam::Black));
	board.SetEntry(0, 4, VirtualBoardEntry(EPieceType::King, ETeam::Black));
	board.SetEntry(0, 5, VirtualBoardEntry(EPieceType::Bishop, ETeam::Black));
	board.SetEntry(0, 6, VirtualBoardEntry(EPieceType::Knight, ETeam::Black));
	board.SetEntry(0, 7, VirtualBoardEntry(EPieceType::Rook, ETeam::Black));
	board.SetEntry(1, 0, VirtualBoardEntry(EPieceType::Pawn, ETeam::Black));
	board.SetEntry(1, 1, VirtualBoardEntry(EPieceType::Pawn, ETeam::Black));
	board.SetEntry(1, 2, VirtualBoardEntry(EPieceType::Pawn, ETeam::Black));
	board.SetEntry(1, 3, VirtualBoardEntry(EPieceType::Pawn, ETeam::Black));
	board.SetEntry(1, 4, VirtualBoardEntry(EPieceType::Pawn, ETeam::Black));
	board.SetEntry(1, 5, VirtualBoardEntry(EPieceType::Pawn, ETeam::Black));
	board.SetEntry(1, 6, VirtualBoardEntry(EPieceType::Pawn, ETeam::Black));
	board.SetEntry(1, 7, VirtualBoardEntry(EPieceType::Pawn, ETeam::Black));

	board.SetEntry(6, 0, VirtualBoardEntry(EPieceType::Pawn, ETeam::White));
	board.SetEntry(6, 1, VirtualBoardEntry(EPieceType::Pawn, ETeam::White));
	board.SetEntry(6, 2, VirtualBoardEntry(EPieceType::Pawn, ETeam::White));
	board.SetEntry(6, 3, VirtualBoardEntry(EPieceType::Pawn, ETeam::White));
	board.SetEntry(6, 4, VirtualBoardEntry(EPieceType::Pawn, ETeam::White));
	board.SetEntry(6, 5, VirtualBoardEntry(EPieceType::Pawn, ETeam::White));
	board.SetEntry(6, 6, VirtualBoardEntry(EPieceType::Pawn, ETeam::White));
	board.SetEntry(6, 7, VirtualBoardEntry(EPieceType::Pawn, ETeam::White));
	board.SetEntry(7, 0, VirtualBoardEntry(EPieceType::Rook, ETeam::White));
	board.SetEntry(7, 1, VirtualBoardEntry(EPieceType::Knight, ETeam::White));
	board.SetEntry(7, 2, VirtualBoardEntry(EPieceType::Bishop, ETeam::White));
	board.SetEntry(7, 3, VirtualBoardEntry(EPieceType::Queen, ETeam::White));
	board.SetEntry(7, 4, VirtualBoardEntry(EPieceType::King, ETeam::White));
	board.SetEntry(7, 5, VirtualBoardEntry(EPieceType::Bishop, ETeam::White));
	board.SetEntry(7, 6, VirtualBoardEntry(EPieceType::Knight, ETeam::White));
	board.SetEntry(7, 7, VirtualBoardEntry(EPieceType::Rook, ETeam::White));
	return board;
}

VirtualBoard VirtualBoardManager::Move(VirtualBoard board, int32 startRow, int32  startCol, int32 endRow, int32  endCol)
{
	//prepare a new board with result of the movee
	VirtualBoard newBoard(board);
	VirtualBoardEntry entryToMove = newBoard.GetBoardEntry(startRow, startCol);
	VirtualBoardEntry entryToGo = newBoard.GetBoardEntry(endRow, endCol);

	//do the move
	newBoard.SetEntry(endRow, endCol, entryToMove);
	newBoard.SetEmptyEntry(startRow, startCol);

	bool bCheck = false;
	if (IsInCheck(newBoard, (entryToMove.Team == ETeam::Black) ? ETeam::White : ETeam::Black))
	{
		bCheck = true;
	}

	//store move notation 
	newBoard.SetBoardMoveNotation(GetMoveName(entryToMove.PieceType, endRow, endCol, (entryToGo.PieceType != EPieceType::NoPiece), bCheck));
	
	//return new board
	return newBoard;
}

bool VirtualBoardManager::IsValidMove(VirtualBoard board, int32 startRow, int32  startCol, int32 endRow, int32  endCol)
{
	//check if proposed move is in the valid (allowed)
	TArray<VirtualBoardMove> moves = GetValidMovesFrom(board, startRow, startCol);
	for (VirtualBoardMove move : moves)
	{
		if (move.EndRow() == endRow && move.EndCol() == endCol)
		{
			return true;
		}
	}
	return false;
}

VirtualBoard VirtualBoardManager::Move(VirtualBoard board, VirtualBoardMove move)
{
	return Move(board, move.StartRow(), move.StartCol(), move.EndRow(), move.EndCol());
}

VirtualBoard VirtualBoardManager::Move(VirtualBoard board, BoardPosition startPos, BoardPosition endPos)
{
	return Move(board, startPos.Row, startPos.Col, endPos.Row, endPos.Col);
}

TArray<VirtualBoardMove> VirtualBoardManager::GetValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol)
{
	TArray<VirtualBoardMove> moves;

	VirtualBoardEntry entry = board.GetBoardEntry(fromRow, fromCol);

	if (entry.PieceType == EPieceType::NoPiece)
	{
		//no moves (no piece)
		return moves;
	}
	switch (entry.PieceType)
	{
	case EPieceType::Bishop:
		moves.Append(GetBishopValidMovesFrom(board, fromRow, fromCol));
		break;
	case EPieceType::Rook:
		moves.Append(GetRookValidMovesFrom(board, fromRow, fromCol));
		break;
	case EPieceType::Knight:
		moves.Append(GetKnightValidMovesFrom(board, fromRow, fromCol));
		break;
	case EPieceType::Pawn:
		moves.Append(GetPawnValidMovesFrom(board, fromRow, fromCol));
		break;
	case EPieceType::King:
		moves.Append(GetKingValidMovesFrom(board, fromRow, fromCol));
		break;
	case EPieceType::Queen:
		moves.Append(GetQueenValidMovesFrom(board, fromRow, fromCol));
		break;
	default:
		break;

	}
	return moves;
}

TArray<VirtualBoardMove> VirtualBoardManager::GetAllMoves(VirtualBoard board, ETeam team)
{
	TArray<VirtualBoardMove> moves;
	for (int row = 0; row < NUMROWS; row++)
	{
		for (int col = 0; col < NUMCOLS; col++)
		{
			VirtualBoardEntry entry = board.GetBoardEntry(row, col);
			if (entry.Team == team)
			{
				moves.Append(GetValidMovesFrom(board, row, col));
			}
		}
	}
	return moves;
}

bool VirtualBoardManager::IsInCheck(VirtualBoard board, ETeam team)
{
	//get all enemy moves and test if one of that is threathening the king
	TArray<VirtualBoardMove> enemyMoves = GetAllMoves(board, (team == ETeam::Black) ? ETeam::White : ETeam::Black);
	for (int row = 0; row < NUMROWS; row++)
	{
		for (int col = 0; col < NUMCOLS; col++)
		{
			VirtualBoardEntry entry = board.GetBoardEntry(row, col);
			if ((entry.Team == team) && (entry.PieceType == EPieceType::King))
			{
				for (VirtualBoardMove move : enemyMoves)
				{
					if ((move.EndRow() == row) && (move.EndCol() == col))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool VirtualBoardManager::IsCheckMate(VirtualBoard board, ETeam team)
{	
	if (!IsInCheck(board, team))
	{
		return false;
	}

	//get all enemy and test if at least one of that would put king in a safe condition 
	TArray<VirtualBoardMove> moves = GetAllMoves(board, team);
	for (VirtualBoardMove move : moves)
	{
		if (!TestMoveForCheck(board, move, team))
		{
			return false;
		}
	}
	return true;
}

bool VirtualBoardManager::IsStaleMate(VirtualBoard board, ETeam team)
{
	if (IsInCheck(board, team))
	{
		return false;
	}

	//if king is not under check but all the moves would result in check condition...
	TArray<VirtualBoardMove> moves = GetAllMoves(board, team);
	for (VirtualBoardMove move : moves)
	{
		if (!TestMoveForCheck(board, move, team))
		{
			return false;
		}
	}
	return true;
}

bool VirtualBoardManager::TestMoveForCheck(VirtualBoard board, VirtualBoardMove move, ETeam team)
{
	VirtualBoard boardAfterMove = Move(board, move);
	if (IsInCheck(boardAfterMove, team))
	{
		return true;
	}
	return false;
}

bool VirtualBoardManager::CheckForPawnToPromote(VirtualBoard& board)
{
	//if pawn has become the end, promote it to queen
	for (int32 row : {0, 7})
	{
		for (int32 col = 0; col < NUMCOLS; col++)
		{
			VirtualBoardEntry entry = board.GetBoardEntry(row, col);
			if (entry.PieceType == EPieceType::Pawn)
			{
				if ((row == 0) && (entry.Team == ETeam::White))
				{
					entry.PieceType = EPieceType::Queen;
					board.SetEntry(row, col, entry);
					return true;
				}
				else if ((row == 7) && (entry.Team == ETeam::Black))
				{
					entry.PieceType = EPieceType::Queen;
					board.SetEntry(row, col, entry);
					return true;
				}
			}
		}
	}
	return false;
}


bool VirtualBoardManager::IsValidPosition(int32 row, int32 col)
{
	if (row < 0) return false;
	if (row > 7) return false;
	if (col < 0) return false;
	if (col > 7) return false;
	return true;
}

TArray<VirtualBoardMove>  VirtualBoardManager::GetBishopValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol)
{
	TArray<VirtualBoardMove> moves;

	VirtualBoardEntry entry = board.GetBoardEntry(fromRow, fromCol);
	VirtualBoardEntry destEntry;

	int32 destRow;
	int32 destCol;
	// Iterate through all diagonal directions
	for (int32 rowDirection : {1, -1})
	{
		for (int32 colDirection : {1, -1})
		{
			for (int32 step = 1; step <= MAXMOVE; ++step)
			{
				destRow = fromRow + step * rowDirection;
				destCol = fromCol + step * colDirection;

				if (!IsValidPosition(destRow, destCol))
				{
					continue;
				}

				destEntry = board.GetBoardEntry(destRow, destCol);
				if (destEntry.PieceType == EPieceType::NoPiece)
				{
					//empty entry, I can move there
					moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
				}
				else if (destEntry.Team != entry.Team)
				{
					//enemy piece,I can move there (by capturing it)
					moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
					break; //I cannot go ahead after an occupied tile
				}
				else
				{
					//friend piece found
					break;
				}
			}
		}
	}
	return moves;
}
TArray<VirtualBoardMove>  VirtualBoardManager::GetRookValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol)
{
	TArray<VirtualBoardMove> moves;

	VirtualBoardEntry entry = board.GetBoardEntry(fromRow, fromCol);
	VirtualBoardEntry destEntry;
	// Vertical-up movements
	for (int32 step = 1; step <= MAXMOVE; ++step)
	{
		int32 destRow = fromRow + step;
		int32 destCol = fromCol;

		if (!IsValidPosition(destRow, destCol))
		{
			continue;
		}

		destEntry = board.GetBoardEntry(destRow, destCol);
		if (destEntry.PieceType == EPieceType::NoPiece)
		{
			//empty entry, I can move there
			moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
		}
		else if (destEntry.Team != entry.Team)
		{
			//enemy piece,I can move there (by capturing it)
			moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
			break; //I cannot go ahead after an occupied tile
		}
		else
		{
			//friend piece found
			break;
		}
	}

	// Vertical-down movements
	for (int32 step = 1; step <= MAXMOVE; ++step)
	{
		int32 destRow = fromRow - step;
		int32 destCol = fromCol;

		if (!IsValidPosition(destRow, destCol))
		{
			continue;
		}

		destEntry = board.GetBoardEntry(destRow, destCol);
		if (destEntry.PieceType == EPieceType::NoPiece)
		{
			//empty entry, I can move there
			moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
		}
		else if (destEntry.Team != entry.Team)
		{
			//enemy piece,I can move there (by capturing it)
			moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
			break; //I cannot go ahead after an occupied tile
		}
		else
		{
			//friend piece found
			break;
		}
	}

	// Horizontal-up movements
	for (int32 step = 1; step <= MAXMOVE; ++step)
	{
		int32 destRow = fromRow;
		int32 destCol = fromCol + step;

		if (!IsValidPosition(destRow, destCol))
		{
			continue;
		}

		destEntry = board.GetBoardEntry(destRow, destCol);
		if (destEntry.PieceType == EPieceType::NoPiece)
		{
			//empty entry, I can move there
			moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
		}
		else if (destEntry.Team != entry.Team)
		{
			//enemy piece,I can move there (by capturing it)
			moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
			break; //I cannot go ahead after an occupied tile
		}
		else
		{
			//friend piece found
			break;
		}
	}

	// Horizontal-down movements
	for (int32 step = 1; step <= MAXMOVE; ++step)
	{
		int32 destRow = fromRow;
		int32 destCol = fromCol - step;

		if (!IsValidPosition(destRow, destCol))
		{
			continue;
		}

		destEntry = board.GetBoardEntry(destRow, destCol);
		if (destEntry.PieceType == EPieceType::NoPiece)
		{
			//empty entry, I can move there
			moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
		}
		else if (destEntry.Team != entry.Team)
		{
			//enemy piece,I can move there (by capturing it)
			moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
			break; //I cannot go ahead after an occupied tile
		}
		else
		{
			//friend piece found
			break;
		}
	}
	return moves;
}

TArray<VirtualBoardMove>  VirtualBoardManager::GetKnightValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol)
{
	TArray<VirtualBoardMove> moves;

	VirtualBoardEntry entry = board.GetBoardEntry(fromRow, fromCol);
	VirtualBoardEntry destEntry;

	int32 moveOffset[8][2] =
	{
		{1, 2},
		{-1, 2},
		{1, -2},
		{-1, -2},
		{2, 1},
		{-2, 1},
		{2, -1},
		{-2, -1}
	};

	for (int i = 0; i < 8; i++)
	{
		int32 destRow = fromRow + moveOffset[i][0];
		int32 destCol = fromCol + moveOffset[i][1];

		if (!IsValidPosition(destRow, destCol))
		{
			continue;
		}

		destEntry = board.GetBoardEntry(destRow, destCol);
		if (destEntry.PieceType == EPieceType::NoPiece)
		{
			//empty entry, I can move there
			moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
		}
		else if (destEntry.Team != entry.Team)
		{
			//enemy piece,I can move there (by capturing it)
			moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
		}
	}
	return moves;
}
TArray<VirtualBoardMove>  VirtualBoardManager::GetPawnValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol)
{
	TArray<VirtualBoardMove> moves;
	VirtualBoardEntry entry = board.GetBoardEntry(fromRow, fromCol);


	// Calculate the direction of movement based on the team 
	int32 forwardStep = (entry.Team == ETeam::White) ? -1 : 1;

	int32 destRow = fromRow + forwardStep;
	int32 destCol = fromCol;

	VirtualBoardEntry destEntry;
	// Check the one step forward
	if (IsValidPosition(destRow, destCol))
	{
		destEntry = board.GetBoardEntry(destRow, destCol);
		if (destEntry.PieceType == EPieceType::NoPiece)
		{
			//empty entry, I can move there
			moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));

			//now I check for double step (valid only at starting position)
			if ((fromRow == 1 && entry.Team == ETeam::Black) ||
				(fromRow == 6 && entry.Team == ETeam::White))
			{
				destRow = fromRow + 2 * forwardStep;
				destCol = fromCol;
				destEntry = board.GetBoardEntry(destRow, destCol);
				if (destEntry.PieceType == EPieceType::NoPiece)
				{
					//empty entry, I can move there
					moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
				}
			}
		}
	}
	// Check diagonal step for capturing
	for (int32 colStep = -1; colStep <= 1; colStep += 2)
	{
		destRow = fromRow + forwardStep;
		destCol = fromCol + colStep;

		if (IsValidPosition(destRow, destCol))
		{
			destEntry = board.GetBoardEntry(destRow, destCol);
			if (destEntry.PieceType != EPieceType::NoPiece)
			{
				if (destEntry.Team != entry.Team)
				{
					//enemy piece,I can move there (by capturing it)
					moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
				}
			}
		}
	}
	return moves;
}
TArray<VirtualBoardMove>  VirtualBoardManager::GetQueenValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol)
{
	TArray<VirtualBoardMove> moves;

	VirtualBoardEntry entry = board.GetBoardEntry(fromRow, fromCol);
	VirtualBoardEntry destEntry;

	int32 destRow;
	int32 destCol;
	// Iterate through all directions
	for (int32 rowDirection : {1, 0, -1})
	{
		for (int32 colDirection : {1, 0, -1})
		{
			if (rowDirection == 0 && colDirection == 0)
			{
				//not a real move
				continue;
			}

			for (int32 step = 1; step <= MAXMOVE; ++step)
			{
				destRow = fromRow + (step * rowDirection);
				destCol = fromCol + (step * colDirection);

				if (!IsValidPosition(destRow, destCol))
				{
					continue;
				}

				destEntry = board.GetBoardEntry(destRow, destCol);
				if (destEntry.PieceType == EPieceType::NoPiece)
				{
					//empty entry, I can move there
					moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
				}
				else if (destEntry.Team != entry.Team)
				{
					//enemy piece,I can move there (by capturing it)
					moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
					break; //I cannot go ahead after an occupied tile
				}
				else
				{
					//friend piece found
					break;
				}
			}
		}
	}
	return moves;
}
TArray<VirtualBoardMove>  VirtualBoardManager::GetKingValidMovesFrom(VirtualBoard board, int32 fromRow, int32 fromCol)
{
	TArray<VirtualBoardMove> moves;

	VirtualBoardEntry entry = board.GetBoardEntry(fromRow, fromCol);
	VirtualBoardEntry destEntry;

	int32 destRow;
	int32 destCol;
	// Iterate through all directions
	for (int32 rowStep : {1, 0, -1})
	{
		for (int32 colStep : {1, 0, -1})
		{
			if (rowStep == 0 && colStep == 0)
			{
				//not a real move
				continue;
			}

			destRow = fromRow + rowStep;
			destCol = fromCol + colStep;

			if (!IsValidPosition(destRow, destCol))
			{
				continue;
			}

			destEntry = board.GetBoardEntry(destRow, destCol);
			if (destEntry.PieceType == EPieceType::NoPiece)
			{
				//empty entry, I can move there
				moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
			}
			else if (destEntry.Team != entry.Team)
			{
				//enemy piece,I can move there (by capturing it)
				moves.Add(VirtualBoardMove(fromRow, fromCol, destRow, destCol));
			}
		}
	}
	return moves;
}

FString VirtualBoardManager::ConverRowColToNotation(int32 row, int32 col)
{
	int notationRow = 0;
	FString notationCol = "";
	switch (row)
	{
	case 0:
		notationRow = 8;
		break;
	case 1:
		notationRow = 7;
		break;
	case 2:
		notationRow = 6;
		break;
	case 3:
		notationRow = 5;
		break;
	case 4:
		notationRow = 4;
		break;
	case 5:
		notationRow = 3;
		break;
	case 6:
		notationRow = 2;
		break;
	case 7:
		notationRow = 1;
		break;
	default:
		break;
	}

	switch (col)
	{
	case 0:
		notationCol = _T("a");
		break;
	case 1:
		notationCol = _T("b");
		break;
	case 2:
		notationCol = _T("c");
		break;
	case 3:
		notationCol = _T("d");
		break;
	case 4:
		notationCol = _T("e");
		break;
	case 5:
		notationCol = _T("f");
		break;
	case 6:
		notationCol = _T("g");
		break;
	case 7:
		notationCol = _T("h");
		break;
	default:
		break;
	}
	return FString::Printf(TEXT("%d%s"), notationRow, *notationCol);

}

FString VirtualBoardManager::GetMoveName(EPieceType pieceType, int32 row, int32 col, bool isCapture, bool isCheck)
{
	if (isCapture)
	{
		return FString::Printf(TEXT("%sx%s%s"), *PieceTypeToString(pieceType), *ConverRowColToNotation(row, col), isCheck ? TEXT("+") : TEXT(""));
	}
	else
	{
		return FString::Printf(TEXT("%s%s%s"), *PieceTypeToString(pieceType), *ConverRowColToNotation(row, col), isCheck ? TEXT("+") : TEXT(""));
	}

}



FString VirtualBoardManager::PieceTypeToString(EPieceType pt)
{
	switch (pt)
	{
	case EPieceType::King:
		return "K";
	case EPieceType::Queen:
		return "Q";
	case EPieceType::Knight:
		return "N";
	case EPieceType::Pawn:
		return "P";
	case EPieceType::Bishop:
		return "B";
	case EPieceType::Rook:
		return "R";
	case EPieceType::NoPiece:
		return "-";
	}
	return "-";
}



