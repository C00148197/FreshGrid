#include <iostream>
using namespace std;
#include "stdafx.h"
#ifndef TILE_H
#define TILE_H

class Tile
{
private:
	//double height;
//	double width;
	SDL_Rect r;
	//r.x = 0;
	//r.y = 0;
	//r.w = 50;
	//r.h = 50;
	bool solid;

public:
	Tile();
	Tile(int x, int y, int width, int height, bool wall);
	Tile(int size);
	void render(SDL_Renderer*, int i);
	SDL_Rect getRect();
	bool getSolid();

	//void setHeight(double);
	//void setWidth(double);
	//void setLenght(double);
	//double getVolume();
	//double getSurfaceArea();
};

#endif

//#include <SDL.h>
//using namespace std;
//
//class Tile
//{
//public:
//	Tile(int x, int y, int w, int h) { xPos = x; yPos = y; width = w; height = h; Tile.x = xPos; Tile.y = yPos; Tile.w = width; Tile.h = height; }
//	SDL_Rect Tile;
//
//private:
//	int xPos;
//	int yPos;
//	int width;
//	int height;
//};	
