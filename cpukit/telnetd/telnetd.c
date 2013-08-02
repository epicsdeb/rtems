/***********************************************************/
/*
 *
 *  The telnet DAEMON
 *
 *  Author: 17,may 2001
 *
 *   WORK: fernando.ruiz@ctv.es 
 *   HOME: correo@fernando-ruiz.com
 *
 * After start the net you can start this daemon.
 * It uses the previously inited pseudo-terminales (pty.c)
 * getting a new terminal with getpty(). This function
 * gives a terminal name passing a opened socket like parameter.
 *
 * With register_telnetd() you add a new command in the shell to start
 * this daemon interactively. (Login in /dev/console of course)
 * 
 * Sorry but OOB is not still implemented. (This is the first version)
 *
 * Till Straumann <strauman@slac.stanford.edu>
 *  - made the 'shell' interface more generic, i.e. it is now
 *    possible to have 'telnetd' run an arbitrary 'shell'
 *    program.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 *  $Id: telnetd.c,v 1.12.2.1 2008/10/15 17:38:12 joel Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <rtems/error.h>
#include <rtems/pty.h>
#include <rtems/shell.h>
#include <rtems/telnetd.h>
#include <rtems/bspIo.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <syslog.h>

#include <rtems/userenv.h>
#include <rtems/error.h>
#include <rtems/rtems_bsdnet.h>

#define PARANOIA

extern char *telnet_get_pty(int socket);
extern int   telnet_pty_initialize(void);

struct shell_args {
  char *devname;
  void *arg;
  char  peername[16];
  char  delete_myself;
};

typedef union uni_sa {
  struct sockaddr_in sin;
  struct sockaddr     sa;
} uni_sa;

static int sockpeername(int sock, char *buf, int bufsz);

static int initialize_telnetd(void);
static int telnetd_askForPassword;

void * telnetd_dflt_spawn(
  const char *name,
  unsigned priority,
  unsigned stackSize,
  void (*fn)(void*),
  void *fnarg
);

/***********************************************************/
rtems_id            telnetd_task_id                 = 0;
uint32_t            telnetd_stack_size              = 32000;
rtems_task_priority telnetd_task_priority           = 0;
bool                telnetd_remain_on_caller_stdio  = false;
void                (*telnetd_shell)(char *, void*) = 0;
void                *telnetd_shell_arg              = NULL;
void *              (*telnetd_spawn_task)(
  const char *,
  unsigned,
  unsigned,
  void (*)(void*),
  void *) = telnetd_dflt_spawn;

static char *grab_a_Connection(
  int des_socket,
  uni_sa *srv,
  char *peername,
  int sz
)
{
  char *rval = 0;
#if 0
  socklen_t size_adr = sizeof(srv->sin);
#else
  /* 4.6 doesn't have socklen_t */
  uint32_t size_adr = sizeof(srv->sin);
#endif
  int acp_sock;

  acp_sock = accept(des_socket,&srv->sa,&size_adr);

  if (acp_sock<0) {
    perror("telnetd:accept");
    goto bailout;
  };

  if ( !(rval=telnet_get_pty(acp_sock)) ) {
    syslog( LOG_DAEMON | LOG_ERR, "telnetd: unable to obtain PTY");
    /* NOTE: failing 'do_get_pty()' closed the socket */
    goto bailout;
  }

  if (sockpeername(acp_sock, peername, sz))
    strncpy(peername, "<UNKNOWN>", sz);

#ifdef PARANOIA
  syslog(LOG_DAEMON | LOG_INFO,
      "telnetd: accepted connection from %s on %s",
      peername,
      rval);
#endif

bailout:

  return rval;
}


static void release_a_Connection(char *devname, char *peername, FILE **pstd, int n)
{

#ifdef PARANOIA
  syslog( LOG_DAEMON | LOG_INFO,
      "telnetd: releasing connection from %s on %s",
      peername,
      devname );
#endif

  while (--n>=0)
    if (pstd[n]) fclose(pstd[n]);

}

static int sockpeername(int sock, char *buf, int bufsz)
{
  uni_sa peer;
  int    rval = sock < 0;
#if 0
  socklen_t len  = sizeof(peer.sin);
#else
  /* 4.6 doesn't have socklen_t */
  uint32_t len  = sizeof(peer.sin);
#endif

  if ( !rval )
    rval = getpeername(sock, &peer.sa, &len);

  if ( !rval )
    rval = !inet_ntop( AF_INET, &peer.sin.sin_addr, buf, bufsz );

  return rval;
}

#if 1
#define INSIDE_TELNETD
#include "check_passwd.c"
#else
#define check_passwd(arg) 0
#endif


