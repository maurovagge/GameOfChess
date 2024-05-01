#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "Piece.h"
#include "Piece_Bishop.h"
#include "Piece_Rook.h"
#include "Piece_Knight.h"
#include "Piece_Queen.h"
#include "Piece_King.h"
#include "Piece_Pawn.h"
#include "VirtualBoardManager.h"
#include "Board.generated.h"

// Definizione di una struttura per rappresentare una tripla di valori
template<typename T, typename U, typename V, typename Z>
struct TQuadVal
{
    T First;
    U Second;
    V Third;
    Z Fourth;

    TQuadVal() : First(T()), Second(U()), Third(V()), Fourth(Z()) {}
    TQuadVal(const T& InFirst, const U& InSecond, const V& InThird, const Z& InFourth) : First(InFirst), Second(InSecond), Third(InThird), Fourth(InFourth) {}
};


UCLASS()
class GAMEOFCHESS_API ABoard : public AActor
{
    GENERATED_BODY()

public:

    // Constructor
    ABoard();

    // Function to initialize the chessboard
    void SetupBoard();

    UPROPERTY(EditDefaultsOnly, Category = "Board")
    TSubclassOf<ATile> TileClass;
    
    // Function to get the tile at a specific location
    ATile* GetTileAtLocation(const FVector& Location) ;

    // Static global instance of Board
    static ABoard* GlobalBoardInstance;

    // Static global instance of BoardManager
    static VirtualBoardManager* GlobalBoardManager;

    // Size of each tile
    UPROPERTY(EditAnywhere, Category = "Board")
    float TileSize;

    // Grid size of the chessboard
    UPROPERTY(EditDefaultsOnly, Category = "Board")
    int32 GridSizeX;

    UPROPERTY(EditDefaultsOnly, Category = "Board")
    int32 GridSizeY;

    UPROPERTY(EditDefaultsOnly, Category = "Board")
    int32 BoardSize;

    // Function to get the piece at a specific location
    APiece* GetPieceAtLocation(int32 Row, int32 Col) ;
    
    // Function to check if tile is on the board
    bool IsValidTile(int32 Row, int32 Col) ;

    // Function to get all tiles
    TArray<class ATile*> GetAllTiles() ;

    // Funtion to reset the board
    UFUNCTION(BlueprintCallable)
    void ResetBoard();

    // Function to get a tile from its coordinates
    ATile* GetTileAtCoordinates(int32 Row, int32 Column) ;

    // Called when an instance of this class is placed (in editor) or spawned
    virtual void OnConstruction(const FTransform& Transform) override;

    void SetKingUnderCheck(ETeam team, bool bCheck);
    TArray<class APiece*> Pieces;


    void SpawnPieces(VirtualBoard vb);
    void Move(VirtualBoardMove vbm);
    void HighlightTiles(TArray<ATile*> tiles, bool highlight, int32 highLightDuration = 0);
    void HighlightTiles(TArray<BoardPosition> positions, bool highlight, int32 highLightDuration = 0);
    void HighlightTiles(TArray<VirtualBoardMove> moves, bool highlight, int32 highLightDuration = 0);
    void HighlightTiles(VirtualBoardMove move, bool highlight, int32 highLightDuration = 0);
    void HighlightTile(ATile* tile, bool highlight, int32 highLightDuration = 0);
    void HighlightTile(BoardPosition position, bool highlight, int32 highLightDuration = 0);


    VirtualBoardManager* VBoardManager;


protected:


    UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
    TSubclassOf<APiece_Bishop> PieceBishopActorClass_B;

    UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
    TSubclassOf<APiece_Bishop> PieceBishopActorClass_W;

    UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
    TSubclassOf<APiece_Rook> PieceRookActorClass_B;

    UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
    TSubclassOf<APiece_Rook> PieceRookActorClass_W;

    UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
    TSubclassOf<APiece_Pawn> PiecePawnActorClass_B;

    UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
    TSubclassOf<APiece_Pawn> PiecePawnActorClass_W;

    UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
    TSubclassOf<APiece_Knight> PieceKnightActorClass_B;

    UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
    TSubclassOf<APiece_Knight> PieceKnightActorClass_W;

    UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
    TSubclassOf<APiece_Queen> PieceQueenActorClass_B;

    UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
    TSubclassOf<APiece_Queen> PieceQueenActorClass_W;

    UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
    TSubclassOf<APiece_King> PieceKingActorClass_B;

    UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
    TSubclassOf<APiece_King> PieceKingActorClass_W;


    // Array to hold the tiles
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board")
    TArray<class ATile*> Tiles;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void LogBoardState(TArray<TQuadVal<int32, int32, EPieceType, bool>> PiecePositions) ;

    TArray<BoardPosition> TmpHighlightedPos;
};

