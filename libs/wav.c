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

#include "wav.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Function:  newWavHeader
 * --------------------
 * Crea una cabecera para un archivo WAV
 *
 *  arguments
 *  numChannels: Número de canales de audio,
 *               constantes disponibles -> MONO=1, * ESTEREO=2
 *  numMuestras: Número de muestras de audio que se escribirán en el archivo
 *  sampleRate: Frecuencia de muestreo, en Hz, ej: 44100
 *
 *  returns: una estructura de tipo WAVHeader
 *           más detalles sobre sus componentes en wav.h
 */
WAVHeader newWAVHeader(int numChannels, int32_t numMuestras,
                       int32_t sampleRate) {
  WAVHeader wav_header;
  int32_t subchunk2_size;
  int32_t chunk_size;
  subchunk2_size = (numMuestras * numChannels) * BITS_PER_SAMPLE / 8;
  chunk_size = 4 + (8 + SUBCHUNK1SIZE) + (8 + subchunk2_size);
  wav_header.ChunkID[0] = 'R';
  wav_header.ChunkID[1] = 'I';
  wav_header.ChunkID[2] = 'F';
  wav_header.ChunkID[3] = 'F';

  wav_header.ChunkSize = chunk_size;

  wav_header.Format[0] = 'W';
  wav_header.Format[1] = 'A';
  wav_header.Format[2] = 'V';
  wav_header.Format[3] = 'E';

  wav_header.Subchunk1ID[0] = 'f';
  wav_header.Subchunk1ID[1] = 'm';
  wav_header.Subchunk1ID[2] = 't';
  wav_header.Subchunk1ID[3] = ' ';

  wav_header.Subchunk1Size = 16;
  wav_header.AudioFormat = 1;  // PCM
  wav_header.NumChannels = numChannels;
  wav_header.SampleRate = sampleRate;
  wav_header.ByteRate = sampleRate * numChannels * BITS_PER_SAMPLE / 8;
  wav_header.BlockAlign = numChannels * BITS_PER_SAMPLE / 8;
  wav_header.BitsPerSample = BITS_PER_SAMPLE;

  wav_header.Subchunk2ID[0] = 'd';
  wav_header.Subchunk2ID[1] = 'a';
  wav_header.Subchunk2ID[2] = 't';
  wav_header.Subchunk2ID[3] = 'a';
  wav_header.Subchunk2Size = subchunk2_size;

  return wav_header;
}

/*
 * Function:  newMuestraMono
 * --------------------
 * Crea una muestra de 1 canal (Mono)
 *
 *  arguments
 *  valor: Valor de la muestra, double de -1 a 1
 *
 *  returns: una estructura MuestraMono
 */
MuestraMono newMuestraMono(double valor) {
  MuestraMono m;
  m.muestra = valor;
  return m;
}

/*
 * Function:  newMuestraEstereo
 * --------------------
 * Crea una muestra de 2 canales (Estéreo)
 *
 *  arguments
 *  left:  Valor de la muestra en el canal izquierdo, double de -1 a 1
 *  right: Valor de la muestra en el canal derecho, double de -1 a 1
 *
 *  returns: una estructura MuestraEstereo
 */
MuestraEstereo newMuestraEstereo(double left, double right) {
  MuestraEstereo m;
  m.left = left;
  m.right = right;
  return m;
}

/*
 * Function:  readHeaderWAV
 * --------------------
 * Lee la cabecera de un archivo WAV y la guarda en una estructura WAVHeader
 *
 *  arguments
 *  *file_p: Apuntador al archivo en modo lectura o lectura/escritura binaria
 *           modos de apertura compatibles: (rb, r+b, w+b, a+b)
 *
 *  returns: Una estructura WAVHeader con todos los datos de la cabecera
 */
WAVHeader readHeaderWAV(FILE *file_p) {
  WAVHeader wh;
  if (file_p != NULL) {
    size_t byte_read;
    byte_read = fread(&wh, sizeof(wh), 1, file_p);
  }
  return wh;
}

/*
 * Function:  writeWAVHeader
 * --------------------
 * Escribe una cabecera en un archivo WAV
 *
 *  arguments
 *  *file_p: Apuntador al archivo en modo escritura o lectura/escritura binaria
 *           modos de apertura compatibles: (wb, r+b, w+b, a+b)
 *  wav_header: Cabecera que se desea escribir (tipo WAVHeader)
 *
 *  returns: un entero indicando el status, 0 = success, 1 = error
 */
int writeWAVHeader(FILE *file_p, WAVHeader wav_header) {
  if (fwrite(&wav_header, sizeof(WAVHeader), 1, file_p) != 1) {
    printf("\nError al escribir la cabecera del archivo!");
    return 0;
  }
  return 1;
}

/*
 * Function:  printHeaderWAV
 * --------------------
 * imprime el contenido de la cabecera en la terminal
 *
 *  arguments
 *  header: Cabecera que se desea imprimir, tipo WAVHeader
 *
 *  returns: <none>
 */
void printHeaderWAV(WAVHeader header) {
  printf("------- Cabecera WAV -------\n");
  printf("ChunkID:\t%s\n", header.ChunkID);
  printf("ChunkSize:\t%d Bytes\n", header.ChunkSize);
  printf("Format: \t%s (WAVE)\n", header.Format);
  printf("Subchunk1ID:\t%s(fmt)\n", header.Subchunk1ID);
  printf("Subchunk1Size:\t%d\n", header.Subchunk1Size);
  printf("AudioFormat:\t%hu\n", header.AudioFormat);
  printf("NumChannels:\t%hu\n", header.NumChannels);
  printf("SampleRate:\t%d Hz\n", header.SampleRate);
  printf("ByteRate:\t%d\n", header.ByteRate);
  printf("BlockAlign:\t%hu\n", header.BlockAlign);
  printf("BitsPerSample:\t%hu\n", header.BitsPerSample);
  printf("Subchunk2ID:\t%s(data)\n", header.Subchunk2ID);
  printf("Subchunk2Size:\t%d Bytes\n", header.Subchunk2Size);
}

