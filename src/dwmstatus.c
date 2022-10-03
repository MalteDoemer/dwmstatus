#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifndef NO_X
#include <X11/Xlib.h>
#endif
#ifdef __OpenBSD__
#define SIGPLUS SIGUSR1 + 1
#define SIGMINUS SIGUSR1 - 1
#else
#define SIGPLUS SIGRTMIN
#define SIGMINUS SIGRTMIN
#endif

#define MAX_CMD_LENGTH 64

/* the maximum length of the status line (probably too much) */
#define STATUS_LENGTH (LENGTH(blocks) * MAX_CMD_LENGTH + LENGTH(status_line) + 1)

#include "util.h"

typedef result_t (*block_fn_t)(const char* arg, char* buffer, size_t buffer_size);

typedef struct {
    block_fn_t block_fn;
    const char* arg;
    size_t interval;
    int signal;
} block_t;

#include "blocks.h"
#include "config.h"

#ifndef __OpenBSD__
static void dummy_sig_handler(int num);
#endif
static void sig_handler(int signum);
static void terminate_handler();
static void setup_signals();

static void update_block(size_t index);
static void update_blocks(size_t time);
static void update_sig_blocks(int signal);
static int update_status_text();
static void status_loop();

static void pstdout();
#ifndef NO_X
/* writes status line to the name of the root window */
static void setroot();
static void (*writestatus)() = setroot;

static void setupX();
static Display* dpy;
static int screen;
static Window root;
#else
/* writes status line to stdout */
static void (*writestatus)() = pstdout;
#endif

/* status text of every block */
static char block_text[LENGTH(blocks)][MAX_CMD_LENGTH] = { 0 };

/*
    - index 0: the current status line text
    - index 1: the last status line text
*/
static char status_text[2][STATUS_LENGTH];

static int running = 1;

void update_block(size_t index)
{
    const block_t* block = &blocks[index];
    result_t res = block->block_fn(block->arg, block_text[index], MAX_CMD_LENGTH);

    if (res == RESULT_ERROR)
        snprintf(block_text[index], MAX_CMD_LENGTH, not_available);
}

void update_blocks(size_t time)
{
    int update_all = time == 0;

    for (size_t i = 0; i < LENGTH(blocks); i++) {
        const block_t* block = &blocks[i];
        if (update_all || (block->interval != 0 && time % block->interval == 0))
            update_block(i);
    }
}

void update_sig_blocks(int signum)
{
    for (size_t i = 0; i < LENGTH(blocks); i++) {
        const block_t* block = &blocks[i];
        if (block->signal == signum)
            update_block(i);
    }
}

static int update_status_text()
{
    char* current = status_text[0];
    char* last = status_text[1];

    // replace the old status text
    strcpy(last, current);

    // scan the status_line string and fill the current buffer
    // Note: the length of the buffer is chosen so that it never overflows

    const char* s = status_line; // pointer into the status_line
    size_t pos = 0;              // index into the current buffer
    size_t block_index = 0;      // index of the block to output

    while (*s) {
        const char* n = s + 1;

        if (*s == '%' && *n == '%') // escaped double percent '%%'
        {
            current[pos++] = '%';
            s += 2;
        } else if (*s == '%' && *n == 's') // string place holder '%s'
        {

            if (block_index >= LENGTH(blocks))
                die("dwmstatus: block index out of bounds at character %ld", s - status_line);

            char* dest = current + pos;
            char* src = block_text[block_index]; // src should be null terminated
            size_t len = strlen(src);

            assert(len < MAX_CMD_LENGTH);

            strcpy(dest, src);

            pos += len;
            block_index += 1;
            s += 2;
        } else if (*s == '%') // error '%d' or '%1'
        {
            die("dwmstatus: malformed format string at character %ld", s - status_line);
        } else // literal character
        {
            current[pos++] = *s;
            s += 1;
        }
    }

    // STATUS_LENGTH should be defined so that we never overflow it
    assert(pos < STATUS_LENGTH);

    // null-termination
    current[pos] = '\0';

    return strcmp(current, last); // return 0 if they are same
}

void setup_signals()
{
#ifndef __OpenBSD__
    /* initialize all real time signals with dummy handler */
    for (int i = SIGRTMIN; i <= SIGRTMAX; i++)
        signal(i, dummy_sig_handler);
#endif

    for (size_t i = 0; i < LENGTH(blocks); i++)
        if (blocks[i].signal > 0)
            signal(SIGMINUS + blocks[i].signal, sig_handler);
}

#ifndef NO_X
void setroot()
{
    if (!update_status_text()) // only set root if text has changed
        return;
    XStoreName(dpy, root, status_text[0]);
    XFlush(dpy);
}

void setupX()
{
    dpy = XOpenDisplay(NULL);
    if (!dpy)
        die("dwmstatus: failed to open display");

    screen = DefaultScreen(dpy);
    root = RootWindow(dpy, screen);
}
#endif

void pstdout()
{
    if (!update_status_text()) // only write out if text has changed
        return;
    printf("%s\n", status_text[0]);
    fflush(stdout);
}

void status_loop()
{
    setup_signals();
    size_t i = 0;

    while (1) {
        update_blocks(i++);
        writestatus();
        if (!running)
            break;
        sleep(1);
    }
}

#ifndef __OpenBSD__
void dummy_sig_handler(int signum)
{
    return;
}
#endif

void sig_handler(int signum)
{
    update_sig_blocks(signum - SIGPLUS);
    writestatus();
}

void terminate_handler()
{
    running = 0;
}

void help()
{
    printf("A status bar for dwm.\n\n"
           "USAGE:\n"
           "\tdwmstatus [OPTIONS]\n\n"
           "OPTIONS:\n"
           "\t-p\tprint status to stdout instead of dwm status bar\n"
           "\t-h\tprint help information and exit\n");

    exit(0);
}

int main(int argc, char const* argv[])
{
    for (int i = 0; i < argc; i++) {
        if (!strcmp("-p", argv[i]))
            writestatus = pstdout;
        else if (!strcmp("-h", argv[i]))
            help();
    }

#ifndef NO_X
    setupX();
#endif

    signal(SIGTERM, terminate_handler);
    signal(SIGINT, terminate_handler);
    status_loop();
#ifndef NO_X
    XCloseDisplay(dpy);
#endif
    return 0;
}
