#pragma once

#include "SDK.hpp"

#include "minhook/MinHook.h"
#pragma comment(lib, "minhook/minhook.lib")

using namespace SDK;

namespace Hooks
{
	bool bIsReady = false;
	bool bHasSpawned = false;
	bool bIsInGame = false;
	bool bHasInitedTheBeacon = false;

	LPVOID(*ProcessEvent)(void*, void*, void*);
	LPVOID ProcessEventHook(UObject* pObject, UFunction* pFunction, LPVOID pParams)
	{
		if (pFunction->GetName().find("BP_PlayButton") != std::string::npos)
		{
			Globals::PC->SwitchLevel(TEXT("Athena_Faceoff"));//Athena_Terrain Athena_Faceoff
			bIsReady = true;
		}

		if (pFunction->GetName().find("ServerPlayEmoteItem"))
		{
			// emote stuff
		}

		if (pFunction->GetName().find("ReceiveTick") != std::string::npos && pObject == Globals::PC && NetHooks::BeaconHost != NULL)
		{
			if (NetHooks::BeaconHost->IsBeaconValid())
			{
				if (NetHooks::BeaconHost->GetNetDriver())
				{
					if (NetHooks::BeaconHost->GetNetDriver()->ClientConnections.Num() != 0)
					{
						auto DeltaSeconds = ((AActor_ReceiveTick_Params*)pParams)->DeltaSeconds;
						NetHooks::NetReplicator->Tick(DeltaSeconds);
					}
				}
			}
		}

		if (pFunction->GetName().find("Tick") != std::string::npos)
		{
			if (GetAsyncKeyState(VK_F1) & 0x1)
			{
				if (!bHasInitedTheBeacon) {
					Replicator::InitOffsets();
					Beacon::InitOffsets();

					NetHooks::Init();
					bHasInitedTheBeacon = true;
				}
			}
			if (GetAsyncKeyState(VK_SPACE) && 0x01) {
				if (Globals::Pawn->CanJump() && !Globals::Pawn->IsJumpProvidingForce()) {
					Globals::Pawn->Jump();
				}
			}

			if(GetAsyncKeyState(VK_SHIFT) & 0x1 && Globals::PC) {
				Globals::Pawn->CurrentMovementStyle = EFortMovementStyle::Sprinting;
			}

			if (GetAsyncKeyState(VK_F2) & 0x1 && Globals::PC)
			{
				if (Globals::PC->Pawn)
				{
					auto Location = Globals::PC->Pawn->K2_GetActorLocation();
					auto NewFortPickup = reinterpret_cast<AFortPickup*>(Util::SpawnActor(AFortPickup::StaticClass(), Location, FRotator()));

					NewFortPickup->PrimaryPickupItemEntry.Count = 1;
					NewFortPickup->PrimaryPickupItemEntry.ItemDefinition = UObject::FindObject<UFortWeaponItemDefinition>("WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03");
					NewFortPickup->OnRep_PrimaryPickupItemEntry();

					NewFortPickup->TossPickup(Location, nullptr, 1, true);

					NetHooks::NetReplicator->Replicate(NewFortPickup); //Replicates the pickup
				}
			}
		}
		if (GetAsyncKeyState(VK_F9) & 0x1 && Globals::PC) {


			SULFUR_LOG("NO CONNECTIONS");
		}
		if (GetAsyncKeyState(VK_F10) & 0x1 && Globals::PC) {


		}
		if (GetAsyncKeyState(VK_F2) & 0x1 && Globals::PC) {

			Globals::PC->Possess(Globals::Pawn);
			SULFUR_LOG("possessed Player!");

		}

		if (GetAsyncKeyState(VK_SHIFT) & 0x1 && Globals::PC) {
			Globals::Pawn->CurrentMovementStyle = EFortMovementStyle::Sprinting;
		}

		if (GetAsyncKeyState(VK_F12) & 0x1 && Globals::PC) {

			Globals::PC->UnPossess();

			SULFUR_LOG("Unpossessed Player!");


		}
		if (GetAsyncKeyState(VK_F6) & 0x1 && Globals::World) {
			Globals::World->GameState[3];
			SULFUR_LOG("Switched GameState");

		}

		if (pFunction->GetName().find("ReadyToStartMatch") != std::string::npos)
		{

			Globals::FortEngine = UObject::FindObject<UFortEngine>("FortEngine_");
			Globals::World = Globals::FortEngine->GameViewport->World;
			Globals::PC = reinterpret_cast<AFortPlayerController*>(Globals::FortEngine->GameInstance->LocalPlayers[0]->PlayerController);

			if (!bHasSpawned) {
				Globals::Pawn = reinterpret_cast<PLAYER_CLASS*>(Util::SpawnActor(PLAYER_CLASS::StaticClass(), FVector(1000, 200, 0), FRotator())); // Terrian [1000] (z)

				auto PlayerState = reinterpret_cast<PLAYER_STATE_CLASS*>(Globals::PC->PlayerState);
			//	Globals::Pawn->Mesh->SetSkeletalMesh(UObject::FindObject<USkeletalMesh>("F_SML_Starter_Epic.F_SML_Starter_Epic"), true); //F_SML_Starter_Epic.F_SML_Starter_Epic and  M_SML_Edgar.M_SML_Edgar


				Globals::PC->Possess(Globals::Pawn);
				Globals::Pawn->EquipWeaponDefinition(UObject::FindObject<UFortWeaponItemDefinition>("WID_Harvest_Pickaxe_Teslacoil_Athena.WID_Harvest_Pickaxe_Teslacoil_Athena"), FGuid());

				bIsReady = true;
				bIsInGame = true;
				bHasSpawned = true;

				

				Globals::Pawn->ServerChoosePart(EFortCustomPartType::Head, UObject::FindObject<UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1"));
				Globals::Pawn->ServerChoosePart(EFortCustomPartType::Body, UObject::FindObject<UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01"));
				PlayerState->OnRep_CharacterParts();

				Globals::PC->ServerReadyToStartMatch();
				static_cast<AGameMode*>(Globals::World->AuthorityGameMode)->StartMatch();

			}
		}

		if (pFunction->GetName().find("LoadingScreenDropped") != std::string::npos)
		{
		}

		if (pFunction->GetName().find(" IsShowingInitilLoadignScreen") != std::string::npos)
		{
			return 0;
		}

		if (pFunction->GetName().find("ServerShortTimeout") != std::string::npos) {
			return NULL;
		}


		return ProcessEvent(pObject, pFunction, pParams);
	}



	static void Init()
	{
		auto FEVFT = *reinterpret_cast<void***>(Globals::FortEngine);
		auto PEAddr = FEVFT[64];

		MH_CreateHook(reinterpret_cast<LPVOID>(PEAddr), ProcessEventHook, reinterpret_cast<LPVOID*>(&ProcessEvent));
		MH_EnableHook(reinterpret_cast<LPVOID>(PEAddr));


	}
}