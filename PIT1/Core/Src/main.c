/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7735\st7735.h"
#include<stdlib.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_TENT 3
#define MAX_ALUNOS_FORA 3
#define MAX_ALUNOS_SYS 99

#define TXTCOLOR WHITE
#define BGCOLOR BLACK

#define LER_BTNDIR()   HAL_GPIO_ReadPin(BTNDIR_GPIO_Port, BTNDIR_Pin)
#define LER_BTNESQ()   HAL_GPIO_ReadPin(BTNESQ_GPIO_Port, BTNESQ_Pin)
#define LER_BTNCIMA()  HAL_GPIO_ReadPin(BTNCIMA_GPIO_Port, BTNCIMA_Pin)
#define LER_BTNBAIXO() HAL_GPIO_ReadPin(BTNBAIXO_GPIO_Port, BTNBAIXO_Pin)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
GPIO_PinState Debounced_BTNCIMA = GPIO_PIN_SET;
volatile uint8_t btn_event_BTNCIMA = 0;
static uint8_t Count_BTNCIMA=20;

GPIO_PinState Debounced_BTNBAIXO = GPIO_PIN_SET;
volatile uint8_t btn_event_BTNBAIXO = 0;
static uint8_t Count_BTNBAIXO=20;

GPIO_PinState Debounced_BTNESQ = GPIO_PIN_SET;
volatile uint8_t btn_event_BTNESQ = 0;
static uint8_t Count_BTNESQ=20;

GPIO_PinState Debounced_BTNDIR = GPIO_PIN_SET;
volatile uint8_t btn_event_BTNDIR = 0;
static uint8_t Count_BTNDIR=20;


volatile uint8_t inicio_aula = 1;
volatile uint8_t estagio_senha = 0;

volatile uint8_t config_inicial = 0;
volatile uint8_t is_init_config_inicial=0;

volatile uint8_t exe_aula = 0;
volatile uint8_t is_init_exe_aula = 0;
volatile uint8_t encerr = 0;

uint8_t user_digit = 0;
char user_digit_display[2];

char first_digit[2];
int8_t first_digit_num = -1;
char second_digit[2];
int8_t second_digit_num = -1;

char third_digit[2];
int8_t third_digit_num=-1;

char forth_digit[2];
int8_t forth_digit_num = -1;

char fifth_digit[2];
int8_t fifth_digit_num = -1;

char sixth_digit[2];
int8_t sixth_digit_num= -1;

uint8_t tentativas_senha = 0;

char max_quant_alunos_display[6];

uint8_t porcentagem;
uint16_t max_quant_alunos=0;
uint16_t quant_presentes=0;
uint8_t alunos_fora=0;




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void DebounceSwitch(GPIO_PinState raw, GPIO_PinState *debounced, uint8_t *counter);

void Aula_init(void);
void finish_incioAula(void);

void consume_btn_events(void);

void Do_action_dir_senha(uint32_t senha);
void Do_action_cima_senha(void);
void Do_action_baixo_senha(void);
void Do_action_esq_senha(void);
void AvaliarSenha(uint32_t senha);


void Do_action_cima_configInicial(void);
void Do_action_baixo_configInicial(void);
void Do_action_dir_configInicial(void);

void config_inicial_init(void);

