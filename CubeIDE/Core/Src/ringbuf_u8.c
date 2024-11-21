#include "ringbuf_u8.h"
#include "assert.h"

/*
 * @file
 * Implementation of ring buffer functions.
 */

#if !defined(ASSERT)
#define ASSERT(x)
#endif

#define RINGBUFFER_IS_POWER_OF_TWO(x) (!((x) & ((x)-1UL)))

void ringbuf_u8_init(ringbuf_u8_s *ctx, uint8_t *buffer, size_t nelems) {
  ASSERT(ctx);
  ASSERT(buffer);
  ASSERT(nelems > 1UL);

  ASSERT(RINGBUFFER_IS_POWER_OF_TWO(nelems));

  ctx->buffer = buffer;
  ctx->mask   = nelems - 1UL;
  ctx->tail   = 0;
  ctx->head   = 0;
}

void ringbuf_u8_queue(ringbuf_u8_s *ctx, uint8_t data) {
  ASSERT(ctx);
  ASSERT(ctx->buffer);
  ASSERT(ctx->mask);

  if (ringbuf_u8_is_full(ctx)) {
    /* Is going to overwrite the oldest byte */
    ctx->tail++;
    ctx->tail &= ctx->mask;
  }

  ctx->buffer[ctx->head] = data;
  ctx->head++;
  ctx->head &= ctx->mask;
}

void ringbuf_u8_queue_array(ringbuf_u8_s *ctx, const uint8_t *data,
                            size_t nelems) {
  ASSERT(ctx);
  ASSERT(data);

  for (size_t i = 0; i < nelems; i++) {
    ringbuf_u8_queue(ctx, data[i]);
  }
}

bool ringbuf_u8_dequeue(ringbuf_u8_s *ctx, uint8_t *data) {
  ASSERT(ctx);
  ASSERT(ctx->buffer);
  ASSERT(ctx->mask);
  ASSERT(data);

  bool result = false;

  if (!ringbuf_u8_is_empty(ctx)) {
    *data = ctx->buffer[ctx->tail];
    ctx->tail++;
    ctx->tail &= ctx->mask;
    result = true;
  }
  return result;
}

size_t ringbuf_u8_dequeue_array(ringbuf_u8_s *ctx, uint8_t *data,
                                size_t nelems) {
  ASSERT(ctx);
  ASSERT(data);

  size_t counter   = 0;
  uint8_t *pointer = data;

  while ((counter < nelems) && ringbuf_u8_dequeue(ctx, pointer)) {
    counter++;
    pointer++;
  }
  return counter;
}

bool ringbuf_u8_peek(ringbuf_u8_s *ctx, uint8_t *data, size_t index) {
  ASSERT(ctx);
  ASSERT(ctx->buffer);
  ASSERT(ctx->mask);
  ASSERT(data);

  bool result = false;
  size_t data_index;

  if (index < ringbuf_u8_nelems(ctx)) {
    data_index = (ctx->tail + index) & ctx->mask;
    *data      = ctx->buffer[data_index];
    result     = true;
  }
  return result;
}

bool ringbuf_u8_is_empty(ringbuf_u8_s *ctx) {
  ASSERT(ctx);

  return (ctx->head == ctx->tail);
}

bool ringbuf_u8_is_full(ringbuf_u8_s *ctx) {
  ASSERT(ctx);
  ASSERT(ctx->mask);

  return (((ctx->head - ctx->tail) & ctx->mask) == ctx->mask);
}

size_t ringbuf_u8_nelems(ringbuf_u8_s *ctx) {
  ASSERT(ctx);
  ASSERT(ctx->mask);

  return ((ctx->head - ctx->tail) & ctx->mask);
}
