// Fill out your copyright notice in the Description page of Project Settings.


#include "CardType.h"

const WannabeEnum ECardType::Values = {
	{"BACKPACK", "BACKPACK" },
	{"CRAYON", "CRAYON" },
	{"ERASER", "ERASER"},
	{"GLOBE", "GLOBE"},
	{"GLUE", "GLUE"},
	{"MARKER", "MARKER"},
	{"PENCIL", "PENCIL"},
	{"RULER", "RULER"},
	{"SCISSORS", "SCISSORS"},
};
const FString ECardType::Path = "/Game/Concentration/FrontFaces/";

const WannabeEnum ECardStyle::Values = {
	{"B1", "Blue 1" },
	{"B2", "Blue 2" },
	{"B3", "Blue 3"},
	{"R1", "Red 1"},
	{"R2", "Red 2"},
	{"R3", "Red 3"},
	{"R4", "Red 4"},
	{"R5", "Red 5"},
	{"R6", "Red 6"},
	{"R6", "Red 7"},
};
const FString ECardStyle::Path = "/Game/Concentration/BackFaces/";