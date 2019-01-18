/*****************************************************************************\
Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.
This file is licensed under the Snes9x License.
For further information, consult the LICENSE file in the root directory.
\*****************************************************************************/

#include "NostalgiaHeadphones.h"
#include "../snes9x.h"
#include "../apu/apu.h"
#include "wsnes9x.h"
#include <process.h>
#include "dxerr.h"
#include "commctrl.h"
#include <iostream>

/* NostalgiaHeadphones
Implements audio output through XAudio2.
Basic idea: one master voice and one source voice are created;
the source voice consumes buffers queued by PushBuffer, plays them through
the master voice and calls OnBufferEnd after each buffer.
ProcessSound copies new samples into the buffer and queues them for playback.
*/

/*  Construction/Destruction
*/
NostalgiaHeadphones::NostalgiaHeadphones(void)
{
	sum_bufferSize = singleBufferBytes \
		= singleBufferSamples = blockCount = 0;
	soundBuffer = NULL;
	initDone = false;
}

NostalgiaHeadphones::~NostalgiaHeadphones(void)
{
	// TODO
}

/*  NostalgiaHeadphones::SetupSound
applies current sound settings by recreating the voice objects and buffers
-----
returns true if successful, false otherwise
*/
bool NostalgiaHeadphones::SetupSound()
{
	blockCount = 8;
	UINT32 blockTime = GUI.SoundBufferSize / blockCount;

	singleBufferSamples = (Settings.SoundPlaybackRate * blockTime) / 1000;
	singleBufferSamples *= (Settings.Stereo ? 2 : 1);
	singleBufferBytes = singleBufferSamples * (Settings.SixteenBitSound ? 2 : 1);
	sum_bufferSize = singleBufferBytes * blockCount;

	soundBuffer = new uint8[sum_bufferSize];
	writeOffset = 0;
	return true;
}

void NostalgiaHeadphones::SetVolume(double volume)
{
	// TODO
}

bool NostalgiaHeadphones::InitSoundOutput()
{
	// TODO
	initDone = true;
	return true;
}

void NostalgiaHeadphones::DeInitSoundOutput()
{
	// TODO
	if (soundBuffer) {
		delete[] soundBuffer;
		soundBuffer = NULL;
	}
}

/*  NostalgiaHeadphones::ProcessSound
The mixing function called by the sound core when new samples are available.
SoundBuffer is divided into blockCount blocks. If there are enough available samples and a free block,
the block is filled and queued to the source voice. bufferCount is increased by pushbuffer and decreased by
the OnBufferComplete callback.
*/
void NostalgiaHeadphones::ProcessSound()
{
	int freeBytes = (blockCount - bufferCount) * singleBufferBytes;

	if (Settings.DynamicRateControl)
	{
		S9xUpdateDynamicRate(freeBytes, sum_bufferSize);
	}

	S9xFinalizeSamples();

	UINT32 availableSamples;

	availableSamples = S9xGetSampleCount();

	if (Settings.DynamicRateControl)
	{
		// Using rate control, we should always keep the emulator's sound buffers empty to
		// maintain an accurate measurement.
		if (availableSamples > (freeBytes >> (Settings.SixteenBitSound ? 1 : 0)))
		{
			S9xClearSamples();
			return;
		}
	}

	if (!initDone)
		return;

	BYTE * curBuffer;


	while (availableSamples > singleBufferSamples && bufferCount < blockCount) {
		curBuffer = soundBuffer + writeOffset;
		S9xMixSamples(curBuffer, singleBufferSamples);
		ProcessBuffer(singleBufferBytes, curBuffer);
		writeOffset += singleBufferBytes;
		writeOffset %= sum_bufferSize;
		availableSamples -= singleBufferSamples;
	}

}

void NostalgiaHeadphones::ProcessBuffer(UINT32 AudioBytes, BYTE *pAudioData) {
	OutputDebugStringW(L"My output string.");
}