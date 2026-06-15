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

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball|Turn")
	int32 TurnRemainingTime; // 모든 클라이언트에 복제될 남은 시간 변수

	FTimerHandle TurnTimerHandle; // 서버가 굴릴 타이머 시계
	int32 CurrentTurnPlayerIndex; // 지금 누구 턴인지 가리키는 번호
public:
	// 게임 시작 시 정답 생성
	void GenerateRandomNumbers();

	// 깐깐한 유효성 검사 (3자리, 문자 제외, 중복 제외)
	bool ValidateInput(const FString& InputStr, FString& OutWarningMessage);

	// 정답 비교 및 결과 반환
	FString CheckAnswer(const FString& InputStr, int32& OutStrikes);

	// 플레이어가 숫자를 입력했을 때 서버가 호출하는 판정 메인 함수
	UFUNCTION(BlueprintCallable, Category = "Baseball")
	void OnPlayerSubmitGuess(AController* PlayerController, const FString& GuessStr);

	// 게임 결과 공지 후 모든 데이터 초기화 및 재시작
	void ResetGame();

	void StartTurnTimer();  // 턴 시작 시 30초 타이머 시동 거는 함수
	void OnTurnTimeTick();  // 1초마다 실행되면서 시간을 1씩 깎는 함수
	void SwitchToNextTurn(); // 시간이 0이 되거나 입력했을 때 다음 사람으로 턴 넘기는 함수
private:
	FString SecretNumber; // 서버만 알고 있는 3자리 정답 숫자
	bool bIsGameOver;     
};
