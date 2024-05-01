#include "Tile.h"
#include "Piece.h"
#include "Components/StaticMeshComponent.h"

ATile::ATile()
{
	// Create and attach the mesh component
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	RootComponent = TileMesh;

	ColumnIndex = 0;
	RowIndex = 0;

	// Set black material
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BlackMaterialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_BlackMaterial.M_BlackMaterial'"));
	BlackMaterial = BlackMaterialAsset.Object;

	// Set white material
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> WhiteMaterialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_WhiteMaterial.M_WhiteMaterial'"));
	WhiteMaterial = WhiteMaterialAsset.Object;

	// Set highlighted material instance
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> HighlightedMaterialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_HighlightedMaterial.M_HighlightedMaterial'"));
	HighlightedMaterial = HighlightedMaterialAsset.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> UnderCheckMaterialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_UnderCheckMaterial.M_UnderCheckMaterial'"));
	UnderCheckMaterial = UnderCheckMaterialAsset.Object;

	// Initialize occupying piece to nullptr
	OccupyingPiece = nullptr;
}

ATile::ATile(ATile& t)
{
	// Create and attach the mesh component
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	RootComponent = TileMesh;

	// Set black material
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BlackMaterialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_BlackMaterial.M_BlackMaterial'"));
	BlackMaterial = BlackMaterialAsset.Object;

	// Set white material
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> WhiteMaterialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_WhiteMaterial.M_WhiteMaterial'"));
	WhiteMaterial = WhiteMaterialAsset.Object;

	// Set highlighted material instance
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> HighlightedMaterialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_HighlightedMaterial.M_HighlightedMaterial'"));
	HighlightedMaterial = HighlightedMaterialAsset.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> UnderCheckMaterialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_UnderCheckMaterial.M_UnderCheckMaterial'"));
	UnderCheckMaterial = UnderCheckMaterialAsset.Object;

	OccupyingPiece = t.OccupyingPiece;
	ColumnIndex = t.ColumnIndex;
	RowIndex = t.RowIndex;
	TileMesh = t.TileMesh;
}


void ATile::BeginPlay()
{
	Super::BeginPlay();

}

//Higlight/reset a tile
void ATile::HighlightTile(bool bHighlight)
{
	APiece* piece = GetOccupyingPiece();
	if (piece != nullptr)
	{
		if (piece->GetPieceType() == EPieceType::King)
			return;
	}

	if (bHighlight)
	{
		// Set highlighted material
		TileMesh->SetMaterial(0, HighlightedMaterial);
	}
	else
	{
		// Determine which material to set based on tile color
		UMaterialInterface* TileMaterial = nullptr;
		if (DefaultBlack)
		{
			TileMaterial = BlackMaterial;
		}
		else
		{
			TileMaterial = WhiteMaterial;
		}

		// Set tile material
		if (TileMaterial)
		{
			TileMesh->SetMaterial(0, TileMaterial);
		}
	}
}

//Set tile color for "check condition"
void ATile::SetTileUnderCheck(bool bCheck)
{
	if (bCheck)
	{
		TileMesh->SetMaterial(0, UnderCheckMaterial);
	}
	else
	{
		// Determine which material to set based on tile color
		UMaterialInterface* TileMaterial = nullptr;
		if (DefaultBlack)
		{
			TileMaterial = BlackMaterial;
		}
		else
		{
			TileMaterial = WhiteMaterial;
		}

		// Set tile material
		if (TileMaterial)
		{
			TileMesh->SetMaterial(0, TileMaterial);
		}
	}
}

//remove highlight from a tile
void ATile::RemoveHighlight()
{
	// Determine which material to set based on tile color
	UMaterialInterface* TileMaterial = nullptr;
	if (DefaultBlack)
	{
		TileMaterial = BlackMaterial;
	}
	else
	{
		TileMaterial = WhiteMaterial;
	}

	// Set tile material
	if (TileMaterial)
	{
		TileMesh->SetMaterial(0, TileMaterial);
	}
}

//Set material for the tile
void ATile::SetTileMaterial(UMaterialInterface* Material)
{
	// Set the specified material
	TileMesh->SetMaterial(0, Material);
}

//return if it is black
bool ATile::IsBlackTile() const
{
	// Check if the material of the tile mesh matches the black material
	return TileMesh && TileMesh->GetMaterial(0) == BlackMaterial;
}

//Set occopying piece pointer into the tile
void ATile::SetPiece(APiece* NewPiece)
{
	// Set the occupying piece
	OccupyingPiece = NewPiece;
}

//return if tile is occupied
bool ATile::IsOccupied() const
{
	return OccupyingPiece != nullptr;
}

//return occupying piece
APiece* ATile::GetOccupyingPiece() const
{
	return OccupyingPiece;
}

//get location of the tile 
FVector ATile::GetTileLocation() const
{
	return GetActorLocation();
}


//reset the tile
void ATile::ResetTile()
{
	//Destroy piece, if any
	if (OccupyingPiece != nullptr)
	{
		OccupyingPiece->DestroyPiece();
	}
	// Reset the tile to its initial state
	OccupyingPiece = nullptr;

	// Reset the tile's visual appearance, if needed
	HighlightTile(false); // For example, remove any highlight
}

//get occupying piece
void ATile::SetOccupyingPiece(APiece* Piece)
{
	// Set the occupying piece
	OccupyingPiece = Piece;
}
