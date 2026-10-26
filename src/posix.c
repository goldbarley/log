#define _POSIX_C_SOURCE (200809L)

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <fcntl.h>
#include <unistd.h>

#include "log/log.h"

#define LOG_MAX_BUF_SIZE (4096U)
#define LOG_MAX_PREFIX_BUF_SIZE (2048U)

struct log_instance
{
	char buffer[LOG_MAX_BUF_SIZE];
	char prefix[LOG_MAX_PREFIX_BUF_SIZE];
	const char *sevstr[LOG_SEV_CNT];
	logfd_t sevfd[LOG_SEV_CNT];
	time_t t;
	uint16_t prefix_size;
	unsigned char mortal;
};

static struct log_instance _instance = {
	.sevstr = {
		[LOG_SEV_INF] = LOG_SEV_INF_STR,
		[LOG_SEV_DBG] = LOG_SEV_DBG_STR,
		[LOG_SEV_WRN] = LOG_SEV_WRN_STR,
		[LOG_SEV_ERR] = LOG_SEV_ERR_STR,
		[LOG_SEV_FTL] = LOG_SEV_FTL_STR
	},
	.sevfd = {
		[LOG_SEV_INF] = LOG_FD_STDOUT,
		[LOG_SEV_DBG] = LOG_FD_STDOUT,
		[LOG_SEV_WRN] = LOG_FD_STDERR,
		[LOG_SEV_ERR] = LOG_FD_STDERR,
		[LOG_SEV_FTL] = LOG_FD_STDERR
	}
};

LOGAPI_ATTR
logret_t log_open(logfd_t fd)
{
	/* If caller provides a file descriptor, use that. */
	if (fd >= 0)
	{
		_instance.mortal = 0;

		_instance.sevfd[LOG_SEV_INF] = fd;
		_instance.sevfd[LOG_SEV_DBG] = fd;
		_instance.sevfd[LOG_SEV_WRN] = fd;
		_instance.sevfd[LOG_SEV_ERR] = fd;
		_instance.sevfd[LOG_SEV_FTL] = fd;

		return LOG_SUCCESS;
	}

	switch (fd)
	{
		/* If caller gives LOG_FD_FILE, create a file. */
		case LOG_FD_FILE:
		{
			char filename[LOG_MAX_BUF_SIZE] = {0};
			snprintf(filename, LOG_MAX_BUF_SIZE, "log%zu.txt", time(NULL));
			fd = open(filename,
				  O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC,
				  0644);
			if (fd == LOG_FD_UNDEFINED)
				return LOG_FAILURE;

			_instance.sevfd[LOG_SEV_INF] = fd;
			_instance.sevfd[LOG_SEV_DBG] = fd;
			_instance.sevfd[LOG_SEV_WRN] = fd;
			_instance.sevfd[LOG_SEV_ERR] = fd;
			_instance.sevfd[LOG_SEV_FTL] = fd;

			_instance.mortal = 1;

			break;
		}

		/* Otherwise use standard file descriptors. */
		default:;
			_instance.mortal = 0;
	}

	return LOG_SUCCESS;
}

LOGAPI_ATTR
logret_t log_close()
{
	if (!_instance.mortal)
		return LOG_PASS;
	return close(*_instance.sevfd);
}

static inline
void _update_prefix(const char *file, const uint32_t line,
		    const enum log_sev sev)
{
	time_t t = time(NULL);
	struct tm *lt = localtime(&t);

	_instance.prefix_size = snprintf(_instance.prefix,
					 LOG_MAX_PREFIX_BUF_SIZE,
					 "[%i-%i-%i@%i:%i:%i][%s:%u][%s]: ",
					 lt->tm_year + 1900,
					 lt->tm_mon + 1, lt->tm_mday,
					 lt->tm_hour, lt->tm_min, lt->tm_sec,
					 file, line, _instance.sevstr[sev]);
	_instance.t = t;
}

LOGAPI_ATTR
logret_t log_writev(enum log_sev sev, logfd_t fd, const char *LOG_RESTRICT file,
		    uint32_t line, const char *LOG_RESTRICT format, va_list args)
{
	char buf[LOG_MAX_BUF_SIZE] = {0};
	logret_t ret = LOG_SUCCESS;

	time_t t = time(NULL);
	struct tm lt;
	localtime_r(&t, &lt);
	int n = strftime(buf, LOG_MAX_BUF_SIZE, "[%F %T]", &lt);

	size_t rem = LOG_MAX_BUF_SIZE - n;

	int m = snprintf(buf + n, LOG_MAX_BUF_SIZE, "[%s:%u][%s]: ", file, line,
			 _instance.sevstr[sev]);
	n += m < rem ? m : rem;

	rem = LOG_MAX_BUF_SIZE - n;

	int l = vsnprintf(buf + n, LOG_MAX_BUF_SIZE, format, args);
	if (l > rem)
	{
		l = rem;
		ret = LOG_INCOMPLETE;
	}
	n += l;

	write(fd, buf, n);

	return ret;
}

LOGAPI_ATTR LOG_FORMAT_PRINTF_4_5
logret_t log_writef(enum log_sev sev, const char *LOG_RESTRICT file,
		    uint32_t line, const char *LOG_RESTRICT format, ...)
{
	logret_t ret = LOG_SUCCESS;
	va_list args;
	va_start(args, format);
	ret = log_writev(sev, _instance.sevfd[sev], file, line, format, args);
	va_end(args);
	return ret;
}

LOGAPI_ATTR LOG_FORMAT_PRINTF_5_6
logret_t log_dwritef(enum log_sev sev, logfd_t fd, const char *LOG_RESTRICT file,
		     uint32_t line, const char *LOG_RESTRICT format, ...)
{
	logret_t ret = LOG_SUCCESS;
	va_list args;
	va_start(args, format);
	ret = log_writev(sev, fd, file, line, format, args);
	va_end(args);
	return ret;
}

LOGAPI_ATTR
logret_t log_write(enum log_sev sev, const char *LOG_RESTRICT file,
		   uint32_t line, const char *LOG_RESTRICT s)
{
	return log_writef(sev, file, line, "%s", s);
}

LOGAPI_ATTR
logret_t log_dwrite(enum log_sev sev, logfd_t fd, const char *LOG_RESTRICT file,
		    uint32_t line, const char *LOG_RESTRICT s)
{
	return log_dwritef(sev, fd, file, line, "%s", s);
}
