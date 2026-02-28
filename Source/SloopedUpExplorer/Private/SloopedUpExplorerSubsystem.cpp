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
	FSoftClassPath FrontClassPath("/Script/Engine.BlueprintGeneratedClass'/SloopedUpExplorer/Wheels/SloopedUpExplorer_FrontWheel.SloopedUpExplorer_FrontWheel_C'");
	FSoftClassPath RearClassPath("/Script/Engine.BlueprintGeneratedClass'/SloopedUpExplorer/Wheels/SloopedUpExplorer_RearWheel.SloopedUpExplorer_RearWheel_C'");
	FrontWheelClass = FrontClassPath.TryLoadClass<UChaosVehicleWheel>();
	RearWheelClass = RearClassPath.TryLoadClass<UChaosVehicleWheel>();
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Loaded wheel classes"));
}

void ASloopedUpExplorerSubsystem::TuneExplorer(UFGWheeledVehicleMovementComponent* vehicleMovementComponent) {
	if (!vehicleMovementComponent || !IsValid(vehicleMovementComponent)) {
		return;
	}
	if (!HasAuthority()) {
		return;
	}
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Tuning Explorer movement component"));

	// VEHICLE SETUP

	vehicleMovementComponent->Mass = 700.0f;
	vehicleMovementComponent->CenterOfMassOverride = FVector(5.0f, 0.0f, -20.0f);

	vehicleMovementComponent->DownforceCoefficient = 0.9f;

	// ENGINE CONFIG

	FVehicleEngineConfig& engineConfig = vehicleMovementComponent->EngineSetup;
	engineConfig.MaxTorque = 575.0f;
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
}

void ASloopedUpExplorerSubsystem::SwapExplorerWheels(UFGWheeledVehicleMovementComponent* vehicleMovementComponent) {
	if (!vehicleMovementComponent || !IsValid(vehicleMovementComponent)) {
		return;
	}
	if (!FrontWheelClass || !RearWheelClass) {
		UE_LOG(LogSloopedUpExplorer, Warning, TEXT("Cannot swap Explorer wheels because wheel classes failed to load"));
		return;
	}
	if (!HasAuthority()) {
		return;
	}
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Swapping Explorer wheels"));

	// WHEEL SETUP

	for (int32 i = 0; i < vehicleMovementComponent->WheelSetups.Num(); i++) {
		FChaosWheelSetup& wheelSetup = vehicleMovementComponent->WheelSetups[i];
		if (wheelSetup.WheelClass) {
			UChaosVehicleWheel* originalCDO = Cast<UChaosVehicleWheel>(wheelSetup.WheelClass->GetDefaultObject());
			if (originalCDO) {
				FChaosWheelSetup newSetup = wheelSetup;
				if (originalCDO->AxleType == EAxleType::Front) {
					newSetup.WheelClass = FrontWheelClass;
				} else if (originalCDO->AxleType == EAxleType::Rear) {
					newSetup.WheelClass = RearWheelClass;
				}
				vehicleMovementComponent->WheelSetups[i] = newSetup;
			}
		}
	}
}

void ASloopedUpExplorerSubsystem::BounceFrontHydraulics(UFGWheeledVehicleMovementComponent* vehicleMovementComponent, float impulseStrength) {
	if (!vehicleMovementComponent || !IsValid(vehicleMovementComponent)) {
		return;
	}
	UPrimitiveComponent* primComp = Cast<UPrimitiveComponent>(vehicleMovementComponent->UpdatedComponent);
	if (!primComp) {
		return;
	}

	FVector forwardVector = primComp->GetForwardVector();
	FVector frontOffset = forwardVector * 150.0f;
	primComp->AddImpulseAtLocation(FVector::UpVector * impulseStrength, primComp->GetComponentLocation() + frontOffset, NAME_None);
}
