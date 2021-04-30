#include "excel.h"


extern void GetData(float* matrix, int i, int j, int iCol, float* value)
{
    *value = matrix[i * iCol + j];
}

extern void PutValue(float* matrix, int i, int j, int iCol, float value)
{
    matrix[i * iCol + j] = value;
}

/*===========================================================================*\
 Função   :

 Descrição:

 Entradas : Lembrar de colcoar \\ entre as pastas

 Saídas   :

 Retorno  :
\*===========================================================================*/
extern void iQtdArq(FILE* file, const char* endereco, int* iRow, int* iCol)
{
    char c = 0x00;
    int irow = 0;
    int icol = 0;

    file = fopen(endereco, "r");

    if (file == NULL)
    {
        printf("\n\n Endereco do arquivo bd.csv nao encontrado.");
        printf("\n Certifique-se de que ele se encontra na pasta moneta dentro do diretorio C do seu computador.");
        fLeitArqv = false;
    }
    else {
        fLeitArqv = true;
        do
        {
            c = fgetc(file);

            if (c == '\n')
            {
                irow++;
            }

            if (c == ';')
            {
                icol++;
            }

        } while (c != EOF);

        icol = icol / irow + 1;

        *iRow = irow;
        *iCol = icol;

        fclose(file);
    }
}

/*===========================================================================*\
 Função   :

 Descrição: Captura os dados do excel de numeros inteiros com 6 digitos de precisão e sinal negativo

 Entradas : Lembrar de colcoar \\ entre as pastas

 Saídas   :

 Retorno  :
\*===========================================================================*/
extern void LeituraArq(FILE* file, const char* endereco, float* BD, int iRow, int iCol)
{
    const char* mode = "r";
    char c = 0x00;
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int iAux;
    int dados[8] = { 0 };
    int value = 0;
    bool fNeg = false;
    bool fDec = false;
    bool fZera = false;
    int iDecPos = 0;
    int count;
    float valor;
    file = fopen(endereco, mode);

    if (file) {

        /* Inicializa a gmCov com zeros */
        for (i = 0; i < iRow; i++)
        {
            for (j = 0; j < iCol; j++)
            {
                PutValue(BD, i, j, iCol, 0);
            }
        }

        i = j = 0;
        do
        {
            c = fgetc(file);
            iAux = c + '0';
            iAux -= 96;

            /* Desliga a variável de zerar o i e j*/
            fZera = false;

            if (c == '-') {
                fNeg = true;
                i--;
            }
            if (c == ',') {
                fDec = true;
                iDecPos = i;
                i--;
            }
            if ((iAux >= 0) && (iAux <= 9))
            {
                
                 dados[i] = iAux;
                
                j++;
            }
            else if ((c == '\n') || (c == ';'))
            {
                /* Recebe a posicao da virgula para divisao futura*/
                count = (fDec?(j - iDecPos):0);

                for (j; j > 0; j--)
                {
                    iAux = j - 1;

                    value += (int)(dados[i - j] * pow(10, iAux));

                    if ((j == 1) && (fNeg)) {
                        value *= -1;
                    }

                    if (j == 1) {
                        valor = value / ((float)pow(10, count));                 
                        PutValue(BD, k, l, iCol, valor);
                        fNeg = fDec = false;
                        fZera = true;
                    }
                }
                /* Limpa o vetor para ser usado novamente depois */
                for (int z = 0; z < 8; z++)
                {
                    dados[z] = 0;
                }

                k++;
                if (c == ';')
                {
                    l++;
                    k--;
                }
                else
                {
                    l = 0;
                }
            }
            value = 0;
            i++;
            if (fZera) {
                j = i = 0;
            }
        } while (c != EOF);


        fclose(file);
    }
}

/*===========================================================================*\
 Função   :

 Descrição:

 Entradas : Lembrar de colcoar \\ entre as pastas

 Saídas   :

 Retorno  :
\*===========================================================================*/
/*
extern void CriaArq(FILE* file, const char* endereco, Agrupamento* pAgrupamento, int iRow, int iCol, int iGrupos)
{
    float value;

    file = fopen(endereco, "w");

    fprintf(file, "Item; Grupo\n");
    for (int i = 0; i < iRow; i++)
    {
        fprintf(file, "%d; %2.0d\n", pAgrupamento[i].iIndex, pAgrupamento[i].iGrupo);

    }
    fprintf(file, "\n");
    fprintf(file, "Medioides\n");
    for (int i = 0; i < iGrupos; i++)
    {
        for (int j = 0; j < iCol; j++)
        {
            GetData(pAgrupamento->pMedioides, i, j, iCol, &value);
            fprintf(file, "%5.2f;", value);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
*/
