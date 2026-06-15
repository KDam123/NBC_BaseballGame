#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseballPlayerController.generated.h"

UCLASS()
class NBC_BASEBALLGAME_API ABaseballPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	// 클라이언트가 입력한 문자열을 '서버 컴퓨터'로 안전하게 배달하는 Server RPC 함수
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Baseball")
	void Server_SubmitGuess(const FString& GuessStr);
};
