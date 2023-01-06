#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <ucontext.h>
#include <stdlib.h>

#include "editor.h"
#include "signal.h"
#include "terminal.h"
#include "terminal_io.h"

#include <dlfcn.h>

#define STACK_DIFFERENCE_MAX 16384


void handle_SIGWINCH(int signal) {
    UNUSED(signal);

    updateWindowSize();

    if (editorState.cursor_y > editorState.screenrows) editorState.cursor_y = editorState.screenrows - 1;
    if (editorState.cursor_x > editorState.screencols) editorState.cursor_x = editorState.screencols - 1;

    refreshScreen();
}

void handle_SIGSEGV(int signal, siginfo_t *si, void *context) {
    disableRawMode();
    clear_screen();

    ucontext_t *uc = (ucontext_t *) context;
    Dl_info dlinfo;

    void *ip = (void *) uc->uc_mcontext.gregs[REG_RIP];
    void *sp = (void *) uc->uc_mcontext.gregs[REG_RSP];
    void **bp = (void **) uc->uc_mcontext.gregs[REG_RBP];

    size_t stack_difference = llabs(sp - (void *) bp);

    void *fault_addr = si->si_addr;
#ifdef ENABLE_DEV
    int f = 0;
    fprintf(stderr, "----- A SEGMENTATION FAULT HAS OCCURRED ----\n");
    dladdr(ip, &dlinfo);
    if (fault_addr != NULL) fprintf(stderr, "At: %p (in %s)\n", fault_addr, stack_difference > STACK_DIFFERENCE_MAX ? dlinfo.dli_fname == NULL ? " (external/not in fedit)" : dlinfo.dli_fname : "fedit");
    if (dlinfo.dli_sname != NULL) fprintf(stderr, "At: %s\n", dlinfo.dli_sname);
    fprintf(stderr, "RIP addr: %p\n", ip);
    fprintf(stderr, "RSP addr: %p\n", sp);
    fprintf(stderr, "RBP addr: %p\n", bp);
    fprintf(stderr, "--------------------------------------------\n");
    if (stack_difference < STACK_DIFFERENCE_MAX) {
        fprintf(stderr, "Stack trace:\n");
        while (bp && ip) {
            if (!dladdr(ip, &dlinfo))
                break;

            fprintf(stderr, "% 2d: %p <%s+%u> (%s)\n",
                    ++f,
                    ip,
                    dlinfo.dli_sname,
                    (unsigned) (ip - dlinfo.dli_saddr),
                    dlinfo.dli_fname);

            if (dlinfo.dli_sname && !strcmp(dlinfo.dli_sname, "main"))
                break;

            ip = bp[1];
            bp = (void **) bp[0];
        }
        fprintf(stderr, "--------------------------------------------\n");
    }
#else
    if (fault_addr != NULL) fprintf(stderr, "fedit unfortunately crashed (%p)\n", fault_addr);
    else fprintf(stderr, "fedit unfortunately crashed\n");
#endif

    exit(1);
}


void register_handlers() {
    signal(SIGWINCH, handle_SIGWINCH);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handle_SIGSEGV;
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGSEGV, &sa, NULL);
}