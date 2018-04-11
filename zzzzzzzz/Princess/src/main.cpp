#define SDL_MAIN_HANDLED

#include <iostream>

#include "LTimer.h"
#include <chrono>
#include <time.h>
#include "Tile.h"
#include "AStar.h"
#include "NodeLayout.h"
#include <thread>


//Our worker thread function
int worker(void* data);
//Data access semaphore
SDL_sem* gDataLock = NULL;
//The "data buffer"
int gData = -1;
SDL_Rect r;

std::vector<SDL_Rect*> enemyVector;

std::vector<Tile*> tileVector;
Vector velo;

int increment = -1;
int dir = 0;
int GAME_SCALE = 3;

std::vector<Vector> nodePositions;
NodeLayout m_nodeLayout;
int *layout;

#define THREAD_NUM std::thread::hardware_concurrency()

int starDist;

AStar* astar;
std::vector<Node*> m_seekPath;
bool m_targetChosen = false;

int distToTar;
int distToSeekNode;

Vector m_vel;

bool case2 = false;

int cap = 500;

int capChaser = 0;

float calculateMagnitude(Vector vec1, Vector vec2)
{
	return sqrt(((vec2.x - vec1.x) * (vec2.x - vec1.x)) + ((vec2.y - vec1.y) * (vec2.y - vec1.y)));
}

