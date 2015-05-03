# include <stdio.h>
# include <signal.h>
# include <termios.h>
# include <stdlib.h>
# include <unistd.h>
# include <termcap.h>
# define BUFFSIZE 100

struct termios oldtermios;

int	ttyraw(int fd)
{
	struct termios newtermios;
	if(tcgetattr(fd, &oldtermios) < 0)
		return(-1);
	newtermios = oldtermios;
	newtermios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	newtermios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	newtermios.c_cflag &= ~(CSIZE | PARENB);
	newtermios.c_cflag |= CS8;
	newtermios.c_oflag &= ~(OPOST);
	newtermios.c_cc[VMIN] = 1;
	newtermios.c_cc[VTIME] = 0;
	if(tcsetattr(fd, TCSAFLUSH, &newtermios) < 0)
		return(-1);
	return(0);
}

int	ttyreset(int fd)
{
	if(tcsetattr(fd, TCSAFLUSH, &oldtermios) < 0)
		return(-1);
	return(0);
}

void	sigcatch(int sig)
{
	(void)sig;
	ttyreset(0);
	exit(0);
}

int	put(int c)
{
	write(1, &c, 1);
	return (1);
}

int	main(void)
{
	int	i;
	char	c[3];
	char	*buffer;
	int	j;

	j = 0;
	buffer = (char *)malloc(sizeof(char) * BUFFSIZE);
	if(ttyraw(0) < 0)
	{
		tputs("Can't go to raw mode.\n\r", 2, put);
		exit(1);
	}
	tputs("$> ", 1, put);
	while( (i = read(0, &c, 3)) > 0)
	{
		if (i == 1)
		{
			if (c[0] == 4)
			{
				free(buffer);
				if(ttyreset(0) < 0)
				{
					tputs("Cannot reset terminal!\n\r", 2, put);
					exit(-1);
				}
				exit(0);
			}
			if (c[0] == 3)
			{
				tputs("\n\r", 1, put);
				tputs("$> ", 1, put);
				continue ;
			}
			c[1] = 0;
			if (c[0] == 13)
			{
				buffer[j++] = 0;
				tputs("\n\r", 1, put);
				tputs(buffer, 1, put);
				tputs("\n\r$> ", 1, put);
				free(buffer);
				j = 0;
				buffer = (char *)malloc(sizeof(char) * BUFFSIZE);
			}
			else
			{
				if (j == 99)
					buffer = realloc(buffer, j + BUFFSIZE);
				buffer[j++] = c[0];
				tputs(c, 1, put);
			}
		}
		else if (i == 3)
		{
			if (c[0] == 27 && c[1] == 91 && c[2] == 68)
				tputs("stanga\n\r", 1, put);
			else if (c[0] == 27 && c[1] == 91 && c[2] == 67)
				tputs("dreapta\n\r", 1, put);
			else if (c[0] == 27 && c[1] == 91 && c[2] == 66)
				tputs("jos\n\r", 1, put);
			else if (c[0] == 27 && c[1] == 91 && c[2] == 65)
				tputs("sus\n\r", 1, put);
		}
	}
	return (0);
}

