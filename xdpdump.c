#include <errno.h>
#include <linux/if_xdp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>

#define FRAME_SIZE 2048
#define NUM_FRAMES 2048

#define FILL_RING_SIZE 512
#define COMPLETION_RING_SIZE 512

int main() {
    int xsk_fd;
    void* umem;
    struct xdp_umem_reg mr;
    int fill_ring_size = FILL_RING_SIZE;
    int completion_ring_size = COMPLETION_RING_SIZE;

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

    memset(&mr, 0, sizeof(struct xdp_umem_reg));
    mr.addr = (uint64_t)umem;
    mr.len = FRAME_SIZE * NUM_FRAMES;
    mr.chunk_size = FRAME_SIZE;
    mr.headroom = 0;
    mr.flags = 0;

    if(setsockopt(xsk_fd, SOL_XDP, XDP_UMEM_REG, &mr, sizeof(mr))) {
        printf("umem reg failed: %s\n", strerror(errno));
        return -1;
    }

    if(setsockopt(xsk_fd, SOL_XDP, XDP_UMEM_FILL_RING, &fill_ring_size, sizeof(fill_ring_size))) {
        printf("fill ring failed: %s\n", strerror(errno));
        return -1;
    }

    if(setsockopt(xsk_fd, SOL_XDP, XDP_UMEM_COMPLETION_RING,
       &completion_ring_size, sizeof(completion_ring_size))) {
        printf("completion ring failed: %s\n", strerror(errno));
        return -1;
    }

    printf("==== success ====\n");

    return 0;
}