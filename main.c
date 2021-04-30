#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "time.h"

#include "statisc.h"

void main() {

    int iDias;
    int iAtivos;
    FILE* fileLeitura = 0;
    const char* szEnderecoLeitura = "C:\\moneta\\bd.csv";
    clock_t t;
    char c = '%';
    char cAux;

    char version[] = {"1.0.0"};

    t = clock();

    printf("\n\n Modelo Moneta - Versao %s - Rafael Godoy\n",version);

    /* Calcula o tamanho do arquivo CSV (Excel) */
    iQtdArq(fileLeitura, szEnderecoLeitura, &iAtivos, &iDias);

    /* Verifica se o arquivo excel foi encontrado*/
    if (fLeitArqv) {

        /* Aloca o ponteiro de dados com o respectivo tamanho*/
        gmBd = malloc(iAtivos * iDias * sizeof(float));

        /* Grava no ponteiro as informaçoes do arquivo */
        LeituraArq(fileLeitura, szEnderecoLeitura, gmBd, iAtivos, iDias);

        /* Aloca a gmCov de covariancia global*/
        gmCov = malloc(iAtivos * iAtivos * sizeof(float));

        printf("\n Foram detectados %d ativos com %d dias de amostragem.", iAtivos, iDias);

#if 0
        /* Exibe o Banco de dados Lido */
        float value = 0;
        printf("\n\n Banco de Dados:\n");
        for (int i = 0; i < iAtivos; i++) {
            printf("\n");
            for (int j = 0; j < iDias; j++) {
                GetData(gmBd, i, j, iDias, &value);
                printf("%.3f ", value);
            }
        }
        printf("\n\n");
#endif

#if 1
        printf("\n\n O que deseja fazer?");
        printf("\n\n [1] - Carteira Recomentada.");
        printf("\n [2] - Avaliacao de Carteira pre-existente.");
        printf("\n [3] - Montar carteira escolhendo risco.");
        printf("\n\n Digite a opcao desejada: ");
        scanf("%c", &cAux);
#endif

        switch (cAux)
        {
        case '1':          
           
            pesos(iAtivos, iDias, gmBd);
            scanf("%c", &c);
            break;

        case '2':

            PesoSel(iAtivos, iDias, gmBd);
            scanf("%c", &c);
            break;

        case '3':
            RiscoSel(iAtivos, iDias, gmBd);
            scanf("%c", &c);
            break;

        default:
            printf("\n\n Opcao invalida !! ");
            break;
        }

        t = clock() - t;
        double time_taken = ((double)t) / CLOCKS_PER_SEC;

        printf("\n\n Tempo de execucao: %.2f minutos ", time_taken/60);
    }

    scanf("%c", &c);

}
