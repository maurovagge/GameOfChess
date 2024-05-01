#include "GoC_Player.h"
#include "GameManager.h"
#include "Board.h"

AGoC_Player::AGoC_Player()
{
    // Set default values for player name and score
    PlayerName = "Player";
    PlayerScore = 0;
}

void AGoC_Player::SetPlayerName(FString& NewName)
{
    PlayerName = NewName;
}

FString AGoC_Player::GetPlayerName() 
{
    return PlayerName;
}

void AGoC_Player::SetPlayerScore(int32 NewScore)
{
    PlayerScore = NewScore;
}

int32 AGoC_Player::GetPlayerScore() 
{
    return PlayerScore;
}



