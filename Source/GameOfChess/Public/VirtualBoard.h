#pragma once
#include "Piece.h"


//board position (row, col)
struct BoardPosition
{
    int32 Row;
    int32 Col;
    BoardPosition() { Row = 0; Col = 0; }
    BoardPosition(int row, int col) : Row(row), Col(col) {}

};

//board entry  (piece type and belonging team)
struct VirtualBoardEntry
{
    EPieceType PieceType;
    ETeam Team;

    VirtualBoardEntry()
    {
        PieceType = EPieceType::NoPiece;
    }
    VirtualBoardEntry(EPieceType type, ETeam team) : PieceType(type), Team(team){}
};

//board move (start pos and end pos)
struct VirtualBoardMove
{
    BoardPosition StartPos;
    BoardPosition EndPos;
    VirtualBoardMove(){}
    VirtualBoardMove(BoardPosition startPos, BoardPosition endPos) : StartPos(startPos), EndPos(endPos) {}
    VirtualBoardMove(int startRow, int startCol, int endRow, int endCol) : StartPos(startRow, startCol), EndPos(endRow, endCol) {}
    int32 StartRow() { return StartPos.Row; }
    int32 StartCol() { return StartPos.Col; }
    int32 EndRow() { return EndPos.Row; }
    int32 EndCol() { return EndPos.Col; }
};

class VirtualBoard 
{
public:

    // Constructor
    VirtualBoard();
    VirtualBoard(const VirtualBoard& vb);
    void operator=(VirtualBoard& vb);

    //set an entry at a given position
    void SetEntry(int row, int col, VirtualBoardEntry entry);

    //get an entry at a given position
    VirtualBoardEntry GetBoardEntry(int row, int col) const;

    //erase an entry
    void SetEmptyEntry(int row, int col);

    //get move notation of current board 
    FString GetBoardMoveNotation();
    void SetBoardMoveNotation(FString notation);

protected:
    VirtualBoardEntry BoardEntries[8][8];
    FString MoveNotation;
};

