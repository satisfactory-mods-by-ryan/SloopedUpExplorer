#include "SloopedUpExplorerSubsystem.h"
#include "Subsystem/SubsystemActorManager.h"
#include "Kismet/GameplayStatics.h"
#include "WheeledVehicles/FGWheeledVehicleMovementComponent.h"

DEFINE_LOG_CATEGORY(LogSloopedUpExplorer);

ASloopedUpExplorerSubsystem::ASloopedUpExplorerSubsystem() {
	ReplicationPolicy = ESubsystemReplicationPolicy::SpawnOnServer_Replicate;
}

ASloopedUpExplorerSubsystem* ASloopedUpExplorerSubsystem::Get(UWorld* world) {
	if (!world) {
		return nullptr;
	}
	USubsystemActorManager* subsystemActorManager = world->GetSubsystem<USubsystemActorManager>();
	if (!subsystemActorManager) {
		return nullptr;
	}
	ASloopedUpExplorerSubsystem* sloopedUpExplorerSubsystem = subsystemActorManager->GetSubsystemActor<ASloopedUpExplorerSubsystem>();
	if (!sloopedUpExplorerSubsystem) {
		sloopedUpExplorerSubsystem = Cast<ASloopedUpExplorerSubsystem>(UGameplayStatics::GetActorOfClass(world, ASloopedUpExplorerSubsystem::StaticClass()));
	}
	return sloopedUpExplorerSubsystem;
}

void ASloopedUpExplorerSubsystem::BeginPlay() {
	Super::BeginPlay();

	// Log what wheel classes are configured
	if (FrontWheelClass && RearWheelClass) {
		UE_LOG(LogSloopedUpExplorer, Log, TEXT("Wheel classes configured: Front=%s, Rear=%s"), *FrontWheelClass->GetName(), *RearWheelClass->GetName());
	} else {
		UE_LOG(LogSloopedUpExplorer, Error, TEXT("Wheel classes NOT configured! Front=%s, Rear=%s. Make sure to use the Blueprint subsystem child class!"), FrontWheelClass ? TEXT("OK") : TEXT("NULL"), RearWheelClass ? TEXT("OK") : TEXT("NULL"));
	}
}

void ASloopedUpExplorerSubsystem::TuneExplorer(UFGWheeledVehicleMovementComponent* vehicleMovementComponent) {
	if (!vehicleMovementComponent || !IsValid(vehicleMovementComponent)) {
		return;
	}
	if (!FrontWheelClass || !RearWheelClass) {
		UE_LOG(LogSloopedUpExplorer, Error, TEXT("Cannot swap Explorer wheels - wheel classes not configured (Front=%s, Rear=%s) for %s"), FrontWheelClass ? TEXT("OK") : TEXT("NULL"), RearWheelClass ? TEXT("OK") : TEXT("NULL"), *vehicleMovementComponent->GetOwner()->GetName());
		return;
	}
	if (!FrontWheelClass->IsChildOf(UChaosVehicleWheel::StaticClass()) || !RearWheelClass->IsChildOf(UChaosVehicleWheel::StaticClass())) {
		UE_LOG(LogSloopedUpExplorer, Error, TEXT("Configured wheel classes are not valid UChaosVehicleWheel subclasses!"));
		return;
	}

	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Tuning Explorer movement component for %s"), *vehicleMovementComponent->GetOwner()->GetName());

	// VEHICLE SETUP

	vehicleMovementComponent->Mass = 700.0f;
	vehicleMovementComponent->CenterOfMassOverride = FVector(5.0f, 0.0f, -10.0f);

	vehicleMovementComponent->mHandbrakeSideSlipMultiplier = 0.7f;
	vehicleMovementComponent->DownforceCoefficient = 0.9f;

	// ENGINE CONFIG

	FVehicleEngineConfig& engineConfig = vehicleMovementComponent->EngineSetup;
	engineConfig.MaxTorque = 600.0f;
	engineConfig.EngineRevDownRate = 50.0f;

	// TRANSMISSION CONFIG

	FVehicleTransmissionConfig& transmissionConfig = vehicleMovementComponent->TransmissionSetup;
	transmissionConfig.bUseAutoReverse = false;
	transmissionConfig.ForwardGearRatios[0] = 2.5;

	// STEERING CONFIG

	FVehicleSteeringConfig& steeringConfig = vehicleMovementComponent->SteeringSetup;
	FRichCurve* steeringCurve = steeringConfig.SteeringCurve.GetRichCurve();
	if (steeringCurve) {
		steeringCurve->Reset();
		FKeyHandle key0 = steeringCurve->AddKey(0.0f, 0.85f);
		steeringCurve->SetKeyInterpMode(key0, RCIM_Cubic);
		FKeyHandle key1 = steeringCurve->AddKey(30.0f, 0.70f);
		steeringCurve->SetKeyInterpMode(key1, RCIM_Cubic);
		FKeyHandle key2 = steeringCurve->AddKey(60.0f, 0.55f);
		steeringCurve->SetKeyInterpMode(key2, RCIM_Cubic);
		FKeyHandle key3 = steeringCurve->AddKey(90.0f, 0.45f);
		steeringCurve->SetKeyInterpMode(key3, RCIM_Cubic);
		FKeyHandle key4 = steeringCurve->AddKey(120.0f, 0.20f);
		steeringCurve->SetKeyInterpMode(key4, RCIM_Cubic);
		FKeyHandle key5 = steeringCurve->AddKey(180.0f, 0.10f);
		steeringCurve->SetKeyInterpMode(key5, RCIM_Cubic);
	}

	// WHEEL SETUP

	for (int32 i = 0; i < vehicleMovementComponent->WheelSetups.Num(); i++) {
		FChaosWheelSetup& wheelSetup = vehicleMovementComponent->WheelSetups[i];
		// Validate wheel class exists
		if (!wheelSetup.WheelClass || !IsValid(wheelSetup.WheelClass)) {
			UE_LOG(LogSloopedUpExplorer, Warning, TEXT("WheelSetup[%d] has invalid WheelClass, skipping"), i);
			continue;
		}
		// Get the default object
		UChaosVehicleWheel* originalCDO = Cast<UChaosVehicleWheel>(wheelSetup.WheelClass->GetDefaultObject());
		if (!originalCDO || !IsValid(originalCDO)) {
			UE_LOG(LogSloopedUpExplorer, Warning, TEXT("WheelSetup[%d] has invalid CDO, skipping"), i);
			continue;
		}
		// Swap based on axle type
		FChaosWheelSetup newSetup = wheelSetup;
		if (originalCDO->AxleType == EAxleType::Front) {
			newSetup.WheelClass = FrontWheelClass;
			UE_LOG(LogSloopedUpExplorer, Verbose, TEXT("Swapped front wheel at index %d"), i);
		} else if (originalCDO->AxleType == EAxleType::Rear) {
			newSetup.WheelClass = RearWheelClass;
			UE_LOG(LogSloopedUpExplorer, Verbose, TEXT("Swapped rear wheel at index %d"), i);
		}
		vehicleMovementComponent->WheelSetups[i] = newSetup;
	}

	// Recreate the physics state to apply wheel changes
	if (UPrimitiveComponent* primComp = Cast<UPrimitiveComponent>(vehicleMovementComponent->UpdatedComponent)) {
		primComp->RecreatePhysicsState();
		UE_LOG(LogSloopedUpExplorer, Log, TEXT("Recreated physics state for %s"), *vehicleMovementComponent->GetOwner()->GetName());
	}
}

