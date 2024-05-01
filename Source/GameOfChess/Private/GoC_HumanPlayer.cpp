#include "GoC_HumanPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager.h"
#include "EngineUtils.h"
#include "Board.h"

AGoC_HumanPlayer::AGoC_HumanPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create a camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//set the camera as RootComponent
	SetRootComponent(Camera);

	selectedRow = -1;
	selectedCol = -1;

}

// Called when the game starts or when spawned
void AGoC_HumanPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGoC_HumanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Handler for left mouse click
bool AGoC_HumanPlayer::LeftMouseButtonClicked()
{
	// Get the player controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Human has not moved yet in this turn
	bHasMoved = false;

	// Check if the player controller is valid
	if (PlayerController)
	{
		// Get the mouse position
		float MouseX, MouseY;
		PlayerController->GetMousePosition(MouseX, MouseY);

		// Get the size of the viewport
		int32 ViewportSizeX, ViewportSizeY;
		PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

		// Convert mouse position to viewport space (0,0 is top-left corner)
		FVector2D MousePosition((MouseX / ViewportSizeX) * ViewportSizeX, (MouseY / ViewportSizeY) * ViewportSizeY);

		// Get the camera manager
		APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;

		// Perform a raycast from the screen space position to the world
		FVector WorldLocation, WorldDirection;
		if (UGameplayStatics::DeprojectScreenToWorld(PlayerController, MousePosition, WorldLocation, WorldDirection))
		{
			// Trace from the camera to the world location
			FHitResult HitResult;
			FVector StartTrace = CameraManager->GetCameraLocation();
			FVector EndTrace = StartTrace + WorldDirection * 10000.0f; // Adjust the distance as needed
			FCollisionQueryParams TraceParams(FName(TEXT("MouseTrace")), true, PlayerController);
			if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, TraceParams))
			{
				// Check if the hit result is valid and contains a valid actor
				if (HitResult.GetActor())
				{
					ATile* destinationTile = nullptr;
					// Get the actor that was hit
					AActor* HitActor = HitResult.GetActor();

					// Check if the hit actor is a piece
					if (HitResult.GetActor()->IsA(APiece::StaticClass()))
					{
						APiece* clickedPiece = Cast<APiece>(HitResult.GetActor());

						// Check if the selected piece belongs to the HumanPlayer team (white team)
						if (clickedPiece && clickedPiece->GetTeam() == ETeam::White)
						{
							selectedRow = clickedPiece->CurrentRow;
							selectedCol = clickedPiece->CurrentCol;

							// Clear any existing highlights
							ClearTileHighlights();

							TArray<VirtualBoardMove> moves = ABoard::GlobalBoardManager->GetValidMovesFrom(ABoard::GlobalBoardManager->GetCurrentBoard(), selectedRow, selectedCol);
							//Now I have to remove all the moves that would result in a check condition for the moving team
							TArray<VirtualBoardMove> validMoves;
							for (VirtualBoardMove move : moves)
							{
								if (!ABoard::GlobalBoardManager->TestMoveForCheck(ABoard::GlobalBoardManager->GetCurrentBoard(), move, ETeam::White))
								{
									validMoves.Add(move);
								}
							}
							//// Highlight each valid move
							ABoard::GlobalBoardInstance->HighlightTiles(validMoves, true);
							return bHasMoved;
						}
						else
						{
							destinationTile = clickedPiece->GetCurrentTile();
						}
					}
					else // If the hit actor is not a piece, check if a tile was clicked
					{
						// Check if the hit actor is not a piece, indicating that a tile was clicked
						if (!HitResult.GetActor()->IsA(APiece::StaticClass()))
						{
							// Get the clicked tile from the hit result
							destinationTile = Cast<ATile>(HitResult.GetActor());
						}
					}
					if ((selectedRow > -1) && destinationTile)
					{
						if (ABoard::GlobalBoardManager->IsValidMove(ABoard::GlobalBoardManager->GetCurrentBoard(), selectedRow, selectedCol, destinationTile->RowIndex, destinationTile->ColumnIndex))
						{
							//Now I have to remove all the moves that would result in a check condition for the moving team
							{
								if (!ABoard::GlobalBoardManager->TestMoveForCheck(ABoard::GlobalBoardManager->GetCurrentBoard(), VirtualBoardMove(selectedRow, selectedCol, destinationTile->RowIndex, destinationTile->ColumnIndex), ETeam::White))
								{
									VirtualBoard vb = ABoard::GlobalBoardManager->Move(ABoard::GlobalBoardManager->GetCurrentBoard(), selectedRow, selectedCol, destinationTile->RowIndex, destinationTile->ColumnIndex);
									ABoard::GlobalBoardManager->SetCurrentBoard(vb);
									ABoard::GlobalBoardInstance->SpawnPieces(ABoard::GlobalBoardManager->GetCurrentBoard());
									bHasMoved = true;
								}
							}
						}
					}
					else
					{
						// If no piece is selected or no tile is clicked, do nothing
						return bHasMoved;
					}
					return bHasMoved;
				}
			}
		}
	}
	return bHasMoved;
}


void AGoC_HumanPlayer::RightMouseButtonClicked()
{
	// Deselect any selected piece
	DeselectPiece();
}


void AGoC_HumanPlayer::ClearTileHighlights()
{
	TArray<ATile*> Tiles = ABoard::GlobalBoardInstance->GetAllTiles();
	// Loop through all tiles on the board
	for (ATile* Tile : Tiles)
	{
		if (Tile)
		{
			// Remove highlight from the tile
			Tile->RemoveHighlight();
		}
	}
}



void AGoC_HumanPlayer::DeselectPiece()
{
	// Clear tile highlights
	ClearTileHighlights();

	// Deselect the current piece
	SelectedPiece = nullptr;
}

AGameManager* AGoC_HumanPlayer::GetGameManager() const
{
	// Get the world
	UWorld* World = GetWorld();

	if (!World)
	{
		// Print error message if world is invalid
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
