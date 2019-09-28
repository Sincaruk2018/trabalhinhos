/* Projeto de compressão de imagens. Versão 0.2
    Feito por: Luiz Gustavo Sincaruk "Russel" Vieira
    Email: sincaruk@usp.br // sincarukl@gmail.com
    Facebook: /sincaruk

	Projeto feito na ferramenta CodeBlocks, usando o compilador mingw32-gcc-5.1.0
	
	Este programa foi um trabalho da matéria de multimídia na Universidade
	de São Paulo, cujo objetivo era a compressão de imagem com perdas em
	arquivos .bin
	
	
	As imagens suportadas neste programa devem:
	1 - Ser bitmaps de 24 bits
	2 - Ter formato QUADRADO e múltiplo de 8 ( Exemplo: 8x8, 512x512, 1024x1024)
	3 - Estar na mesma pasta do programa
	
	Para a decompressão, é necessário apenas estar em mesma pasta.
	
	É recomendado que faça-se a transformação DCT antes de usar a compressão
	sem perdas.	
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define PI 3.1415926536 // Aproximação razoável de PI

/* Transformação discreta de cossenos. Técnica de compressão com perdas. As variáveis indiceX e indice Y são
para indicar em qual lugar da matriz será feita a operação. Outras funções com as variáveis de mesmo nome
seguem a mesma lógica e propósito.*/
void DCT(int** matrix, int N, float** transformada, int indiceX, int  indiceY)
{
    float n = (float) N;
    int i, j , k, l;
    float soma;
    float dctAux;
    int auxK;
    int auxl;
    int auxX;
    int auxY;
    /* Corresponde à somatória dupla*/
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            soma = 0;
            /* Corresponde à somatória da DCT*/
            for (k = 0; k < N; k++)
            {
                for(l = 0; l < N; l++)
                {
                    auxK = (8*indiceY) + k;
                    auxl = (8* indiceX) + l;
                   dctAux = matrix[auxK][auxl] *cos(((2*k+1)* i * PI) / (2 * n)) * cos(((2 * l + 1)* j * PI) / (2 * n));
                   soma = soma + dctAux;
                }
            }
            auxX = (8*indiceX) + j;
            auxY = (8*indiceY) + i;
           if (i == 0 && j == 0)
           {
               transformada[auxY][auxX] = (sqrt(1)/sqrt(n)) * soma * (sqrt(1)/sqrt(n));
           }
           else if (i == 0)
           {
               transformada[auxY][auxX] = (sqrt(1)/sqrt(n)) * soma * (sqrt(2)/sqrt(n));
           }
           else if ( j == 0)
           {
                transformada[auxY][auxX] = (sqrt(1)/sqrt(n)) * soma * (sqrt(2)/sqrt(n));
           }
           else
           {
                transformada[auxY][auxX] = (sqrt(2)/sqrt(n)) * soma * (sqrt(2)/sqrt(n));

           }
        }
    }
}
/* Quantização. Diminui-se os valores da matriz imagem através de divisão*/
void Quantz (float** first, int** second, int indiceX, int indiceY)
{
    int i,j;
    int auxX;
    int auxY;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            auxX = (8*indiceX) + j;
            auxY = (8*indiceY) + i;
            first[auxY][auxX] = (int)round(first[auxY][auxX]/second[i][j]);
        }
    }
}

/* Reverte-se a quantização. VOlta-se a um valor aproximado do original*/
void deQuant (float** first, int** second, int indiceX, int indiceY)
{
    int i,j;
    int auxX,auxY;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            auxX = (8*indiceX) + j;
            auxY = (8*indiceY) + i;
            first[auxY][auxX] = first[auxY][auxX]*second[i][j];
        }
    }
}

