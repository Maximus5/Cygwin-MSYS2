/* sys/strace.h

   Copyright 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004,
   2005 Red Hat, Inc.

This file is part of Cygwin.

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details. */

/* This file contains routines for tracing system calls and other internal
   phenomenon.

   When tracing system calls, try to use the same style throughout:

   result = syscall (arg1, arg2, arg3) [optional extra stuff]

   If a system call can block (eg: read, write, wait), print another message
   before hanging so the user will know why the program has stopped.

   Note: __seterrno will also print a trace message.  Have that printed
   *first*.  This will make it easy to always know what __seterrno is
   refering to.  For the same reason, try not to have __seterrno messages
   printed alone.
*/

#ifndef _SYS_STRACE_H
#define _SYS_STRACE_H

#include <stdarg.h>

#ifdef __cplusplus

class child_info;
class strace
{
  int vsprntf (char *buf, const char *func, const char *infmt, va_list ap);
  void write (unsigned category, const char *buf, int count);
  unsigned char _active;
public:
  strace ();
  int microseconds ();
  int version;
  int lmicrosec;
  bool execing;
  void hello () __attribute__ ((regparm (1)));
  void prntf (unsigned, const char *func, const char *, ...) /*__attribute__ ((regparm(3)))*/;
  void vprntf (unsigned, const char *func, const char *, va_list ap) /*__attribute__ ((regparm(3)))*/;
  void wm (int message, int word, int lon) __attribute__ ((regparm(3)));
  void write_childpid (child_info&, unsigned long) __attribute__ ((regparm (2)));
  bool attached () const {return _active == 3;}
  bool active () const {return _active & 1;}
  unsigned char& active_val () {return _active;}
};

extern strace strace;

#endif /* __cplusplus */

#define _STRACE_INTERFACE_ACTIVATE_ADDR  -1
#define _STRACE_INTERFACE_ACTIVATE_ADDR1 -2
#define _STRACE_CHILD_PID -3

/* Bitmasks of tracing messages to print.  */

#define _STRACE_ALL	 0x00001 // so behaviour of strace=1 is unchanged
#define _STRACE_FLUSH	 0x00002 // flush output buffer after every message
#define _STRACE_INHERIT  0x00004 // children inherit mask from parent
#define _STRACE_UHOH	 0x00008 // unusual or weird phenomenon
#define _STRACE_SYSCALL	 0x00010 // system calls
#define _STRACE_STARTUP	 0x00020 // argc/envp printout at startup
#define _STRACE_DEBUG    0x00040 // info to help debugging
#define _STRACE_PARANOID 0x00080 // paranoid info
#define _STRACE_TERMIOS	 0x00100 // info for debugging termios stuff
#define _STRACE_SELECT	 0x00200 // info on ugly select internals
#define _STRACE_WM	 0x00400 // trace windows messages (enable _strace_wm)
#define _STRACE_SIGP	 0x00800 // trace signal and process handling
#define _STRACE_MINIMAL	 0x01000 // very minimal strace output
#define _STRACE_PTHREAD	 0x02000 // pthread calls
#define _STRACE_EXITDUMP 0x04000 // dump strace cache on exit
#define _STRACE_SYSTEM	 0x08000 // cache strace messages
#define _STRACE_NOMUTEX	 0x10000 // don't use mutex for synchronization
#define _STRACE_MALLOC	 0x20000 // trace malloc calls
#define _STRACE_THREAD	 0x40000 // cygthread calls
#define _STRACE_NOTALL	 0x80000 // don't include if _STRACE_ALL

#ifdef __cplusplus
extern "C" {
#endif

void small_printf (const char *, ...);
void strace_printf (unsigned, const char *func, const char *, ...);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#ifdef NOSTRACE
#define define_strace(c, f)
#define define_strace1(c, f)
#else
#define strace_printf_wrap(what, fmt, args...) \
   ((void) ({\
	if ((_STRACE_ ## what & _STRACE_SYSTEM) || strace.active ()) \
	  strace.prntf(_STRACE_ ## what, __PRETTY_FUNCTION__, fmt, ## args); \
	0; \
    }))
#define strace_printf_wrap1(what, fmt, args...) \
    ((void) ({\
	if ((_STRACE_ ## what & _STRACE_SYSTEM) || strace.active ()) \
	  strace.prntf((_STRACE_ ## what) | _STRACE_NOTALL, __PRETTY_FUNCTION__, fmt, ## args); \
	0; \
    }))

#define debug_printf(fmt, args...) strace_printf_wrap(DEBUG, fmt , ## args)
#define malloc_printf(fmt, args...) strace_printf_wrap1(MALLOC, fmt , ## args)
#define minimal_printf(fmt, args...) strace_printf_wrap1(MINIMAL, fmt , ## args)
#define paranoid_printf(fmt, args...) strace_printf_wrap1(PARANOID, fmt , ## args)
#define pthread_printf(fmt, args...) strace_printf_wrap1(PTHREAD, fmt , ## args)
#define select_printf(fmt, args...) strace_printf_wrap(SELECT, fmt , ## args)
#define sigproc_printf(fmt, args...) strace_printf_wrap(SIGP, fmt , ## args)
#define syscall_printf(fmt, args...) strace_printf_wrap(SYSCALL, fmt , ## args)
#define system_printf(fmt, args...) strace_printf_wrap(SYSTEM, fmt , ## args)
#define termios_printf(fmt, args...) strace_printf_wrap(TERMIOS, fmt , ## args)
#define thread_printf(fmt, args...) strace_printf_wrap1(THREAD, fmt , ## args)
#define wm_printf(fmt, args...) strace_printf_wrap(WM, fmt , ## args)
#endif /*NOSTRACE*/
#endif /* __cplusplus */
#endif /* _SYS_STRACE_H */
