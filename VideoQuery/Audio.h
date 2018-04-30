#pragma once
#ifndef AUDIO_H
#define AUDIO_H
#include "SDL.h"
#include "SDL_mixer.h"
namespace VideoQuery {
	class Audio {
		public:
			Audio();
			~Audio();
			void LoadAudio(char* filePath);
			void PlayAudio(int frame);
			void PauseAudio();

		private:
			Mix_Chunk* chunk;
			Mix_Chunk* tChunk;
			int channel;
	};


}



#endif