/* IDCT consertada. Desfaz a DCT e retorna matriz aos valores originais. Segue-se o mesmo princípio da operação DCT*/
void IDCT(float** matrix, int N, float** transformada,int indiceX,int indiceY)
{
    int i, j, k, l;
    int auxK;
    int auxl;
    int auxX;
    int auxY;
    float soma;
    float multK;
    float multL;

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            soma = 0;
            for (k = 0; k < 8; k++)
            {
                for (l = 0; l < 8; l++)
                {
                    if (l == 0)
                    {
                        multL = 1 / sqrt(2);
                    }
                    else
                    {
                        multL = 1;
                    }
                    if ( k == 0)
                    {
                        multK = 1 / sqrt(2);
                    }
                    else
                    {
                        multK = 1;
                    }
                    auxK = (8*indiceY) + k;
                    auxl = (8* indiceX) + l;
                    soma = soma + (matrix[auxK][auxl] * cos((2 * i + 1) * k * PI / (2 * N)) * cos((2 * j + 1) * l * PI / (2 * N)) * multK * multL); // Coração da função
                }
            }
            auxX = (8*indiceX) + j;
            auxY = (8*indiceY) + i;
            transformada[auxY][auxX] = soma / 4;
        }
    }
}

/* Subtrai-se todos os números de uma Matriz, de tamanho variável, em 128.*/
void cvs (int** matrix, int altura)
{
    int i,j;
    for (i = 0; i < altura; i++)
    {
        for (j = 0; j < altura; j++)
        {
            matrix[i][j] = matrix[i][j] - 128;
        }
    }
}