/*
 * Function:  printSamplesWAV
 * --------------------
 * imprime el valor de cada una de las muestras del archivo WAV
 * se detecta automáticamente si el archivo es MONO o ESTEREO
 *
 *  arguments
 *  file_p: Apuntador al archivo en modo lectura o lectura/escritura binaria
 *          modos de apertura compatibles: (rb, r+b, w+b, a+b)
 *  hader:  Cabecera que se desea imprimir, tipo WAVHeader
 *
 *  returns: <none>
 */
void printSamplesWAV(FILE *file_p, WAVHeader header) {
  int numMuestras = header.Subchunk2Size / (header.BitsPerSample / 8);
  int canales = header.NumChannels;
  if (header.NumChannels == MONO) {
    MuestraMono muestra;
    for (int i = 0; i < numMuestras; i++) {
      muestra = readSampleMono(file_p, i);
      printf("\n[%d] %f", i, muestra.muestra);
    }
  } else if (header.NumChannels == ESTEREO) {
    MuestraEstereo muestra;
    for (int i = 0; i < numMuestras; i++) {
      muestra = readSampleEstereo(file_p, i);
      printf("\n[%d] left: %f\tright: %f", i, muestra.left, muestra.right);
    }
  }
  fseek(file_p, 0, SEEK_SET);  // Regreso el puntero a su posición inicial
}

/*
 * Function:  readSampleMono
 * --------------------
 * lee la muestra número index del archivo wav de 1 canal (MONO)
 *
 *  arguments
 *  file_p: Apuntador al archivo en modo lectura o lectura/escritura binaria
 *          modos de apertura compatibles: (rb, r+b, w+b, a+b)
 *  index:  Número de la muestra a leer
 *
 *  returns: Una muestra de 1 canal - tipo MuestraMono
 */
MuestraMono readSampleMono(FILE *file_p, int index) {
  MuestraMono muestra;
  int16_t mread;
  // se coloca el puntero al inicio de la muestra a leer
  fseek(file_p, sizeof(WAVHeader) + index * sizeof(int16_t), SEEK_SET);
  fread(&mread, sizeof(int16_t), 1, file_p);
  muestra.muestra = (double)mread / 32767.0;
  return muestra;
}

/*
 * Function:  readSampleEstereo
 * --------------------
 * lee la muestra número index del archivo wav de 2 canales (ESTEREO)
 *
 *  arguments
 *  file_p: Apuntador al archivo en modo lectura o lectura/escritura binaria
 *          modos de apertura compatibles: (rb, r+b, w+b, a+b)
 *  index:  Número de la muestra a leer
 *
 *  returns: Una muestra de 2 canales - tipo MuestraEstereo
 */
MuestraEstereo readSampleEstereo(FILE *file_p, int index) {
  MuestraEstereo muestra;
  int16_t *mread = malloc(2 * sizeof(int16_t));
  // se coloca el puntero al inicio de la muestra a leer
  fseek(file_p, sizeof(WAVHeader) + index * sizeof(int16_t), SEEK_SET);
  fread(mread, sizeof(int16_t), 2, file_p);

  muestra.left = (double)mread[0] / 32767.0;
  muestra.right = (double)mread[1] / 32767.0;
  return muestra;
}

/*
 * Function:  writeSampleMono
 * --------------------
 * escribe una muestra de 1 canal en el archivo wav
 *
 *  arguments
 *  file_p:  Apuntador al archivo en modo escritura o lectura/escritura binaria
 *           modos de apertura compatibles: (wb, r+b, w+b, a+b)
 *  muestra: Muestra a escribir - tipo MuestraMono
 *
 *  returns: <none>
 */
void writeSampleMono(FILE *file_p, MuestraMono muestra) {
  if (muestra.muestra > 1.0) muestra.muestra = 1.0;
  if (muestra.muestra < -1.0) muestra.muestra = -1.0;
  int16_t mwrite = (int16_t)(muestra.muestra * 32767);
  fwrite(&mwrite, sizeof(int16_t), 1, file_p);
}

void writeSamplesMono(FILE *file_p, ArrayDouble muestras) {
  for (int i = 0; i < muestras.length; i++) {
    MuestraMono muestra = newMuestraMono(muestras.items[i]);
    writeSampleMono(file_p, muestra);
  }
}

/*
 * Function:  writeSampleEstereo
 * --------------------
 * escribe una muestra de 2 canales en el archivo wav
 *
 *  arguments
 *  file_p:  Apuntador al archivo en modo escritura o lectura/escritura binaria
 *           modos de apertura compatibles: (wb, r+b, w+b, a+b)
 *  muestra: Muestra a escribir - tipo MuestraEstereo
 *
 *  returns: <none>
 */
void writeSampleEstereo(FILE *file_p, MuestraEstereo muestra) {
  int16_t mwrite_l = (int16_t)(muestra.left * 32767);
  int16_t mwrite_r = (int16_t)(muestra.right * 32767);
  fwrite(&mwrite_l, sizeof(int16_t), 1, file_p);
  fwrite(&mwrite_r, sizeof(int16_t), 1, file_p);
}
