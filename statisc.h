#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "stdarg.h"
#include "time.h" 
#include "excel.h"

#ifdef _DEBUG
#define iteracaoLocal pow(10, 5)
#else
#define iteracaoLocal pow(10, 6)
#endif // _DEBUG

#define tolerancia 0.05

float* gmCov; // Matriz global covariancia
float* gmBd;  // Matriz global com o banco de dados

void pesos(int iAtivos, int iDias, float* pAtivo);

void PesoSel(int iAtivos, int iDias, float* pAtivo);

void RiscoSel(int iAtivos, int iDias, float* pAtivo);
