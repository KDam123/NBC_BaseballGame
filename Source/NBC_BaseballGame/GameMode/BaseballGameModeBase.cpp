#include "BaseballGameModeBase.h"
#include "NBC_BaseballGame/PlayerState/BaseballPlayerState.h"// 💡 PlayerState 변수를 가져오기 위해 인클루드 필수!
#include "GameFramework/PlayerController.h"

ABaseballGameModeBase::ABaseballGameModeBase()
{
	bIsGameOver = false;
}

void ABaseballGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	GenerateRandomNumbers();
}

// 🎯 1. GenerateRandomNumbers : 중복 없는 3자리 정답 생성 (1~9 사이)
void ABaseballGameModeBase::GenerateRandomNumbers()
{
	TArray<int32> Digits;
	for (int32 i = 1; i <= 9; ++i) Digits.Add(i);

	// 무작위로 숫자 섞기 (셔플)
	for (int32 i = Digits.Num() - 1; i > 0; --i)
	{
		int32 J = FMath::RandRange(0, i);
		Digits.Swap(i, J);
	}

	// 섞인 배열에서 앞의 3개만 뽑아 정답 문자열 생성
	SecretNumber = FString::Printf(TEXT("%d%d%d"), Digits[0], Digits[1], Digits[2]);

	// 💡 [요구사항] UE_LOG를 사용하여 서버 로그창에 정답 찍기
	UE_LOG(LogTemp, Warning, TEXT("=========================================="));
	UE_LOG(LogTemp, Warning, TEXT("★ [서버 치트키] 이번 게임의 정답은: %s 입니다 ★"), *SecretNumber);
	UE_LOG(LogTemp, Warning, TEXT("=========================================="));
}

// 🎯 2. 예외 상황 방어 (깐깐한 입력 검사)
bool ABaseballGameModeBase::ValidateInput(const FString& InputStr, FString& OutWarningMessage)
{
	// 예외 1: 3자리가 아닐 때
	if (InputStr.Len() != 3)
	{
		OutWarningMessage = TEXT("3자리 숫자를 입력해주세요.");
		return false;
	}

	// 예외 2: 숫자가 아닌 문자(예: 'abc')가 들어왔을 때
	for (int32 i = 0; i < 3; ++i)
	{
		if (!FChar::IsDigit(InputStr[i]))
		{
			OutWarningMessage = TEXT("문자는 입력할 수 없습니다. 숫자만 입력하세요.");
			return false;
		}
	}

	// 예외 3: 중복되는 숫자가 있을 때 (예: 112)
	if (InputStr[0] == InputStr[1] || InputStr[0] == InputStr[2] || InputStr[1] == InputStr[2])
	{
		OutWarningMessage = TEXT("중복되지 않은 숫자를 입력해주세요.");
		return false;
	}

	return true;
}

