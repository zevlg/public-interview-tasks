#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <string.h>             /* memmem() */
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


void
fatal(const char why[static 1])
{
        perror(why);
        exit(-1);
}

void
usage(char progname[static 1])
{
        printf("usage: %s <FILE.h264>\n", progname);
        printf("Вынимает кадры из <FILE.h264> и печатает их тип.\n");
        exit(0);
}

int
main(int ac, char* av[static 1])
{
        if (ac < 2)
                usage(av[0]);

        int fd = open(av[1], O_RDONLY);
        if (fd < 0)
                fatal("open()");

        struct stat fdst;
        fstat(fd, &fdst);

        size_t data_sz = fdst.st_size;
        uint8_t* data = mmap(NULL, data_sz, PROT_READ, MAP_PRIVATE, fd, 0);
        if (data == (void*)-1)
                fatal("mmap()");

        uint8_t* spos;
        while ((spos = memmem(data, data_sz, "\x00\x00\x00\x01", 4))) {
                uint8_t nal_type = spos[4] & 0x3f;
                switch (nal_type) {
                case 1:
                        printf("I");
                        break;
                case 2 ... 5:
                        printf("%d", nal_type);
                        break;
                }

                data_sz -= (spos - data);
                data = spos + 4;
        }
        return 0;
}
