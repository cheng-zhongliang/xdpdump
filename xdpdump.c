#include <errno.h>
#include <linux/if_xdp.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>

#define FRAME_SIZE 2048
#define NUM_FRAMES 2048

int main() {
    int xsk_fd;
    void* umem;
    struct xdp_umem_reg mr;

    xsk_fd = socket(AF_XDP, SOCK_RAW, 0);
    if(xsk_fd < 0) {
        printf("xsk create failed: %s\n", strerror(errno));
        return -1;
    }

    umem = mmap(0, FRAME_SIZE * NUM_FRAMES, PROT_READ | PROT_WRITE,
    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(umem == MAP_FAILED) {
        printf("mmap failed: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}