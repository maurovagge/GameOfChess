#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Piece.h"
#include "Tile.h"
#include "Piece_Bishop.generated.h"

UCLASS()
class GAMEOFCHESS_API APiece_Bishop : public APiece
{
    GENERATED_BODY()

public:
    // Constructor
    APiece_Bishop();
};
