#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifndef NO_X
#include <X11/Xlib.h>
#endif

/* the maximum length of the status line (should be the same as in dwm) */
#define STATUS_LENGTH 512

#define BLOCK_LENGTH STATUS_LENGTH / LENGTH(blocks)

#include "util.h"

typedef result_t (*block_fn_t)(const char* arg, char* buffer, size_t buffer_size);

typedef struct {
    const char* fmt;
    block_fn_t block_fn;
    const char* arg;
    const char* on_click;
    size_t interval;
    int signal;
} block_t;

#include "blocks.h"
#include "config.h"

static void sig_handler(int signum);
static void dummy_sig_handler(int num);
static void click_sig_handler(int signum, siginfo_t* si, void* ucontext);
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
static char block_text[LENGTH(blocks)][BLOCK_LENGTH] = { 0 };

/* the current and last status lines */
static char status_text[2][STATUS_LENGTH];

static int running = 1;

void update_block(size_t index)
{
    const block_t* block = &blocks[index];
    char* buffer = block_text[index];

    if (!block->block_fn)
        return;

    result_t res = block->block_fn(block->arg, buffer, BLOCK_LENGTH);

    if (res == RESULT_ERROR)
        snprintf(buffer, BLOCK_LENGTH, not_available);
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

    size_t offset = 0;

    for (size_t i = 0; i < LENGTH(blocks); i++) {
        int res;
        const block_t* block = &blocks[i];
        const char* text = block_text[i];

        res = snprintf(current + offset, STATUS_LENGTH - offset, block->fmt, text);
        if (res < 0) {
            warn("snprintf:");
            continue;
        }

        offset += (size_t)res;

        res = snprintf(current + offset, STATUS_LENGTH - offset, "%c", block_seperator);
        if (res < 0) {
            warn("snprintf:");
            continue;
        }

        offset += (size_t)res;
    }

    return strcmp(current, last); // return 0 if nothing changed
}

void setup_signals()
{
    /* initialize all real time signals with dummy handler */
    for (int i = SIGRTMIN; i <= SIGRTMAX; i++)
        signal(i, dummy_sig_handler);

    for (size_t i = 0; i < LENGTH(blocks); i++)
        if (blocks[i].signal > 0)
            signal(SIGRTMIN + blocks[i].signal, sig_handler);

    struct sigaction sa = { .sa_sigaction = click_sig_handler, .sa_flags = SA_SIGINFO };
    sigaction(SIGRTMIN + SIG_CLICK, &sa, NULL);
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

void sig_handler(int signum)
{
    update_sig_blocks(signum - SIGRTMIN);
    writestatus();
}

void dummy_sig_handler(int signum)
{
    return;
}

void click_sig_handler(int signum, siginfo_t* info, void* ucontext)
{
    int index = info->si_value.sival_int;
    const char* cmd = blocks[index].on_click;

    if (!cmd)
        return;

    FILE* p = popen(cmd, "r");

    if (!p)
        return;

    pclose(p);
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
