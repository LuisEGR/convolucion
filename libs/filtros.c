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

#include "filtros.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"

/*
 * Function:  obtenerMuestrasPasabajasRC
 * --------------------
 * Genera un arreglo de muestras de un fitro pasabajas
 * creado con un circuito RC.
 *
 *  arguments
 *  cantidad:  cantidad de muestras a generar
 *  fc :  frecuencia de corte en hz
 *  fm :  frecuencia de muestreo en hz
 *
 *  returns: estructura ArrayDouble con las muestras del filtro
 */
ArrayDouble obtenerMuestrasPasabajasRC(int cantidad, float fc, float fm) {
  int u;
  float RC = 1 / (2 * M_PI * fc);
  // float RC = 1000;
  float t = 1.0 / fm;
  ArrayDouble muestras = newArrayDouble(cantidad);
  printf("RC: %f ... 1/RC: %f\n", RC, 1 / RC);
  for (int i = 0; i < cantidad; i++) {
    // Respuesta al impulso
    u = i < 0 ? 0 : 1;  // Heviside u(t)
    // muestras.items[i] = (1 / RC) * exp(-(t * i) / RC) * u;
    muestras.items[i] = exp(-(t * i) / RC) * u;
  }
  return muestras;
}
