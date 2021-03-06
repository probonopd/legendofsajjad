#include "boss.hpp"

#include "../util/surfaceEditor.hpp"
#include "../util/surfaceCreator.hpp"

#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>

namespace los {
	Boss::Boss(SDL_Renderer *renderer, const signed char ID, const char *spritePath, const float posx, const float posy)
		: IDENTIFIER(ID) {
		SDL_Surface *sprite = IMG_Load(spritePath);
		if (sprite == nullptr)
			std::cerr << "SRC: boss.cpp\tERR: Failed to load sprite " << spritePath << "\n";

		if (sprite->w != 160 || sprite->h != 80)
			std::cerr << "SRC: boss.cpp\tERR: Sprite " << spritePath << " has incorrect dimensions\n";

		SDL_Surface *boss = SurfaceEditor::crop(sprite, 0, 0, 48, 48);
		SDL_Surface *annc = SurfaceEditor::crop(sprite, 0, 48, 160, 32);

		const float anPosx = (800 - 160 * 3) / 2;
		const float anPosy = 600;

		m_boss = new Sprite(renderer, boss, posx, posy, boss->w * 6.0f, boss->h * 6.0f);
		m_announcement = new Sprite(renderer, annc, anPosx, anPosy, annc->w * 3.0f, annc->h * 3.0f);

		m_animStart = SDL_GetTicks();
		m_animFinish = m_animStart + 5000;

		SDL_FreeSurface(sprite);
		SDL_FreeSurface(boss);
		SDL_FreeSurface(annc);
	
		m_health = 400;
		m_invincibleUntil = 0;
	}

	Boss::~Boss() {
		delete m_boss;
		delete m_announcement;
	}

	void Boss::hit() {
		m_health -= 50;
		m_invincibleUntil = SDL_GetTicks() + 1000;
	}

	void Boss::render(SDL_Renderer *renderer) {
		if ((isInvincible() && SDL_GetTicks() % 2 == 0) || !isInvincible())
			m_boss->render(renderer);

		if (SDL_GetTicks() < m_animFinish)
			m_announcement->render(renderer);
	}

	void Boss::update(const double tslf, const float worldX, const float worldY, const float xOff, const float yOff) {
		const float SPEED = 5.0f;
		
		m_boss->move(xOff, yOff);
		
		if (SDL_GetTicks() > m_animStart && SDL_GetTicks() < m_animStart + 1000)
			m_announcement->move(0.0f, static_cast<float>(-tslf * 120));

		if (SDL_GetTicks() > m_animStart + 4000 && SDL_GetTicks() < m_animFinish)
			m_announcement->move(0.0f, static_cast<float>(tslf * 120));
	
		static float xDir = static_cast<float>(rand() % 2);
		static float yDir = static_cast<float>(rand() % 2);
		static Uint32 nextUpdate = SDL_GetTicks() + 3000;

		if (SDL_GetTicks() > nextUpdate) {
			if (rand() % 10 >= 5) {
				nextUpdate = SDL_GetTicks() + 2000;
				xDir = worldX + 1200 - m_boss->getWidthResized() / 2.0f - m_boss->getX();
				yDir = worldY + 900 - m_boss->getHeightResized() / 2.0f - m_boss->getY();

				float pythLength = std::sqrt(xDir * xDir + yDir * yDir);
				xDir /= pythLength;
				yDir /= pythLength;

				xDir *= SPEED;
				yDir *= SPEED;
			} else {
				xDir = static_cast<float>(rand() % 2);
				yDir = static_cast<float>(rand() % 2);
			}
			nextUpdate = SDL_GetTicks() + 3000;
		}

		float bossX = m_boss->getX();
		float bossY = m_boss->getY();
		if (bossX < worldX)
			xDir = SPEED;
		if (bossX > worldX + 2400)
			xDir = -SPEED;
		if (bossY < worldY)
			yDir = SPEED;
		if (bossY > worldY + 1800)
			yDir = -SPEED;

		m_boss->move(static_cast<float>(xDir * tslf * 256), static_cast<float>(yDir * tslf * 256));
	}
};
