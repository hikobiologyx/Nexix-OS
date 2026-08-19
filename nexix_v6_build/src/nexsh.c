#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#define VERSION "6.0"
#define BANNER "\033[1;37mNEXIX OS\033[0m \033[0;33mVersion 6.0 [Retro Core]\033[0m\n"
#define PROMPT "\033[1;32mnexix@core\033[0m:\033[1;34m~\033[0m$ "

void cmd_help() {
    printf("\nAvailable commands:\n");
    printf("  help     - Show this help\n");
    printf("  version  - Show system version\n");
    printf("  clear    - Clear screen\n");
    printf("  ls       - List directory contents\n");
    printf("  cd       - Change directory\n");
    printf("  cat      - Display file content\n");
    printf("  echo     - Print text\n");
    printf("  mkdir    - Create directory\n");
    printf("  rm       - Remove file/directory\n");
    printf("  reboot   - Reboot system\n");
    printf("  exit     - Exit shell\n\n");
}

void cmd_version() {
    printf("\n\033[1;37mNEXIX OS Kernel v%s\033[0m\n", VERSION);
    printf("\033[0;36m[Retro Core Build]\033[0m\n");
    printf("\033[0;33mCopyright (c) 2015-2024 Nexix Project\033[0m\n\n");
}

void cmd_ls(const char *path) {
    DIR *dir = opendir(path && strlen(path) > 0 ? path : ".");
    if (!dir) {
        printf("ls: cannot access '%s': No such directory\n", path);
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        printf("%s  ", entry->d_name);
    }
    printf("\n");
    closedir(dir);
}

void cmd_cat(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("cat: %s: No such file\n", filename);
        return;
    }
    int c;
    while ((c = fgetc(f)) != EOF) putchar(c);
    fclose(f);
    printf("\n");
}

void cmd_mkdir(const char *dirname) {
    if (mkdir(dirname, 0755) != 0) {
        printf("mkdir: cannot create directory '%s'\n", dirname);
    } else {
        printf("Directory '%s' created\n", dirname);
    }
}

void cmd_rm(const char *filename) {
    if (remove(filename) != 0) {
        printf("rm: cannot remove '%s': No such file\n", filename);
    } else {
        printf("'%s' removed\n", filename);
    }
}

int main() {
    char line[256], cmd[64], arg[192];
    
    printf("\033[2J\033[H");
    printf(BANNER);
    printf("\033[0;32m[System Initialized]\033[0m\n");
    printf("\033[0;37m[Memory OK] [Disk OK] [Retro Mode Active]\033[0m\n\n");
    printf("Type 'help' for available commands.\n\n");

    while (1) {
        printf(PROMPT);
        fflush(stdout);
        
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = 0;
        arg[0] = '\0';
        
        if (sscanf(line, "%63s %191[^\n]", cmd, arg) < 1) {
             strcpy(cmd, line);
             arg[0] = '\0';
        }
        
        if (strcmp(cmd, "help") == 0) cmd_help();
        else if (strcmp(cmd, "version") == 0) cmd_version();
        else if (strcmp(cmd, "clear") == 0 || strcmp(cmd, "cls") == 0) printf("\033[2J\033[H");
        else if (strcmp(cmd, "ls") == 0 || strcmp(cmd, "dir") == 0) cmd_ls(arg);
        else if (strcmp(cmd, "cd") == 0) {
            if (strlen(arg) > 0) {
                if (chdir(arg) != 0) printf("cd: %s: No such directory\n", arg);
            }
        }
        else if (strcmp(cmd, "cat") == 0 || strcmp(cmd, "type") == 0) {
            if (strlen(arg) > 0) cmd_cat(arg);
            else printf("cat: missing file operand\n");
        }
        else if (strcmp(cmd, "echo") == 0) {
            printf("%s\n", line + 5);
        }
        else if (strcmp(cmd, "mkdir") == 0) {
            if (strlen(arg) > 0) cmd_mkdir(arg);
        }
        else if (strcmp(cmd, "rm") == 0 || strcmp(cmd, "del") == 0) {
            if (strlen(arg) > 0) cmd_rm(arg);
        }
        else if (strcmp(cmd, "reboot") == 0) {
            printf("Rebooting...\n");
            exit(0);
        }
        else if (strcmp(cmd, "exit") == 0) break;
        else if (strlen(cmd) > 0) {
            printf("nexix: %s: command not found\n", cmd);
        }
    }
    
    printf("\n\033[0;33mSession terminated. Goodbye.\033[0m\n");
    return 0;
}
