#include "BaseballPlayerController.h"
#include "NBC_BaseballGame/GameMode/BaseballGameModeBase.h"

void ABaseballPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 💡 데디케이티드 서버(Dedicated Server)에서는 오직 '진짜 클라이언트'만 UI를 띄워야 합니다.
	// C++의 IsLocalController()가 블루프린트의 IsLocalController 노드와 완전히 같은 역할을 합니다.
	if (IsLocalController())
	{
		// 마우스 커서를 화면에 보이게 설정합니다.
		bShowMouseCursor = true;

		// 게임과 UI를 동시에 조작할 수 있도록 입력 모드를 설정합니다.
		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);
	}
}

// 💡 클라이언트가 호출하면, 이 안의 알맹이 코드는 '서버 컴퓨터' 안에서 실행됩니다!
void ABaseballPlayerController::Server_SubmitGuess_Implementation(const FString& GuessStr)
{
	// 💡 서버에 도달했으니, 서버 전용 클래스인 GameMode를 안전하게 가져옵니다.
	ABaseballGameModeBase* GameMode = Cast<ABaseballGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		// C++ GameMode에 만들어 둔 판정기로 입력값 배달 완료!
		GameMode->OnPlayerSubmitGuess(this, GuessStr);
	}
}
