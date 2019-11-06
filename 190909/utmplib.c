#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>

#define NRECS 16
#define NULLUT ((struct utmp*)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];
static int num_recs;
static int cur_rec;
static int fd_utmp = -1;

utmp_open(char *filename)
{
    fd_utmp = open(filename, O_RDONLY);
    cur_rec = num_recs = 0;
    return fd_utmp;
}

struct utmp *utmp_next()
{
    struct utmp *recp;
    if(fd_utmp == -1)
        return NULLUT;
    if(cur_rec == num_recs && utmp_reload() == 0)
        return NULLUT;

    recp = (struct utmp*) &utmpbuf[cur_rec * UTSIZE];
    cur_rec ++;
    return recp;
}

int utmp_reload()
{
    int amt_read;
    amt_read = read(fd_utmp, utmpbuf, NRECS*UTSIZE);
    num_recs = amt_read/UTSIZE;

    cur_rec = 0;
    return num_recs;
}

int utmp_close()
{
    if(fd_utmp != -1)
        close(fd_utmp);
}
