#pragma once

void _audio_initializeMixer();

void _audio_loadAndPlay(char* file, int channel);
void _audio_loadAndPlayLoop(char* file, int channel, int loops);

void _audio_stopOnChannel(int channel);

void _audio_cleanup();
