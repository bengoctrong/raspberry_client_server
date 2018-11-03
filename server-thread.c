
//gcc server-thread.c -o servth -lpthread

#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h>     
#include <sys/types.h>  
#include <netinet/in.h> 
#include <pthread.h>    

#define MAXPENDING 5
#define MAXLED 8
#define BUFFSIZE 10
#define PORT 8888
//ham xu ly rieng cho tung client
void *XuLy(void *socket_desc);
//ham thong bao loi
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int servSock, clntSock, indexSock; 
    socklen_t clntLen; 
    char buffer[BUFFSIZE];
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t threadID;
	//khoi tao socket server
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        error("Khoi tao that bai");
    //gan cac gia tri cho bien dia chi server
    memset(&serv_addr, 0, sizeof(serv_addr));       
    serv_addr.sin_family = AF_INET;                
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serv_addr.sin_port = htons(PORT);         
	//lien ket dia chi voi socket server
    if (bind(servSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
	//doi ket noi toi
    if (listen(servSock, MAXPENDING) < 0)
        error("ERROR on listening");
	//gan kich thuoc dia chi client vao bien clntLen
    clntLen = sizeof(cli_addr);

    while(1){
       //chap nhan yeu cau ket noi tu client
       if( (clntSock = accept(servSock, (struct sockaddr*) &cli_addr, &clntLen)) < 0)
            error("accept() failed !");
        //lay thong tin dia chi client
        getpeername(clntSock, (struct sockaddr *) &cli_addr, &clntLen);
		//gan socket client bang indexSock de truyen vao ham XuLy 
		indexSock = clntSock;
		//khoi tao thread ung voi client
        if (pthread_create(&threadID, NULL, XuLy, (void *) &indexSock) != 0)
            error("pthread_create() failed");
		//thong bao co client ket noi toi
        printf("\n+ New client[%d][Addr:%s][Port:%d]\n\n", 
             clntSock,inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
    }
	//dong socket server
    close(servSock);

    return 0; 
}

void *XuLy(void *indexSock){
    static int led[MAXLED] = {0,0,0,0,0,0,0,0}; //khoi tao mang trang thai led
    static int index, kt;
    int clntSock;
    int recvMsgSize; 
    char buffer[BUFFSIZE];
    bzero(buffer,BUFFSIZE);

    clntSock = *((int *) indexSock);
    while(1){
	recvMsgSize = recv(clntSock,buffer,BUFFSIZE,0);
	//2 ung voi laptop
	if(recvMsgSize == 2) {
		//lay du lieu
		int lenh = atoi(buffer);
           	kt = lenh%10;
	    	index = lenh/10;
		//khoi tao gbuffer de gui du lieu
		/*
		du lieu gui la xau xyz, voi x la vi tri led, y la trang thai led, z la bien kiem tra
		*/
		char gbuffer[BUFFSIZE]; 
		if(kt==led[index-1]){
		//ham chuyen cac so thanh 1 string
		sprintf(gbuffer, "%d%d1", index,led[index-1]);
		//gui du lieu ve cho laptop
		send(clntSock,gbuffer,strlen(gbuffer),0);
            	}
		else if(kt==2){
		sprintf(gbuffer, "%d%d2", index,led[index-1]);
		send(clntSock,gbuffer,strlen(gbuffer),0);
		}
		else {
		int tt = led[index-1]+1;
		if (tt>1) tt=0;
		sprintf(gbuffer, "%d%d3", index,tt);
		//gui yeu cau cho esp
		send(4,gbuffer,strlen(gbuffer),0);
		//phan hoi lai laptop
		send(clntSock,gbuffer,strlen(gbuffer),0);
		}
		//xoa du lieu trong buffer
		bzero(buffer,strlen(buffer));
		bzero(gbuffer,strlen(gbuffer));
        } else
	//8 ung voi esp
	if(recvMsgSize == 8) {
		//chuyen buffer nhan duoc thanh so 
		int tt = atoi(buffer);
		//luu va in ra mang trang thai
		printf("----------------------");
		for(int i =7;i>=0;i--) {led[i] = tt%10; tt/=10; }
		for(int i =0;i<8;i++) {printf("led %d =  %d\n", i,led[i]);}
		//xoa du lieu trong buffer
        bzero(buffer,strlen(buffer));}
	else
	bzero(buffer,strlen(buffer));
    }
	//dong client
    close(clntSock);
}


