#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "../include/password.h"
#include "../include/config.h"

static void set_stdin_echo(int enable) {
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(hStdin, &mode)) {
        if (enable) {
            mode |= ENABLE_ECHO_INPUT;
        } else {
            mode &= ~ENABLE_ECHO_INPUT;
        }
        SetConsoleMode(hStdin, mode);
    }
#else
    struct termios tty;
    if (tcgetattr(STDIN_FILENO, &tty) == 0) {
        if (enable) {
            tty.c_lflag |= ECHO;
        } else {
            tty.c_lflag &= ~ECHO;
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);
    }
#endif
}

void vexis_password_wipe(char *buf, size_t len) {
    if (buf == NULL || len == 0) {
        return;
    }
    volatile unsigned char *p = (volatile unsigned char *)buf;
    //this volatile is used because it keeps compiler from completely removing pwd from memory
    while (len--) {
        *p++ = 0;
    }
}

static int strip_newline(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') {
        s[n - 1] = '\0';
        n--;
        if (n > 0 && s[n - 1] == '\r') {
            s[n - 1] = '\0';
        }
        return 0; //means success in removing \n
    }
    return 1; //means couldn't remove \n
}

static void drain_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF){
    }
}

static vexis_err_t password_size_check(size_t len) { //this is redundant acc to me rn
    if (len == 0 || len >= VEXIS_PWD_MAX) {
        return VEXIS_ERR_PWD;
    }
    return VEXIS_SUCC;
}

vexis_err_t vexis_password_prompt(const char *prompt, char *out, size_t out_size) {
    if (out == NULL || out_size == 0) {
        return VEXIS_ERR_GEN;
    }
    out[0] = '\0';

    if (prompt != NULL) {
        fprintf(stderr, "%s", prompt);
        fflush(stderr);
    }

    set_stdin_echo(0);
    char *res = fgets(out, (int)out_size, stdin);
    set_stdin_echo(1);
    fprintf(stderr, "\n");

    if (res == NULL) {
        out[0] = '\0';
        return VEXIS_ERR_PWD;
    }

    if (strip_newline(out)) {
        drain_stdin();
        vexis_password_wipe(out, out_size);
        out[0] = '\0';
        return VEXIS_ERR_PWD;
    }

    if (password_size_check(strlen(out)) != VEXIS_SUCC) {
        vexis_password_wipe(out, out_size);
        out[0] = '\0';
        return VEXIS_ERR_PWD;
    }

    return VEXIS_SUCC;
}

vexis_err_t vexis_password_prompt_confirm(char *out, size_t out_size) {
    char confirm[VEXIS_PWD_MAX];
    vexis_err_t err;

    if (out == NULL || out_size == 0) {
        return VEXIS_ERR_GEN;
    }
    out[0] = '\0';

    err = vexis_password_prompt("Enter password: ", out, out_size);
    if (err != VEXIS_SUCC) {
        vexis_password_wipe(out, out_size);
        out[0] = '\0';
        return err;
    }

    err = vexis_password_prompt("Confirm password: ", confirm, sizeof(confirm));
    if (err != VEXIS_SUCC) {
        vexis_password_wipe(out, out_size);
        vexis_password_wipe(confirm, sizeof(confirm));
        out[0] = '\0';
        return err;
    }

    if (strcmp(out, confirm) != 0) {
        vexis_password_wipe(out, out_size);
        vexis_password_wipe(confirm, sizeof(confirm));
        out[0] = '\0';
        return VEXIS_ERR_PWD;
    }

    vexis_password_wipe(confirm, sizeof(confirm));
    return VEXIS_SUCC;
}
