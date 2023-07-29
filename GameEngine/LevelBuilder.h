#pragma once

struct GAME_LEVEL
{
	char name[250];
	int sizeX = 50;
	int sizeY = 50;
	int blockSize = 128;
	std::vector<int> data;
	int startX, startY;
};

GAME_LEVEL GenerateGameLevel();