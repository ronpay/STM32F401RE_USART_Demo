#include "stm32f4xx.h"

static void NVIC_Configuration(void) {
#if EN_USART1_RX
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //��������ж�
    NVIC_InitTypeDef NVIC_InitStructure;

    // Usart1 NVIC(�жϿ�����) ����
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;  //����1�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;            //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;            //�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;       // IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                                      //����ָ���Ĳ�����ʼ��NVIC�Ĵ�����

#endif
}

void USART_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* ʹ�� USART GPIO ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	/* ʹ�� USART ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	/* ���� PXx �� USARTx_Tx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	/* ���� PXx �� USARTx__Rx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	/* GPIO ��ʼ�� */
	/* ��������ģʽ*/
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/* ��������*/
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	/* ���ø���ģʽx*/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	/* �����������50MHz*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* TX ��ʼ�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/* RX ��ʼ�� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/* ����USART */
	/* ���������� */
	USART_InitStructure.USART_BaudRate=9600;
	/* �ֳ�(����λ+У��λ)��8 */
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	/* ֹͣλ��1 ��ֹͣλ */
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	/* У��λѡ�񣺲�ʹ��У�� */
	USART_InitStructure.USART_Parity=USART_Parity_No;
	/* Ӳ�������ƣ���ʹ��Ӳ���� */
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	/* USART ģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	/* ��� USART ��ʼ������ */
	USART_Init(USART1,&USART_InitStructure);
	
	/* Ƕ�������жϿ����� NVIC ���� */
	NVIC_Configuration();
	
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	/* ʹ�ܴ��� */
	USART_Cmd(USART1,ENABLE);
}

/*****************  ����һ���ַ� **********************/
static void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	/* ����һ���ֽ����ݵ�USARTx */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  ����һ���ַ� **********************/
unsigned char USARTGetByte(USART_TypeDef * pUSARTx,unsigned char* GetData)
{   	   
        if(USART_GetFlagStatus(pUSARTx, USART_FLAG_RXNE) == RESET)
        {  return 0;//û���յ����� 
		}
        *GetData = USART_ReceiveData(pUSARTx); 
        return 1;//�յ�����
}

/*****************  �����շ����� **********************/
void USARTTest(USART_TypeDef * pUSARTx)
{
       unsigned char i = 0;

       while(1)
       {    
		 while(USARTGetByte(pUSARTx,&i))
        {
         Usart_SendByte(pUSARTx,i);
        }      
       }     
} 

void delay_us(u16 time) {
    u16 i = 0;
    while (time--) {
        i = 10;
        while (i--)
            ;
    }
}

void SystemInit() {}

int main(void) {
    SystemInit();
	
	/*��ʼ�� USART ����,�жϽ���*/
    USART_Config();

    while (1) {
	/*�����շ�����*/
        USARTTest(USART1);
        delay_us(0xffff);
    }
}
