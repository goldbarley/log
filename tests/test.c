#include <unistd.h>

#include <log/log.h>

// int main(void)
// {
// 	log_open(LOG_FD_DEFAULT);
//
// 	LOG_INFF("LOG_INFF\n");
// 	LOG_DBGF("LOG_DBGF\n");
// 	LOG_WRNF("LOG_WRNF\n");
// 	LOG_ERRF("LOG_ERRF\n");
// 	LOG_FTLF("LOG_FTLF\n");
// 	LOG_DINFF(LOG_FD_STDOUT, "LOG_DINFF\n");
// 	LOG_DDBGF(LOG_FD_STDOUT, "LOG_DDBGF\n");
// 	LOG_DWRNF(LOG_FD_STDOUT, "LOG_DWRNF\n");
// 	LOG_DERRF(LOG_FD_STDOUT, "LOG_DERRF\n");
// 	LOG_DFTLF(LOG_FD_STDOUT, "LOG_DFTLF\n");
// 	LOG_INF("LOG_INF\n");
// 	LOG_DBG("LOG_DBG\n");
// 	LOG_WRN("LOG_WRN\n");
// 	LOG_ERR("LOG_ERR\n");
// 	LOG_FTL("LOG_FTL\n");
// 	LOG_DINF(LOG_FD_STDOUT, "LOG_DINF\n");
// 	LOG_DDBG(LOG_FD_STDOUT, "LOG_DDBG\n");
// 	LOG_DWRN(LOG_FD_STDOUT, "LOG_DWRN\n");
// 	LOG_DERR(LOG_FD_STDOUT, "LOG_DERR\n");
// 	LOG_DFTL(LOG_FD_STDOUT, "LOG_DFTL\n");
//
// 	log_close();
// }

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
