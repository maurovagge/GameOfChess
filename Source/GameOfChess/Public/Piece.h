#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Board.h"
#include "Tile.h"
#include "Piece.generated.h"


UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
	Forward UMETA(DisplayName = "Forward"),
	Backward UMETA(DisplayName = "Backward"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
	ForwardLeft UMETA(DisplayName = "ForwardLeft"),
	ForwardRight UMETA(DisplayName = "ForwardRight"),
	BackwardLeft UMETA(DisplayName = "BackwardLeft"),
	BackwardRight UMETA(DisplayName = "BackwardRight"),
	Knight UMETA(DisplayName = "Knight"),
};

UENUM(BlueprintType)
enum class EPieceType : uint8
{
	NoPiece,
	Pawn,
	Rook,
	Knight,
	Bishop,
	Queen,
	King
};

UENUM(BlueprintType)
enum class ETeam : uint8
{
	White UMETA(DisplayName = "White"),
	Black UMETA(DisplayName = "Black"),
};

USTRUCT(BlueprintType)
struct FMove
{
	GENERATED_BODY()

	FVector StartPosition;
	FVector EndPosition;
	EPieceType PieceType;

	FMove() {}
	FMove(FVector Start, FVector End, EPieceType Piece) : StartPosition(Start), EndPosition(End), PieceType(Piece){}
};


UCLASS()
class GAMEOFCHESS_API APiece : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APiece();


	// Function to set the current tile of the piece
	void SetCurrentTile(class ATile* NewTile);

	// Function to get the current tile of the piece
	class ATile* GetCurrentTile() ;

	// Function to get the team of the piece
	ETeam GetTeam() const { return Team; }

	// Material for black pieces
	UPROPERTY(EditDefaultsOnly, Category = "Piece")
	class UMaterialInterface* BlackMaterial;

	// Material for white pieces
	UPROPERTY(EditDefaultsOnly, Category = "Piece")
	class UMaterialInterface* WhiteMaterial;

	// Function to get the type of the piece
	EPieceType GetPieceType() ;

	// Function to select this piece
	void Select();

	// Function to set the type of the piece
	void SetPieceType(EPieceType NewPieceType);

	// Function to set the team of the piece
	void SetTeam(ETeam NewTeam);

	// Function to destroy the the piece
	void DestroyPiece();


	int32 CurrentRow;
	int32 CurrentCol;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Mesh component for the piece
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Piece")
	class UStaticMeshComponent* PieceMesh;

	// Current tile of the piece
	UPROPERTY()
	class ATile* CurrentTile;

	// Team of the piece
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece")
	ETeam Team;

	// Type of the piece
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece")
	EPieceType PieceType;
  
	bool IsLocationInBoard(FVector location) ;
};