static void
spawned_shell(void *arg);

/***********************************************************/
static void
rtems_task_telnetd(void *task_argument)
{
  int                des_socket;
  uni_sa             srv;
  char              *devname;
  char               peername[16];
  int                i=1;
  int                size_adr;
  struct shell_args *arg = NULL;

  if ((des_socket=socket(PF_INET,SOCK_STREAM,0))<0) {
    perror("telnetd:socket");
    telnetd_task_id=0;
    rtems_task_delete(RTEMS_SELF);
  };
  setsockopt(des_socket,SOL_SOCKET,SO_KEEPALIVE,&i,sizeof(i));

  memset(&srv,0,sizeof(srv));
  srv.sin.sin_family=AF_INET;
  srv.sin.sin_port=htons(23);
  size_adr=sizeof(srv.sin);
  if ((bind(des_socket,&srv.sa,size_adr))<0) {
    perror("telnetd:bind");
    close(des_socket);
    telnetd_task_id=0;
    rtems_task_delete(RTEMS_SELF);
  };
  if ((listen(des_socket,5))<0) {
    perror("telnetd:listen");
          close(des_socket);
    telnetd_task_id=0;
    rtems_task_delete(RTEMS_SELF);
  };

  /* we don't redirect stdio as this probably
   * was started from the console anyways..
   */
  do {
    if ( telnetd_remain_on_caller_stdio ) {
      char device_name[32];
      ttyname_r( 1, device_name, sizeof(device_name) );
      if ( !telnetd_askForPassword || (0 == check_passwd(arg->peername)) )
	telnetd_shell(device_name, telnetd_shell_arg);
    } else {
      devname = grab_a_Connection(des_socket, &srv, peername, sizeof(peername));

      if ( !devname ) {
	/* if something went wrong, sleep for some time */
	sleep(10);
	continue;
      }

      arg = malloc( sizeof(*arg) );

      arg->devname = devname;
      arg->arg = telnetd_shell_arg;
      strncpy(arg->peername, peername, sizeof(arg->peername));

      if ( !telnetd_spawn_task( &devname[5], telnetd_task_priority,
               telnetd_stack_size, spawned_shell, arg) ) {
        FILE *dummy;

        if ( telnetd_spawn_task != telnetd_dflt_spawn ) {
          fprintf(stderr,"Telnetd: Unable to spawn child task\n");
        }

        /* hmm - the pty driver slot can only be
         * released by opening and subsequently
         * closing the PTY - this also closes
         * the underlying socket. So we mock up
         * a stream...
         */

        if ( !(dummy=fopen(devname,"r+")) )
          perror("Unable to dummy open the pty, losing a slot :-(");
        release_a_Connection(devname, peername, &dummy, 1);
        free(arg);
        sleep(2); /* don't accept connections too fast */
      }
    }
  } while(1);

  /* TODO: how to free the connection semaphore? But then - 
   *       stopping the daemon is probably only needed during
   *       development/debugging.
   *       Finalizer code should collect all the connection semaphore
   *       counts and eventually clean up...
   */
  close(des_socket);
  telnetd_task_id=0;
}

/***********************************************************/
static int initialize_telnetd(void) {
  
  if (telnetd_task_id         ) return RTEMS_RESOURCE_IN_USE;
  if (telnetd_stack_size<=0   ) telnetd_stack_size   =32000;

  if ( !telnetd_spawn_task("TNTD", telnetd_task_priority,
          RTEMS_MINIMUM_STACK_SIZE, rtems_task_telnetd, 0) ) {
    return -1;
  }
  return 0;
}

/***********************************************************/
int rtems_telnetd_initialize(
  void               (*cmd)(char *, void *),
  void                *arg,
  bool                 remainOnCallerSTDIO,
  size_t               stack,
  rtems_task_priority  priority,
  bool                 askForPassword
)
{
  rtems_status_code sc;

#if 0
  printf("This is rtems-telnetd (modified by Till Straumann)\n");
  printf("$Id: telnetd.c,v 1.12.2.1 2008/10/15 17:38:12 joel Exp $\n");
  printf("Release $Name: rtems-4-9-6 $\n");
#endif

  if ( !telnetd_shell && !cmd ) {
    fprintf(stderr,"startTelnetd(): setup error - NO SHELL; bailing out\n");
    return 1;
  }

  if (telnetd_task_id) {
    fprintf(stderr,"ERROR:telnetd already started\n");
    return 1;
  };

  if ( !telnet_pty_initialize() ) {
    fprintf(stderr,"PTY driver probably not properly registered\n");
    return 1;
  }

  telnetd_askForPassword = askForPassword;

  if (cmd)
    telnetd_shell = cmd;
  telnetd_shell_arg     = arg;
  telnetd_stack_size    = stack;
  if ( !priority ) {
    priority = rtems_bsdnet_config.network_task_priority;
  }
  if ( priority < 2 )
    priority = 100;
  telnetd_task_priority          = priority;
  telnetd_remain_on_caller_stdio = remainOnCallerSTDIO;

  sc = initialize_telnetd();
  if (sc != RTEMS_SUCCESSFUL) return sc;

  if ( !telnetd_remain_on_caller_stdio )
    fprintf(stderr, "rtems_telnetd() started with stacksize=%u,priority=%d\n",
      (unsigned)telnetd_stack_size,(int)telnetd_task_priority);
  return 0;
}

