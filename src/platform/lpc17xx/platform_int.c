// MBED interrupt support

// Generic headers
#include "platform.h"
#include "platform_conf.h"
#include "elua_int.h"
#include "common.h"

#include <stdio.h>

// Platform-specific headers
#include "LPC17xx.h"
#include "LPC17xx_timer.h"
#include "mbed_rtc.h"
#include "core_cm3.h"
#include "queue.h"

// UART Interrupt
IRQn_Type uart_irqs[4] = {UART0_IRQn, UART1_IRQn, UART2_IRQn, UART3_IRQn}; 
extern LPC_UART_TypeDef * uart[4];
queue uart_queue[4];

/*
void UART0_IRQHandler(void)
{
  if (LPC_UART0->IIR & 0x05) // We have an Receive Data Avaliable Interrupt
    cmn_int_handler( INT_UART_RX, 0 ); // Add an elua interrupt

  // Enqueue the received char
  // Interrupt is cleared by reading the data
  enqueue(&(uart_queue[0]), LPC_UART0->RBR);

  NVIC_ClearPendingIRQ(UART0_IRQn);
}
*/

void UART1_IRQHandler(void)
{
  if (LPC_UART1->IIR & 0x05) // We have an Receive Data Avaliable Interrupt
    cmn_int_handler( INT_UART_RX, 1 ); // Add an elua interrupt

  // Enqueue the received char
  // Interrupt is cleared by reading the data
  enqueue(&(uart_queue[1]), LPC_UART1->RBR);

  NVIC_ClearPendingIRQ(UART1_IRQn);
}

void UART2_IRQHandler(void)
{
  if (LPC_UART2->IIR & 0x05) // We have an Receive Data Avaliable Interrupt
    cmn_int_handler( INT_UART_RX, 2 ); // Add an elua interrupt

  // Enqueue the received char
  // Interrupt is cleared by reading the data
  enqueue(&(uart_queue[2]), LPC_UART2->RBR);

  NVIC_ClearPendingIRQ(UART2_IRQn);
}

void UART3_IRQHandler(void)
{
  if (LPC_UART3->IIR & 0x05) // We have an Receive Data Avaliable Interrupt
    cmn_int_handler( INT_UART_RX, 3 ); // Add an elua interrupt

  // Enqueue the received char
  // Interrupt is cleared by reading the data
  enqueue(&(uart_queue[3]), LPC_UART3->RBR);

  NVIC_ClearPendingIRQ(UART3_IRQn);
}

static int int_uart_rx_get_status( elua_int_resnum resnum )
{
  return NVIC_GetActive( uart_irqs[resnum] );
}

static int int_uart_rx_set_status( elua_int_resnum resnum, int status )
{
  int prev = int_uart_rx_get_status( resnum ); 

  if (status == PLATFORM_CPU_ENABLE)
  {
    // Enable uart interrupt
    uart[resnum]->IER = 1; // Enable Receive Data Avaliable Interrupt
    NVIC_ClearPendingIRQ(uart_irqs[resnum]);
    NVIC_EnableIRQ(uart_irqs[resnum]);
//    NVIC_SetPriority(uart_irqs[resnum], (0x01<<3)); // <- important!
  }
  else
  {
    // Enable uart interrupt
    NVIC_DisableIRQ(uart_irqs[resnum]);
  }
  
  return prev;
}

static int int_uart_rx_get_flag( elua_int_resnum resnum, int clear )
{
  int status = (uart[resnum]->IIR & 0x05) != 0;

  /*
  if( clear )
    // Clear interrupt flag
    LPC_RTC->ILR = 2;
  */

  return status;
}

// ****************************************************************************

// RTC Interrupt
void RTC_IRQHandler(void)
{
  // Add an elua interrupt
  cmn_int_handler( INT_RTC_ALARM, 0 );

  // Clear interrupt flag
  LPC_RTC->ILR = 2;
}

static int int_rtc_alarm_get_status( elua_int_resnum resnum )
{
  return NVIC_GetActive( RTC_IRQn );
}

static int int_rtc_alarm_set_status( elua_int_resnum resnum, int status )
{
  int prev = int_rtc_alarm_get_status( resnum ); 

  if (status == PLATFORM_CPU_ENABLE)
  {
    // Enable alarm interrupt
    NVIC_ClearPendingIRQ(RTC_IRQn);
    NVIC_EnableIRQ(RTC_IRQn);
    NVIC_SetPriority(RTC_IRQn, ((0x01<<3)|0x01)); // <- important!

    // Clear interrupt flag
    LPC_RTC->ILR = 2;
  }
  else
  {
    // Enable alarm interrupt
    NVIC_DisableIRQ(RTC_IRQn);
  }
  
  return prev;
}

static int int_rtc_alarm_get_flag( elua_int_resnum resnum, int clear )
{
  int status = (LPC_RTC->ILR & 2) != 0;

  if( clear )
    // Clear interrupt flag
    LPC_RTC->ILR = 2;

  return status;
}

// ****************************************************************************
// Timer interrupts
extern LPC_TIM_TypeDef *tmr[];
extern u8 mbed_timer_int_periodic_flag[ NUM_PHYS_TIMER ];
static int int_tmr_match_get_status( elua_int_resnum resnum )
{
  switch (resnum)
  {
    case 0: return NVIC_GetActive( TIMER0_IRQn ); break;
    case 1: return NVIC_GetActive( TIMER1_IRQn ); break;
    case 2: return NVIC_GetActive( TIMER2_IRQn ); break;
    case 3: return NVIC_GetActive( TIMER3_IRQn ); break;
  }
}