bool ASloopedUpExplorerSubsystem::BounceFrontHydraulics(UFGWheeledVehicleMovementComponent* vehicleMovementComponent) {
	if (!vehicleMovementComponent || !IsValid(vehicleMovementComponent)) {
		return false;
	}
	UPrimitiveComponent* primComp = Cast<UPrimitiveComponent>(vehicleMovementComponent->UpdatedComponent);
	if (!primComp) {
		return false;
	}
	// Apply an upward impulse at the front wheels to bounce the front of the Explorer
	const float impulseStrength = 220000.0f;
	FVector forwardVector = primComp->GetForwardVector();
	FVector frontOffset = forwardVector * 150.0f;
	primComp->AddImpulseAtLocation(FVector::UpVector * impulseStrength, primComp->GetComponentLocation() + frontOffset, NAME_None);
	return true;
}

bool ASloopedUpExplorerSubsystem::Unstuck(UFGWheeledVehicleMovementComponent* vehicleMovementComponent) {
	if (!vehicleMovementComponent || !IsValid(vehicleMovementComponent)) {
		return false;
	}
	UPrimitiveComponent* primComp = Cast<UPrimitiveComponent>(vehicleMovementComponent->UpdatedComponent);
	if (!primComp) {
		return false;
	}
	// Some wheels must be off the ground (at least 2 out of 4)
	const bool bWheelsInAir = vehicleMovementComponent->mNumWheelsOnGround <= 2;
	// Vehicle must not be moving
	const FVector velocity = primComp->GetPhysicsLinearVelocity();
	const float speedThreshold = 27.78f; // cm/s (~1 km/h)
	const bool bNotMoving = velocity.Size() < speedThreshold;
	// Vehicle must not be rotating
	const FVector angularVelocity = primComp->GetPhysicsAngularVelocityInRadians();
	const float angularThreshold = 0.1f; // radians/sec
	const bool bNotRotating = angularVelocity.Size() < angularThreshold;
	// Check conditions
	if (!bWheelsInAir || !bNotMoving || !bNotRotating) {
		return false;
	}
	// Apply unstuck impulse
	const float impulseStrength = 200000.0f;
	float randomAngle = FMath::FRandRange(0.0f, 2.0f * PI);
	FVector randomDirection = FVector(0.5f * FMath::Cos(randomAngle), 0.5f * FMath::Sin(randomAngle), 1.0f).GetSafeNormal();
	primComp->AddImpulse(randomDirection * impulseStrength, NAME_None);
	return true;
}
