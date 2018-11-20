/*
 * daemonize.c
 * This example daemonizes a process, writes a few log messages,
 * sleeps 20 seconds and terminates afterwards.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <string.h>

static void skeleton_daemon()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }

    /* Open the log file */
    openlog ("firstdaemon", LOG_PID, LOG_DAEMON);
}

void checkCPUUsage(char* dirLog){
	char command[50];
	strcpy( command, "cd /home/diego/Documentos/; sh CPUsage" );
   	system(command);
	FILE *fp;
   	char buff[255];
	fp = fopen("/tmp/temporal", "r");
   	fscanf(fp, "%s", buff);
   	printf("1 : %s\n", buff );
	fclose(fp);
	char configCPU[255];
	fp = fopen("/tmp/tempCPU", "r");
   	fscanf(fp, "%s", configCPU);
   	printf("1 : %s\n", configCPU );
	fclose(fp);
	int buffInteger;
	sscanf(buff,"%d",&buffInteger);
	int compareInteger; //compare is the number from the config file.
	sscanf(configCPU,"%d",&compareInteger);
	if(buffInteger >= compareInteger){
		char message[80];
		strcpy(message, "logger ");
		strcat(message, "-s ");
		strcat(message, "Sample alert message: [CRITICAL] – CPU Usage is currently ");
		strcat(message, buff);
		strcat(message, "%");
		strcat(message, " which is over ");
		strcat(message, configCPU);
		strcat(message, " 2>> ");
		strcat(message, dirLog);
		syslog (LOG_NOTICE, message);
		strcpy(command, message);
   		system(command);
	}
}

void checkMemory(char* dirLog){
	char command[50];
	strcpy( command, "cd /home/diego/Documentos/; sh MemoryUsage" );
   	system(command);
	FILE *fp;
   	char buff[255];
	fp = fopen("/tmp/temporal2", "r");
   	fscanf(fp, "%s", buff);
   	printf("1 : %s\n", buff );
	fclose(fp);
	char configMEM[255];
	fp = fopen("/tmp/tempMEM", "r");
   	fscanf(fp, "%s", configMEM);
   	printf("1 : %s\n", configMEM);
	fclose(fp);
	int buffInteger;
	sscanf(buff,"%d",&buffInteger);
	int compareInteger = 90; //comparememory number
	sscanf(configMEM,"%d",&compareInteger);
	if(buffInteger >= compareInteger){
		char message[80];
		strcpy(message, "logger ");
		strcat(message, "-s ");
		strcat(message, "Sample alert message: [CRITICAL] – Memory Usage is currently ");
		strcat(message, buff);
		strcat(message, "%");
		strcat(message, " which is over ");
		strcat(message, configMEM);
		strcat(message, " 2>> ");
		strcat(message, dirLog);
		syslog (LOG_NOTICE, message);
		strcpy(command, message);
   		system(command);	
	}

}

void checkFileSys(char* dirLog){
	
}

void checkCritical(char* dirLog){


	char command[50];
	strcpy( command, "cd /home/diego/Documentos/; sh CriticalMess" );
   	system(command);
	FILE *fp;
   	char buff[255];
	fp = fopen("/tmp/temporal4", "r");
   	fscanf(fp,"%*s %99[^\n]", buff);
	syslog (LOG_NOTICE, "------------------------");
	syslog (LOG_NOTICE, buff);
	syslog (LOG_NOTICE, "------------------------");
	char buff1[255];
	strcpy( buff1, buff ); // To fix the problem with the string
	char StringToSearch[]="CRITICAL"; 
	char *tmp; 
	tmp = strstr(buff,StringToSearch); 
	if (tmp != NULL) {
		syslog (LOG_NOTICE, "ERROR CRITICO");
		char message[80];
		strcpy(message, "logger ");
		strcat(message, "-s ");
		strcat(message, "Sample alert message: [CRITICAL] – System critical error has been detected:");
		strcat(message, buff1);
		strcat(message, " 2>> ");
		strcat(message, dirLog);
		syslog (LOG_NOTICE, message);
		strcpy(command, message);
   		system(command);
	}
	char StringToSearch1[]="critical"; 
	char *tmp1; 
	tmp1 = strstr(buff,StringToSearch1); 
	if (tmp1 != NULL) {
		char message[80];
		strcpy(message, "logger ");
		strcat(message, "-s ");
		strcat(message, "Sample alert message: [CRITICAL] – System critical error has been detected:");
		strcat(message, buff1);
		strcat(message, " 2>> ");
		strcat(message, dirLog);
		syslog (LOG_NOTICE, message);
		strcpy(command, message);
   		system(command);
	}
	
}

void updateConfig(){
	char command[50];
	strcpy( command, "cd /home/diego/Documentos/; sh readConfig" );
   	system(command);    	
	
}

int main()
{
    	skeleton_daemon(); 
	updateConfig();
	char configDIR[255];
	FILE *fp;
	fp = fopen("/tmp/tempLogDir", "r");
   	fscanf(fp, "%s", configDIR);
   	printf("1 : %s\n", configDIR);
	fclose(fp);
	//while(1){ 
		//checkMemory(configDIR);
		//checkCPUUsage(configDIR);
		//checkFileSys(configDIR);
         	checkCritical(configDIR);
	//}
    	syslog (LOG_NOTICE, "First daemon terminated.");
    	closelog();
    	return EXIT_SUCCESS;
}


