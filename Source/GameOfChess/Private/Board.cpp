#include "Board.h"
#include "Piece.h"
#include "GoC_AIPlayer.h"



// Forward declaration of APiece class
class APiece;

//Initialization of Static Aboard*
ABoard* ABoard::GlobalBoardInstance = nullptr;
VirtualBoardManager* ABoard::GlobalBoardManager = nullptr;

ABoard::ABoard()
{
	TileClass = ATile::StaticClass();

	// Set default grid size
	GridSizeX = 8;
	GridSizeY = 8;
	BoardSize = 8;

	TileSize = 100.0f;
}






void ABoard::SpawnPieces(VirtualBoard vb)
{
	ResetBoard();
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			VirtualBoardEntry vbe = vb.GetBoardEntry(row, col);

			// Calculate tile location
			FVector PieceLocation = FVector(col * 100.0f, row * 100.0f, 105.0f);
			bool isWhite = (vbe.Team == ETeam::White);
			// Create the piece actor
			APiece* NewPiece = nullptr;
			switch (vbe.PieceType)
			{
			case EPieceType::Bishop:
				NewPiece = GetWorld()->SpawnActor<APiece_Bishop>(isWhite ? PieceBishopActorClass_W : PieceBishopActorClass_B, PieceLocation, FRotator::ZeroRotator);
				break;
			case EPieceType::Rook:
				NewPiece = GetWorld()->SpawnActor<APiece_Rook>(isWhite ? PieceRookActorClass_W : PieceRookActorClass_B, PieceLocation, FRotator::ZeroRotator);
				break;
			case EPieceType::Knight:
				NewPiece = GetWorld()->SpawnActor<APiece_Knight>(isWhite ? PieceKnightActorClass_W : PieceKnightActorClass_B, PieceLocation, FRotator::ZeroRotator);
				break;
			case EPieceType::Pawn:
				NewPiece = GetWorld()->SpawnActor<APiece_Pawn>(isWhite ? PiecePawnActorClass_W : PiecePawnActorClass_B, PieceLocation, FRotator::ZeroRotator);
				break;
			case EPieceType::King:
				NewPiece = GetWorld()->SpawnActor<APiece_King>(isWhite ? PieceKingActorClass_W : PieceKingActorClass_B, PieceLocation, FRotator::ZeroRotator);
				break;
			case EPieceType::Queen:
				NewPiece = GetWorld()->SpawnActor<APiece_Queen>(isWhite ? PieceQueenActorClass_W : PieceQueenActorClass_B, PieceLocation, FRotator::ZeroRotator);
				break;
			default:
				break;

			}



			if (NewPiece)
			{
				// Set the piece type and team
				NewPiece->SetPieceType(vbe.PieceType);
				NewPiece->SetTeam(vbe.Team);

				NewPiece->CurrentRow = row;
				NewPiece->CurrentCol = col;

				// Set the piece position on the board
				ATile* Tile = ABoard::GlobalBoardInstance->GetTileAtCoordinates(row, col);
				if (Tile)
				{
					Tile->SetOccupyingPiece(NewPiece);
					NewPiece->SetCurrentTile(Tile);
					Pieces.Add(NewPiece);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("GameManager: Unable to find tile at coordinates (%d, %d)."), row, col);
					NewPiece->Destroy(); // Destroy the piece if the tile is not found
				}
			}
			else
			{
				if (vbe.PieceType != EPieceType::NoPiece)
				{
					UE_LOG(LogTemp, Error, TEXT("GameManager: Failed to spawn piece actor."));
				}
			}
		}
	}
}

void ABoard::Move(VirtualBoardMove vbm)
{
	ATile* startTile = GetTileAtCoordinates(vbm.StartRow(), vbm.StartCol());
	if (startTile != nullptr)
	{
		APiece* piece = startTile->GetOccupyingPiece();
		if (piece != nullptr)
		{
			ATile* endTile = GetTileAtCoordinates(vbm.EndRow(), vbm.EndCol());

			piece->SetActorLocation(endTile->GetTileLocation() + FVector(0.0f, 0.0f, 105.0f));
		}
	}
}

void ABoard::BeginPlay()
{
	Super::BeginPlay();
}

//Setup the chessboard
void ABoard::SetupBoard()
{
	bool bIsBlackTile = false;

	// Create the chessboard tiles
	for (int32 Y = 0; Y < GridSizeY; ++Y)
	{
		for (int32 X = 0; X < GridSizeX; ++X)
		{
			// Calculate tile location
			FVector TileLocation = FVector(X * 100.0f, Y * 100.0f, 0.0f);

			// Spawn a new tile
			ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass, TileLocation, FRotator::ZeroRotator);

			// Set tile material based on color
			if (bIsBlackTile)
			{
				NewTile->SetTileMaterial(NewTile->BlackMaterial);
				NewTile->DefaultBlack = true;
			}
			else
			{
				NewTile->SetTileMaterial(NewTile->WhiteMaterial);
				NewTile->DefaultBlack = false;
			}

			// Set row and column indices
			NewTile->RowIndex = Y; // Assuming Y is the row index
			NewTile->ColumnIndex = X; // Assuming X is the column index

			bIsBlackTile = !bIsBlackTile; // Toggle between black and white tiles
			// Add the tile to the array
			Tiles.Add(NewTile);
		}

		// Toggle between black and white for each row
		bIsBlackTile = !bIsBlackTile;
	}

	//setup global instance of object used all over the project
	GlobalBoardInstance = this;
	GlobalBoardManager = new VirtualBoardManager();
}


