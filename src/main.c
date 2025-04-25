#include "device_registers.h"
#include "clocks_and_modes.h"
#include "ADC.h"
#include "port.h"

unsigned int FND_DATA[10] = { 0x7E, 0x0C, 0xB6, 0x9E, 0xCC, 0xDA, 0xFA, 0x4E, 0xFE, 0xCE };
unsigned int num, num0, num1, num2, num3 = 0; /* num0:'1', num1:'10', num2:'100', num3:'1000'*/
unsigned int External_PIN = 0; /* External_PIN:SW External input Assignment */

void FTM_init(void);
void FTM0_CH1_PWM(int i);
void FTM0_CH2_PWM(int i);
void NVIC_init_IRQs(void);
void LPIT0_Ch1_IRQHandler(void);
void LPIT0_Ch0_IRQHandler(void);
void PORTC_IRQHandler(void);

void PORT_init(void) 
{
  D_CLK_EN;       /* Enable clock for PORTD   */
  D_MUX_PWM(0); 	/* D0:  MUX = ALT2, FTM0CH2 */
	D_MUX_PWM(16); 	/* D16: MUX = ALT2, FTM0CH1 */
  D_OUT(0); D_OUT(16);

	D_MUX_GPIO(2);  /* D2:  MUX = GPIO, FND_A   */
	D_MUX_GPIO(3);  /* D3:  MUX = GPIO, FND_B   */
	D_MUX_GPIO(4);  /* D4:  MUX = GPIO, FND_C   */
	D_MUX_GPIO(5);  /* D5:  MUX = GPIO, FND_D   */
	D_MUX_GPIO(6);  /* D6:  MUX = GPIO, FND_E   */
	D_MUX_GPIO(7);  /* D7:  MUX = GPIO, FND_F   */
	D_MUX_GPIO(8);  /* D8:  MUX = GPIO, FND_G   */
	D_MUX_GPIO(9);  /* D9:  MUX = GPIO, FND_COM6 (1000) */
	D_MUX_GPIO(10); /* D10: MUX = GPIO, FND_COM5 (100)  */
	D_MUX_GPIO(11); /* D11: MUX = GPIO, FND_COM4 (10)   */
	D_MUX_GPIO(12); /* D12: MUX = GPIO, FND_COM3 (1)    */

  D_OUT(2); D_OUT(3); D_OUT(4); D_OUT(5); D_OUT(6); D_OUT(7); D_OUT(8);
	D_OUT(9); D_OUT(10); D_OUT(11); D_OUT(12); D_OUT(13);

  C_CLK_EN;       /* Enable clock for PORTC   */
	C_MUX_GPIO(11); /* C11: MUX = GPIO, Button BREAK  */
	C_MUX_GPIO(12); /* C12: MUX = GPIO, Button GO FORWARD */
	C_MUX_GPIO(13); /* C13: MUX = GPIO, Button GO BACKWARD */
	C_MUX_GPIO(15); /* C15: MUX = GPIO, Realy switch control */

  C_IN(11); C_IN(12); C_IN(13); C_OUT(15);

	PORTC->PCR[11] |= (8 << 16);  // PORTC_11 Falling-edge Set	
	//PORTC->PCR[12] |= (8 << 16);
  //PORTC->PCR[13] |= (8 << 16);
}

int main(void) {
  uint32_t adcResultInMv = 0; /* ADC0 Result in miliVolts */
  int D = 0;
  SOSC_init_8MHz();      	/* Initialize system oscillator for 8 MHz xtal */
  SPLL_init_160MHz();    	/* Initialize SPLL to 160 MHz with 8 MHz SOSC */
  NormalRUNmode_80MHz(); 	/* Init clocks: 80 MHz SPLL & core, 40 MHz bus, 20 MHz flash */
  PORT_init();
  FTM_init();
  ADC_init(); 						/* Init ADC resolution 12 bit			*/
  NVIC_init_IRQs(); 			/* Enable desired interrupts and priorities */
  LPIT0_init();

  for (;;) {
    convertAdcChan(12);
    while (adc_complete() == 0) {}

    adcResultInMv = read_adc_chx();
    num = (5000 - adcResultInMv) / 50;

    D = adcResultInMv * 1.6;
    FTM0_CH1_PWM(100);
    FTM0_CH2_PWM(D);

    //if ((PTC->PDIR & (1 << 11)) == 0)
    //{
    //	FTM0_CH2_PWM(8000);
    //	num=0;
    //}
    if ((PTC->PDIR & (1 << 12)) == 0) {
      PTC->PCOR |= (1 << 15);
    }
    if ((PTC->PDIR & (1 << 13)) == 0) { 
			PTC->PSOR |= (1 << 15); 
		}
  }
}

