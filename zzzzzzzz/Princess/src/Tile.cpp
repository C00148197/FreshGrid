#include "Tile.h"

Tile::Tile()
{
	r.x = 0;
	r.y = 0;
	r.w = 8;
	r.y = 8;
}

Tile::Tile(int x, int y, int width, int height, bool wall)
{
	r.x = x;
	r.y = y;
	r.w = width;
	r.h = height;
	solid = wall;
}

Tile::Tile(int size)
{
	r.w = size;
	r.h = size;
}

SDL_Rect Tile::getRect()
{
	return r;
}

bool Tile::getSolid()
{
	return solid;
}

void Tile::render(SDL_Renderer* rend, int i)
{
	//SDL_RenderClear(rend);

	if (solid == false)
	{
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 200); //black space
		SDL_RenderFillRect(rend, &r);
	}
	else if (solid == true)
	{
		SDL_SetRenderDrawColor(rend, 125, 125, 125, 200); //grey wall
		SDL_RenderFillRect(rend, &r);
	}
	//SDL_RenderPresent(r);
	
}