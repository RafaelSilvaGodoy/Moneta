#include "statisc.h"

float mean(float* pAtivo, int iNumAtiv, int iDias) {

    float sum = 0;
    float value;
    for (int j = 0; j < iDias; j++)
    {
        GetData(pAtivo, iNumAtiv, j, iDias, &value);
        sum += value;
    }

    return (sum / iDias);
}

float DesvPad(float* pesos, int iNumAtiv) {
    
    float value = 0;
    float Aux = 0;

    for (int j = 0; j < iNumAtiv; j++) {

        GetData(gmCov, j, j, iNumAtiv, &value);
        Aux += (float)(pesos[j] * pow(value, 0.5));
    }

    return Aux;

}

float cov(float* pAtivo, int iNumAtiv1, int iNumAtiv2 ,int iDias) {

    float mean1, mean2;
    float Aux = 0;
    float value1 = 0;
    float value2 = 0;

    mean1 = mean(pAtivo, iNumAtiv1, iDias);
    mean2 = mean(pAtivo, iNumAtiv2, iDias);

    for (int j = 0; j < iDias; j++) {

        GetData(pAtivo, iNumAtiv1, j, iDias, &value1);
        GetData(pAtivo, iNumAtiv2, j, iDias, &value2);
        Aux += (value1 - mean1) * (value2 - mean2);
    }

    return (Aux / iDias);
}

void CovMat(int iDias, int iAtiv, float* pAtivo) {

    float value;

    for (int i = 0; i < iAtiv; i++) {
        for (int j = 0; j < iAtiv; j++) {

            value = cov(pAtivo, i, j, iDias);
            PutValue(gmCov, i, j, iAtiv, value);
        }
    }
}

float Risco(float* peso, int iAtivos) {

    float aux = 0;
    float* pAux;
    float sum = 0;
    float value;

    pAux = malloc(iAtivos * 8);

    /* Multiplica 1xn com nxn */
    for (int j = 0; j < iAtivos; j++) {
        for (int i = 0; i < iAtivos; i++) {

            GetData(gmCov, i, j, iAtivos, &value);
            aux += peso[i] * value;
        }
        if (pAux) {
            pAux[j] = aux;
        }
        aux = 0;
    }

    /* Multiplica 1xn com nx1 */
    for (int z = 0; z < iAtivos; z++) {

        if (pAux) {
            sum += peso[z] * pAux[z];
        }
    }

    free(pAux);

    return (1 / sum);
}

float Retorno(float* peso, int iAtivos, int iDias, float* pAtivo) {

    float aux = 0;

    for (int i = 0; i < iAtivos; i++) {
        aux += peso[i] * mean(pAtivo, i, iDias);
    }

    return aux;
}

void TrataResul(float* pesos, int iAtivos, int iDias, float* pAtivo) {

    float ret = Retorno(pesos, iAtivos, iDias, pAtivo);
    float desvPad = DesvPad(pesos, iAtivos);
    float min, max;
    char c = '%';

    min = ret - desvPad;
    max = ret + desvPad;

    printf("\n\n Retorno Esperado da Carteira: %.2f%c", ret, c);
    printf("\n Risco da Carteira: 68,62%c de chance do retorno ficar entre %.2f%c e %.2f%c", c, min, c, max, c);
}
void PesoRand(int* iControle, int iAtivos, float* pesos) {

    float sum = 0;

    for (int i = 0; i < iAtivos; i++) {
        pesos[i] = (float)(rand() % 100);
    }

    for (int i = 0; i < iAtivos; i++) {
        pesos[i] = pesos[i] * iControle[i];
        sum += pesos[i];   
    }

    for (int i = 0; i < iAtivos; i++) {
        pesos[i] = pesos[i]/sum;
    }

}