void DrawProgressBar(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void update_aula_status(void);



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  ST7735_Init();
  HAL_TIM_Base_Start_IT(&htim2);

  user_digit_display[0]= user_digit+48;
  user_digit_display[1]= '\0';
  first_digit[1]= '\0';
  second_digit[1] = '\0';
  third_digit[1] = '\0';
  forth_digit[1] = '\0';
  fifth_digit[1] = '\0';
  sixth_digit[1] = '\0';



  Aula_init();

  uint32_t senha=0;



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      if(inicio_aula){
          if(btn_event_BTNBAIXO || btn_event_BTNCIMA || btn_event_BTNDIR || btn_event_BTNESQ){
              finish_incioAula();
              srand(HAL_GetTick());
              senha = 100000 + (rand() % (999999-100000+1));
              consume_btn_events();

          }

      }

      if(estagio_senha){
          if(btn_event_BTNDIR){
            Do_action_dir_senha(senha);

          }
          else if(btn_event_BTNESQ){
            Do_action_esq_senha();

          }
          else if(btn_event_BTNCIMA){
             Do_action_cima_senha();
          }
          else if(btn_event_BTNBAIXO){
             Do_action_baixo_senha();
          }
      }

      if(config_inicial){
          if(!is_init_config_inicial){
            config_inicial_init();
            is_init_config_inicial=1;
          }

          if(btn_event_BTNCIMA) Do_action_cima_configInicial();
          else if(btn_event_BTNBAIXO) Do_action_baixo_configInicial();
          else if(btn_event_BTNDIR) Do_action_dir_configInicial();


      }

      if(exe_aula){
    	  if(!is_init_exe_aula) {
    		  update_aula_status();
    		  is_init_exe_aula=1;
    	  }
      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 799;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 49;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ST7735_DC_Pin|ST7735_RES_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ST7735_CS_Pin */
  GPIO_InitStruct.Pin = ST7735_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ST7735_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ST7735_DC_Pin ST7735_RES_Pin */
  GPIO_InitStruct.Pin = ST7735_DC_Pin|ST7735_RES_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : BTNESQ_Pin */
  GPIO_InitStruct.Pin = BTNESQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BTNESQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BTNBAIXO_Pin BTNDIR_Pin BTNCIMA_Pin */
  GPIO_InitStruct.Pin = BTNBAIXO_Pin|BTNDIR_Pin|BTNCIMA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void DebounceSwitch(GPIO_PinState raw, GPIO_PinState *debounced, uint8_t *counter){
    if(raw == *debounced) *counter = 20;
    else{
        if(--(*counter)==0){
            *debounced = raw;
            *counter= 20;
        }
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    GPIO_PinState raw_btncima= LER_BTNCIMA();
    GPIO_PinState old_btncima = Debounced_BTNCIMA;
    DebounceSwitch(raw_btncima, &Debounced_BTNCIMA, &Count_BTNCIMA);
    if(old_btncima == GPIO_PIN_SET && Debounced_BTNCIMA == GPIO_PIN_RESET){
        btn_event_BTNCIMA =1;
    }

    GPIO_PinState raw_btnbaixo= LER_BTNBAIXO();
    GPIO_PinState old_btnbaixo = Debounced_BTNBAIXO;
    DebounceSwitch(raw_btnbaixo, &Debounced_BTNBAIXO, &Count_BTNBAIXO);
    if(old_btnbaixo == GPIO_PIN_SET && Debounced_BTNBAIXO == GPIO_PIN_RESET){
        btn_event_BTNBAIXO =1;
    }
    GPIO_PinState raw_btnesq= LER_BTNESQ();
    GPIO_PinState old_btnesq = Debounced_BTNESQ ;
    DebounceSwitch(raw_btnesq, &Debounced_BTNESQ, &Count_BTNESQ);
    if(old_btnesq == GPIO_PIN_SET && Debounced_BTNESQ == GPIO_PIN_RESET){
        btn_event_BTNESQ =1;
    }
    GPIO_PinState raw_btndir= LER_BTNDIR();
    GPIO_PinState old_btndir = Debounced_BTNDIR;
    DebounceSwitch(raw_btndir, &Debounced_BTNDIR, &Count_BTNDIR);
    if(old_btndir == GPIO_PIN_SET && Debounced_BTNDIR == GPIO_PIN_RESET){
        btn_event_BTNDIR =1;
    }


}

void Aula_init(void){
    ST7735_FillScreen(BGCOLOR);
    ST7735_WriteString(0, 0, "Clique em qualquer botao para iniciar.", Font_7x10, TXTCOLOR, BGCOLOR);
}

void finish_incioAula(void){
    inicio_aula = 0;
    estagio_senha = 1;
    ST7735_FillScreen(BGCOLOR);
    ST7735_WriteString(0, 0, "Insira a senha", Font_7x10, TXTCOLOR, BGCOLOR);
    ST7735_WriteString(0, 15, user_digit_display, Font_7x10, TXTCOLOR, BGCOLOR);
    consume_btn_events();
}

void consume_btn_events(void){
    btn_event_BTNBAIXO=0;
    btn_event_BTNCIMA=0;
    btn_event_BTNDIR=0;
    btn_event_BTNESQ =0;
}

void AvaliarSenha(uint32_t senha){
    tentativas_senha++;
    uint32_t user_senha = first_digit_num*100000 + second_digit_num*10000 + third_digit_num*1000 + forth_digit_num*100 + fifth_digit_num*10 + sixth_digit_num;
    if(user_senha == senha){
        tentativas_senha=0;
        estagio_senha =0;
        config_inicial=1;
        ST7735_FillScreenFast(BGCOLOR);
        ST7735_WriteString(0, 0, "Senha Correta!",Font_16x26 , TXTCOLOR, BGCOLOR);
        HAL_Delay(2000);
        ST7735_FillScreenFast(BGCOLOR);

    }

    else{
        ST7735_FillScreenFast(BGCOLOR);
        ST7735_WriteString(0, 0, "Senha Incorreta!",Font_16x26 , TXTCOLOR, BGCOLOR);
        HAL_Delay(1000);

        if(tentativas_senha==MAX_TENT){
            ST7735_FillScreenFast(BGCOLOR);
            ST7735_WriteString(0, 0, "Aguarde 3 segundos ate inserir a senha        novamente",Font_11x18 , TXTCOLOR, BGCOLOR);
            HAL_Delay(3000);

        }

        first_digit_num = -1; second_digit_num =-1; third_digit_num=-1; forth_digit_num=-1; fifth_digit_num=-1; sixth_digit_num=-1;


        ST7735_FillScreen(BGCOLOR);
        ST7735_WriteString(0, 0, "Insira a senha", Font_7x10, TXTCOLOR, BGCOLOR);
        ST7735_WriteString(0, 15, user_digit_display , Font_7x10, TXTCOLOR, BGCOLOR);

    }
}

void Do_action_dir_senha(uint32_t senha){

     if(first_digit_num<0) {
         first_digit[0] = user_digit+48;
         first_digit_num = user_digit;
         ST7735_WriteString(59, 45, first_digit, Font_7x10, TXTCOLOR, BGCOLOR);

     }
     else if(second_digit_num<0){
         second_digit[0] = user_digit+48;
         second_digit_num = user_digit;
         ST7735_WriteString(59+7, 45, second_digit, Font_7x10, TXTCOLOR, BGCOLOR);

     }
     else if(third_digit_num<0){
         third_digit[0] = user_digit+48;
         third_digit_num = user_digit;
         ST7735_WriteString(2*7+59, 45, third_digit, Font_7x10, TXTCOLOR, BGCOLOR);
     }
     else if(forth_digit_num<0){
         forth_digit[0] = user_digit+48;
         forth_digit_num = user_digit;
         ST7735_WriteString(3*7+59, 45, forth_digit, Font_7x10, TXTCOLOR, BGCOLOR);
     }
     else if(fifth_digit_num<0){
         fifth_digit[0] = user_digit + 48;
         fifth_digit_num = user_digit;
         ST7735_WriteString(4*7+59, 45, fifth_digit, Font_7x10, TXTCOLOR, BGCOLOR);
     }
     else if(sixth_digit_num<0){
         sixth_digit[0] = user_digit + 48;
         sixth_digit_num = user_digit;
         ST7735_WriteString(5*7+59, 45, sixth_digit, Font_7x10, TXTCOLOR, BGCOLOR);

     }
     else{
           AvaliarSenha(senha);
     }

     consume_btn_events();
}

void Do_action_cima_senha(void){
     if(user_digit !=9) user_digit++;
     else  user_digit =0;

     user_digit_display[0]= user_digit+48;

     ST7735_WriteString(0, 20, user_digit_display, Font_7x10, TXTCOLOR, BGCOLOR);

     consume_btn_events();
}

void Do_action_baixo_senha(void){
     if(user_digit !=0) user_digit--;
     else user_digit = 9;

     user_digit_display[0]= user_digit+48;


     ST7735_WriteString(0, 20, user_digit_display, Font_7x10, TXTCOLOR, BGCOLOR);

     consume_btn_events();
}

void Do_action_esq_senha(void){
    if(sixth_digit_num>=0) ST7735_FillRectangleFast(5*7+59, 45, 5*7+59+7, 50, BGCOLOR);
    else if(fifth_digit_num>=0) ST7735_FillRectangleFast(4*7+59, 45, 4*7+59+7, 55, BGCOLOR);
    else if(forth_digit_num>=0) ST7735_FillRectangleFast(3*7+59, 45, 3*7+59+7, 55, BGCOLOR);
    else if(third_digit_num>=0) ST7735_FillRectangleFast(2*7+59, 45, 2*7+59+7, 55, BGCOLOR);
    else if(second_digit_num>=0) ST7735_FillRectangleFast(1*7+59, 45, 1*7+59+7, 55, BGCOLOR);
    else if(first_digit_num>=0) ST7735_FillRectangleFast(59, 45, +59+7, 55, BGCOLOR);
    consume_btn_events();
}

void config_inicial_init(void){
    ST7735_WriteString(0, 0, "Max. de alunos na sala", Font_7x10, TXTCOLOR, BGCOLOR);
    sprintf(max_quant_alunos_display, "%u", max_quant_alunos);
    ST7735_WriteString(69, 30, max_quant_alunos_display, Font_11x18, TXTCOLOR, BGCOLOR);

}

void Do_action_cima_configInicial(void){
    if(max_quant_alunos<MAX_ALUNOS_SYS){
        max_quant_alunos++;
        sprintf(max_quant_alunos_display, "%u", max_quant_alunos);
        ST7735_WriteString(69, 30, max_quant_alunos_display, Font_11x18, TXTCOLOR, BGCOLOR);

    }
    consume_btn_events();
}
void Do_action_baixo_configInicial(void){

    if(max_quant_alunos>0){
        max_quant_alunos--;
        sprintf(max_quant_alunos_display, "%u", max_quant_alunos);
        ST7735_WriteString(69, 30, max_quant_alunos_display, Font_11x18, TXTCOLOR, BGCOLOR);


    }
    consume_btn_events();
}
void Do_action_dir_configInicial(void){
    consume_btn_events();
    if(max_quant_alunos==0){
     ST7735_FillScreenFast(BGCOLOR);
     ST7735_WriteString(0, 0, "A sala deve possuir ao menos 1 aluno.", Font_7x10, TXTCOLOR, BGCOLOR);
     HAL_Delay(5000);
     config_inicial_init();
}
    else{
        ST7735_FillScreenFast(BGCOLOR);
        ST7735_WriteString(0, 0, "Max. de alunos salvo.", Font_7x10, TXTCOLOR, BGCOLOR);
        HAL_Delay(5000);
        config_inicial =0;
        exe_aula=1;
    }

    }

void DrawProgressBar(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    porcentagem = (quant_presentes * 100) / max_quant_alunos;
    ST7735_FillRectangle(x, y, w, 1, WHITE);
    ST7735_FillRectangle(x, y + h, w, 1, WHITE);
    ST7735_FillRectangle(x, y, 1, h, WHITE);
    ST7735_FillRectangle(x + w, y, 1, h, WHITE);
    ST7735_FillRectangle(x+1, y+1, w-2, h-2, BLACK);
    uint16_t fill = (w * porcentagem) / 100;

   if (fill > 2)
   {
       ST7735_FillRectangle(x+1, y+1, fill-2, h-2, GREEN);
   }
}



void update_aula_status(void){

	ST7735_FillScreenFast(BGCOLOR);
	char display_alunos_fora[10];
	if(alunos_fora<MAX_ALUNOS_FORA) sprintf(display_alunos_fora, "FORA: %u", alunos_fora);
	else sprintf(display_alunos_fora, "FORA: MAX");


	char indicador_presentes[20];
	sprintf(indicador_presentes ,"%d/%d", quant_presentes, max_quant_alunos);

	DrawProgressBar(20, 70, 100, 15);
	ST7735_WriteString(0, 0, indicador_presentes, Font_7x10, TXTCOLOR, BGCOLOR);
    ST7735_WriteString(67, 0, display_alunos_fora, Font_7x10, TXTCOLOR, BGCOLOR);



}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