// 🎯 3. CheckAnswer : 정답 비교 및 결과 반환 (Strike / Ball / OUT 판정)
FString ABaseballGameModeBase::CheckAnswer(const FString& InputStr, int32& OutStrikes)
{
	int32 Strikes = 0;
	int32 Balls = 0;

	// 💡 2중 for문 구조를 활용한 자릿수와 숫자 비교
	for (int32 i = 0; i < 3; ++i)
	{
		for (int32 j = 0; j < 3; ++j)
		{
			if (InputStr[i] == SecretNumber[j])
			{
				if (i == j)
				{
					Strikes++; // 숫자와 자리가 모두 일치 (Strike)
				}
				else
				{
					Balls++; // 숫자는 존재하지만 자리가 다름 (Ball)
				}
			}
		}
	}

	OutStrikes = Strikes;

	// 하나도 일치하는 숫자가 없을 때
	if (Strikes == 0 && Balls == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS %dB"), Strikes, Balls);
}

// 🎯 4. 메인 입력 처리 및 승리/무승부 판정
void ABaseballGameModeBase::OnPlayerSubmitGuess(AController* PlayerController, const FString& GuessStr)
{
	if (bIsGameOver || !PlayerController) return;

	// 💡 우리가 만든 나만의 BaseballPlayerState로 안전하게 캐스팅
	ABaseballPlayerState* BPS = PlayerController->GetPlayerState<ABaseballPlayerState>();
	if (!BPS) return;

	// 💡 [예외 처리] 이미 기회를 3번 다 쓴 플레이어는 입력 차단
	if (BPS->GetCurrentAttempts() >= BPS->GetMaxAttempts())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]님은 기회를 모두 소모하여 더 이상 입력할 수 없습니다."), *BPS->GetPlayerName());
		return;
	}

	// 깐깐한 유효성 검사 실행
	FString WarningMsg;
	if (!ValidateInput(GuessStr, WarningMsg))
	{
		// 💡 [요구사항] 조건에 맞지 않으면 안내를 출력하고 기회를 소진하지 않음
		UE_LOG(LogTemp, Warning, TEXT("[%s의 잘못된 입력]: %s (기회 유지됨)"), *BPS->GetPlayerName(), *WarningMsg);
		return;
	}

	// 규칙에 맞는 정답을 입력했으므로 시도 횟수 1 증가
	BPS->IncreaseCurrentAttempts();

	int32 Strikes = 0;
	FString Result = CheckAnswer(GuessStr, Strikes);

	// 💡 [요구사항] 숫자를 입력할 때마다 현재 시도 횟수와 최대 시도 횟수가 보이게 출력 [현재 / 최대]
	UE_LOG(LogTemp, Log, TEXT("> %s님의 입력 [%s] 결과: %s %s"),
		*BPS->GetPlayerName(), *GuessStr, *Result, *BPS->GetAttemptsStatusString());

	// 🏆 승리 판정: 3번의 기회 안에 3S를 먼저 달성하면 즉시 승리 판정
	if (Strikes == 3)
	{
		bIsGameOver = true;
		UE_LOG(LogTemp, Warning, TEXT("🏆 [게임 종료] %s님이 승리했습니다! Winner 위젯을 공지합니다."), *BPS->GetPlayerName());

		// 💡 [요구사항] 결과 공지 후 5초 뒤 게임 리셋 및 재시작 설정
		FTimerHandle ResetTimer;
		GetWorld()->GetTimerManager().SetTimer(ResetTimer, this, &ABaseballGameModeBase::ResetGame, 5.0f, false);
		return;
	}

	// 🤝 무승부 판정: 모든 플레이어가 3번의 기회를 모두 소모했는지 검사
	bool bAllPlayersExhausted = true;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			ABaseballPlayerState* EachBPS = PC->GetPlayerState<ABaseballPlayerState>();
			if (EachBPS && EachBPS->GetCurrentAttempts() < EachBPS->GetMaxAttempts())
			{
				bAllPlayersExhausted = false; // 아직 기회가 남은 생존자가 있다면 무승부 아님
				break;
			}
		}
	}

	if (bAllPlayersExhausted)
	{
		bIsGameOver = true;
		UE_LOG(LogTemp, Warning, TEXT("🤝 [게임 종료] 모든 플레이어가 기회를 소진했습니다! 무승부입니다."));

		// 5초 뒤 게임 리셋 및 재시작
		FTimerHandle ResetTimer;
		GetWorld()->GetTimerManager().SetTimer(ResetTimer, this, &ABaseballGameModeBase::ResetGame, 5.0f, false);
	}
}

// 🎯 5. ResetGame : 승리/무승부 시 게임 데이터를 새로고침하고 초기화
void ABaseballGameModeBase::ResetGame()
{
	bIsGameOver = false;
	GenerateRandomNumbers(); // 정답 숫자 새로 생성

	// 접속 중인 모든 플레이어의 시도 횟수를 0으로 리셋
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			ABaseballPlayerState* BPS = PC->GetPlayerState<ABaseballPlayerState>();
			if (BPS)
			{
				BPS->ResetAttempts();
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("=== 모든 플레이어의 시도 횟수가 초기화되었으며 새 게임이 시작되었습니다! ==="));
}
