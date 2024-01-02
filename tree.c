// authors : shay finegold 311165609 , dan monsonego 313577595

#include "myShell.h"

void print_tree(const char *path, int level) {
    DIR *dir = opendir(path);
    if (!dir) {
        fprintf(stderr, "Could not open directory '%s': %s\n", path, strerror(errno));
        return;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        char *subpath = malloc(strlen(path) + strlen(entry->d_name) + 2); // +2 for the '/' and '\0'
        sprintf(subpath, "%s/%s", path, entry->d_name);
        
        struct stat info;
        if (stat(subpath, &info) == -1) {
            fprintf(stderr, "Could not get information for '%s': %s\n", subpath, strerror(errno));
            free(subpath);
            continue;
        }
        
        if (S_ISDIR(info.st_mode)) {
            printf("%*s[%s]\n", level*2, "", entry->d_name);
            print_tree(subpath, level + 1);
        } else if (S_ISREG(info.st_mode)) {
            printf("%*s- %s\n", level*2, "", entry->d_name);
        }
        
        free(subpath);
    }
    
    if (closedir(dir) == -1) {
        fprintf(stderr, "Could not close directory '%s': %s\n", path, strerror(errno));
    }
}

int main(int argc, char **argv) {

    print_tree(argv[1], 0);
    return 0;
    
}