void ABoard::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

ATile* ABoard::GetTileAtLocation(const FVector& Location)
{
	// Calculate row and column indices based on the given location and tile size
	int32 RowIndex = FMath::FloorToInt(Location.Y / TileSize);
	int32 ColumnIndex = FMath::FloorToInt(Location.X / TileSize);

	// Calculate the tile index based on row and column indices
	int32 TileIndex = (GridSizeY * RowIndex) + ColumnIndex;

	// Check if the tile index is valid within the Tiles array
	if (Tiles.IsValidIndex(TileIndex))
	{
		return Tiles[TileIndex];
	}

	return nullptr; // Return nullptr if tile index is not valid
}

APiece* ABoard::GetPieceAtLocation(int32 Row, int32 Col)
{
	// Check if the specified position is valid
	if (IsValidTile(Row, Col))
	{
		// Get the reference to the tile corresponding to the specified position
		ATile* Tile = Tiles[Row * GridSizeY + Col];  // Assuming Tiles is an array of pointers to ATile*

		// Check if the tile is valid and if it has a piece
		if (Tile && Tile->IsOccupied())
		{
			// Return the piece on the tile
			return Tile->GetOccupyingPiece();
		}
	}

	// If the position is not valid or there is no piece, return nullptr
	return nullptr;
}


bool ABoard::IsValidTile(int32 Row, int32 Col)
{
	// Verifica se Row e Col sono all'interno dei limiti della scacchiera
	return Row >= 0 && Row < GridSizeX && Col >= 0 && Col < GridSizeY;
}

TArray<class ATile*> ABoard::GetAllTiles()
{
	return Tiles;
}
void ABoard::SetKingUnderCheck(ETeam team, bool bCheck)
{
	for (auto& Tile : Tiles)
	{
		Tile->HighlightTile(false);
		APiece* piece = Tile->GetOccupyingPiece();
		if (piece != nullptr)
		{
			if (piece->GetPieceType() == EPieceType::King)
			{
				if (piece->GetTeam() == team)
				{
					Tile->SetTileUnderCheck(bCheck);
				}
			}
		}
	}
}

void ABoard::ResetBoard()
{
	// Iterate through all tiles on the board
	for (auto& Tile : Tiles)
	{
		// Reset each tile to its initial state
		Tile->ResetTile();
	}

}


ATile* ABoard::GetTileAtCoordinates(int32 Row, int32 Column)
{
	// Check if the coordinates are valid
	if (Row < 0 || Row >= GridSizeX || Column < 0 || Column >= GridSizeY)
	{
		UE_LOG(LogTemp, Error, TEXT("Board: Invalid coordinates (%d, %d)."), Row, Column);
		return nullptr;
	}

	// Calculate the index of the tile in the two-dimensional Tiles array
	int32 TileIndex = Row * GridSizeY + Column;

	// Check if the index is valid
	if (TileIndex >= 0 && TileIndex < Tiles.Num())
	{
		return Tiles[TileIndex];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Board: Invalid tile index for coordinates (%d, %d)."), Row, Column);
		return nullptr;
	}
}

void ABoard::HighlightTiles(TArray<BoardPosition> positions, bool highlight, int32 highLightDuration)
{
	TArray<ATile*> tiles;
	for (BoardPosition pos : positions)
	{
		ATile* t = GetTileAtCoordinates(pos.Row, pos.Col);
		tiles.Add(t);
	}
	HighlightTiles(tiles, highlight, highLightDuration);
}
void ABoard::HighlightTile(BoardPosition position, bool highlight, int32 highLightDuration)
{
	TArray<BoardPosition> positions;
	positions.Add(position);
	HighlightTiles(positions, highlight, highLightDuration);
}

void ABoard::HighlightTiles(TArray<VirtualBoardMove> moves, bool highlight, int32 highLightDuration)
{
	TArray<ATile*> tiles;
	for (VirtualBoardMove move : moves)
	{
		ATile* tStart = GetTileAtCoordinates(move.StartRow(), move.StartCol());
		ATile* tEnd = GetTileAtCoordinates(move.EndRow(), move.EndCol());
		tiles.Add(tStart);
		tiles.Add(tEnd);
	}
	HighlightTiles(tiles, highlight, highLightDuration);
}

void ABoard::HighlightTiles(VirtualBoardMove move, bool highlight, int32 highLightDuration)
{
	TArray<VirtualBoardMove> moves;
	moves.Add(move);
	HighlightTiles(moves, highlight, highLightDuration);
}

void ABoard::HighlightTile(ATile* tile, bool highlight, int32 highLightDuration)
{
	TArray<ATile*> tiles;
	tiles.Add(tile);
	HighlightTiles(tiles, highlight, highLightDuration);
}

void ABoard::HighlightTiles(TArray<ATile*> tiles, bool highlight, int32 highLightDuration)
{

	TmpHighlightedPos.Empty();
	for (ATile* t : tiles)
	{
		t->HighlightTile(highlight);

		if ((highlight) && (highLightDuration > 0))
		{
			TmpHighlightedPos.Add(BoardPosition(t->RowIndex, t->ColumnIndex));
		}
	}

	if (highLightDuration > 0)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
			{
				for (BoardPosition pos : TmpHighlightedPos)
				{
					ATile* t = GetTileAtCoordinates(pos.Row, pos.Col);
					t->HighlightTile(false);
				}
				TmpHighlightedPos.Empty();

			}, highLightDuration, false);
	}
}



