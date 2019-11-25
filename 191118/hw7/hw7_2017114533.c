#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define oops(m,x) { perror(m); exit(x); }
#define MAX 1000

int main(int ac, char* av[])
{
	int thepipe[MAX][2];
	int pid[MAX];

	int i = 0;

	pipe(thepipe[i]);
	pid[i] = fork();

	if (pid[i] > 0)
	{
		close(thepipe[i][1]);
		dup2(thepipe[i][0], 0);
		close(thepipe[i][0]);

		execlp(av[ac - 2 - i], av[ac - 2 - i], NULL);

	}

	else
	{
		close(thepipe[i][0]);
		dup2(thepipe[i][1], 1);
		close(thepipe[i][1]);

		while (i < ac - 3) {
			i++;
			pipe(thepipe[i]);
			pid[i] = fork();

			if (pid[i] > 0)
			{
				close(thepipe[i][1]);
				dup2(thepipe[i][0], 0);
				close(thepipe[i][0]);

				execlp(av[ac - 2 - i], av[ac - 2 - i], NULL);
			}

			else
			{
				close(thepipe[i][0]);
				dup2(thepipe[i][1], 1);
				close(thepipe[i][1]);

				continue;
			}

		}

		execlp(av[ac - 2 - i], av[ac - 2 - i], NULL);

	}