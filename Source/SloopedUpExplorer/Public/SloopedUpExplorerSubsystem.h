#pragma once

#include "Subsystem/ModSubsystem.h"
#include "SloopedUpExplorerSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSloopedUpExplorer, Log, All);

UCLASS()
class SLOOPEDUPEXPLORER_API ASloopedUpExplorerSubsystem : public AModSubsystem {
	GENERATED_BODY()

public:
	ASloopedUpExplorerSubsystem();
	static ASloopedUpExplorerSubsystem* Get(UWorld* world);

	virtual void BeginPlay() override;

	// BP function to determine if a vehicle should be tuned
	UFUNCTION(BlueprintImplementableEvent, Category = "SloopedUpExplorer")
	bool ShouldTuneVehicle(UFGWheeledVehicleMovementComponent* vehicleMovementComponent);

	// Modifies all relevant parameters for the Explorer to give it a more powerful, stable,
	// and responsive feel, while still being drivable and not too overpowered
	UFUNCTION(BlueprintCallable, Category = "SloopedUpExplorer")
	void TuneExplorer(UFGWheeledVehicleMovementComponent* vehicleMovementComponent);

	// Swaps the Explorer's wheels to use custom wheel classes with different physics
	UFUNCTION(BlueprintCallable, Category = "SloopedUpExplorer")
	void SwapExplorerWheels(UFGWheeledVehicleMovementComponent* vehicleMovementComponent);

	// Bounce the front of the Explorer by applying an upward impulse at the front wheels
	UFUNCTION(BlueprintCallable, Category = "SloopedUpExplorer|Hydraulics")
	void BounceFrontHydraulics(UFGWheeledVehicleMovementComponent* vehicleMovementComponent, float impulseStrength = 240000.0f);

protected:
	// Front wheel class to use for swapping. Set in BP Subsystem
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SloopedUpExplorer|Wheels")
	TSubclassOf<class UChaosVehicleWheel> FrontWheelClass;

	// Rear wheel class to use for swapping. Set in BP Subsystem
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SloopedUpExplorer|Wheels")
	TSubclassOf<class UChaosVehicleWheel> RearWheelClass;

};
