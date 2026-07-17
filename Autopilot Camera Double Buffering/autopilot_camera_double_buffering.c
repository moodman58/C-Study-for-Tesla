/*
 * ============================================================
 * Autopilot Camera Double-Buffering
 * ============================================================
 *
 * Tesla's Autopilot cameras capture frames continuously while the
 * perception pipeline processes the PREVIOUS frame. This requires
 * double-buffering: one buffer is written by the camera while the
 * other is read by the processing pipeline, and they SWAP roles
 * each frame (no copying -- swapping pointers is fast, copying isn't).
 *
 * Implement:
 *   1. fill_buffer()      - write pixel data into a buffer using
 *                           POINTER ARITHMETIC (not array indexing)
 *   2. sum_buffer()       - read pixel data back out, again using
 *                           pointer arithmetic
 *   3. swap_buffers()     - swap which buffer is "active" (write target)
 *                           vs "ready" (read target), by swapping the
 *                           POINTERS THEMSELVES -- must take pointer-
 *                           to-pointer params so it can modify the
 *                           caller's pointers
 *   4. process_frame()    - simulates one full frame cycle: fill the
 *                           active buffer, then swap, then sum the
 *                           now-ready buffer and print the result
 * ============================================================
 */

#include <stdio.h>

#define BUFFER_SIZE 16   // pixels per frame (simplified/small for this exercise)

typedef struct {
    unsigned char pixels[BUFFER_SIZE];
} frame_buffer_t;

// Fills `buf` with `count` values, starting at `start_value` and
// incrementing by 1 each pixel (e.g. start_value=10 -> 10,11,12,...).
// MUST use pointer arithmetic (e.g. *(buf + i) or incrementing a
// pointer directly) rather than buf->pixels[i] array indexing.
void fill_buffer(frame_buffer_t *buf, unsigned char start_value) {
    // TODO
}

// Returns the sum of all pixel values in `buf`.
// MUST use pointer arithmetic to walk the buffer, not array indexing.
int sum_buffer(const frame_buffer_t *buf) {
    // TODO
}

// Swaps which buffer pointer is considered "active" vs "ready".
// active and ready are POINTERS TO POINTERS -- this function must
// actually swap what the CALLER's pointers point to, not just swap
// local copies.
void swap_buffers(frame_buffer_t **active, frame_buffer_t **ready) {
    // TODO
}

// Simulates one frame cycle:
//   1. fill the buffer pointed to by *active with `frame_number` as
//      the start_value
//   2. swap active/ready so the freshly-filled buffer becomes "ready"
//   3. sum the now-ready buffer and print the frame_number + sum
void process_frame(frame_buffer_t **active, frame_buffer_t **ready, int frame_number) {
    // TODO
}

int main(void) {
    frame_buffer_t buffer_a = {0};
    frame_buffer_t buffer_b = {0};

    frame_buffer_t *active = &buffer_a;   // camera writes here
    frame_buffer_t *ready  = &buffer_b;   // pipeline reads here

    for (int frame = 1; frame <= 4; frame++) {
        process_frame(&active, &ready, frame);
    }

    return 0;
}