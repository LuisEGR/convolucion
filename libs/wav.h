/****************************************************************************
Copyright (c) 2017, Luis E. González R., Instituto Politécnico Nacional.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
****************************************************************************/

#ifndef WAV_H
#define WAV_H

#define SUBCHUNK1SIZE (16)
#define AUDIO_FORMAT (1) /*formato PCM*/
#define BITS_PER_SAMPLE (16)
#define MONO (1)
#define ESTEREO (2)

#include <stdint.h>
#include <stdio.h>

#include "estructuras.h"

typedef struct WAVHeader {
  char ChunkID[4];   /*  4 bytes   */
  int32_t ChunkSize; /*  4 bytes   */
  char Format[4];    /*  4 bytes  */

  char Subchunk1ID[4];   /*  4 bytes   */
  int32_t Subchunk1Size; /*  4 bytes  */
  int16_t AudioFormat;   /*  2 bytes   */
  int16_t NumChannels;   /*  2 bytes   */
  int32_t SampleRate;    /*  4 bytes   */
  int32_t ByteRate;      /*  4 bytes   */
  int16_t BlockAlign;    /*  2 bytes   */
  int16_t BitsPerSample; /*  2 bytes   */

  char Subchunk2ID[4];   /*  4 bytes   */
  int32_t Subchunk2Size; /*  4 bytes   */

} WAVHeader;

typedef struct MuestraMono { double muestra; } MuestraMono;

typedef struct MuestraEstereo {
  double left;
  double right;
} MuestraEstereo;

// Creación
WAVHeader newWAVHeader(int numChannels, int32_t numMuestras,
                       int32_t sampleRate);
MuestraMono newMuestraMono(double valor);
MuestraEstereo newMuestraEstereo(double left, double right);

// Escritura
int writeWAVHeader(FILE *file_p, WAVHeader wav_header);
void writeSampleMono(FILE *file_p, MuestraMono muestra);
void writeSampleEstereo(FILE *file_p, MuestraEstereo muestra);

void writeSamplesMono(FILE *file_p, ArrayDouble muestras);

// Lectura
WAVHeader readHeaderWAV(FILE *file_p);
MuestraMono readSampleMono(FILE *file_p, int index);
MuestraEstereo readSampleEstereo(FILE *file_p, int index);

// Mostrar en terminal
void printHeaderWAV(WAVHeader header);
void printSamplesWAV(FILE *file_p, WAVHeader header);

#endif
