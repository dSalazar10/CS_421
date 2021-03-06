#include "apue.h"
#include <errno.h>	/* for definition of errno */
#include <stdarg.h>	/* ISO C variable arguments. */ 
#include <syslog.h>

/*
 * Error functions that output to standard error
 */

static void err_doit(int, int, const char *, va_list);

/*
 * Nonfatal error related to a syste call.
 * Print a message and return.
 */
void err_ret(const char *fmt, ...)
{
   va_list	ap;

   va_start(ap, fmt);
   err_doit(1, errno, fmt, ap);
   va_end(ap);
}
/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void err_sys(const char *fmt, ...)
{
   va_list	ap;

   va_start(ap, fmt);
   err_doit(1, errno, fmt, ap);
   va_end(ap);
   exit(1);
}
/*
 * Nonfatal error unrelated to system call.
 * Error code passed as explicit parameter.
 * Print a message and return.
 */
void err_cont(int error, const char *fmt, ...)
{
   va_list	ap;

   va_start(ap, fmt);
   err_doit(1, error, fmt, ap);
   va_end(ap);
}
/*
 * Fatal error unrelated to a system call.
 * Error code passed as explicit parameter.
 * Print message and terminate.
void err_exit(int error, const char *fmt, ...)
{
   va_list	ap;

   va_start(ap, fmt);
   err_doit(1, error, fmt, ap);
   va_end(ap);
   exit(1);
}
/*
 * Fatal error related to a system call.
 * Print a message, dump core, and terminate.
 */
void err_dump(const char *fmt, ...)
{
   va_list	ap;

   va_start(ap, fmt);
   err_doit(1, errno, fmt, ap);

   va_end(ap);
   abort();	/* dump core and terminate */
   exit(1);
}
/*
 * Nonfatal error unrelated to a system call.
 * Print a message and return.
 */
void err_msg(const char *fmt, ...)
{
   va_list	ap;

   va_start(ap, fmt);
   err_doit(0, 0, fmt, ap);
   va_end(ap);
}
/*
 * Fatal error unrelated to system call.
 * Print message and terminate.
 */
void err_quit(const char *fmt, ...)
{
   va_list	ap;

   va_start(ap, fmt);
   err_doit(0, 0, fmt, ap);
   va_end(ap);
   exit(1);
}
/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag".
 */
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
   char		buf[MAXLINE];

   vsnprintf(buf, MAXLINE-1, fmt, ap);
   if (errnoflag)
      snprintf(buf+strlen(buf), MAXLINE-strlen(buf)-1, ": %s",
        strerror(error));
   strlcat(buf, "\n");
   fflush(stdout);	/* in case stdout and stderr are the same */
   fputs(buf, stderr);
   fflush(NULL);	/* flushes all stdio output streams */
}

/* End Error Functions */

/*
 * Error routines for programs that can run as a daemon.
 */

static void log_doit(int, int, int, const char *, va_list ap);

/*
 * Caller must define and set this: nonzero if
 * interactive, zero if daemon
 */
extern int log_to_stderr;

/*
 * Initialize syslog(), if running as daemon.
 */
void log_open(const char *ident, int option, int facility)
{
   if (log_to_stderr == 0)
      openlog(ident, option, facility);
}
/*
 * Nonfatal error related to a system call.
 * Print a message with the system's errno value and return.
 */
void log_ret(const char *fmt, ...)
{
   va_list	ap;

   va_start(ap, fmt);
   log_doit(1, errno, LOG_ERR, fmt, ap);
   va_end(ap);
}
/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void log_sys(const char *fmt, ...)
{
   va_list	ap;

   va_start(ap, fmt);
   log_doit(1, errno, LOG_ERR, fmt, ap);
   va_end(ap);
   exit(2);
}
/*
 * Nonfatal error unrelated to system call.
 * Print a message and return.
 */
void log_msg(const char *fmt, ...)
{
   va_list	ap;

   va_start(ap, fmt);
   log_doit(0, 0, LOG_ERR, fmt, ap);
   va_end(ap);
}
/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 */
void log_quit(const char *fmt, ...)
{
   va_list	ap;

   va_start(ap, fmt);
   log_doit(0, 0, LOG_ERR, fmt, ap);
   va_end(ap);
   exit(2);
}
/*
 * Fatal error related to a system call.
 * Error number passed as an explicit parameter.
 * Print a message and terminate.
 */
void log_exit(int error, const char *fmt, ...)
{
   va_list	ap;

   va_start(ap, fmt);
   log_doit(1, error, LOG_ERR, fmt, ap);
   va_end(ap);
   exit(2);
}
/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag" and "priority".
 */
static void log_doit(int errnoflag, int error, int priority, const char *fmt,
                     va_list ap)
{
   char	buf[MAXLINE];

   vsnprintf(buf, MAXLINE-1, fmt, ap);
   if (errnoflag)
      snprintf(buf+strlen(buf), MAXLINE-strlen(buf)-1, ": %s",
         strerror(error));
   strlcat(buf, "\n");
   if (log_to_stderr) {
      fflush(stdout);
      fputs(buf, stderr);
      fflush(stderr);
   } else {
      syslog(priority, "%s", buf);
   }
}
