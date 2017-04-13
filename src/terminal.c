/* terminal management */

#include "inc/mred.h"
#include "inc/input.h"
#include "inc/terminal.h"

void
enable_raw_mode ()
{
	if (isatty (ED.stdin) && isatty (ED.stdout))
	{
		if (tcgetattr (ED.stdin, &ED.orig_termios) == -1)
			die ("ERR: tcgetattr");
		atexit (disable_raw_mode);

		struct termios raw = ED.orig_termios;
		tcgetattr (ED.stdin, &raw);

		raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
		raw.c_oflag &= ~(OPOST);
		raw.c_cflag |= (CS8);
		raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

		raw.c_cc[VMIN] = 0;
		raw.c_cc[VTIME] = 1;

		tcsetattr (ED.stdin, TCSAFLUSH, &raw);
		if (get_window_size (&ED.screenrows, &ED.screencols) == -1)
			die ("ERR: get window size");
	}
	else
	{
		ED.screenrows = 24;
		ED.screencols = 80;
		if (MRED_ALLOW_NOTTY)
		{
			char buf[16];
			memset (&buf, '\0', 16);

			if (read (ED.stdin, &buf, 11) != 11)
				die ("ERR: stdin invalid read size");

			if (strncmp ("MRED:STDIN\n", buf, 11) != 0)
				die ("ERR: invalid stdin");

			memset (&buf, '\0', 16);
		}
		else
		{
			die ("not running from a tty?");
		}
	}
}


void
disable_raw_mode ()
{
	if (tcsetattr (ED.stdin, TCSAFLUSH, &ED.orig_termios) == -1)
		die ("ERR: tcsetattr");
}


int mred_read_key ()
{
	int nread;
	char c;
	while ((nread = read (ED.stdin, &c, 1)) != 1)
	{
		if (nread == -1 && errno != EAGAIN)
			die ("ERR: read key");
	}
	if (c == '\x1b')
	{
		char seq[3];
		if (read (ED.stdin, &seq[0], 1) != 1)
			return ('\x1b');
		if (read (ED.stdin, &seq[1], 1) != 1)
			return ('\x1b');
		if (seq[0] == '[')
		{
			if (seq[1] >= '0' && seq[1] <= '9')
			{
				if (read (ED.stdin, &seq[2], 1) != 1)
					return ('\x1b');
				if (seq[2] == '~')
				{
					switch (seq[1])
					{
						case '1': return (HOME_KEY);
						case '3': return (DEL_KEY);
						case '4': return (END_KEY);
						case '5': return (PAGE_UP);
						case '6': return (PAGE_DOWN);
						case '7': return (HOME_KEY);
						case '8': return (END_KEY);
					}
				}
			}
			else
			{
				switch (seq[1])
				{
					case 'A': return (ARROW_UP);
					case 'B': return (ARROW_DOWN);
					case 'C': return (ARROW_RIGHT);
					case 'D': return (ARROW_LEFT);
					case 'H': return (HOME_KEY);
					case 'F': return (END_KEY);
				}
			}
		}
		return ('\x1b');
	}
	else
	{
		return (c);
	}
}


int
get_window_size (int *rows, int *cols)
{
	struct winsize ws;
	if (ioctl (ED.stdout, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
	{
		if (write (ED.stdout, "\x1b[999C\x1b[999B", 12) != 12)
			return (-1);
		return (get_cursor_position (rows, cols));
	}
	else
	{
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return (0);
	}
}


int
get_cursor_position (int *rows, int *cols)
{
	char buf[32];
	unsigned int i = 0;
	if (write (ED.stdout, "\x1b[6n", 4) != 4)
		return (-1);
	while (1 < sizeof (buf) -1)
	{
		if (read (ED.stdin, &buf[i], 1) != 1)
			break;
		if (buf[i] == 'R')
			break;
		i++;
	}
	buf[i] = '\0';
	if (buf[0] != '\x1b' || buf[1] != '[')
		return (-1);
	if (sscanf (&buf[2], "%d;%d", rows, cols) != 2)
		return (-1);
	return (0);
}
