#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>

int dirs, files = 1;

void
scan(char *path) {
  FILE *fp;

  // Ignore certain files by extension
  char *slash = strrchr(path, '/');

  // A path with absolutely no slashes? hm, ok.
  if (!slash)
    return;

  char *ext = strrchr(slash, '.');
  if (ext) {
    if (strcmp(ext, ".db") == 0
        || strcmp(ext, ".DS_Store") == 0
        || strcmp(ext, ".raw") == 0 
        || strcmp(ext, ".o") == 0 
        || strcmp(ext, ".a") == 0 
        || strcmp(ext, ".node") == 0) {
      printf(" skipping: %s\n", path);
      return;
    }
  } else {
    // Ignore files with no extension
    printf(" skipping: %s\n", path);
    return;
  }

  printf(" scanning: %s\n", path);
  files++;

  fp = fopen(path, "r");

  for (;;) {
    size_t pos = ftell(fp);
    unsigned char c = fgetc(fp);
    unsigned char d = fgetc(fp);
    unsigned char e = fgetc(fp);

    if (feof(fp))
      break;

    // https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-42574
    // Evil characters are [\u202a-\u202e\u2066-\u2069]
    // In UTF-8 that's 0xE280AA-0xE280AE and 0xE281A6-0xE281A9

    // https://certitude.consulting/blog/en/invisible-backdoor/
    // also include \u3164 in UTF-8 that's 0xE385A4 

    if ((c == 0xe2 && d == 0x80 && (e >= 0xaa && e <= 0xae))
        || (c == 0xe2 && d == 0x81 && (e >= 0xa6 && e <= 0xa9))
        || (c == 0xe3 && d == 0x85 && e == 0xa4)) {
      printf("  illegal character %02x%02x%02x found in file %s at position %ld\n", c, d, e, path, ftell(fp));
      assert(0);
    }

    fseek(fp, -2, SEEK_CUR);
  }

  fclose(fp);
}

void
checkFiles(char *basePath) {
  char path[1000];
  struct dirent *dp;
  DIR *dir = opendir(basePath);

  // Unable to open directory (is file)
  if (!dir) {
    scan(basePath);
    return;
  }

  printf("dir: %s\n", basePath);
  dirs++;

  while ((dp = readdir(dir)) != NULL) {
    if (strcmp(dp->d_name, ".") != 0
        && strcmp(dp->d_name, "..") != 0
        && strcmp(dp->d_name, ".git") != 0) {
      strcpy(path, basePath);
      strcat(path, "/");
      strcat(path, dp->d_name);
      checkFiles(path);
    }
  }

  closedir(dir);
}

int
main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Directory path required.\n");
    return 1;
  }

  time_t begin, end;
  begin = time(NULL);
  checkFiles(argv[1]);
  end = time(NULL);

  printf("OK: Scanned %d files in %d directories in %f seconds\n", files, dirs, difftime(end, begin));

  return 0;
}
