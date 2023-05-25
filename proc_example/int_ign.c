#include <signal.h>

int	main(void) {
	signal(SIGINT, SIG_IGN);
	while(42);
	return (0);
}
