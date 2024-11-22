#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*
 * @file
 * Prototypes and structures for the ring buffer module.
 */

#ifndef RINGBUF_U8_H
#define RINGBUF_U8_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Structure which holds a ring buffer.
 * The buffer contains a buffer array
 * as well as metadata for the ring buffer.
 */
typedef struct {
  uint8_t *buffer;
  size_t mask;
  size_t tail;
  size_t head;
} ringbuf_u8_s;

/*
 * Initializes the ring buffer pointed to by <em>buffer</em>.
 * This function can also be used to empty/reset the buffer.
 * The resulting buffer can contain <em>nelems-1</em> elements.
 * @param ctx The ring buffer context to initialize.
 * @param buffer The buffer allocated for the ringbuffer.
 * @param nelems The nelems of the allocated ringbuffer.
 */
void ringbuf_u8_init(ringbuf_u8_s *ctx, uint8_t *buffer, size_t nelems);

/*
 * Adds a byte to a ring buffer.
 * @param ctx The buffer context in which the data should be placed.
 * @param data The byte to place.
 */
void ringbuf_u8_queue(ringbuf_u8_s *ctx, uint8_t data);

/*
 * Adds an array of bytes to a ring buffer.
 * @param ctx The buffer context in which the data should be placed.
 * @param data A pointer to the array of bytes to place in the queue.
 * @param nelems The number of elements of the array.
 */
void ringbuf_u8_queue_array(ringbuf_u8_s *ctx, const uint8_t *data,
                            size_t nelems);

/*
 * Returns the oldest byte in a ring buffer.
 * @param ctx The buffer context from which the data should be returned.
 * @param data A pointer to the location at which the data should be placed.
 * @return true if data was returned; false otherwise.
 */
bool ringbuf_u8_dequeue(ringbuf_u8_s *ctx, uint8_t *data);

/*
 * Returns the <em>len</em> oldest bytes in a ring buffer.
 * @param ctx The buffer context from which the data should be returned.
 * @param data A pointer to the array at which the data should be placed.
 * @param nelems The maximum number of bytes to return.
 * @return The number of elements returned.
 */
size_t ringbuf_u8_dequeue_array(ringbuf_u8_s *ctx, uint8_t *data,
                                size_t nelems);

/*
 * Peeks a ring buffer, i.e. returns an element without removing it.
 * @param ctx The buffer from which the data should be returned.
 * @param data A pointer to the location at which the data should be placed.
 * @param index The index to peek.
 * @return true if data was returned; false otherwise.
 */
bool ringbuf_u8_peek(ringbuf_u8_s *ctx, uint8_t *data, size_t index);

/*
 * Returns whether a ring buffer is empty.
 * @param ctx The buffer for which it should be returned whether it is empty.
 * @return true if empty; false otherwise.
 */
bool ringbuf_u8_is_empty(ringbuf_u8_s *ctx);

/*
 * Returns whether a ring buffer is full.
 * @param ctx The buffer for which it should be returned whether it is full.
 * @return true if full; false otherwise.
 */
bool ringbuf_u8_is_full(ringbuf_u8_s *ctx);

/*
 * Returns the number of elements in a ring buffer.
 * @param ctx The buffer for which the number of items should be returned.
 * @return The number of elements in the ring buffer.
 */
size_t ringbuf_u8_nelems(ringbuf_u8_s *ctx);

bool ringbuf_u8_take_last(ringbuf_u8_s *ctx, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif /* RINGBUF_U8_H */
