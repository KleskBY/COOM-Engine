#include "pch.h"
#include "LevelBuilder.h"
#include "render.h"

const int sizeX = 50;
const int sizeY = 50;
const int blockSize = 128;

std::vector<int> maze(sizeX* sizeY, 0);
void connect_cells(int x1, int y1, int x2, int y2)
{
    // Determine the direction of the line
    int dx = (x2 - x1) > 0 ? 1 : -1;
    int dy = (y2 - y1) > 0 ? 1 : -1;

    // Draw the line of red cells
    int x = x1;
    int y = y1;
    while (x != x2 || y != y2)
    {
        int cord = x * sizeX + y;
        maze[cord] = 0;
        if (abs(x2 - x) > abs(y2 - y))
        {
            x += dx;
        }
        else
        {
            y += dy;
        }
    }
}

unsigned seed;
unsigned dumb_rand(unsigned* seed) { return (*seed = (1103515245 * *seed + 12345) % 2147483648); }
#define RAND (dumb_rand(&seed)) // helpers for dumb rand, must have local var called seed for all of these
#define RAND01 ((double)RAND / 2147483648.0) // random float in the range 0-1
#define random_number(lo,hi) ((RAND % (1 + (hi) - (lo))) + (lo)) // random int in the range lo to hi
#define RANDF(lo,hi) (RAND01 * ((hi) - (lo)) + (lo)) // random float in the range lo to hi
#define RANDP(pct) (RAND01 * 100.0 <= (double)(pct)) // random true/false, true pct percent of the time
#define RANDBOOL (RAND % 2 == 0) // randomly true or false 50/50


GAME_LEVEL GenerateGameLevel()
{
    GAME_LEVEL ret = {};

    int startX = random_number(2, sizeX - 2);
    int startY = 1;


    int finishX = random_number(2, sizeX - 2);
    int finishY = sizeY - random_number(2, sizeY / 4);

    

    //Random points to connect through
    std::vector<int> checkpointsX(11);
    std::vector<int> checkpointsY(11);
    checkpointsX[0] = startX;
    checkpointsY[0] = startY + 1;
    for (int i = 1; i < 10; i++)
    {
        checkpointsX[i] = rand() % (sizeX - 2) + 1;
        checkpointsY[i] = rand() % (sizeY - 2) + 1;
    }
    checkpointsX[10] = finishX;
    checkpointsY[10] = finishY - 1;

    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            int cord = x * sizeX + y;
            if (x == startX && y == startY)
            {
                maze[cord] = 2;
                continue;
            }
            if (x == finishX && y == finishY)
            {
                maze[cord] = 2;
                continue;
            }
            if (x == finishX && y == finishY - 1)
            {
                continue;
            }

            if (x == 0 || y == 0 || x == sizeX - 1 || y == sizeY - 1)
            {
                maze[cord] = 1;
            }
            if (random_number(0, 100) > 50)
            {
                maze[cord] = 1;
            }
        }
    }

    for (int i = 0; i < checkpointsX.size() - 1; i++)
    {
        connect_cells(checkpointsX[i], checkpointsY[i], checkpointsX[i + 1], checkpointsY[i + 1]);
    }

    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            if (maze[sizeX * x + y] == 1) printf("X");
            else if (maze[sizeX * x + y] == 2) printf("0");
            else printf(" ");
        }
        printf("\n");
    }

    ret.startX = startX;
    ret.startY = startY;
    ret.data = maze;
    return ret;
}

void DrawGameLevel(ID3D11DeviceContext* context)
{
    GAME_LEVEL lvl = GenerateGameLevel();
    Vector3 cubeSize = Vector3(4.0f, 4.0f, 4.0f);
    for (int x = 0; x < lvl.sizeX; x++)
    {
        for (int y = 0; y < lvl.sizeY; y++)
        {
            if (lvl.data[lvl.sizeX * x + y] == 1)
            {
                GeometricPrimitive::VertexCollection cubeVerts;
                GeometricPrimitive::IndexCollection cubeIndices;
                GeometricPrimitive::CreateBox(cubeVerts, cubeIndices, cubeSize);
                for (auto& it : cubeVerts)
                {
                    it.position.x = it.position.x + x * cubeSize.x;
                    it.position.z = it.position.z + y * cubeSize.x;
                }
                LevelObjects.push_back(GeometricPrimitive::CreateCustom(context, cubeVerts, cubeIndices));

                BoundingBox bb;
                bb.Center = Vector3(x * cubeSize.x, 0, y * cubeSize.x);
                bb.Extents = cubeSize / 2;
                LevelBBoxes.push_back(bb);

            }
            else if (lvl.data[lvl.sizeX * x + y] == 2)
            {
                GeometricPrimitive::VertexCollection cubeVerts;
                GeometricPrimitive::IndexCollection cubeIndices;
                GeometricPrimitive::CreateBox(cubeVerts, cubeIndices, cubeSize / 4.0f);

                for (auto& it : cubeVerts)
                {
                    it.position.x = it.position.x + x * cubeSize.x;
                    it.position.z = it.position.z + y * cubeSize.x;
                }
                LevelObjects.push_back(GeometricPrimitive::CreateCustom(context, cubeVerts, cubeIndices));
            }
        }
    }
    for (int x = 0; x < lvl.sizeX; x++)
    {
        for (int y = 0; y < lvl.sizeY; y++)
        {
            GeometricPrimitive::VertexCollection cubeVerts;
            GeometricPrimitive::IndexCollection cubeIndices;
            GeometricPrimitive::CreateBox(cubeVerts, cubeIndices, cubeSize);
            for (auto& it : cubeVerts)
            {
                it.position.x = it.position.x + x * cubeSize.x;
                it.position.z = it.position.z + y * cubeSize.x;
                it.position.y = it.position.y + -cubeSize.x;
            }
            LevelObjects.push_back(GeometricPrimitive::CreateCustom(context, cubeVerts, cubeIndices));

            BoundingBox bb;
            bb.Center = Vector3(x * cubeSize.x, -cubeSize.x, y * cubeSize.x);
            bb.Extents = cubeSize / 2;
            LevelBBoxes.push_back(bb);
        }
    }
}