/* loglevel.c - routines to modify kernel console loglevel
 *
 * Released under GPL v2.
 * (c) 2007 Tim Dijkstra
 */

#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>


static FILE *printk_file;
static FILE *kmsg_file;
static int proc_mounted = 0;

void open_printk(void)
{
	struct stat stat_buf;
	char *procname = "/proc/sys/kernel/printk";

	if (stat(procname, &stat_buf) && errno == ENOENT) {
		if (mount("none", "/proc", "proc", 0, NULL)) {
			fprintf(stderr, "resume: Could not mount proc\n");
			return;
		} else
			proc_mounted = 1;
	}

        printk_file = fopen(procname, "r+");
}

int get_kernel_console_loglevel(void)
{
        int level = -1;

        if (printk_file) {
                rewind(printk_file);
                fscanf(printk_file, "%d", &level);
        }
        return level;
}

void set_kernel_console_loglevel(int level)
{
        if (printk_file) {
                rewind(printk_file);
                fprintf(printk_file, "%d\n", level);
                fflush(printk_file);
        }

}

void close_printk(void)
{
        if (printk_file)
                fclose(printk_file);

	if (proc_mounted)
		umount("/proc");
}

void open_kmsg(void) {
        kmsg_file = fopen("/dev/kmsg", "w");
        if (kmsg_file) {
                setvbuf(kmsg_file, NULL, _IOLBF, 0);
        }
}

void printk(const char *fmt, ...) {
        va_list args;

        if (kmsg_file) {
                fprintf(kmsg_file, "<5>hibernate: ");
                va_start(args, fmt);
                vfprintf(kmsg_file, fmt, args);
                va_end(args);
        }
}

void close_kmsg(void) {
        if (kmsg_file) {
                fclose(kmsg_file);
                kmsg_file = NULL;
        }
}

void timer_start(struct timeval *start) {
        gettimeofday(start, NULL);
}

void timer_print(struct timeval *start, const char *string) {
        struct timeval end;
        gettimeofday(&end, NULL);
        timersub(&end, start, start);
        printk("%s %g\n", string, start->tv_usec / 1000000.0 + start->tv_sec);
}