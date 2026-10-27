#include <log/log.h>

int main(void)
{
	/*
	 * Create a file using LOG_FD_FILE.
	 *
	 * Alternatively, provide your own file descriptor.
	 */
	log_open(LOG_FD_FILE);

	LOG_DINFF(LOG_FD_STDOUT, "Yin Yang.\n");
	LOG_WRN("Little bit of bitch inside the nice.\n");
	LOG_ERRF("%s", "A little bit of nice inside the bitch.\n");

	/*
	 * Will only close if LOG_FD_FILE. Otherwise,
	 * you are responsible for any file descriptor
	 * that you explicitly opened yourself.
	 */
	log_close();

	return 0;
}
