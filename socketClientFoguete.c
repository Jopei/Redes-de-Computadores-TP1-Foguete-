//Bibliotecas
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <locale.h>
// Definições para comunicação com o server
#define SERVER "127.0.0.1" // IP do servidor UDP
#define BUFF 512          // Tamanho do buffer
#define PORTA 8888       // A porta para envio de dados ao servidor
// -------------------------------------------------------------
int main()
{
	//Set de linguagem para portugues do Brasil 
	setlocale(LC_ALL, "Portuguese");
    // variavies para comunicação
    WSADATA wsa;
    SOCKET soc;
    struct sockaddr_in si_other;
    // -------------------------------------------------------------
    //Variaveis para o envio da mensagem
    int slen=sizeof(si_other); 
    char buf[BUFF];
    char message[BUFF];
	// -------------------------------------------------------------
    // Tratamento do winsock
    printf("Inicializando o Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Cliente: WSAStartup falhou com o erro: %d\n",WSAGetLastError());
        system("pause"); exit(EXIT_FAILURE);
    }
    // -------------------------------------------------------------
    // pega informacoes da maquina para saber o apostador
    char Apostador[NI_MAXHOST];        // Variavel para guarda o nome da maquina 
    gethostname(Apostador,NI_MAXHOST); // Chamando o nome da maquina 
    printf("\nApostador: %s",Apostador);
	// ------------------------------------------------------------- 
    // Criando um socket
    if ( (soc=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        printf("Cliente: erro ao criar o socket %d" , WSAGetLastError());
        system("pause"); exit(EXIT_FAILURE);
    }
    // ------------------------------------------------------------- 
    //Conexão com o endereço remoto 
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORTA);
    si_other.sin_addr.S_un.S_addr = inet_addr(SERVER); 
    // ------------------------------------------------------------- 
    while(1)
    {
        printf("\n");
        //printf("Digite uma mensagem : ");
        printf("Quantas vezes quer apostar na subida: ");
        gets(message);
        // Envia a mensagem para o servidor
        if (sendto(soc, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
        {
            printf("O envio de mensagem falhou com o erro : %d\n" , WSAGetLastError());
            system("pause"); exit(EXIT_FAILURE);
        }       
        // Limpa o buffer 
        memset(buf,'\0', BUFF);
        // aguarda ate receber dados do servidor
        if (recvfrom(soc, buf, BUFF, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)
        {
            printf("A comunicação com o ervidor falhou com o erro : %d\n" , WSAGetLastError());
            system("pause"); exit(EXIT_FAILURE);
        }
        // exibe os detalhes do pacote recebido
        printf("\nA casa de aposta enviou: %s\n",buf);
    }
    // ------------------------------------------------------------- 
    // fecha o socket e o winsock
    printf("\n");
    printf("Fechando socket...\n");
    closesocket(soc);
    WSACleanup();
    system("pause");

    return 0;
}
