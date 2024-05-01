#include "GoC_PlayerController.h"
#include "Components/InputComponent.h"
#include "GoC_HumanPlayer.h"
#include "GameManager.h"
#include "EngineUtils.h"


AGoC_PlayerController::AGoC_PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void AGoC_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(GoCContext, 0);
	}
}

void AGoC_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AGoC_PlayerController::ClickOnGrid);
	}
}

//Handle the click on the grid, to identified if it is on the board and pass them to players
void AGoC_PlayerController::ClickOnGrid()
{
	const auto HumanPlayer = GetGameManager()->GetHumanPlayerInstance();
	if (!GetGameManager()->CanPlay())
	{
		return;
	}
	if (GetGameManager()->IsMyTurn(HumanPlayer))
	{
		if (IsValid(HumanPlayer))
		{
			bool turnEnded = HumanPlayer->LeftMouseButtonClicked();
			if (turnEnded)
			{
				GetGameManager()->EndPlayerTurn();
			}
		}
	}
}



AGameManager* AGoC_PlayerController::GetGameManager() const
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
