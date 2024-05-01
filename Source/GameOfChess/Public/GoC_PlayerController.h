#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GoC_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEOFCHESS_API AGoC_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AGoC_PlayerController();

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* GoCContext;


	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ClickAction;

	void ClickOnGrid();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Function to get a GameManager instance
	AGameManager* GetGameManager() const;
};
