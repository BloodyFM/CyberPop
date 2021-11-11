// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomSpawner.h"


// Sets default values
ARoomSpawner::ARoomSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoomSpawner::BeginPlay()
{
	Super::BeginPlay();
	int32 n = FMath::RandRange(0, 4);
	int32 totalRooms = 1;
	float roomDistance = 1142.f;
	for (int32 x = 0; x < 11; x++)
	{
		for (int32 y = 0; y < 11; y++)
		{
			bRooms[x][y] = false;
		}
	}
	bRooms[5][5] = true;
	while (true)
	{
		while (true)
		{
			int32 x = FMath::RandRange(1, 9);
			int32 y = FMath::RandRange(1, 9);
			if (bRooms[x + 1][y] == true || bRooms[x - 1][y] == true || bRooms[x][y + 1] == true || bRooms[x][y - 1] == true) {
				if (bRooms[x][y] != true) {
					bRooms[x][y] = true;
					totalRooms++;
					break;
				}
			}
		}
		if (totalRooms >= 10)
			break;
	}

	while (true)
	{
		int32 x = FMath::RandRange(1, 9);
		int32 y = FMath::RandRange(1, 9);
		if (bRooms[x][y] == true)
		{
			if (x == 5)
			{
				if (y != 4 && y != 5 && y != 6)
				{
					SpawnExit(FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f));
					break;
				}
			}
			else if (y == 5)
			{
				if (x != 4 && x != 5 && x != 6)
				{
					SpawnExit(FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f));
					break;
				}
			}
			else
			{
				SpawnExit(FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f));
				break;
			}
		}
	}

	for (int32 x = 1; x < 10; x++)
	{
		for (int32 y = 1; y < 10; y++)
		{
			if (bRooms[x][y] == true) {
				int32 exits = 0;
				if (bRooms[x + 1][y] == true) {
					exits++;
				}
				if (bRooms[x - 1][y] == true) {
					exits++;
				}
				if (bRooms[x][y + 1] == true) {
					exits++;
				}
				if (bRooms[x][y - 1] == true) {
					exits++;
				}
				switch (exits)
				{
				case 1:
					if (bRooms[x + 1][y]) {
						SpawnRoom(0, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 270.f, 0.f));
					}
					else if (bRooms[x][y + 1]) {
						SpawnRoom(0, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 0.f, 0.f));
					}
					else if (bRooms[x - 1][y]) {
						SpawnRoom(0, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 90.f, 0.f));
					}
					else {
						SpawnRoom(0, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 180.f, 0.f));
					}
					break;
				case 2:
					if (bRooms[x + 1][y]) {
						if (bRooms[x][y + 1]) {
							SpawnRoom(1, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 0.f, 0.f));
						}
						else if (bRooms[x][y - 1]) {
							SpawnRoom(1, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 270.f, 0.f));
						}
						else {
							SpawnRoom(2, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 90.f, 0.f));
						}
					}
					else if (bRooms[x - 1][y]) {
						if (bRooms[x][y + 1]) {
							SpawnRoom(1, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 90.f, 0.f));
						}
						else if (bRooms[x][y - 1]) {
							SpawnRoom(1, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 180.f, 0.f));
						}
					}
					else {
						SpawnRoom(2, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 0.f, 0.f));
					}
					break;
				case 3:
					if (!bRooms[x + 1][y]) {
						SpawnRoom(3, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 180.f, 0.f));
					}
					else if (!bRooms[x][y + 1]) {
						SpawnRoom(3, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 270.f, 0.f));
					}
					else if (!bRooms[x - 1][y]) {
						SpawnRoom(3, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 0.f, 0.f));
					}
					else {
						SpawnRoom(3, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 90.f, 0.f));
					}
					break;
				case 4:
					SpawnRoom(4, FVector((x - 5) * roomDistance, (y - 5) * roomDistance, 0.f), FRotator(0.f, 0.f, 0.f));
					break;
				default:
					break;
				}
			}
		}
	}
}

// Called every frame
void ARoomSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoomSpawner::SpawnRoom_Implementation(int32 roomNr, FVector location, FRotator rotation)
{
}

void ARoomSpawner::SpawnExit_Implementation(FVector location)
{
}