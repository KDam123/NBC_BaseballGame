#include "BaseballPlayerController.h"
#include "NBC_BaseballGame/GameMode/BaseballGameModeBase.h"

void ABaseballPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		// 마우스 커서를 화면에 보이게 설정합니다.
		bShowMouseCursor = true;

		// 게임과 UI를 동시에 조작할 수 있도록 입력 모드를 설정합니다.
		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);
	}
}

// 클라이언트가 호출하면, 이 안의 코드는 '서버 컴퓨터' 안에서 실행된다.
void ABaseballPlayerController::Server_SubmitGuess_Implementation(const FString& GuessStr)
{
	// 서버에 도달했으니, 서버 전용 클래스인 GameMode를 가져온다.
	ABaseballGameModeBase* GameMode = Cast<ABaseballGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnPlayerSubmitGuess(this, GuessStr);
	}
}
