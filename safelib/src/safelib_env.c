#include "../include/safelib.h"
#ifdef USE_SECURE_LIB
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

#define __SEC_POPEN_TOKEN " ;"
FILE *_popen_safe(const char *cmd, const char *type)
{
	pid_t pid;
	int pfd[2];
	int rpipe = 0, wpipe = 0, i;
	char **argv;
	char *ptr;
	char *cmdcpy;


	if(cmd == NULL || cmd == "")
		return(NULL);

	if(type[0] != 'r' && type[0] != 'w')
		return(NULL);

	if ((cmdcpy = strdup(cmd)) == NULL)
		return(NULL);

	argv = NULL;
	if( (ptr = strtok(cmdcpy, __SEC_POPEN_TOKEN)) == NULL)
	{
		free(cmdcpy);
		return(NULL);
	}

	for(i = 0;; i++)
	{
		if( ( argv = (char **) realloc(argv, (i+1) * sizeof(char *)) ) == NULL)
		{
			free(cmdcpy);
			return(NULL);
		}

		if( (*(argv+i) = (char *) malloc((strlen_safe(ptr)+1) * sizeof(char))) == NULL)
		{
			free(cmdcpy);
			return(NULL);
		}

		strncpy_safe(argv[i], ptr, (strlen_safe(ptr)+1));

		if( (ptr = strtok(NULL, __SEC_POPEN_TOKEN)) == NULL)
		{
			if( ( argv = (char **) realloc(argv, (i+2) * sizeof(char *))) == NULL)
			{
				free(cmdcpy);
				return(NULL);
			}
			argv[i+1] = NULL;
			break;
		}
	}


	if(type[0] == 'r')
		rpipe = 1;
	else
		wpipe = 1;

	if (pipe(pfd) < 0)
	{
		free(cmdcpy);
		return(NULL);
	}

	if((pid = fork()) < 0)
	{
		close(pfd[0]);
		close(pfd[1]);
		free(cmdcpy);
		return(NULL);
	}

	if(pid == 0)		// child
	{
		if((pid = fork()) < 0)
		{
			close(pfd[0]);
			close(pfd[1]);
			free(cmdcpy);
			return(NULL);
		}
		if(pid > 0)
		{
			exit(0);	// child nr. 1 exits
		}

		// child nr. 2
		if(rpipe)
		{
			close(pfd[0]);	// close reading end, we don't need it
			if (pfd[1] != STDOUT_FILENO)
				dup2(pfd[1], STDOUT_FILENO);	// redirect stdout to writing end of pipe
			dup2(STDOUT_FILENO, STDERR_FILENO);
		}
		else
		{
			close(pfd[1]);	// close writing end, we don't need it
			if (pfd[0] != STDIN_FILENO)
				dup2(pfd[0], STDIN_FILENO);		// redirect stdin to reading end of pipe
		}

		if(strchr(argv[0], '/') == NULL)
			execvp(argv[0], argv);	// search in $PATH
		else
			execv(argv[0], argv);

		close(pfd[0]);
		close(pfd[1]);
		free(cmdcpy);
		return(NULL);	// exec failed.. ooops!
	}
	else					// parent
	{
		waitpid(pid, NULL, 0); // wait for child nr. 1

		if(rpipe)
		{
			close(pfd[1]);
			free(cmdcpy);
			return(fdopen(pfd[0], "r"));
		}
		else
		{
			close(pfd[0]);
			free(cmdcpy);
			return(fdopen(pfd[1], "w"));
		}

	}
}
int _usleep_safe (__useconds_t usec)
{
	struct timeval tv;

	tv.tv_sec = (long int) (usec / 1000000);
	tv.tv_usec = (long int) (usec % 1000000);
	return select(0, NULL, NULL, NULL, &tv);
}

#endif	// end of file

