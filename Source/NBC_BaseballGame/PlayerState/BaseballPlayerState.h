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

	// 💡 멀티플레이어 변수 동기화를 위한 언리얼 필수 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 💡 [요구사항] 시도 횟수 관리 및 Getter 함수
	void IncreaseCurrentAttempts();
	void ResetAttempts();

	UFUNCTION(BlueprintPure, Category = "Baseball")
	int32 GetCurrentAttempts() const { return CurrentAttempts; }

	UFUNCTION(BlueprintPure, Category = "Baseball")
	int32 GetMaxAttempts() const { return MaxAttempts; }

	// 💡 [요구사항] 플레이어가 숫자를 입력할 때마다 보일 UI용 텍스트 반환 [현재/최대]
	UFUNCTION(BlueprintPure, Category = "Baseball")
	FString GetAttemptsStatusString() const;

protected:
	// 💡 멀티플레이 네트워크 복제 변수 (서버와 클라이언트 동기화)
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 CurrentAttempts;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 MaxAttempts;
};
