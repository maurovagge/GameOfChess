#include "Piece.h"
#include "Board.h"

// Sets default values
APiece::APiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Create and attach the mesh component
    PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));
    RootComponent = PieceMesh;

    // Set black material
    static ConstructorHelpers::FObjectFinder<UMaterialInstance> BlackMaterialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_BlackMaterial.M_BlackMaterial'"));
    BlackMaterial = BlackMaterialAsset.Object;

    // Set white material
    static ConstructorHelpers::FObjectFinder<UMaterialInstance> WhiteMaterialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_WhiteMaterial.M_WhiteMaterial'"));
    WhiteMaterial = WhiteMaterialAsset.Object;

}


void APiece::SetCurrentTile(ATile* NewTile)
{
    CurrentTile = NewTile;
}

ATile* APiece::GetCurrentTile() 
{
    return CurrentTile;
}


// Called when the game starts or when spawned
void APiece::BeginPlay()
{
	Super::BeginPlay();
}

EPieceType APiece::GetPieceType() 
{
    return PieceType;
}

void APiece::Select()
{
}


void APiece::DestroyPiece()
{
    int i = 0;
    // Iterate through all pieces on the board
    for (auto& Piece : ABoard::GlobalBoardInstance->Pieces)
    {       
        if (Piece == this)
        {
            break;
        }
        i++;
    }     
    ABoard::GlobalBoardInstance->Pieces[i] = nullptr;
    CurrentTile = nullptr;
    Destroy();    
}

void APiece::SetPieceType(EPieceType NewPieceType)
{
    PieceType = NewPieceType;
}

void APiece::SetTeam(ETeam NewTeam)
{
    Team = NewTeam;
}

bool APiece::IsLocationInBoard(FVector location) 
{
    if (location.X < 0.0f || location.X > 700.0f
        || location.Y < 0.0f || location.Y > 700.0f)
    {
        return false;
    }
    return true;
}
