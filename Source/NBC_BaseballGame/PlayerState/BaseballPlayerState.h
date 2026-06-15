#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BaseballPlayerState.generated.h"

UCLASS()
class NBC_BASEBALLGAME_API ABaseballPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABaseballPlayerState();

	// 멀티플레이어 변수 동기화
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 시도 횟수 관리 및 Getter 함수
	void IncreaseCurrentAttempts();
	void ResetAttempts();

	UFUNCTION(BlueprintPure, Category = "Baseball")
	int32 GetCurrentAttempts() const { return CurrentAttempts; }

	UFUNCTION(BlueprintPure, Category = "Baseball")
	int32 GetMaxAttempts() const { return MaxAttempts; }

	// 플레이어가 숫자를 입력할 때마다 보일 UI용 텍스트 반환 
	UFUNCTION(BlueprintPure, Category = "Baseball")
	FString GetAttemptsStatusString() const;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball|Turn")
	int32 TurnRemainingTime;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball|Turn")
	bool bIsMyTurn; // 내 턴인가를 확인하는 변수
protected:
	// 멀티플레이 네트워크 복제 변수 
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 CurrentAttempts;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 MaxAttempts;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball|Turn")
	bool bHasSubmittedThisTurn; // 이번 턴에 숫자를 입력했는지 체크하는 변수

	
};
