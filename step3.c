#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#define max 2048

int main()
{
	int socket1;
	struct sockaddr_in serveraddr;
	struct hostent *hostt;
	struct in_addr **address_list;
	char Sreply[max],ip[50],message[max];
	int n,i=0,total_len=0;
	FILE *f;
	char hostname[50];
	char x;
	

start:
	printf("Enter the HostName \t");
	scanf("%s", hostname);
	//SOCKET CREATION
	socket1=socket(AF_INET,SOCK_STREAM,0);
	if(socket1 == -1)
	{
		printf("Socket Creation Failed !!!");
		return 0;
	}
	
	//GETTING HOSTNAME
	hostt = gethostbyname(hostname);
	
	if(hostt == NULL)
	{
		printf("ERROR !!! want to Retry Y/N \n");
		scanf(" %c",&x);
		if(x == 'Y')
		{
			close(socket1);
			goto start;
		}
		else
		{
			close(socket1);
			return 0;
		}
	}
	
	address_list = (struct in_addr **)hostt->h_addr_list;
	for(i = 0; address_list[i] != NULL; i++) {
        //Copying the IP address
        strcpy(ip , inet_ntoa(*address_list[i]) );
    	}   
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=inet_addr(ip);
	serveraddr.sin_port=htons(80);
	
	//CHECKING FOR CONNECTION
	if(connect(socket1,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) == 0)
	{
	printf("Connected to Server \n");
	}

	//Page message
	snprintf(message, 99, "GET / HTTP/1.1\r\n" "Host: %s\r\n" "\r\n\r\n", hostname);
	if (send(socket1, message, strlen(message), 0) < 0)
	{
        printf("Send failed!\n");
        return 1;
    	}
    	printf("Data Sent Successful \n");
	
	f = fopen("/home/adityasingh/Downloads/file1.html", "w+");
	if(f == NULL)
	{
		printf("check the save file location and Retry\n");
	}
	else
		printf("file opened \n");
	while(1)
    {
        int received_len = recv(socket1, Sreply , sizeof(Sreply) , 0);

        if( received_len == 0 ){
            printf("Recieving Over\n");
            break;
        }

       	 total_len = total_len + received_len;
	
        //server_reply 
        fwrite(Sreply , received_len , 1, f);
	received_len=0;
       
    }
	if(total_len > 0) 
    		printf("Reply received of size %d Bytes\n",total_len);
	else
		puts("Recieving failed");

    	fclose(f);

    	return 0;

}