/* utility wrapper */
static void
spawned_shell(void *targ)
{
  rtems_status_code    sc;
  FILE                *nstd[3]={0};
  FILE                *ostd[3]={ stdin, stdout, stderr };
  int                  i=0;
  struct shell_args  *arg = targ;

  sc=rtems_libio_set_private_env();

  /* newlib hack/workaround. Before we change stdin/out/err we must make
         * sure the internal data are initialized (fileno(stdout) has this sideeffect).
   * This should probably be done from RTEMS' libc support layer...
   * (T.S., newlibc-1.13; 2005/10)
         */

  fileno(stdout);

  if (RTEMS_SUCCESSFUL != sc) {
    rtems_error(sc,"rtems_libio_set_private_env");
    goto cleanup;
  }

  /* redirect stdio */
  for (i=0; i<3; i++) {
    if ( !(nstd[i]=fopen(arg->devname,"r+")) ) {
      perror("unable to open stdio");
      goto cleanup;
    }
  }

  stdin  = nstd[0];
  stdout = nstd[1];
  stderr = nstd[2];

  #if 0
    printk("STDOUT is now %x (%x) (FD %i/%i)\n",
           stdout,nstd[1],fileno(stdout),fileno(nstd[1]));
    printf("hello\n");
    write(fileno(stdout),"hellofd\n",8);
  #endif

  /* call their routine */
  if ( !telnetd_askForPassword || (0 == check_passwd(arg->peername)) )
    telnetd_shell(arg->devname, arg->arg);

  stdin  = ostd[0];
  stdout = ostd[1];
  stderr = ostd[2];

cleanup:
  release_a_Connection(arg->devname, arg->peername, nstd, i);
  free(arg);
}

struct wrap_delete_args {
  void (*t)(void *);
  void           *a;
};

static rtems_task
wrap_delete(rtems_task_argument arg)
{
  struct wrap_delete_args     *pwa = (struct wrap_delete_args *)arg;
  register void              (*t)(void *) = pwa->t;
  register void               *a   = pwa->a;

  /* free argument before calling function (which may never return if
   * they choose to delete themselves)
   */
  free(pwa);
  t(a);
  rtems_task_delete(RTEMS_SELF);
}

void *
telnetd_dflt_spawn(const char *name, unsigned int priority, unsigned int stackSize, void (*fn)(void *), void* fnarg)
{
  rtems_status_code        sc;
  rtems_id                 task_id;
  char                     nm[4] = {'X','X','X','X' };
  struct wrap_delete_args *pwa = malloc(sizeof(*pwa));

  strncpy(nm, name, 4);

  if ( !pwa ) {
    perror("Telnetd: no memory\n");
    return 0;
  }

  pwa->t = fn;
  pwa->a = fnarg;

  if ((sc=rtems_task_create(
    rtems_build_name(nm[0], nm[1], nm[2], nm[3]),
      (rtems_task_priority)priority,
      stackSize,
      RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES | RTEMS_FLOATING_POINT,
      &task_id)) ||
    (sc=rtems_task_start(
      task_id,
      wrap_delete,
      (rtems_task_argument)pwa))) {
        free(pwa);
        rtems_error(sc,"Telnetd: spawning task failed");
        return 0;
  }
  return (void*)task_id;
}

/* convenience routines for CEXP (retrieve stdio descriptors
 * from reent structure)
 */
#ifdef stdin
static __inline__ FILE *
_stdin(void)  { return stdin; }
#undef stdin
FILE *stdin(void)  { return _stdin(); }
#endif
#ifdef stdout
static __inline__ FILE *
_stdout(void) { return stdout; }
#undef stdout
FILE *stdout(void) { return _stdout(); }
#endif
#ifdef stderr
static __inline__ FILE *
_stderr(void) { return stderr; }
#undef stderr
FILE *stderr(void) { return _stderr(); }
#endif

/* MUST NOT USE stdin & friends below here !!!!!!!!!!!!! */