static int int_tmr_match_set_status( elua_int_resnum resnum, int status )
{
  int prev = int_tmr_match_get_status( resnum ); 

  if (status == PLATFORM_CPU_ENABLE)
  {
    // Enable timer interrupt & clear interrupt flag
    switch (resnum)
    {
      case 0: {
                LPC_TIM0->IR |= 1;
                NVIC_EnableIRQ( TIMER0_IRQn );
                break;
              }
      case 1: {
                LPC_TIM1->IR |= 1;
                NVIC_EnableIRQ( TIMER1_IRQn );
                break;
              }

      case 2: {
                LPC_TIM2->IR |= 1;
                NVIC_EnableIRQ( TIMER2_IRQn );
                break;
              }
      case 3: {
                LPC_TIM3->IR |= 1;
                NVIC_EnableIRQ( TIMER3_IRQn );
                break;
              }
    }

  }
  else
  {
    // Disable timer interrupt
    switch (resnum)
    {
      case 0: NVIC_DisableIRQ( TIMER0_IRQn ); break;
      case 1: NVIC_DisableIRQ( TIMER1_IRQn ); break;
      case 2: NVIC_DisableIRQ( TIMER2_IRQn ); break;
      case 3: NVIC_DisableIRQ( TIMER3_IRQn ); break;
    }
  }
  
  return prev;
}

static int int_tmr_match_get_flag( elua_int_resnum resnum, int clear )
{
    switch (resnum)
    {
      case 0: return LPC_TIM0->IR & 1; break;
      case 1: return LPC_TIM1->IR & 1; break;
      case 2: return LPC_TIM2->IR & 1; break;
      case 3: return LPC_TIM3->IR & 1; break;
    }

    if (clear)
      switch (resnum)
      {
        case 0: LPC_TIM0->IR = 1; break;
        case 1: LPC_TIM1->IR = 1; break;
        case 2: LPC_TIM2->IR = 1; break;
        case 3: LPC_TIM3->IR = 1; break;
      }
}

static void tmr_int_handler( int id )
{
  /*
  TIM_ClearFlag( base, TIM_FLAG_OC1 );
  TIM_CounterCmd( base, TIM_CLEAR );
  if( id == VTMR_TIMER_ID )
  {
    cmn_virtual_timer_cb();
    cmn_systimer_periodic();
  }
  else
  */

  // Timer OFF
  tmr[ id ]->TCR &= ~(1 << 0); 

  // Reset
  tmr[ id ]->TCR |= 1 << 1; 
  tmr[ id ]->TCR &= ~(1 << 1);

  if( mbed_timer_int_periodic_flag[ id ] == PLATFORM_TIMER_INT_CYCLIC )
  {
    // Timer ON
    tmr[ id ]->TCR |= 1 << 0; 
  }

  cmn_int_handler( INT_TMR_MATCH, id );
}

void TIMER0_IRQHandler(void)
{
  if ((LPC_TIM0->IR & 0x01) == 0x01) // if MR0 interrupt
  {
    LPC_TIM0->IR |= 1 << 0; // Clear MR0 interrupt flag
    tmr_int_handler( 0 );
  }
}

void TIMER1_IRQHandler(void)
{
  if ((LPC_TIM1->IR & 0x01) == 0x01) // if MR0 interrupt
  {
    LPC_TIM1->IR |= 1 << 0; // Clear MR0 interrupt flag
    tmr_int_handler( 1 );
  }
}

void TIMER2_IRQHandler(void)
{
  if ((LPC_TIM2->IR & 0x01) == 0x01) // if MR0 interrupt
  {
    LPC_TIM2->IR |= 1 << 0; // Clear MR0 interrupt flag
    tmr_int_handler( 2 );
  }
}

void TIMER3_IRQHandler(void)
{
  if ((LPC_TIM3->IR & 0x01) == 0x01) // if MR0 interrupt
  {
    LPC_TIM3->IR |= 1 << 0; // Clear MR0 interrupt flag
    tmr_int_handler( 3 );
  }
}


// Interrupt table
// Must have a 1-to-1 correspondence with the interrupt enum in platform_conf.h!

const elua_int_descriptor elua_int_table[ INT_ELUA_LAST ] = 
{
  { int_rtc_alarm_set_status, int_rtc_alarm_get_status, int_rtc_alarm_get_flag },
  { int_tmr_match_set_status, int_tmr_match_get_status, int_tmr_match_get_flag },
  { int_uart_rx_set_status, int_uart_rx_get_status, int_uart_rx_get_flag }
};

// ****************************************************************************
// Interrupt initialization

void platform_int_init()
{
  // Init queues
  init_queue(&(uart_queue[0])); 
  init_queue(&(uart_queue[1])); 
  init_queue(&(uart_queue[2])); 
  init_queue(&(uart_queue[3])); 

  // Clear RTC interrupt flag
  LPC_RTC->ILR = 2;
  
  // Clear TIMER interrupt flags
  TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
  TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);
  TIM_ClearIntPending(LPC_TIM2, TIM_MR0_INT);
  TIM_ClearIntPending(LPC_TIM3, TIM_MR0_INT);
}

