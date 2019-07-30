#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char buf[512];

int main(int argc, char *argv[]){

    if (argc < 3) {
        printf("Please pass in source & target file: writer <src.bin> <tar.bin>\n");
        return 0;
    }
    
    char *src = argv[1];
    char *tar = argv[2];

    
    FILE *fsrc = fopen(src, "r");
	fseek(fsrc, 0L, SEEK_END);
	size_t src_size = ftell(fsrc);
    printf("src size: %d bytes\n", src_size);
    // fseek(fsrc, 0, SEEK_SET);
    rewind(fsrc);

    FILE *ftar = fopen(tar, "r+");

    src_size = fread((void *)buf, 1, src_size, fsrc);
    printf("read %u bytes from src\n", src_size);

    size_t wrote_size = fwrite((void *)buf, 1, src_size, ftar);
    printf("wrote %u bytes to tar\n", wrote_size);

    fclose(fsrc); fclose(ftar);
    return 0;
}