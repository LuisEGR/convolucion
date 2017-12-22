/****************************************************************************
Copyright (c) 2017, Luis E. González R., ESCOM, Instituto Politécnico Nacional.

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

// convulución con respuesta impulso del circuito RC
#include <stdio.h>
#include "libs/dsp.h"
#include "libs/filtros.h"
#include "libs/wav.h"
int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Uso: %s <entrada.wav> <salida.wav>\n", argv[0]);
    return 0;
  }
  WAVHeader wh, whNew;
  FILE *entrada, *salida;
  ArrayDouble pasabajas;
  float fc, fm;
  int cantidadMuestrasPasabajas;

  // Abro los archivos de entrada y de salida,
  // si el archivo de salida no existe se crea
  entrada = fopen(argv[1], "rb");
  salida = fopen(argv[2], "wb+");
  if (entrada == NULL || salida == NULL) {
    printf("\nError al abrir el archivo!");
    exit(0);
  }
  // Se lee la cabecera del archivo de entrada
  wh = readHeaderWAV(entrada);
  int cantidadDeMuestras = wh.Subchunk2Size / 2;

  // Se generan las muestras del filtro pasabajas
  fc = 1000.0;   // Frecuencia de corte en hz
  fm = 44100.0;  // Frecuencia de muestreo en hz
  cantidadMuestrasPasabajas = 20;

  fc = 3000;
  fm = 16000;
  cantidadMuestrasPasabajas = 10;

  pasabajas = obtenerMuestrasPasabajasRC(cantidadMuestrasPasabajas, fc, fm);

  // Se imprimen las muestras generadas del filtro pasabajas
  printf("Coeficientes del Filtro Pasabajas RC...\n");
  printArrayDouble(pasabajas);

  // Se realiza la convolución del archivo de entrada con el filtro
  // pasabajas RC y se guarda en un arreglo de doubles
  ArrayDouble conv = convolucionPorFormula(entrada, wh, pasabajas);
  conv = mapArrayDouble(conv, -1, 1);  // Se mapea el arreglo de -1 a 1

  // Se cre una cabecera para el archivo de salida y se escribe
  whNew = newWAVHeader(MONO, conv.length, wh.SampleRate);
  writeWAVHeader(salida, whNew);

  // Se escriben las muestras resultadas de la convolución
  writeSamplesMono(salida, conv);

  // Muestro las cabeceras para comparar
  printf("\nEntrada:\n");
  printHeaderWAV(wh);

  printf("\nSalida:\n");
  printHeaderWAV(whNew);

  // Cierro los archivos
  fclose(entrada);
  fclose(salida);
}
