#include <stdio.h>
#include <windows.h>
#include <math.h>

#pragma comment(lib, "winmm.lib")
#define AUDIO_BITS_PER_SAMPLE 16
#define AUDIO_SAMPLE_RATE 8000
#define TRANSMISSION_START_FREQUENCY 2000
#define BIT_TONE_FREQUENCY_ON 600
#define BIT_TONE_FREQUENCY_OFF 800
#define TRANSMISSION_END_FREQUENCY 1500
#define BIT_TONE_FREQUENCY_NEXT 1200
#define TONE_THRESHOLD 500

DWORD dwGlobal_TransmissionStarted = 0;
DWORD dwGlobal_BitsReceived = 0;
BYTE bGlobal_RecvBits[8];
DWORD dwGlobal_LastToneType = 0;
BYTE bGlobal_AudioBuffer[AUDIO_SAMPLE_RATE * sizeof(WORD)];
double CalculateToneMagnitude(short* pSamples, DWORD dwSampleCount, DWORD dwTargetFrequency)
{
	DWORD dwK = 0;
	double dScalingFactor = 0;
	double dW = 0;
	double dSine = 0;
	double dCosine = 0;
	double dCoeff = 0;
	double dQ0 = 0;
	double dQ1 = 0;
	double dQ2 = 0;
	double dMagnitude = 0;
	dScalingFactor = (double)dwSampleCount / 2.0;
	dwK = (DWORD)(0.5 + (((double)dwSampleCount * (double)dwTargetFrequency) / AUDIO_SAMPLE_RATE));
	dW = (2.0 * 3.14159 * (double)dwK) / (double)dwSampleCount;
	dSine = sin(dW);
	dCosine = cos(dW);
	dCoeff = 2.0 * dCosine;
	for (DWORD i = 0; i < dwSampleCount; i++)
	{
		dQ0 = (dCoeff * dQ1) - dQ2 + pSamples[i];
		dQ2 = dQ1;
		dQ1 = dQ0;
	}
	dMagnitude = (double)sqrtf((float)((dQ1 * dQ1) + (dQ2 * dQ2) - (dQ1 * dQ2 * dCoeff)));
	dMagnitude /= 100;
	return dMagnitude;
}
DWORD ProcessSamples()
{
	BYTE bRecvByte = 0;
	DWORD dwSamplesRemaining = 0;
	DWORD dwCurrSampleIndex = 0;
	DWORD dwCurrChunkSize = 0;
	short* pCurrSamplePtr = NULL;
	double dFrequencySignal_TransmissionStart = 0;
	double dFrequencySignal_TransmissionStart2 = 0;
	double dFrequencySignal_TransmissionEnd = 0;
	double dFrequencySignal_NextBit = 0;
	double dFrequencySignal_BitOn = 0;
	double dFrequencySignal_BitOff = 0;
	double dStrongestTone = 0;
	DWORD dwStrongestToneType = 0;
	dwSamplesRemaining = sizeof(bGlobal_AudioBuffer) / sizeof(WORD);
	dwCurrSampleIndex = 0;
	for (;;)
	{
		if (dwSamplesRemaining == 0)
		{
			break;
		}
		dwCurrChunkSize = (AUDIO_SAMPLE_RATE / 40);
		if (dwSamplesRemaining < dwCurrChunkSize)
		{
			dwCurrChunkSize = dwSamplesRemaining;
		}
		pCurrSamplePtr = (short*)&bGlobal_AudioBuffer[dwCurrSampleIndex * sizeof(WORD)];
		if (dwGlobal_TransmissionStarted == 0)
		{
			dFrequencySignal_TransmissionStart = CalculateToneMagnitude(pCurrSamplePtr, dwCurrChunkSize, TRANSMISSION_START_FREQUENCY);
			if (dFrequencySignal_TransmissionStart >= TONE_THRESHOLD)
			{
				dwGlobal_BitsReceived = 0;
				dwGlobal_LastToneType = TRANSMISSION_START_FREQUENCY;
				dwGlobal_TransmissionStarted = 1;
			}
		}
		else
		{
			dFrequencySignal_TransmissionStart = CalculateToneMagnitude(pCurrSamplePtr, dwCurrChunkSize, TRANSMISSION_START_FREQUENCY);
			dFrequencySignal_BitOn = CalculateToneMagnitude(pCurrSamplePtr, dwCurrChunkSize, BIT_TONE_FREQUENCY_ON);
			dFrequencySignal_BitOff = CalculateToneMagnitude(pCurrSamplePtr, dwCurrChunkSize, BIT_TONE_FREQUENCY_OFF);
			dFrequencySignal_NextBit = CalculateToneMagnitude(pCurrSamplePtr, dwCurrChunkSize, BIT_TONE_FREQUENCY_NEXT);
			dFrequencySignal_TransmissionEnd = CalculateToneMagnitude(pCurrSamplePtr, dwCurrChunkSize, TRANSMISSION_END_FREQUENCY);
			dStrongestTone = 0;
			dwStrongestToneType = 0;
			if (dFrequencySignal_TransmissionStart > dStrongestTone)
			{
				dStrongestTone = dFrequencySignal_TransmissionStart;
				dwStrongestToneType = TRANSMISSION_START_FREQUENCY;
			}
			if (dFrequencySignal_BitOn > dStrongestTone)
			{
				dStrongestTone = dFrequencySignal_BitOn;
				dwStrongestToneType = BIT_TONE_FREQUENCY_ON;
			}
			if (dFrequencySignal_BitOff > dStrongestTone)
			{
				dStrongestTone = dFrequencySignal_BitOff;
				dwStrongestToneType = BIT_TONE_FREQUENCY_OFF;
			}
			if (dFrequencySignal_NextBit > dStrongestTone)
			{
				dStrongestTone = dFrequencySignal_NextBit;
				dwStrongestToneType = BIT_TONE_FREQUENCY_NEXT;
			}
			if (dFrequencySignal_TransmissionEnd > dStrongestTone)
			{
				dStrongestTone = dFrequencySignal_TransmissionEnd;
				dwStrongestToneType = TRANSMISSION_END_FREQUENCY;
			}
			if (dStrongestTone < TONE_THRESHOLD)
			{
				if (dwGlobal_BitsReceived != 0)
				{
					printf("\n 数据损坏 \n");
				}
				dwGlobal_TransmissionStarted = 0;
			}
			else
			{
				if (dwStrongestToneType != dwGlobal_LastToneType)
				{
					if (dwStrongestToneType == TRANSMISSION_START_FREQUENCY)
					{
						if (dwGlobal_BitsReceived != 0)
						{
							printf("\n 数据损坏 \n");
						}

						dwGlobal_TransmissionStarted = 0;
					}
					else if (dwStrongestToneType != BIT_TONE_FREQUENCY_NEXT)
					{
						if (dwStrongestToneType == BIT_TONE_FREQUENCY_ON || dwStrongestToneType == BIT_TONE_FREQUENCY_OFF)
						{
							if (dwGlobal_BitsReceived == 0)
							{
								printf(" 接收数据 \n");
							}
							if (dwStrongestToneType == BIT_TONE_FREQUENCY_ON)
							{
								bGlobal_RecvBits[7 - (dwGlobal_BitsReceived % 8)] = 1;
							}
							else
							{
								bGlobal_RecvBits[7 - (dwGlobal_BitsReceived % 8)] = 0;
							}
							dwGlobal_BitsReceived++;
							if (dwGlobal_BitsReceived % 8 == 0)
							{
								bRecvByte = 0;
								for (DWORD i = 0; i < 8; i++)
								{
									bRecvByte |= (bGlobal_RecvBits[i] << i);
								}
								printf("%c", bRecvByte);
							}
						}
						else if (dwStrongestToneType == TRANSMISSION_END_FREQUENCY)
						{
							if (dwGlobal_BitsReceived != 0)
							{
								printf("\n 传输结束 (已传输 %u 字节) \n", dwGlobal_BitsReceived / 8);
							}
							dwGlobal_TransmissionStarted = 0;
						}
					}
					dwGlobal_LastToneType = dwStrongestToneType;
				}
			}
		}
		dwSamplesRemaining -= dwCurrChunkSize;
		dwCurrSampleIndex += dwCurrChunkSize;
	}
	return 0;
}
int main()
{
	HWAVEIN hWave = NULL;
	WAVEHDR WaveHeaderData;
	WAVEFORMATEX WaveFormatData;
	DWORD dwRetnVal = 0;
	HANDLE hWaveEvent = NULL;
	printf("音频接收器\n\n");
	hWaveEvent = CreateEvent(NULL, 1, 0, NULL);
	if (hWaveEvent == NULL)
	{
		return 1;
	}
	memset((void*)&WaveFormatData, 0, sizeof(WaveFormatData));
	WaveFormatData.wFormatTag = WAVE_FORMAT_PCM;
	WaveFormatData.wBitsPerSample = AUDIO_BITS_PER_SAMPLE;
	WaveFormatData.nChannels = 1;
	WaveFormatData.nSamplesPerSec = AUDIO_SAMPLE_RATE;
	WaveFormatData.nAvgBytesPerSec = AUDIO_BITS_PER_SAMPLE * (AUDIO_BITS_PER_SAMPLE / 8);
	WaveFormatData.nBlockAlign = AUDIO_BITS_PER_SAMPLE / 8;
	WaveFormatData.cbSize = 0;
	if (waveInOpen(&hWave, WAVE_MAPPER, &WaveFormatData, (DWORD)hWaveEvent, 0, CALLBACK_EVENT | WAVE_FORMAT_DIRECT) != MMSYSERR_NOERROR)
	{
		CloseHandle(hWaveEvent);
		return 1;
	}
	for (;;)
	{
		memset((void*)&WaveHeaderData, 0, sizeof(WaveHeaderData));
		WaveHeaderData.lpData = (LPSTR)bGlobal_AudioBuffer;
		WaveHeaderData.dwBufferLength = sizeof(bGlobal_AudioBuffer);
		WaveHeaderData.dwBytesRecorded = 0;
		WaveHeaderData.dwUser = 0;
		WaveHeaderData.dwFlags = 0;
		WaveHeaderData.dwLoops = 0;
		if (waveInPrepareHeader(hWave, &WaveHeaderData, sizeof(WaveHeaderData)) != MMSYSERR_NOERROR)
		{
			CloseHandle(hWaveEvent);
			waveInClose(hWave);
			return 1;
		}
		if (waveInAddBuffer(hWave, &WaveHeaderData, sizeof(WaveHeaderData)) != MMSYSERR_NOERROR)
		{
			CloseHandle(hWaveEvent);
			waveInClose(hWave);
			return 1;
		}
		ResetEvent(hWaveEvent);
		if (waveInStart(hWave) != MMSYSERR_NOERROR)
		{
			CloseHandle(hWaveEvent);
			waveInClose(hWave);

			return 1;
		}
		for (;;)
		{
			WaitForSingleObject(hWaveEvent, INFINITE);
			if (WaveHeaderData.dwFlags & WHDR_DONE)
			{
				break;
			}
		}
		if (waveInUnprepareHeader(hWave, &WaveHeaderData, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
		{
			CloseHandle(hWaveEvent);
			waveInClose(hWave);
			return 1;
		}
		if (ProcessSamples() != 0)
		{
			CloseHandle(hWaveEvent);
			waveInClose(hWave);
			return 1;
		}
	}
	waveInClose(hWave);
	CloseHandle(hWaveEvent);
	return 0;
}
