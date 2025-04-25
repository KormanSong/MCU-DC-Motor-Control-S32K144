#ifndef PORT_H
#define PORT_H

#include "device_registers.h"
#include "clocks_and_modes.h"

#define A_CLK_EN PCC->PCCn[PCC_PORTA_INDEX] = PCC_PCCn_CGC_MASK
#define B_CLK_EN PCC->PCCn[PCC_PORTB_INDEX] = PCC_PCCn_CGC_MASK
#define C_CLK_EN PCC->PCCn[PCC_PORTC_INDEX] = PCC_PCCn_CGC_MASK
#define D_CLK_EN PCC->PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK
#define E_CLK_EN PCC->PCCn[PCC_PORTE_INDEX] = PCC_PCCn_CGC_MASK


#define A_IN(g) (PTA->PDDR &= ~ (1U << (g)))
#define B_IN(g) (PTB->PDDR &= ~ (1U << (g)))
#define C_IN(g) (PTC->PDDR &= ~ (1U << (g)))
#define D_IN(g) (PTD->PDDR &= ~ (1U << (g)))
#define E_IN(g) (PTE->PDDR &= ~ (1U << (g)))

#define A_OUT(g) (PTA->PDDR |= (1U << (g)))
#define B_OUT(g) (PTB->PDDR |= (1U << (g)))
#define C_OUT(g) (PTC->PDDR |= (1U << (g)))
#define D_OUT(g) (PTD->PDDR |= (1U << (g)))
#define E_OUT(g) (PTE->PDDR |= (1U << (g)))

#define A_MUX_GPIO(g) (PORTA->PCR[(g)] = PORT_PCR_MUX(1))
#define B_MUX_GPIO(g) (PORTB->PCR[(g)] = PORT_PCR_MUX(1))
#define C_MUX_GPIO(g) (PORTC->PCR[(g)] = PORT_PCR_MUX(1))
#define D_MUX_GPIO(g) (PORTD->PCR[(g)] = PORT_PCR_MUX(1))
#define E_MUX_GPIO(g) (PORTE->PCR[(g)] = PORT_PCR_MUX(1))

#define A_MUX_PWM(g) (PORTA->PCR[(g)] = PORT_PCR_MUX(2))
#define B_MUX_PWM(g) (PORTB->PCR[(g)] = PORT_PCR_MUX(2))
#define C_MUX_PWM(g) (PORTC->PCR[(g)] = PORT_PCR_MUX(2))
#define D_MUX_PWM(g) (PORTD->PCR[(g)] = PORT_PCR_MUX(2))
#define E_MUX_PWM(g) (PORTE->PCR[(g)] = PORT_PCR_MUX(2))

#define A_GPIO_MUX_FOR_KEYPAD_IN(g) (PORTD->PCR[(g)] = PORT_PCR_MUX(1) | PORT_PCR_PFE(1) | PORT_PCR_PE(1) | PORT_PCR_PS(0))
#define B_GPIO_MUX_FOR_KEYPAD_IN(g) (PORTD->PCR[(g)] = PORT_PCR_MUX(1) | PORT_PCR_PFE(1) | PORT_PCR_PE(1) | PORT_PCR_PS(0))
#define C_GPIO_MUX_FOR_KEYPAD_IN(g) (PORTD->PCR[(g)] = PORT_PCR_MUX(1) | PORT_PCR_PFE(1) | PORT_PCR_PE(1) | PORT_PCR_PS(0))
#define D_GPIO_MUX_FOR_KEYPAD_IN(g) (PORTD->PCR[(g)] = PORT_PCR_MUX(1) | PORT_PCR_PFE(1) | PORT_PCR_PE(1) | PORT_PCR_PS(0))
#define E_GPIO_MUX_FOR_KEYPAD_IN(g) (PORTE->PCR[(g)] = PORT_PCR_MUX(1) | PORT_PCR_PFE(1) | PORT_PCR_PE(1) | PORT_PCR_PS(0))

#define D_SET_OUT_1(g) 	(PTD->PSOR = (1U << (g)))
#define D_SET_OUT_0(g) 	(PTD->PCOR = (1U << (g)))
#define D_SET_OUT_0_ALL (PTD->PCOR = 0xffffffff)

#define PT(g) (1U << (g))

#endif /* PORT_H */
