#include "queue.h"

inline void enqueue(queue * q, uint8_t data)
{
  q->buff[q->end] = data;
  q->end = (q->end + 1) % UART_BUFFER_SIZE;

  if (q->end == q->beg)
    q->beg = (q->beg + 1) % UART_BUFFER_SIZE;
}

inline uint8_t dequeue(queue * q)
{
  if (q->end == q->beg)
    return 0;

  uint16_t tmp = q->beg;
  q->beg = (q->beg + 1) % UART_BUFFER_SIZE;

  return q->buff[tmp];
}

inline uint16_t count(queue * q)
{
  if ((q->beg == 0) && (q->end == 0))
    return 0;

  if (q->beg <= q->end)
    return q->end - q->beg;

  return UART_BUFFER_SIZE - q->beg + q->end;
}

void init_queue(queue * q)
{
  q->buff = (uint8_t *)malloc(UART_BUFFER_SIZE * sizeof(uint8_t));
  q->beg = 0;
  q->end = 0;
}

void init_queue_v(queue * q, uint8_t * v)
{
  q->buff = v;
  q->beg = 0;
  q->end = 0;
}