float calculateMagnitude(Vector vec) {
	return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

void normalise(Vector &v) {
	float magnitude = calculateMagnitude(v);

	if (magnitude > 0)
	{
		v.x = v.x / magnitude;
		v.y = v.y / magnitude;
	}
}

void seek(float deltaTime, Vector v, float dist, bool data) {


	if (capChaser < 2)
	{
		capChaser++;
	}

	else
	{

		int temp = calculateMagnitude(v);

		float vX = v.x;
		float vY = v.y;


		vX /= temp;
		vY /= temp;

		if (vX > 0 && vX < 1)
		{
			vX = 1;
		}
		if (vX < 0 && vX > -1)
		{
			vX = -1;
		}

		if (vY > 0 && vY < 1)
		{
			vY = 1;
		}
		if (vY < 0 && vY > -1)
		{
			vY = -1;
		}
	//	cout << vX << endl;

		for (int i = 0; i < enemyVector.size(); i++)
		{
			enemyVector.at(i)->x += vX;
			enemyVector.at(i)->y += vY;
		}

		capChaser = 0;
	}

}


void seekPath(float deltaTime) {

	for (int i = 0; i < enemyVector.size(); i++)
	{


		Vector v1{ r.x,r.y };
		Vector v2{ enemyVector.at(i)->x,  enemyVector.at(i)->y };

		Vector vecToTargets{ v1.x - v2.x, v1.y - v2.y };

		distToTar = calculateMagnitude(vecToTargets);

	

		//// if there are nodes to seek to
		if (!m_seekPath.empty()) {
			// directional vector to next node
			Vector vecToNextPoint = Vector{ m_seekPath.at(0)->getPos().x - v2.x, m_seekPath.at(0)->getPos().y - v2.y };

			// distance to next node
			distToSeekNode = calculateMagnitude(vecToNextPoint);
			cout << m_seekPath.size() << endl;
			// if the next node is closer than the worker
			if (distToSeekNode < distToTar) {
				seek(deltaTime, vecToNextPoint, distToSeekNode, false);
				//cout << distToTar << endl;
				if (distToSeekNode < 1) {
					m_seekPath.erase(m_seekPath.begin());
				}
			}
			// if the worker is closer than the next node
			else {
				seek(deltaTime, vecToTargets, distToTar, true);
			}
		}
		// if there aren't nodes to seek to
		else {
			seek(deltaTime, vecToTargets, distToTar, true);
		}



	}
}





void chooseTarget() 
{
	float closestDistTarget = 99999;

	cout << "choose tar" << endl;
	float dist = calculateMagnitude(Vector{ r.x, r.y }, Vector{ enemyVector.at(0)->x, enemyVector.at(0)->y });

	if (dist < closestDistTarget) 
	{
			starDist = dist;
			m_targetChosen = true;
	}
	
}

void setupSeekPath() {
	for (int j = 0; j < enemyVector.size(); j++)
	{

		int indexClosestToTarget;
		int indexClosestToSelf;

		float closestdistTarget = 99999;
		float closestdistSelf = 99999;

		for (int i = 0; i < m_nodeLayout.getNoOfNodes() - 1; i++) {
			float distTarget = calculateMagnitude(m_nodeLayout.getNodes()[i]->getPos(), Vector{ r.x,r.y });

			if (distTarget < closestdistTarget) {
				closestdistTarget = distTarget;
				indexClosestToTarget = i;
			}

			float distSelf = calculateMagnitude(m_nodeLayout.getNodes()[i]->getPos(), Vector{ enemyVector.at(j)->x, enemyVector.at(j)->y });

			if (distSelf < closestdistSelf) {
				closestdistSelf = distSelf;
				indexClosestToSelf = i;
			}
		}

		if (!m_seekPath.empty()) {
			// if the node that is closest (the destination) to the player has changed
			if (m_nodeLayout.getNodes()[indexClosestToTarget] != m_seekPath.at(m_seekPath.size() - 1)) {
				m_seekPath.clear();
				astar->calculatePath(m_nodeLayout.getNodes()[indexClosestToTarget], m_nodeLayout.getNodes()[indexClosestToSelf], m_seekPath);
			}
		}
		else {
			// create initial path
			astar->calculatePath(m_nodeLayout.getNodes()[indexClosestToTarget], m_nodeLayout.getNodes()[indexClosestToSelf], m_seekPath);
		}

	}
}



int main()
{

	

	SDL_Window* gameWindow = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1028, 1028, SDL_WINDOW_SHOWN);
	//SDL_Renderer* gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_Event *e = new SDL_Event();

	//init semaphore
	gDataLock = SDL_CreateSemaphore(4);

	unsigned int lastTime = 0;
	float deltaTime = 0;
	unsigned int currentTime = 0;
	std::srand(time(NULL));

	cout << "thread stuff " << THREAD_NUM << endl;

	bool debug = false;

	//srand(time(NULL));

	const int SCREEN_FPS = 60;

	const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;


	//The frames per second timer
	LTimer fpsTimer;

	//The frames per second cap timer
	LTimer capTimer;

	//Start counting frames per second
	int countedFrames = 0;

	fpsTimer.start();


	// Setup renderer
	// sets renderer for window
	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);

	r.x = 64;
	r.y = 448;
	r.w = 16;
	r.h = 16;

	//why even

	//nodePositions.push_back(Vector{ 4 *16, 256 }); //top

	//nodePositions.push_back(Vector{ 4* 16, 31*16 }); //bottom wall 1


	//nodePositions.push_back(Vector{ 14 * 16, 8 }); //bottom

	//nodePositions.push_back(Vector{ 14 * 16, 31 * 16 }); //top wall 2

	//nodePositions.push_back(Vector{ 24 * 16, 8 }); //top


	//nodePositions.push_back(Vector{ 31 * 16, 16 * 16 }); //top


	//nodePositions.push_back(Vector{ 24 * 16, 31 * 16 }); //top wall 2

	
	
	////////4444444444444444
	for (int i = 0; i < 256 / 4; i++) 
	{
		for (int j = 0; j < 144 / 4; j++) 
		{
			if (i == 0) {
				tileVector.push_back(new Tile(i * 20, j * 20, 20, 20, true));
				nodePositions.push_back(Vector{ i * 20, j * 20 });
			}
			else if (i == 255 / 4) {
				tileVector.push_back(new Tile( i * 20, j * 20 , 20, 20, true));
				nodePositions.push_back(Vector{ i * 20, j * 20 });
			}
			else if (j == 0) {
				tileVector.push_back(new Tile(i * 20, j * 20 , 20, 20, true));
				nodePositions.push_back(Vector{ i * 20, j * 20 });
			}
			else if (j == 143 / 4) {
				tileVector.push_back(new Tile(i * 20, j * 20 , 20, 20, true));
				nodePositions.push_back(Vector{ i * 20, j * 20 });

			}
			else if (i > 40 / 4 && i <= 55 / 4 && j > 0 / 4 && j <= 125 / 4) {
				tileVector.push_back(new Tile(i * 20, j * 20 , 20, 20, true));
				nodePositions.push_back(Vector{ i * 20, j * 20 });
			}
			else if (i > 110 / 4 && i <= 125 / 4 && j > 10 / 4 && j <= 155 / 4) {
				tileVector.push_back(new Tile(i * 20, j * 20, 20, 20, true));
				nodePositions.push_back(Vector{ i * 20, j * 20 });
			}
			else if (i > 180 / 4 && i <= 195 / 4 && j > 20 / 4 && j <= 110 / 4) {
				tileVector.push_back(new Tile(i * 20, j * 20, 20, 20, true));
				nodePositions.push_back(Vector{ i * 20, j * 20 });
			}
			else {
				nodePositions.push_back(Vector{ i * 20, j * 20 });
				tileVector.push_back(new Tile(i * 20, j * 20, 20, 20, false));
			}
		}
	}

	///////////////444444444444444444444

	int count = 0;
	for (int i = 0; i < 5; i++)
	{
		enemyVector.push_back(new SDL_Rect({ 486, 256, 16, 16 }));
		count = i;
	}

	//cout << "num astar ai/collidable ai " << count << endl;

	m_nodeLayout = nodePositions; //addarcsandstuff
	astar = new AStar(m_nodeLayout);

	//set size and nodes per lines pls they're junk.


	//for (int i = 0; i < 32; i++)
	//{
	//	for (int j = 0; j < 32; j++)
	//	{
	//		bool wall = false;


	//		if (i == 5 && j >= 0 && j < 27)
	//		{
	//			wall = true;
	//		}


	//		if (i == 15 && j > 5 && j < 27)
	//		{
	//			wall = true;
	//		}

	//		if (i == 25 && j > 5 && j < 32)
	//		{
	//			wall = true;
	//		}
	//		
	//		tileVector.push_back(new Tile(i * 16, j * 16, 16, 16, wall));
	//	}
	//}

	/////$$$$$$$$$$$$$$$$
	//Run the threads
	std::srand(SDL_GetTicks());
	SDL_Thread* threadA = SDL_CreateThread(worker, "Thread A", (void*)"Thread A");
	SDL_Delay(16 + rand() % 32);
	SDL_Thread* threadB = SDL_CreateThread(worker, "Thread B", (void*)"Thread B");
	///$$$$$$$$$$$



	while (7 == 7)
	{
		dir = 0;
		velo = Vector{ 0,0 };

		while (SDL_PollEvent(e))
		{
			switch (e->type)
			{
				/* Look for a keypress */
			case SDL_KEYDOWN:
				/* Check the SDLKey values and move change the coords */
				switch (e->key.keysym.sym)
				{
				case SDLK_LEFT:
				
					dir = 4;
					break;
				case SDLK_RIGHT:
					
					dir = 6;
					break;
				case SDLK_UP:
				
					dir = 8;
					break;
				case SDLK_DOWN:
					
					dir = 2;
					break;
				default:
					break;
				}
			}
		}

		if (dir == 2)
		{
			velo.y = 8;
		}
		else if (dir == 8)
		{
			velo.y = -8;
		}
		else if (dir == 4)
		{
			velo.x = -8;
		}
		else if (dir == 6)
		{
			velo.x = 8;
		}


		r.x += velo.x;
		r.y += velo.y;


		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200); //default white

		SDL_RenderClear(renderer);

		for (int i = 0; i < tileVector.size(); i++)
		{
			tileVector.at(i)->render(renderer, i);
		}

		//Calculate and correct fps
		int avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 2000000)
		{
			avgFPS = 0;
		}

		////$$$$$$$$
		if (m_targetChosen == false)
		{
			chooseTarget();
		}
		else
		{
			setupSeekPath();
			seekPath(deltaTime); //this crap should be added to some threads.
		}
		//$$$$$$$

		//Set text to be rendered
		if (avgFPS > 1)
		{
		}
		++countedFrames;

		//If frame finished early
		int frameTicks = capTimer.getTicks();

		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);

			currentTime = SDL_GetTicks();
			if (currentTime > lastTime)
			{
				deltaTime = ((float)(currentTime - lastTime)) / 1000;

				lastTime = currentTime;
			}

			for (int i = 0; i < nodePositions.size(); i++)
			{
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 200); //yellow
				SDL_Rect temp{ nodePositions.at(i).x, nodePositions.at(i).y, 12,12 };
				SDL_RenderFillRect(renderer, &temp);
			}

			SDL_SetRenderDrawColor(renderer, 255, 200, 0, 200); //yellow

			// Render rect
			SDL_RenderFillRect(renderer, &r);

			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 200); //yellow

		

			SDL_RenderFillRect(renderer, enemyVector.at(0));



			//for (int i = 0; i < nodePositions.size(); i++)
			//{
			//	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 200); //yellow
			//	SDL_Rect temp{ nodePositions.at(i).x, nodePositions.at(i).y, 12,12 };
			//	SDL_RenderFillRect(renderer, &temp);
			//}
	/////		SDL_RenderDrawLine(renderer, nodePositions.at(0).x, nodePositions.at(0).y, nodePositions.at(1).x, nodePositions.at(1).y);
			//SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			//SDL_RenderDrawLine(renderer, nodePositions.at(0).x, nodePositions.at(0).y, nodePositions.at(1).x, nodePositions.at(1).y);
			//SDL_RenderDrawLine(renderer, nodePositions.at(1).x, nodePositions.at(1).y, nodePositions.at(2).x, nodePositions.at(2).y);
			//SDL_RenderDrawLine(renderer, nodePositions.at(2).x, nodePositions.at(2).y, nodePositions.at(3).x, nodePositions.at(3).y);
			//SDL_RenderDrawLine(renderer, nodePositions.at(3).x, nodePositions.at(3).y, nodePositions.at(4).x, nodePositions.at(4).y);
			//SDL_RenderDrawLine(renderer, nodePositions.at(4).x, nodePositions.at(4).y, nodePositions.at(5).x, nodePositions.at(5).y);

			////bonus
			//SDL_RenderDrawLine(renderer, nodePositions.at(1).x, nodePositions.at(1).y, nodePositions.at(3).x, nodePositions.at(3).y);
			//SDL_RenderDrawLine(renderer, nodePositions.at(2).x, nodePositions.at(2).y, nodePositions.at(4).x, nodePositions.at(4).y);

	
			//SDL_RenderDrawLine(renderer, nodePositions.at(3).x, nodePositions.at(3).y, nodePositions.at(6).x, nodePositions.at(6).y);
			//SDL_RenderDrawLine(renderer, nodePositions.at(4).x, nodePositions.at(4).y, nodePositions.at(6).x, nodePositions.at(6).y);
			SDL_RenderPresent(renderer);

		//	cout << "test tick" << endl;
		}
	}


	//Wait for threads to finish
	SDL_WaitThread(threadA, NULL);
	SDL_WaitThread(threadB, NULL);

	SDL_RenderPresent(renderer);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(gameWindow);

	IMG_Quit();
	SDL_Quit();

	return 0;
}



