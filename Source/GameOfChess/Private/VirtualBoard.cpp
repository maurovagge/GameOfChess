#include "VirtualBoard.h"


//default constructor
VirtualBoard::VirtualBoard()
{

}

//copy constructor
VirtualBoard::VirtualBoard(const VirtualBoard &vb)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            VirtualBoardEntry vbe = vb.GetBoardEntry(i, j);
            BoardEntries[i][j] = vbe;
        }
    }
    MoveNotation = vb.MoveNotation;
}

//assignment operator
void VirtualBoard::operator=(VirtualBoard& vb)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            BoardEntries[i][j] = vb.GetBoardEntry(i, j);
        }
    }
    MoveNotation = vb.MoveNotation;

}

//set an entry at a given position
void VirtualBoard::SetEntry(int row, int col, VirtualBoardEntry entry)
{
    BoardEntries[row][col] = entry;
}

VirtualBoardEntry VirtualBoard::GetBoardEntry(int row, int col) const
{
    return BoardEntries[row][col];
}

//empty an entry at a given position
void VirtualBoard::SetEmptyEntry(int row, int col)
{
    SetEntry(row, col, VirtualBoardEntry(EPieceType::NoPiece, ETeam::Black));
}

//Return the move notation string of the last move
FString VirtualBoard::GetBoardMoveNotation()
{
    return MoveNotation;
}

//Set the move notation string of the last move
void VirtualBoard::SetBoardMoveNotation(FString notation)
{
    MoveNotation = notation;
}


