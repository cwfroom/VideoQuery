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
	//Console::WriteLine("Audio Length " + chunk->alen);
}

void Audio::PlayAudio(int frame) {
	tChunk->abuf = chunk->abuf + 5880 * frame;
	tChunk->alen = chunk->alen - 5880 * frame;
	tChunk->allocated = chunk->allocated;
	tChunk->volume = chunk->volume;
	channel = Mix_PlayChannel(-1, tChunk, 0);
}

void Audio::PauseAudio() {
	//Mix_Pause(channel);
	Mix_HaltChannel(channel);
}
