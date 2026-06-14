#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseballGameModeBase.generated.h"

UCLASS()
class NBC_BASEBALLGAME_API ABaseballGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABaseballGameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	// 💡 [요구사항] 게임 시작 시 정답 생성
	void GenerateRandomNumbers();

	// 💡 [요구사항] 깐깐한 유효성 검사 (3자리, 문자 제외, 중복 제외)
	bool ValidateInput(const FString& InputStr, FString& OutWarningMessage);

	// 💡 [요구사항] 정답 비교 및 결과 반환
	FString CheckAnswer(const FString& InputStr, int32& OutStrikes);

	// 💡 플레이어가 숫자를 입력했을 때 서버가 호출하는 판정 메인 함수
	UFUNCTION(BlueprintCallable, Category = "Baseball")
	void OnPlayerSubmitGuess(AController* PlayerController, const FString& GuessStr);

	// 💡 [요구사항] 게임 결과 공지 후 모든 데이터 초기화 및 재시작
	void ResetGame();

private:
	FString SecretNumber; // 서버만 알고 있는 3자리 정답 숫자 (예: "386")
	bool bIsGameOver;     // 승리나 무승부로 게임이 끝났는지 체크하는 플래그
};
