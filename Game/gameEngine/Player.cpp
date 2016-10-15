#include "player.h"

Player::Player()
{

	
	playerImage = "dot.png";
	posX = 0;
	posY = 0;
	velX = 0;
	velY = 0;


	playerCollider.w = WIDTH;
	playerCollider.h = HEIGHT;
}


Player::~Player()
{
}


void Player::renderPlayer(SDL_Texture* tex, SDL_Renderer* ren, int posX, int posY) {
	SDL_Rect dst;
	dst.w = WIDTH;
	dst.h = HEIGHT;
	dst.x = posX;
	dst.y = posY;

	SDL_RenderCopy(ren, tex, NULL, &dst);
}

void Player::handleEvents(SDL_Event evnt) {

	if (evnt.type == SDL_KEYDOWN && evnt.key.repeat == 0) {

		switch (evnt.key.keysym.scancode) {

		case SDL_SCANCODE_LEFT:
			velX -= maxSpeed;
			break;

		case SDL_SCANCODE_RIGHT:
			velX += maxSpeed;
			break;

		case SDL_SCANCODE_UP:
			velY -= maxSpeed;
			break;

		case SDL_SCANCODE_DOWN:
			velY += maxSpeed;
			break;
		}
	}

	if (evnt.type == SDL_KEYUP) {
		switch (evnt.key.keysym.scancode) {

		case SDL_SCANCODE_LEFT:
			if (velX < 0) {
				velX = 0;
			}
			break;

		case SDL_SCANCODE_RIGHT:
			if (velX > 0) {
				velX = 0;
			}
			break;


		case SDL_SCANCODE_UP:
			if (velY < 0) {
				velY = 0;
			}
			break;

		case SDL_SCANCODE_DOWN:
			if (velY > 0) {
				velY = 0;
			}
			break;
		}
	}
}

void Player::move(SDL_Rect wall) {

	posX += velX;
	playerCollider.x = posX;

	if (posX < 0) {
		posX = 0;
		playerCollider.x = posX;
	}
	if (posX > game.SCREEN_WIDTH - WIDTH) {
		posX = game.SCREEN_WIDTH - WIDTH;
		playerCollider.x = posX;
	}

	posY += velY;
	playerCollider.y = posY;

	if (posY < 0) {
		posY = 0;
		playerCollider.y = posY;
	}
	if (posY > game.SCREEN_HEIGHT - HEIGHT) {
		posY = game.SCREEN_HEIGHT - HEIGHT;
		playerCollider.y = posY;
	}

	if (checkCollision(playerCollider, wall)) {
		posX -= velX;
		posY -= velY;

		playerCollider.x = posX;
		playerCollider.y = posY;
	}

}


bool Player::checkCollision(SDL_Rect a, SDL_Rect b) {
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int botA, botB;

	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	botA = a.y + a.h;

	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	botB = b.y + b.h;

	if (botA <= topB) {
		return false;
	}
	if (topA >= botB) {
		return false;
	}
	if (rightA <= leftB) {
		return false;
	}
	if (leftA >= rightB) {
		return false;
	}

	return true;
}