#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


class APiece;

UCLASS()
class GAMEOFCHESS_API ATile : public AActor
{
    GENERATED_BODY()

public:
    // Constructor
    ATile();
    ATile(ATile& t);


    // Function to highlight the tile
    void HighlightTile(bool bHighlight);

    //set under check
    void SetTileUnderCheck(bool bCheck);

    //is it black
    bool IsBlackTile() const;

    // Function to set tile material
    void SetTileMaterial(class UMaterialInterface* Material);

    // Function to remove Highlighiting from a tile
    void RemoveHighlight();

    // Material for black tiles
    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UMaterialInterface* BlackMaterial;

    // Material for white tiles
    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UMaterialInterface* WhiteMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UMaterialInterface* UnderCheckMaterial;

    // Function to set the piece on this tile
    void SetPiece(APiece* NewPiece);

    // Function to check if this tile is occupied
    bool IsOccupied() const;

    // Function to get the occupying piece
    APiece* GetOccupyingPiece() const;

    // Row and column indices of the tile
    int32 RowIndex;
    int32 ColumnIndex;

    // Function to get the location of the tile
    FVector GetTileLocation() const;

    // Funtion to reset a tile to its initial conditions
    void ResetTile();

    // Function to set the occupying piece of the tile
    void SetOccupyingPiece(APiece* Piece);

    //Store if the tile has black default color
    bool DefaultBlack;

protected:
    // Mesh component for the tile
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
    class UStaticMeshComponent* TileMesh;

    // Material to use when tile is highlighted
    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UMaterialInterface* HighlightedMaterial;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Piece occupying this tile
    APiece* OccupyingPiece;
};