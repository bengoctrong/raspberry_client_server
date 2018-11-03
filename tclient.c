#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h>     
#include <sys/types.h>  
#include <netinet/in.h> 

#define BUFFSIZE 10
#define PORT 8888
//ham thong bao loi
void error(const char *msg){
    perror(msg);
    exit(0);
}
int main(int argc, char **argv){
	int knsock, index, kt;
	struct sockaddr_in serv_addr;
	char gbuffer[BUFFSIZE], nbuffer[BUFFSIZE];
    	char *servIP;
 
	//gan dia chi server
	servIP = argv[1];
	
	//khoi tao socket de ket noi
	if ((knsock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	error("Khoi tao socket that bai");

	//ghi dia chi server
	memset(&serv_addr, 0, sizeof(serv_addr));  
	serv_addr.sin_family      = PF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(servIP);          
    serv_addr.sin_port        = htons(PORT);
	
	//ket noi toi server
	if (connect(knsock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
	error("Ket noi that bai");
	
	//gui tin hieu
	while(1){
	index = -1;
	kt = -1;
	while(index<1 || index>8){
	printf("Nhap led: ");
	scanf("%d",&index);
	fflush(stdin);
	}
	bzero(nbuffer,BUFFSIZE);
	while(kt<0 || kt >3){
	printf("{1} - Bat | {0} - Tat | {2} - Kiem Tra | {3} - Thoat \n");
	scanf("%d",&kt);
	fflush(stdin);	
	}
	if(kt!=3){
	//gui thong tin cho sv
        bzero(gbuffer,BUFFSIZE);
        sprintf(gbuffer, "%d%d", index,kt);
        send(knsock,gbuffer,strlen(gbuffer),0);
	//nhan thong tin tu sv
	while(recv(knsock,nbuffer,BUFFSIZE,0)<=0) {}
	int lenh = atoi(nbuffer);
	char tthai[BUFFSIZE] = "";
	    kt = lenh%10;
	    lenh/=10;
	int tt = lenh%10;
	if(tt==1) strcpy(tthai,"BAT");
	else  strcpy(tthai,"TAT");
	    if(kt==1) printf("Led[%d] dang %s\n",index,tthai);
		
	    if(kt==2) printf("Kiem tra - Led[%d] :  %s\n",index,tthai);
            
	    if(kt==3) printf("Da %s Led[%d]\n",tthai,index);
	    fflush(stdout);
	    bzero(nbuffer,BUFFSIZE);
	}
	else break;
	}
	
    // Dong client
    close(knsock);
	return 0;
}
