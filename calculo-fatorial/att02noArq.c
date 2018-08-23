#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int calculo(int numFatoreal){
    int metade, i = 0;
    pid_t filho1, filho2;
    FILE *pont_arq;
    
    //define o que cada filho vai sexecutar
    metade = numFatoreal / 2;
    printf("Filho 1 executara 1 ate o %d\nFilho 2 executara do %d ate o %d \n", metade, metade+1, numFatoreal);
    
    //cria um arquivo temporario para armazenar o resultado
    pont_arq = pont_arq = fopen("result.txt","w");
    
    //criando o filho 1
    filho1 = fork();
    if(filho1 == 0){
        long int fat = 1;
        i = 1;
        //fazendo o calculo fatoreal
        printf("filho 1: ");
        while(i <= metade){
            fat = fat * i;
            //exibe os valores que irao ser calculado
            if(i != metade)
                printf("%d x ", i);
            else
                printf("%d = ", i);
            i++;
        }
        //exibe os resultado do fatoreal
        printf("%li\n", fat);
        //armazena o valor no arquivo temporario
        fprintf(pont_arq, "\n%li", fat);
        //fecha o arquivo temporario
        fclose(pont_arq);
        //finaliza o filho 1
        exit(0);
    }else{
        //criando o filho 2
        filho2 = fork();
        if(filho2 == 0){
            long int fat = 1;
            printf("filho 2: ");
            //faz o calculo fatoreal 
            while(metade+1 <= numFatoreal){
                fat = fat * numFatoreal;
                //exibe os valores que irao ser calculado
                if(numFatoreal-1 != metade)
                    printf("%d x ", numFatoreal);
                else
                    printf("%d = ", numFatoreal);
                numFatoreal--;
            }
            //exibe os valores de fatoreal
            printf("%li\n", fat);
            fprintf(pont_arq, "\n%li", fat);
            fclose(pont_arq);
            
            exit(0);
            
        // o que o pai vai executar depois que os filhos terminhar
        }else{
            //esperando a rexecucao dos filhos
            waitpid(filho1, NULL, 0);
            waitpid(filho2, NULL, 0);
            //return 0;
        }
    }
    
   
}

int main(){
    int numFatoreal = 0;
    char valor[50] = "";
    FILE *pont_arq;
    
    //faz a abertura como leitua do arquivo de entrada 
    pont_arq = fopen("entrada.txt","r");
    
    long int valorint, resultado = 1;
    
    while(numFatoreal == 0){
        
        // caso o arquivo nao foi aberto sera gerado um novo
        if(pont_arq == NULL){
            printf("Nao foi possivel abri o aqurivo ou talves o arquivo nao existe, O arquivo sera gerado\n");
            pont_arq = fopen("entrada.txt","w");
        }else{
            //le o arquvo de entrada, caso ele seja vazio, ele fica aguardado o preenchimento para releitura 
            if( (fgets(valor, sizeof(valor), pont_arq))!=NULL ){
                printf("Valor Informa e: %s\n", valor);
                numFatoreal = atoi(valor);   
                //fecha o arquivo de entrada
                fclose(pont_arq);
                calculo(numFatoreal);
            }else{
                printf("preencha o arquivo entrada.txt, depois Pressione ENTER\n");
                getchar();
            }
            
        }
    }
    
    //abre o arquivo temporario
    pont_arq = fopen("result.txt","a+");
    //le todas a linha 
    while( (fgets(valor, sizeof(valor), pont_arq))!=NULL ){
        //convete o valor lido de string para int
        valorint = atoi(valor);
        //verifica se o valor e = 0, caso seja nao e efetuado a conta
        if(valorint != 0)
            resultado = resultado * valorint;
    }
    //remove o arquivo temporario
    remove("result.txt");
    //imprime o resulrado
    printf("Fatoreal do numero %d: %li\n", numFatoreal, resultado);
}
