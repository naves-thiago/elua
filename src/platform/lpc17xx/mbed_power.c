// MBED specific low power support
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "platform.h"
#include "lrotable.h"
#include "platform_conf.h"
#include "auxmods.h"
#include "LPC17xx.h"
#include "core_cm3.h"

int mbed_off();
static int mbed_power_sleep( lua_State *L )
{  
  // Clear all interrupts
  NVIC->ICPR[0] = 0xFF;
  NVIC->ICPR[1] = 7;

  // Disable SysTick Interrupt
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

  mbed_off();
//  SCB->SCR |= 0x04; // Deep Sleep
  __WFI();

  // Re enable SysTick
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

  return 0;
}

// Module function map
#define MIN_OPT_LEVEL 2
#include "lrodefs.h" 
const LUA_REG_TYPE mbed_power_map[] =
{
  { LSTRKEY( "sleep" ),  LFUNCVAL( mbed_power_sleep ) },
  { LNILKEY, LNILVAL }
};

