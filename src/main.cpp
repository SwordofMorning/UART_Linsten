#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "uartwrap.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <baudrate> <device>\n", argv[0]);
        return -1;
    }

    int baudrate = atoi(argv[1]);
    char *dev_name = argv[2];

    int fd = UARTWRAP_InitDevice(dev_name, baudrate);
    if (fd < 0) {
        printf("Failed to initialize device.\n");
        return -1;
    }

    unsigned char buf[1024];
    ssize_t n;

    while (1) {
        n = read(fd, buf, sizeof(buf));
        if (n > 0) {
            printf("Received data: ");
            for (int i = 0; i < n; i++) {
                printf("%02X ", buf[i]);
            }
            printf("\n");
        }
    }

    UARTWRAP_ReleaseDevice(fd);
    return 0;
}