//Bibliotecas
#include<stdio.h>
#include<winsock2.h>
#include <ws2tcpip.h>
#include <time.h>
#include <locale.h>
// ------------------------------------------------------------- 
//Definições para comunicação(espelhadas)
#define BUFF 512  // Tamanho dos buffer
#define PORTA 8888   // A porta de recebimento 
// ------------------------------------------------------------- 

int main()
{
	//Definir a linguagem para portugues do Brasil
	setlocale(LC_ALL, "Portuguese");
	// ------------------------------------------------------------- 
    // variavies para comunicação com o cliente(espelhadas)
    WSADATA wsa;
    SOCKET soc;
    int var = 0;
    // ------------------------------------------------------------- 
    // estruturas para manipular enderecos de internet
    struct sockaddr_in server, si_other;
    int slen = sizeof(si_other); 
    int iResult;                  
    char buf[BUFF];
	// ------------------------------------------------------------- 
    // Inicializa o Winsock
    printf("Iniciando Comunicação...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Comunição falhou com o erro: %d\n",WSAGetLastError());
        system("pause"); exit(EXIT_FAILURE);
    }
    printf("\nInicializado.\n");
    // ------------------------------------------------------------- 
    // pega informacoes da maquina
    char hostname[NI_MAXHOST];        // guarda nome da maquina
    gethostname(hostname,NI_MAXHOST); // pega nome da maquina
    printf("Nome do Host: %s\n",hostname);
    // ------------------------------------------------------------- 
    // pega o ip da maquina e exibe
    PHOSTENT phe = gethostbyname(hostname);
    int i;
    for (i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        printf("IP (%d): %s\n",i, inet_ntoa(addr));

    }
    // ------------------------------------------------------------- 
    // Cria um socket do tipo UDP - SOCK_DGRAM
    if((soc = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
    {
        printf("Erro ao Criar o Socket: %d\n", WSAGetLastError());
        system("pause"); exit(EXIT_FAILURE);
    }
    // ------------------------------------------------------------- 
    // Prepara a estrutura sockaddr_in de endereco local para receber dados
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORTA );
    // ------------------------------------------------------------- 
    // Vincula (bind) o socket ao endereco e porta
    if( bind(soc ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Erro ao criar o Bind : %d\n" , WSAGetLastError());
        system("pause"); exit(EXIT_FAILURE);
    }
    printf("Bind executado\n");
    // ------------------------------------------------------------- 
    // Aguarda a recepcao de dados
    while(1)
    {
        printf("\n");
        printf("Esperando apostador...\n");
        // limpa o buffer
        memset(buf,'\0', BUFF);

        // aguarda ate receber dados de algum cliente
        if ((iResult = recvfrom(soc, buf, BUFF, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
        {
            printf("O servidor falhou com o erro : %d\n" , WSAGetLastError());
            system("pause"); exit(EXIT_FAILURE);
        }
        int aux = 0;
        aux = atoi(buf);
        int aux2 = 0;
        // Mostra o pacote recebido
        printf("Aposta recebida[%s:%d]\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        int i;
        for(i =0;i < aux;i++){
            printf("\nTentativa %i ", i+1);
            srand( (unsigned)time(NULL) );
            var +=  rand() %10;
            printf("Digite o valor de subida do Foguete: %i ao seu valor", var);
            if(var > 8){
                printf("\n O foguete explodiu");
                var = 0;
                aux2 = 1;
                break;
            }
        }
        if(aux2 == 1){// Quando cliente perde sua variavel de valor de limpa 
            char texto[40] = "O fogute explodiu, você perdeu tudo ";
            if (sendto(soc, texto, strlen(texto), 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
            {
                printf("Servidor: sendto() falhou com o erro : %d" , WSAGetLastError());
                system("pause"); exit(EXIT_FAILURE);
            }
        }else{ // Conversão de mensagem para enviar ao cliente 
            char texto[40] = " O foguete subiu, valor agora e: ";
            char aux[40] = "";
            itoa(var, aux, 10);
            strcat(texto, aux);
            if (sendto(soc, texto, strlen(texto), 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
            {
                printf("Servidor: sendto() falhou com o erro : %d" , WSAGetLastError());
                system("pause"); exit(EXIT_FAILURE);
            }
        }
    }
    // ------------------------------------------------------------- 
    // encerra, fecha o socket e o winsock
    printf("\n");
    printf("Fechando socket...\n");
    system("pause");
    closesocket(soc);
    WSACleanup();

    return 0;
}
