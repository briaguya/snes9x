/*****************************************************************************\
Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.
This file is licensed under the Snes9x License.
For further information, consult the LICENSE file in the root directory.
\*****************************************************************************/

#ifndef NOSTALGIAHEADPHONES_H
#define NOSTALGIAHEADPHONES_H
#include "../snes9x.h"
#include <windows.h>
#include "IS9xSoundOutput.h"

class NostalgiaHeadphones : public IS9xSoundOutput
{
private:
	volatile LONG bufferCount;				// currently submitted XAudio2 buffers

	UINT32 sum_bufferSize;					// the size of soundBuffer
	UINT32 singleBufferSamples;				// samples in one block
	UINT32 singleBufferBytes;				// bytes in one block
	UINT32 blockCount;						// soundBuffer is divided into blockCount blocks
											// currently set to 4

	UINT32 writeOffset;						// offset into the buffer for the next block
	uint8 *soundBuffer;						// the buffer itself
	bool initDone;							// has init been called successfully?

public:
	NostalgiaHeadphones(void);
	~NostalgiaHeadphones(void);

	void ProcessBuffer(UINT32, BYTE*);

	// Inherited from IS9xSoundOutput
	bool InitSoundOutput(void);
	void DeInitSoundOutput(void);
	bool SetupSound(void);
	void SetVolume(double volume);
	void ProcessSound();
};

#endif
