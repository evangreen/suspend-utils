/* loglevel.h - routines to modify kernel console loglevel
 *
 * Released under GPL v2.
 * (c) 2007 Tim Dijkstra
 */

void open_printk(void);
int get_kernel_console_loglevel(void);
void set_kernel_console_loglevel(int level);
void close_printk(void);


void open_kmsg(void);
void printk(const char *fmt, ...);
void close_kmsg(void);
void timer_start(struct timeval *start);
void timer_print(struct timeval *start, const char *string);