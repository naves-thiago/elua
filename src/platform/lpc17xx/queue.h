/*
 * queue.h
 *
 * Created: 6/9/2011 19:45:30
 *  Author: Thiago
 */ 

#ifndef QUEUE_H_
#define QUEUE_H_

#include "type.h"
#include <ctype.h>
#include <inttypes.h>
#include <stdlib.h>

/*! \brief Buffer size-1 for UART devices (set this to the desired buffer size + 1) */
#define UART_BUFFER_SIZE 20

/*! \brief Queue struct handles the queue data and position pointers */
typedef struct Queue
{
  uint8_t * buff;	//!< Data vector
  uint16_t beg;		//!< Begin of data pointer
  uint16_t end;		//!< End of data pointer
} queue;

/*! \brief Adds a byte to the buffer
 * \param *q is the queue struct pointer
 * \param data is the byte to be addded to the buffer
 */
void enqueue(queue * q, uint8_t data);

/*! \brief Returns the first (oldest) byte on the buffer and deletes it
 * \param *q is the queue struct pointer
 * \return The first byte of the buffer
 */
uint8_t dequeue(queue * q);

/*! \brief Returns how many bytes are stored in the buffer
 * \param *q is the queue struct pointer
 * \return The buffer data length
 */
uint16_t count(queue * q);

/*! \brief Initializes the queue struct
 * sets the position pointers to 0 and allocs the data buffer
 * \param *q is the queue struct pointer
 */
void init_queue(queue * q);

/*! \brief Initializes the queue struct
 * sets the position pointers to 0 and sets the data buffer to the given pointer (v)
 * \param *q is the queue struct pointer
 * \param *v is the data buffer pointer
 */
void init_queue_v(queue * q, uint8_t *v);

#endif /* QUEUE_H_ */
