#include "Audio.h"
using namespace VideoQuery;
using namespace System;

Audio::Audio() {
	tChunk = new Mix_Chunk;
}

Audio::~Audio() {
	Mix_FreeChunk(chunk);
	Mix_FreeChunk(tChunk);
}

void Audio::LoadAudio(char* filePath) {
	chunk = Mix_LoadWAV(filePath);
}

void Audio::PlayAudio(int frame) {
	tChunk->abuf = chunk->abuf + 2940 * frame;
	tChunk->alen = chunk->alen - 2940 * frame;
	tChunk->allocated = chunk->allocated;
	tChunk->volume = chunk->volume;
	channel = Mix_PlayChannel(-1, tChunk, 0);
}

void Audio::PauseAudio() {
	Mix_Pause(channel);
}