void pesos(int iAtivos, int iDias, float* pAtivo) {

    float* aux;
    float sum = 0;
    float max = 0;
    long int count = 0;
    long int icount = 0;
    float* pesos;
    int iQtd = 0;
    int* iControle;
    char c = '%';
    int iIterCont = 0;
    float Auxwarning;
    int Auxwarning2;

    aux = malloc(iAtivos * 8);
    pesos = malloc(iAtivos * 8);
    iControle = malloc(iAtivos * 8);

    /* Muda o aleatorio de acordo com o timer do computador*/
    srand((unsigned int)time(0));

    /* Popula a gmCov global*/
    CovMat(iDias, iAtivos, pAtivo);

    printf("\n Calculando Pesos do Portifolio...");

    /* Popula o controle do gerador de numero */
    for (int i = 0; i < iAtivos; i++)
    {        
         iControle[i] = 1;
    }

    Auxwarning = (float) iAtivos * 2;
    while (icount < round(sqrt(Auxwarning))) {
        while (count < iteracaoLocal)
        {
            iIterCont++;

            PesoRand(iControle, iAtivos, aux);
            
#if 0
            printf("\nret = %f  risc = %f", Retorno(aux, iAtivos, iDias, pAtivo), Risco(aux, iAtivos));
#endif

            /*Caulcula o numero com base nos pesos testados*/
            sum += Retorno(aux, iAtivos, iDias, pAtivo) + Risco(aux, iAtivos);

            /* Lógica para armazenar o peso max */
            if (!count && !icount) {
                max = sum;
                for (int i = 0; i < iAtivos; i++) {
                    pesos[i] = aux[i];
                }
            }
            else
            {
                if (sum > max)
                {
                    /* Debug para analisar convergencia */
#ifdef _DEBUG
                    float aux2;

                    aux2 = sum;
                    aux2 -= max;
                    /* A aritmética com o aux2 foi para tirar warning */
                    printf("\n\nNovo Max[%i]: %f  | %f maior que o anterior", iIterCont, sum, aux2);
#endif
                    max = sum;
                    for (int i = 0; i < iAtivos; i++) {
                        pesos[i] = aux[i];
                    }
                }
            }

            sum = 0;
            count++;
        }

        iQtd = 0;

        for (int i = 0; i < iAtivos; i++)
        {
            if (pesos[i] < 0.01)
            {
                iControle[i] = 0;
            }
            iQtd += iControle[i];
        }

        count = 0;
        icount++;
        
        Auxwarning2 = 100 * iIterCont;
       int k = (int)round(Auxwarning2 / (round(sqrt(Auxwarning))*iteracaoLocal));
       printf("\n\n %d%c da compilacao...", k, c);
            
    }

    TrataResul(pesos, iAtivos, iDias, pAtivo);

    printf("\n\n Pesos otimos: ");
    for (int i = 0; i < iAtivos; i++) {
        if (pesos[i]) {
            Auxwarning = pesos[i] * 100;
            printf("\n [%2.2d] %.1f%c ", (i + 1), Auxwarning, c);
        }
    }

}

void PesoSel(int iAtivos, int iDias, float* pAtivo) {

    float* pesos;
    float aux = 0;
    float sum = 0;

    pesos = malloc(iAtivos * 8);

    printf("\n Digite a proporcao do ativo na sua carteira:\n\n");

    for (int i = 0; i < iAtivos; i++)
    {
        printf(" ativo [%2.2i]: ", (i+1));
        scanf("%f", &aux);
        if (pesos) {
            sum += aux;
            pesos[i] = aux/100;
        }
    }

    /* Popula a gmCov global*/
    CovMat(iDias, iAtivos, pAtivo);

    if ((sum > 99) && (sum < 101))
    {
        TrataResul(pesos, iAtivos, iDias, pAtivo);
    } 
    else {
        printf("\n A soma dos pesos nao representa 100.");
    }
}

float* RetExtremos(int iAtivos, int iDias, float* pAtivo, bool fMax) {

    /* Atenção, chamar só depois da matriz cov ser inicializada (CovMat) !!!!!!!!!*/

    float* aux;
    float* pesos;
    float ret = 0;
    float retM = 0;

    aux = malloc(iAtivos * 8);
    pesos = malloc(iAtivos * 8);

    for (int i = 0; i < iAtivos; i++)
    {
        for (int j = 0; j < iAtivos; j++)
        {
            if (aux) aux[j] = 0;
        }

        if (aux) aux[i] = 1;
        

        /*Caulcula o numero com base nos pesos testados*/
        ret = Retorno(aux, iAtivos, iDias, pAtivo);

        /* Lógica para armazenar o peso max */
        if (!i) {
            retM = ret;
            for (int i = 0; i < iAtivos; i++) {
                if (pesos) if(aux) pesos[i] = aux[i];                
            }
        }
        else {

            if (fMax) {
                /* MAX */
                if (ret > retM) {
                    retM = ret;
                    for (int i = 0; i < iAtivos; i++) {
                        pesos[i] = aux[i];
                    }
                }
            }
            else {
                /* MIN */
                if (ret < retM)
                {
                    retM = ret;
                    for (int i = 0; i < iAtivos; i++) {
                        pesos[i] = aux[i];
                    }
                }
            }
        }
    }

    return pesos;
}

