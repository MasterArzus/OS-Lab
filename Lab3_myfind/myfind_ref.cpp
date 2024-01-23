#include <cstdio>
#include <cstring>
#include <getopt.h>
#include <dirent.h> // find files in certain dir
#include <ctime>
#include <cstdlib>
#include <sys/stat.h>  // find the status of file

using namespace std;

__attribute__((unused)) int opterr = 0;
char path[1024] = "";
char targetname[100] = "";
int modifiedtime = -1;

char match(char *first, char *second)
{
    if (*first == NULL && *second ==NULL)
        return 1;
    if (*first == '*' && *(first+1) != NULL && *second == NULL)
        return 0;
    if (*first == '?' || *first == *second)
        return match(first+1, second+1);
    if (*first == '*')
        return match(first+1, second) || match(first, second+1);
    return 0;
}

char find_file(char *name, time_t mt){
    time_t now = time(0);
    int spd = 24*3600;
    int mtd = (now - mt)/spd;

    //printf("filename: %s target: %s", name, targetname);
    if(match(targetname, name) == 1){
        if(modifiedtime != -1 && mtd > modifiedtime) return 0;
        return 1;
    }
    return 0;
}

void my_findDir(char *path) {
    DIR *d; // directory here
    struct dirent *dir;

    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != nullptr) {
            char newpath[1024];
            snprintf(newpath, sizeof(newpath), "%s/%s", path, dir->d_name);
            if (dir->d_type == DT_DIR) {
                if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) continue;
                my_findDir(newpath);
            } else {
                struct stat buf{};
                if(stat(newpath, &buf)==0){
                    if(find_file(dir->d_name, buf.st_mtime)){
                        printf("%s/%s", path, dir->d_name);
                    }
                }
            }
        }
        closedir(d);
    }
}

int main(int argc, char *argv[]){

    printf("Use following format: myfind [PATH] [-option] [para]\n");

    char *optstr = "p:n:m::";

    struct option opts[] = {
            {"path", 1, nullptr, 'p'},
            {"name", 1, nullptr, 'n'},
            {"mtime", 1, nullptr, 'm'},
            {0, 0, 0, 0},
    };
    int opt;
    while((opt = getopt_long(argc, argv, optstr, opts, nullptr)) != -1){
        switch(opt) {
            case 'p':
                strcpy(path, optarg);
                break;
            case 'n':
                strcpy(targetname, optarg);
                break;
            case 'm':
                modifiedtime = atoi(optarg);
                break;
            case '?':
                if(strchr(optstr, optopt) == nullptr){
                    fprintf(stderr, "unknown option '-%c'", optopt);
                }else{
                    fprintf(stderr, "option requires an argument '-%c'", optopt);
                }
                return 1;
            default:
                break;
        }
    }
    my_findDir(path);
    return 0;
}