void close()
{
	//Free semaphore
	SDL_DestroySemaphore(gDataLock);
	gDataLock = NULL;
}

int worker(void* data)
{
//	srand(SDL_GetTicks());

	while (9 == 9)
	{
		SDL_SemWait(gDataLock);

		increment++;


		if (increment > 2303)
		{
			increment = 0;
		}

		gData = rand() % 256;

		int tempIndex = increment;

		if (tempIndex > 2303) //make a val for this plx
		{
			tempIndex = 0;
		}

		//unlock data
		SDL_SemPost(gDataLock);

		if (tileVector.at(tempIndex)->getSolid())
		{
			SDL_Rect temp = tileVector.at(tempIndex)->getRect();

			if (SDL_HasIntersection(&temp, &r))
			{
				cout << "coll" << endl;
				if (dir == 2)
				{
					r.y -= 8;
				}
				if (dir == 8)
				{
					r.y += 8;
				}
				if (dir == 4)
				{
					r.x += 8;
				}
				if (dir == 6)
				{
					r.x -= 8;
				}
			}

			SDL_Rect resrec;

			for (int i = 0; i < enemyVector.size(); i++)
			{


				if (SDL_IntersectRect(&temp, enemyVector.at(i), &resrec))
				{
					cout << "coll ai " << i << endl;

					if (resrec.x > enemyVector.at(i)->x) //right
					{
						enemyVector.at(i)->x += 16;
					}

					else if (resrec.x < enemyVector.at(i)->x) //leftern
					{
						enemyVector.at(i)->x -= 16;
					}
				}
			} //
		}
	}

	return 0;
}