void RiscoSel(int iAtivos, int iDias, float* pAtivo) {

    float* aux;
    float* pesos;
    float maxRet = 0;
    float minRet = 0;
    float maxRisc = 0;
    float minRisc = 0;
    char c = '%';
    float ret;
    float AuxWarn;

    aux = malloc(iAtivos * 8);
    pesos = malloc(iAtivos * 8);

    /* Muda o aleatorio de acordo com o timer do computador*/
    srand((unsigned int)time(0));

    /* Popula a gmCov global*/
    CovMat(iDias, iAtivos, pAtivo);   

    /* Calcula máximo retorno */
    aux = RetExtremos(iAtivos, iDias, pAtivo, true);
    maxRet = Retorno(aux, iAtivos, iDias, pAtivo);
    maxRisc = DesvPad(aux, iAtivos);
            
    /* Calcula minimo retorno */
    aux = RetExtremos(iAtivos, iDias, pAtivo, false);
    minRet = Retorno(aux, iAtivos, iDias, pAtivo);
    minRisc = DesvPad(aux, iAtivos);
    

    printf("\n Com base nos ativos fornecidos, e possivel montar uma carteira entre a seguinte faixa de retorno:");
    printf("\n\n Max Retorno: %.2f%c com risco: %.2f%c", maxRet, c, maxRisc, c);
    printf("\n Min Retorno: %.2f%c com risco: %.2f%c", minRet, c, minRisc, c);

    printf("\n\n Digite o valor de retorno desejado para a elaboracao da carteira: ");
    scanf("%f", &ret);

    if (ret > (maxRet - tolerancia * maxRet)) {
        pesos = RetExtremos(iAtivos, iDias, pAtivo, true);
    }
    else if (ret < (minRet + tolerancia * minRet)) {
        pesos = RetExtremos(iAtivos, iDias, pAtivo, false);
    }
    else {
        float sum = 0;
        float max = 0;
        long int count = 0;
        long int icount = 0;
        int* iControle;
        int iIterCont = 0;
        float retfaix = 0;        
        int iAuxWarn;

        iControle = malloc(iAtivos * 8);

        /* Muda o aleatorio de acordo com o timer do computador*/
        srand((unsigned int)time(0));
        printf("\n Calculando Pesos do Portifolio...");

        /* Popula o controle do gerador de numero */
        for (int i = 0; i < iAtivos; i++)
        {
            iControle[i] = 1;
        }
        AuxWarn = (float)iAtivos * 2;
        while (icount < round(sqrt(AuxWarn))) {
            while (count < iteracaoLocal)
            {
                iIterCont++;

                PesoRand(iControle, iAtivos, aux);

                /*Caulcula o numero com base nos pesos testados*/
                retfaix = Retorno(aux, iAtivos, iDias, pAtivo);

                if ((retfaix > ret - ret * tolerancia) && (retfaix < ret + ret * tolerancia)) {
                    sum += Retorno(aux, iAtivos, iDias, pAtivo) + Risco(aux, iAtivos);
                }
                else
                {
                    sum = 0;
                }

                /* Lógica para armazenar o peso max */
                if (!count && !icount) {
                    max = sum;
                    for (int i = 0; i < iAtivos; i++) {
                        pesos[i] = aux[i];
                    }
                }
                else
                {
                    if (sum > max)
                    {
                        max = sum;
                        for (int i = 0; i < iAtivos; i++) {
                            pesos[i] = aux[i];
                        }
                    }
                }

                sum = 0;
                count++;
            }

            count = 0;
            icount++;
            iAuxWarn = 100 * iIterCont;
            int k = (int)round(iAuxWarn / (round(sqrt(AuxWarn)) * iteracaoLocal));
            printf("\n\n %d%c da compilacao...", k, c);
        }
    }

    /* Trata resultados obtidos */
    TrataResul(pesos, iAtivos, iDias, pAtivo);

    printf("\n\n Pesos otimos: ");
    for (int i = 0; i < iAtivos; i++) {
        if (pesos[i]) {
            AuxWarn = pesos[i] * 100;
            printf("\n [%2.2d] %.1f%c ", (i + 1), AuxWarn, c);
        }
    }
}
