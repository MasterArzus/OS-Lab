#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>

struct cmd_struct {
    char* command;
    int (*cmd_handler)(char* ori_path, char* para);
};

// def different functions
int match_name(char* ori_path, char* para);
int match_modify(char* ori_path, char* mtime);
int match_create(char* ori_path, char* ctime);

// def commands here
struct cmd_struct commands[] = {
    {(char*)"-name", match_name},
    {(char*)"-mtime", match_modify},
    {(char*)"-ctime", match_create},
};

int match_name(char* ori_path, char* para) {
    DIR* dir;
    struct dirent* file;

    if ((dir = opendir(ori_path)) != NULL) {
        while ((file = readdir(dir)) != NULL) {
            char* tmp = strchr(para, '*');
            int len = 0;
            if (tmp == NULL) {
                len = strlen(para);
            }
            else {
                len = tmp - para;
            }
            printf("The len is %d\n", len);
            if (file->d_type == DT_REG && (strncmp(file->d_name, para, len) == 0)) {
                printf("The target file path is %s/%s\n", ori_path, para);
                closedir(dir);
                return 0;
            }
            else if (file->d_type == DT_DIR) {
                if (file->d_name[0] == '.')
                    continue;
                char dir_name[200];
                char new_path[200];
                strcpy(dir_name, file->d_name);
                sprintf(new_path, "%s/%s", ori_path, dir_name);
                match_name(new_path, para);
            }
        }
        printf("No such file in %s directory.\n", ori_path);
        closedir(dir);
        return 0;
    }
    else {
        printf("%s is a invalid directory path.\n", ori_path);
        return 0;
    }

    return 0;
}

int match_modify(char* ori_path, char* mtime) {
    DIR* dir;
    struct dirent* file;
    int time_interval = std::atoi(mtime);
    if ((dir = opendir(ori_path)) != NULL) {
        while ((file = readdir(dir)) != NULL) {
            if (file->d_type == DT_REG) {
                char file_path[200];
                struct stat fp;

                sprintf(file_path, "%s/%s", ori_path, file->d_name);
                if ((stat(file_path, &fp)) == -1) {
                    printf("The file %s is not existed.\n", file_path);
                }
                else {
                    time_t m_time = fp.st_mtime;
                    time_t current_time;
                    time(&current_time);
                    if (current_time - m_time > time_interval)
                        printf("%s : modify time is %ld\n", file_path, fp.st_mtime);
                }
            }
            else if (file->d_type == DT_DIR) {
                if (file->d_name[0] == '.') continue;
                char dir_name[100];
                char new_path[100];
                strcpy(dir_name, file->d_name);
                sprintf(new_path, "%s/%s", ori_path, dir_name);
            }
        }
        closedir(dir);
        return 0;
    }
    else {printf("%s is a invalid directory path.\n", ori_path);}
    return 0;
}

int match_create(char* ori_path, char* mtime) {
    DIR* dir;
    struct dirent* file;
    int time_interval = std::atoi(mtime);

    if ((dir = opendir(ori_path)) != NULL) {
        while ((file = readdir(dir)) != NULL) {
            if (file->d_type == DT_REG) {
                char file_path[200];
                struct stat fp;

                sprintf(file_path, "%s/%s", ori_path, file->d_name);
                if ((stat(file_path, &fp)) == -1) {
                    printf("The file %s is not existed.\n", file_path);
                }
                else {
                    time_t m_time = fp.st_mtime;
                    time_t current_time;
                    time(&current_time);
                    // printf("The current time is %ld\n", current_time);
                    if (current_time - m_time > time_interval)
                        printf("%s : create time is %ld\n", file_path, fp.st_mtime);
                }
            }
            else if (file->d_type == DT_DIR) {
                if (file->d_name[0] == '.')
                    continue;
                char dir_name[200];
                char new_path[200];
                strcpy(dir_name, file->d_name);
                sprintf(new_path, "%s/%s", ori_path, dir_name);
            }
        }
        closedir(dir);
        return 0;
    }
}

int main(int argc, char* argv[]) {
    char ori_dir[200];
    char option[100];
    char para[100];

    if (argc != 4) {
        printf("Usage: myfind [PATH] [-option] [Parameters]\n");
        return 0;
    }
    else {
        strcpy(ori_dir, argv[1]);
        strcpy(option, argv[2]);
        strcpy(para, argv[3]);
        for (int i = 0; i < commands.size(); i++) {
            if (strcmp(option, commands[i].command) == 0) {
                return commands[i].cmd_handler(ori_dir, para);
            }
        }
        return 0;
    }

}

