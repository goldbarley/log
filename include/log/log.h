#ifndef LOG_H_
#define LOG_H_ 1

#include <stdarg.h>
#include <stdint.h>

#if defined(_MSC_VER)
#ifdef LOG_LIB_STATIC
#define LOGAPI_ATTR
#elif defined(LOG_LIB_SHARED)
#define LOGAPI_ATTR __declspec(dllexport)
#else
#define LOGAPI_ATTR __declspec(dllimport)
#endif /* LOG_LIB_STATIC/SHARED */
#elif defined(__GNUC__) || defined(__clang__)
#ifdef LOG_LIB_SHARED
#define LOGAPI_ATTR __attribute__((__visibility__("default")))
#else
#define LOGAPI_ATTR
#endif /* LOG_LIB_SHARED */
#else
#define LOGAPI_ATTR
#endif /* Compilers */

#if defined(__GNUC__) || defined(__clang__)
#define LOG_FORMAT_PRINTF_4_5 __attribute__((__format__(__printf__, 4, 5)))
#define LOG_FORMAT_PRINTF_5_6 __attribute__((__format__(__printf__, 5, 6)))
#else
#define LOG_FORMAT_PRINTF_4_5
#define LOG_FORMAT_PRINTF_5_6
#endif /* Compilers */

#ifndef __cplusplus
#define LOG_RESTRICT restrict
#else
#define LOG_RESTRICT __restrict
#endif /* __cplusplus */

enum log_sev
{
	LOG_SEV_INF = 0,
	LOG_SEV_DBG = 1,
	LOG_SEV_WRN = 2,
	LOG_SEV_ERR = 3,
	LOG_SEV_FTL = 4,
	LOG_SEV_CNT
};

#define LOG_SEV_INF_STR "INF"
#define LOG_SEV_DBG_STR "DBG"
#define LOG_SEV_WRN_STR "WRN"
#define LOG_SEV_ERR_STR "ERR"
#define LOG_SEV_FTL_STR "FTL"

typedef enum logret
{
	LOG_SUCCESS = 0,
	LOG_INCOMPLETE = 1,
	LOG_PASS = 2,
	LOG_FAILURE = -1,
} logret_t;

typedef int logfd_t;

#define LOG_FD_FILE (-3)
#define LOG_FD_DEFAULT (-2)
#define LOG_FD_UNDEFINED (-1)
#define LOG_FD_STDIN (0)
#define LOG_FD_STDOUT (1)
#define LOG_FD_STDERR (2)

LOGAPI_ATTR
logret_t log_open(logfd_t fd);

LOGAPI_ATTR
logret_t log_close();

LOGAPI_ATTR
logret_t log_writev(enum log_sev sev, logfd_t fd, const char *LOG_RESTRICT file,
		    uint32_t line, const char *LOG_RESTRICT format, va_list args);

LOGAPI_ATTR LOG_FORMAT_PRINTF_4_5
logret_t log_writef(enum log_sev sev, const char *LOG_RESTRICT file,
		    uint32_t line, const char *LOG_RESTRICT format, ...);

LOGAPI_ATTR LOG_FORMAT_PRINTF_5_6
logret_t log_dwritef(enum log_sev sev, logfd_t fd, const char *LOG_RESTRICT file,
		     uint32_t line, const char *LOG_RESTRICT format, ...);

LOGAPI_ATTR
logret_t log_write(enum log_sev sev, const char *LOG_RESTRICT file,
		   uint32_t line, const char *LOG_RESTRICT s);

LOGAPI_ATTR
logret_t log_dwrite(enum log_sev sev, logfd_t fd, const char *LOG_RESTRICT file,
		   uint32_t line, const char *LOG_RESTRICT s);

#define LOG_INFF(...) \
	do \
		log_writef(LOG_SEV_INF, __FILE__, __LINE__, __VA_ARGS__); \
	while (0)

#define LOG_DBGF(...) \
	do \
		log_writef(LOG_SEV_DBG, __FILE__, __LINE__, __VA_ARGS__); \
	while (0)

#define LOG_WRNF(...) \
	do \
		log_writef(LOG_SEV_WRN, __FILE__, __LINE__, __VA_ARGS__); \
	while (0)

#define LOG_ERRF(...) \
	do \
		log_writef(LOG_SEV_ERR, __FILE__, __LINE__, __VA_ARGS__); \
	while (0)

#define LOG_FTLF(...) \
	do \
		log_writef(LOG_SEV_FTL, __FILE__, __LINE__, __VA_ARGS__); \
	while (0)

#define LOG_DINFF(d, ...) \
	do \
		log_dwritef(LOG_SEV_INF, d, __FILE__, __LINE__, __VA_ARGS__); \
	while (0)

#define LOG_DDBGF(d, ...) \
	do \
		log_dwritef(LOG_SEV_DBG, d, __FILE__, __LINE__, __VA_ARGS__); \
	while (0)

#define LOG_DWRNF(d, ...) \
	do \
		log_dwritef(LOG_SEV_WRN, d, __FILE__, __LINE__, __VA_ARGS__); \
	while (0)

#define LOG_DERRF(d, ...) \
	do \
		log_dwritef(LOG_SEV_ERR, d, __FILE__, __LINE__, __VA_ARGS__); \
	while (0)

#define LOG_DFTLF(d, ...) \
	do \
		log_dwritef(LOG_SEV_FTL, d, __FILE__, __LINE__, __VA_ARGS__); \
	while (0)

#define LOG_INF(s) \
	do \
		log_write(LOG_SEV_INF, __FILE__, __LINE__, s); \
	while (0)

#define LOG_DBG(s) \
	do \
		log_write(LOG_SEV_DBG, __FILE__, __LINE__, s); \
	while (0)

#define LOG_WRN(s) \
	do \
		log_write(LOG_SEV_WRN, __FILE__, __LINE__, s); \
	while (0)

#define LOG_ERR(s) \
	do \
		log_write(LOG_SEV_ERR, __FILE__, __LINE__, s); \
	while (0)

#define LOG_FTL(s) \
	do \
		log_write(LOG_SEV_FTL, __FILE__, __LINE__, s); \
	while (0)

#define LOG_DINF(d, s) \
	do \
		log_dwrite(LOG_SEV_INF, d, __FILE__, __LINE__, s); \
	while (0)

#define LOG_DDBG(d, s) \
	do \
		log_dwrite(LOG_SEV_DBG, d, __FILE__, __LINE__, s); \
	while (0)

#define LOG_DWRN(d, s) \
	do \
		log_dwrite(LOG_SEV_WRN, d, __FILE__, __LINE__, s); \
	while (0)

#define LOG_DERR(d, s) \
	do \
		log_dwrite(LOG_SEV_ERR, d, __FILE__, __LINE__, s); \
	while (0)

#define LOG_DFTL(d, s) \
	do \
		log_dwrite(LOG_SEV_FTL, d, __FILE__, __LINE__, s); \
	while (0)

#endif /* LOG_H_ */
