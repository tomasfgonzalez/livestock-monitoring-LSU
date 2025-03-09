#include "tim2.h"

// You can adjust this if you want a different frequency
#define TIMER_FREQUENCY_HZ 1

void TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /*
    For an APB1 clock of 16 MHz on the L031K6:
      - Prescaler = 16000 - 1  => Timer tick = 16 MHz / 16000 = 1 kHz
      - Period    = 1000 / TIMER_FREQUENCY_HZ - 1 => 999 for 1 Hz => 1-second period

    If your APB1 clock or desired frequency differs, tweak these values.
  */

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 4000 - 1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999; // 1 Hz blink if we set TIMER_FREQUENCY_HZ = 1
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    // You can replace this with your own error function if you like
    Error_Handler();
  }

  // Configure TIM2 clock source
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  // Master configuration (usually default for simple time base)
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
