#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

char * executeCommand(char* command, char *buff) {
	FILE * fpipe;
	char path[1024];
	
	if ( !(fpipe = (FILE*)popen(command, "r")) ) {
		printf("Error : with pipe");
		exit(1);
	}

	while(fgets(path, sizeof(path)-1, fpipe) != NULL) {
		strcat(buff, path);
	}
	pclose(fpipe);
	return buff;
}

void processChild (int sockfd) {
	FILE *fp;
	int status;
	char path[1024];

	int n;
	char buf[1024];
	char buffer[1024];
	char buffer2[1024];

	char exitTest[1024] = "exit";
	
	getcwd(path, sizeof(path));
	strcat(path, "$ ");
	write(sockfd, path,1024);

	while ((n = read(sockfd,buf,1024))>0) 
	{
		getcwd(path, sizeof(path));
		strncpy (buffer, buf, strlen(buf)-2);
		if (strcmp(buffer, exitTest) == 0) 
			break;
		executeCommand(buffer,buffer2);
		strcat(path, "$ ");
		strcat(path, buffer2);
		n = write(sockfd, path,1024);

		bzero(path, 1024);
		bzero(buf, 1024);
		bzero(buffer,1024);
		bzero(buffer2,1024);
	}
}

int main(int argc, const char *argv[])
{
    int lfd, cfd;
    socklen_t clilen;
	pid_t childpid;
	struct sockaddr_in servaddr, cliaddr;

	if (argc != 2) 
	{
		printf("Error : server port missing\n");
		exit(-1);
	}

	lfd = socket(AF_INET, SOCK_STREAM, 0);

	if (lfd < 0) 
	{
		printf("Error : couldn't opening socket\n");
		exit(-1);
	}

	bzero((char *) &servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(atoi(argv[1]));

	if (bind(lfd, (struct sockaddr *) & servaddr, sizeof(servaddr)) < 0) 
	{
		printf("Error : binding port number\n");
	}

	listen(lfd, 5);

	clilen = sizeof(cliaddr);

	for(;;) 
	{
		if ((cfd = accept(lfd, (struct sockaddr *) & cliaddr, &clilen)) < 0) 
		{
			printf("Error : accept\n");
			exit(-1);
		}

		childpid = fork();
		if (childpid < 0) 
		{
			error("Error : fork\n");
		}
		if (childpid == 0) 
		{
			close(lfd);
			processChild(cfd);
			exit(0);
		}
		close(cfd);
		
	}

	return 0;
}