int main()
{
    /* declarando tamanho e variaveis*/
    int altura,largura, limite, i, j, k, l, m, n, o, p;
    char buffer[100]; // Header Bitmap
    int tamanhoMatriz = 8;//Blocos 8x8 serão feitos com esta variável como parâmetro

    /* Declarando a matriz de quantização */
    int** quant = (int**) malloc(tamanhoMatriz * sizeof(int*));
    for (i = 0; i < tamanhoMatriz; i++)
    {
        quant[i] = (int*) malloc(tamanhoMatriz* sizeof(int));
    }
    /* Colocando manualmente os elementos na matriz padrão de quantização*/
    quant[0][0] = 16;
    quant[0][1] = 11;
    quant[0][2] = 10;
    quant[0][3] = 16;
    quant[0][4] = 24;
    quant[0][5] = 40;
    quant[0][6] = 51;
    quant[0][7] = 61;
    quant[1][0] = 12;
    quant[1][1] = 12;
    quant[1][2] = 14;
    quant[1][3] = 19;
    quant[1][4] = 26;
    quant[1][5] = 58;
    quant[1][6] = 60;
    quant[1][7] = 55;
    quant[2][0] = 14;
    quant[2][1] = 13;
    quant[2][2] = 16;
    quant[2][3] = 24;
    quant[2][4] = 40;
    quant[2][5] = 57;
    quant[2][6] = 69;
    quant[2][7] = 56;
    quant[3][0] = 14;
    quant[3][1] = 17;
    quant[3][2] = 22;
    quant[3][3] = 29;
    quant[3][4] = 51;
    quant[3][5] = 87;
    quant[3][6] = 80;
    quant[3][7] = 62;
    quant[4][0] = 18;
    quant[4][1] = 22;
    quant[4][2] = 37;
    quant[4][3] = 56;
    quant[4][4] = 68;
    quant[4][5] = 109;
    quant[4][6] = 103;
    quant[4][7] = 77;
    quant[5][0] = 24;
    quant[5][1] = 35;
    quant[5][2] = 55;
    quant[5][3] = 64;
    quant[5][4] = 81;
    quant[5][5] = 104;
    quant[5][6] = 113;
    quant[5][7] = 92;
    quant[6][0] = 49;
    quant[6][1] = 64;
    quant[6][2] = 78;
    quant[6][3] = 87;
    quant[6][4] = 103;
    quant[6][5] = 121;
    quant[6][6] = 120;
    quant[6][7] = 101;
    quant[7][0] = 72;
    quant[7][1] = 92;
    quant[7][2] = 95;
    quant[7][3] = 98;
    quant[7][4] = 112;
    quant[7][5] = 100;
    quant[7][6] = 103;
    quant[7][7] = 99;

    char filename[50];
    char filenameFinal[50];
    while (1) //Loop infinito para que não haja a necessidade de abrir e fechar o programa constantemente
    {
        printf("\n");
        printf("Digite o nome do arquivo a ser manipulado\n");
        scanf("%s",filename);
        printf("\n");
        FILE* arquivo = fopen(filename,"rb");
        /* Programa para automaticamente com um arquivo inválido*/
        if (arquivo == NULL)
        {
            return 1;
        }

        printf("Escolha o que fazer com ele:\n1-Transformar DCT\n2-Transformar IDCT\n3-Comprimir RLE\n4-Decomprimir RLE\n5-Sair\n"); // Tela de opções
        int escolha;
        scanf("%d",&escolha);
        printf("\n");
        /* Pega-se, aqui, o header do bitmap e o tamanho dele*/
        fread(buffer,54,1,arquivo);
        altura = *(int*)&buffer[18];
        largura = altura * 3; // O programa, por enquanto só aceita imagens quadradas, então a largura será sempre esse valor
        limite = altura/8;

        /* Algumas partes estão redundantes, mas permitiram maior controle sob o programa ao consertar certos
        problemas*/
        switch (escolha)
        {
        case 1:
            getchar();
            /* Alocando os canais e a imagem*/
            int** imagem = (int**) malloc(altura * sizeof(int*));
            int** copyImagem = malloc(altura * sizeof(int*));
            for (i = 0; i < altura; i++)
            {
                imagem[i] = (int*) malloc(largura* sizeof(int));
                copyImagem[i] = (int*)malloc(largura * sizeof(int));
            }
            int** R = malloc(altura * sizeof(int*));
            int** G = malloc(altura * sizeof(int*));
            int** B = malloc(altura * sizeof(int*));
            /* Matrizes Float e mais alocação*/
            float** copyR = malloc(altura * sizeof(float*));
            float** copyG = malloc(altura * sizeof(float*));
            float** copyB = malloc(altura * sizeof(float*));
            /* Parque de alocações heap*/
            for (i = 0; i < altura; i++)
            {
                R[i] = (int*) malloc(altura * sizeof(int));
                G[i] =(int*) malloc(altura * sizeof(int));
                B[i] = (int*) malloc(altura * sizeof(int));
                copyR[i] = (float*) malloc(altura * sizeof(float));
                copyG[i] = (float*) malloc(altura * sizeof(float));
                copyB[i] = (float*) malloc(altura * sizeof(float));
            }
            /* Pega-se da imagem produzida e bota em novo arquiVo*/
            for(i = 0; i < altura; i++)
            {
                for (j = 0; j < largura; j++)
                {
                    imagem[i][j] = (int)fgetc(arquivo);
                    /* Para evitar que haja overflow na hora de gravar os valores em int e que
                    pixels rebeldes apareçam*/
                    if (imagem[i][j] > 254)
                    {
                        imagem[i][j] = 254;
                    }
                    if (imagem[i][j] < 1)
                    {
                        imagem[i][j] = 1;
                    }
                }
            }

            /* Separação em 3 canais, armazena-se em 3 matrizes diferentes. Ordem das cores: BGR*/
            i = 0;
            j = 0;
            l = 0;
            k = 0;
            while (k < altura)
            {
                if (j >= altura)
                {
                    j = 0;
                    i++;
                }
                B[i][j] = imagem[k][l];
                j++;
                l = l + 3;
                if (l >= largura)
                {
                    l = l-largura;
                    k++;
                }
            }
            i = 0;
            j = 0;
            l = 1;
            k = 0;
             while (k < altura)
            {
                if (j >= altura)
                {
                    j = 0;
                    i++;
                }
                G[i][j] = imagem[k][l];
                l = l + 3;
                j++;
                if (l >= largura)
                {
                    l = l-largura;
                    k++;
                }
            }
            i = 0;
            j = 0;
            l = 2;
            k = 0;

            while (k < altura)
            {
                if (j >= altura)
                {
                    j = 0;
                    i++;
                }
                R[i][j] = imagem[k][l];
                l = l + 3;
                j++;
                if (l >= largura)
                {
                    l = l-largura;
                    k++;
                }
            }

            cvs(R,altura);
            cvs(G,altura);
            cvs(B,altura);

            /* Transformação discreta e quantização em um mesmo loop duplo*/
            for (i = 0; i < limite; i++)
            {
                for (j = 0; j < limite; j++)
                {
                DCT(B,8,copyB,i,j); // O loop duplo e a DCT fazem com que a eficiência do programa chegue a O(N^6) (!!!)
                DCT(G,8,copyG,i,j);
                DCT(R,8,copyR,i,j);
                Quantz(copyB,quant,i,j);
                Quantz(copyG,quant,i,j);
                Quantz(copyR,quant,i,j);
                }
            }

            /* Junção dos 3 canais em uma única matriz*/
            i = 0;
            j =0;
            l = 0;
            k = 0;
            m = 0;
            n = 0;
            o = 0;
            p = 0;
            int tempo = 0;
            for (i = 0; i < altura; i++)
            {
                for (j = 0; j < largura; j++)
                {
                    tempo = tempo%3;
                    /* // A soma de 128 é com intenção de sumir com valores negativos e evitar "underflows" na hora de armazenar
                    os inteiros. Ou seja, procura-se guardar valores entre 0 a 255 (00 a FF)*/
                    switch (tempo)
                    {
                        case 0:
                            copyImagem[i][j] = (int)round(copyB[k][l])+128;
                            l++;
                            if (l >= altura)
                            {
                                l = 0;
                                k++;
                            }
                            tempo++;
                            break;
                        case 1:
                            copyImagem[i][j] = (int)round(copyG[m][n])+128;
                            n++;
                            if (n >= altura)
                            {
                                n = 0;
                                m++;
                            }
                            tempo++;
                            break;
                        case 2:
                            copyImagem[i][j] = (int)round(copyR[o][p])+128;
                            p++;
                            if (p >= altura)
                            {
                                p = 0;
                                o++;
                            }
                            tempo++;
                            break;
                        default:
                        return 1;
                    }
                }
            }

            /* Sintetizando o arquivo compresso*/
            printf("Digite o nome do arquivo binario:\n");
            scanf("%s",filenameFinal);
            FILE* sintese = fopen(filenameFinal,"wb");
            for (i = 0; i < 54; i++) // Insere-se os 54 bytes do header. Por segurança, serão constantes
            {
                fputc(buffer[i],sintese);
            }
            for (i = 0; i < altura;i++)
            {
                for(j = 0; j < largura; j++)
                {
                    fputc(copyImagem[i][j],sintese); // Arquivo e feito.
                }
            }
            /* Matrizes são liberadas e arquivos fechados.*/
            for (i = 0; i < altura; i++)
            {
                free(R[i]);
                free(G[i]);
                free(B[i]);
                free(imagem[i]);
                free(copyB[i]);
                free(copyR[i]);
                free(copyG[i]);
                free(copyImagem[i]);
            }
            free(R);
            free(G);
            free(B);
            free(imagem);
            free(copyB);
            free(copyG);
            free(copyR);
            free(copyImagem);
            fclose(arquivo);
            fclose(sintese);
            printf("Operacao 1 realizada com sucesso.\n");
            break;
        case 2:
            getchar();
             /* Alocando os proto-canais (matrizes float) e a síntese destes */
            int** compresso = (int**) malloc(altura * sizeof(int*));
            int** sintesseImagem = malloc(altura * sizeof(int*));
            for (i = 0; i < altura; i++)
            {
                compresso[i] = (int*) malloc(largura* sizeof(int));
                sintesseImagem[i] = (int*)malloc(largura * sizeof(int));
            }
            float** protoR = malloc(altura * sizeof(float*));
            float** protoG = malloc(altura * sizeof(float*));
            float** protoB = malloc(altura * sizeof(float*));
            /* Matrizes que resultarão na síntese de valores aproximados ao dos canais originais*/
            float** synthR = malloc(altura * sizeof(float*));
            float** synthG = malloc(altura * sizeof(float*));
            float** synthB = malloc(altura * sizeof(float*));
            /* Parque de alocações*/
            for (i = 0; i < altura; i++)
            {
                protoR[i] = (float*) malloc(altura * sizeof(float));
                protoG[i] =(float*) malloc(altura * sizeof(float));
                protoB[i] = (float*) malloc(altura * sizeof(float));
                synthR[i] = (float*) malloc(altura * sizeof(float));
                synthG[i] = (float*) malloc(altura * sizeof(float));
                synthB[i] = (float*) malloc(altura * sizeof(float));
            }

             /* Pega-se da imagem produzida e coloca-a em novo arquivo*/
            for(i = 0; i < altura; i++)
            {
                for (j = 0; j < largura; j++)
                {
                    compresso[i][j] = (int)fgetc(arquivo) - 128; // Subtração é feita com a intenção de voltar aos valores originais da matriz DCT
                }
            }
            /* Separação em 3 canais. Ordem das cores: BGR */
            i = 0;
            j = 0;
            l = 0;
            k = 0;
            while (k < altura)
            {
                if (j >= altura)
                {
                    j = 0;
                    i++;
                }
                protoB[i][j] = compresso[k][l];
                j++;
                l = l + 3;
                if (l >= largura)
                {
                    l = l-largura;
                    k++;
                }
            }
            i = 0;
            j = 0;
            l = 1;
            k = 0;
             while (k < altura)
            {
                if (j >= altura)
                {
                    j = 0;
                    i++;
                }
                protoG[i][j] = compresso[k][l];
                l = l + 3;
                j++;
                if (l >= largura)
                {
                    l = l-largura;
                    k++;
                }
            }
            i = 0;
            j = 0;
            l = 2;
            k = 0;

            while (k < altura)
            {
                if (j >= altura)
                {
                    j = 0;
                    i++;
                }
                protoR[i][j] = compresso[k][l];
                l = l + 3;
                j++;
                if (l >= largura)
                {
                    l = l-largura;
                    k++;
                }
            }

            /* Dequantizat antes para resultados mais precisos*/
            for (i = 0; i < limite; i++)
            {
                for (j = 0; j < limite; j++)
                {
                    deQuant(protoB,quant,i,j);
                    deQuant(protoG,quant,i,j);
                    deQuant(protoR,quant,i,j);
                    IDCT(protoB,8,synthB,i,j);
                    IDCT(protoG,8,synthG,i,j);
                    IDCT(protoR,8,synthR,i,j);
                }
            }



               /* Junção dos 3 canais em uma única matriz*/
            i = 0;
            j =0;
            l = 0;
            k = 0;
            m = 0;
            n = 0;
            o = 0;
            p = 0;
            int tym = 0;
            for (i = 0; i < altura; i++)
            {
                for (j = 0; j < largura; j++)
                {
                    tym = tym%3;
                    switch (tym)
                    {
                        /*O valor 127 foi escolhido para as somas com a finalidade de evitar overflow e, por consequência,
                        pixels rebeldes*/
                        case 0:
                            sintesseImagem[i][j] = (int)round(synthB[k][l]) + 127;
                            l++;
                            if (l >= altura)
                            {
                                l = 0;
                                k++;
                            }
                            tym++;
                            break;
                        case 1:
                            sintesseImagem[i][j] = (int)round(synthG[m][n]) + 127;
                            n++;
                            if (n >= altura)
                            {
                                n = 0;
                                m++;
                            }
                            tym++;
                            break;
                        case 2:
                            sintesseImagem[i][j] = (int)round(synthR[o][p]) + 127;
                            p++;
                            if (p >= altura)
                            {
                                p = 0;
                                o++;
                            }
                            tym++;
                            break;
                        default:
                        return 1;
                    }
                }
            }

            printf("Digite o nome do arquivo .bmp:\n");
            scanf("%s",filenameFinal);
            printf("\n");
            FILE* imagemFinal = fopen(filenameFinal,"wb");
            for (i = 0; i < 54; i++) // Insere-se os 54 bytes do header. Por segurança, mexer-se-á neles o mínimo possível
            {
                fputc(buffer[i],imagemFinal);
            }
            for (i = 0; i < altura;i++)
            {
                for(j = 0; j < largura; j++)
                {
                    /* Os estatamentos 'if' previnem que pixels rebeldes apareçam, pois é colocado um limite
                    em seus valores*/
                    if (sintesseImagem[i][j] > 254)
                    {
                        sintesseImagem[i][j] = 255;
                    }
                    if (sintesseImagem[i][j] < 0)
                    {
                        sintesseImagem[i][j] = 0;
                    }
                    fputc(sintesseImagem[i][j],imagemFinal);
                }
            }
            /* Fecham-se os arquivos e liberam-se as matrizes*/
            for (i = 0; i < altura; i++)
            {
                free(protoR[i]);
                free(protoG[i]);
                free(protoB[i]);
                free(synthB[i]);
                free(synthR[i]);
                free(synthG[i]);
                free(sintesseImagem[i]);
                free(compresso[i]);
            }
            free(compresso);
            free(protoR);
            free(protoG);
            free(protoB);
            free(synthB);
            free(synthG);
            free(synthR);
            free(sintesseImagem);
            fclose(arquivo);
            fclose(imagemFinal);
            printf("Operacao 2 realizada com sucesso.\n");
            break;
        case 3:
            getchar();
            int proximo;
            fseek(arquivo,0,SEEK_SET); //Como o cabeçalho já foi pego, foi mais simples resetar o marcador pra 0 e  agir como se nenhuma operação fosse realizada.
            printf("Digite o nome do arquivo comprimido a ser gerado:\n");
            scanf("%s",filenameFinal);
            printf("\n");
            FILE* saida = fopen(filenameFinal,"wb");
           int atual = fgetc(arquivo);
           int count = 1;
            while((proximo = fgetc(arquivo))!=EOF)
            {
                if (atual != proximo || count >= 255) //Para evitar Overflow, quando 255 valores repetidos forem encontrados, eles são diretamente armazenados
                {
                    fputc(count,saida);
                    fputc(atual,saida);
                    count = 1;
                }
                else
                {
                    count++;
                }

                atual = proximo;
            }
            fputc(count,saida);
            fputc(atual,saida);
            fclose(arquivo);
            fclose(saida);
            printf("Operacao 3 realizada com sucesso.\n");
            break;
        case 4:
            getchar();
            fseek(arquivo,0,SEEK_SET);
            printf("Digite o nome do .bin decompresso\n");
            scanf("%s",filenameFinal);
            FILE* copia = fopen(filenameFinal,"wb");
            while(1) //Procedimento simples. Atual vê a quantidade, próximo vê o valor.
            {
                atual = fgetc(arquivo);
                proximo = fgetc(arquivo);
                if (atual == EOF) // Faz com que loop 'infinito' pare no fim do arquivo
                {
                    break;
                }
                int i;
                for (i = 0; i < atual; i++)
                {
                    fputc(proximo,copia);
                }
            }
            /* Fecha-se arquivos.*/
            fclose(arquivo);
            fclose(copia);
            printf("Operacao 4 realizada com sucesso.\n");
            break;
        case 5:
            printf("Programa finalizado\n");
            return 0;
        default:
            printf("Digite uma opção válida");
            break;
        }
    }
    return 0;
}

