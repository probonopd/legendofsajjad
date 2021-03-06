#pragma once

#include "gfx/sprite.hpp"

#include <SDL2/SDL.h>

namespace los {
	class UI {
		public:
			UI(SDL_Renderer*);
			~UI();
		public:
			void render(SDL_Renderer*);
		private:
			short m_penguinDeaths = 0;
		public:
			void setPlayerHealth(signed short h);
			short getPlayerHealth();
		public:
			void setPenguinDeaths(signed short i);
			short getPenguinDeaths();
		private:
			Sprite *m_heart;
		private:
			signed short m_heartCount = 8;
			signed short m_playerHealth = 8 * 4;
	};
};
