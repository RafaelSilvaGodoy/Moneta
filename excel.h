#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "stdbool.h"

bool fLeitArqv; // Flag para leitura do excel

extern void GetData(float* matrix, int i, int j, int iCol, float* value);
extern void PutValue(float* matrix, int i, int j, int iCol, float value);
extern void iQtdArq(FILE* file, const char* endereco, int* iRow, int* iCol);
extern void LeituraArq(FILE* file, const char* endereco, float* BD, int iRow, int iCol);
//extern void CriaArq(FILE* file, const char* endereco, Agrupamento* pAgrupamento, int iRow, int iCol, int iGrupos);
