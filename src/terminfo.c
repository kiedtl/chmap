#include <sys/ioctl.h>

int
ttywidth(void)
{
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	return (int) w.ws_col;
}