void FTM_init(void) {
  //FTM0 clocking
  PCC->PCCn[PCC_FTM0_INDEX] &= ~PCC_PCCn_CGC_MASK;   //Ensure clk diabled for config
  PCC->PCCn[PCC_FTM0_INDEX] |= PCC_PCCn_PCS(0b010)   //Clocksrc=1, 8MHz SIRCDIV1_CLK
                               | PCC_PCCn_CGC_MASK;  //Enable clock for FTM regs
  //FTM0 Initialization
  FTM0->SC = FTM_SC_PWMEN1_MASK  	//Enable PWM channel 1 output
						|FTM_SC_PWMEN2_MASK		//Enable PWM channel 1 output
						|FTM_SC_PS(0);     		//TOIE(timer overflow Interrupt Ena) = 0 (deafault)

  FTM0->MOD = 8000 - 1;  // FTM0 counter final value (used for PWM mode)
                         // FTM0 Period = MOD-CNTIN+0x0001~=8000 ctr clks=4ms
  FTM0->CNTIN = FTM_CNTIN_INIT(0);
  FTM0->CONTROLS[1].CnSC |= FTM_CnSC_MSB_MASK;
  FTM0->CONTROLS[1].CnSC |= FTM_CnSC_ELSA_MASK; /* FTM0 ch1: edge-aligned PWM, low true pulses 		*/
  FTM0->CONTROLS[2].CnSC |= FTM_CnSC_MSB_MASK;
  FTM0->CONTROLS[2].CnSC |= FTM_CnSC_ELSA_MASK; /* FTM0 ch2: edge-aligned PWM, low true pulses 		*/
}

void FTM0_CH1_PWM(int i) {  //uint32_t i){
  FTM0->CONTROLS[1].CnV = i;  //8000~0 duty; ex(7200=> Duty 0.1 / 800=>Duty 0.9)
  FTM0->SC |= FTM_SC_CLKS(3);  //start FTM0 clk source = external clock (SOSCDIV1_CLK)
}

void FTM0_CH2_PWM(int i) {  //uint32_t i){
  FTM0->CONTROLS[2].CnV = i;  //8000~0 duty; ex(7200=> Duty 0.1 / 800=>Duty 0.9)
  FTM0->SC |= FTM_SC_CLKS(3); //start FTM0 clk source = external clock (SOSCDIV1_CLK)
}

void NVIC_init_IRQs(void) {
  /*LPIT ch0 overflow set*/
  S32_NVIC->ICPR[1] |= 1 << (48 % 32);
  S32_NVIC->ISER[1] |= 1 << (48 % 32);
  S32_NVIC->IP[48] = 0x0;
  /*LPIT ch1 overflow set*/
  S32_NVIC->ICPR[1] |= 1 << (49 % 32);
  S32_NVIC->ISER[1] |= 1 << (49 % 32);
  S32_NVIC->IP[49] = 0x0;

  S32_NVIC->ICPR[1] |= 1 << (61 % 32);  // Clear any pending IRQ61
  S32_NVIC->ISER[1] |= 1 << (61 % 32);  // Enable IRQ61
  S32_NVIC->IP[61] = 0xC0;              // Priority 11 of 15
}

void LPIT0_Ch1_IRQHandler(void) { /* FND out */
  LPIT0->MSR |= LPIT_MSR_TIF1_MASK; /* Clear LPIT0 timer flag 1 */
	volatile static unsigned int k = 0;

	D_SET_OUT_0_ALL;
  num3 = (num / 1000) % 10;
  num2 = (num / 100) % 10;
  num1 = (num / 10) % 10;
  num0 = num % 10;

  switch (k) {
		case 0: // 1000
		D_SET_OUT_1(9);
		PTD->PSOR = FND_DATA[num3]<<1;
		k++; break;

    case 1: // 100
		D_SET_OUT_1(10);
		PTD->PSOR = FND_DATA[num2]<<1;
		k++; break;

    case 2: // 10
		D_SET_OUT_1(11);
		PTD->PSOR = FND_DATA[num1]<<1;
		k++; break;

    case 3: // 1
		D_SET_OUT_1(12);
		PTD->PSOR = FND_DATA[num0]<<1;
		k = 0; break;
  }
}

void LPIT0_Ch0_IRQHandler(void) {
  LPIT0->MSR |= LPIT_MSR_TIF0_MASK; /* Clear LPIT0 timer flag 0 */
}

void PORTC_IRQHandler(void) {
	PORTC->PCR[11] |= 0x01000000;
	PORTC->PCR[12] |= 0x01000000;
	PORTC->PCR[13] |= 0x01000000;
	
	if ((PORTC->ISFR & (1 << 11)) != 0) {
    External_PIN = 1;
  }
	//else if ((PORTC->ISFR & (1 << 12)) != 0) {
  //  External_PIN = 2;
  //}
	//else if ((PORTC->ISFR & (1 << 13)) != 0) {
  //  External_PIN = 3;
  //}
	
  switch (External_PIN) {
    case 1:
      FTM0_CH2_PWM(8000);
			//PTC->PTOR |= (1 << 15);
      num = 0;
      External_PIN = 0;
      break;
  //  case 2:
  //    PTC->PCOR |= (1 << 15);
  //    External_PIN = 0;
  //    break;
	//	case 3:
   //   PTC->PSOR |= (1 << 15);
    //  External_PIN = 0;
      //break;
  }
}
