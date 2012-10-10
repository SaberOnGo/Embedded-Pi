#include "coshining.h"
#include "xgpio.h"
#include "xuart.h"
#include "xsysctl.h"
#include "photo.h"
#include "xcore.h"
#include "xhw_ints.h"
//#include "sdcard.h"

unsigned long ulPhotoLen = 0;   //ͼ��ĳ���
unsigned long ulPhotoCount = 0;   //ͼ��ĳ���
unsigned char ucPhotoBuf[512];    //����ͼ�񻺳���,�Ѿ��㹻����

//�����������(����ucGetPhotoCmd����)����Ϊconst����,�Ϳ��Դ洢��rom,��ʡram

//��λ����
unsigned char const ucResetCmd[] = 
{
	0x56,
	0x00,
	0x26,
	0x00
};

//��λ��������
unsigned char const ucResetRcv [] =
{ 
	0x76,
	0x00,
	0x26,
	0x00
};

//��ʼ��������
unsigned char const ucStartPhotoCmd [] =
{
	0x56,
	0x00,
	0x36,
	0x01,
	0x00
};

//��ʼ���շ�������
unsigned char const ucStartPhotoRcv [] =
{
	0x76,
	0x00,
	0x36,
	0x00,
	0x00
};

//����������
unsigned char const ucReadLenCmd [] = 
{
	0x56,
	0x00,
	0x34,
	0x01,
	0x00
};
//�����ȷ��������ǰ�߸��ֽ�,��������ĺ������ֽ�Ϊͼ�񳤶�
unsigned char const ucReadLenRcv [] =
{
	0x76,
	0x00,
	0x34,
	0x00,
	0x04,
	0x00,
	0x00
};

//ȡͼ������,Ҳ���Ƕ�������
unsigned char ucGetPhotoCmd [] =
{
	0x56,
	0x00,
	0x32, 
	0x0C, 
	0x00, 
	0x0A,
	 
	0x00, 
	0x00, 
	0x00, 
	0x00,
	
	0x00, 
	0x00,
	0x00, 
	0x00,
	
	0x00,
	0xFF              //���ʱ����Ҫע��,Ӧ�����һ��,��������Сһ��,��Сһ��Ļ���������̫������׳��ֽ��մ���

};
unsigned char const ucGetPhotoRcv [] =
{
	0x76,
	0x00,
	0x32,
	0x00,
	0x00
};


//����ѹ��������,������Ϊ0x80 : 1,����ʹͼ��С
unsigned char const ucSetZipRateCmd [] =
{
	0x56,
	0x00, 
	0x31, 
	0x05,  
	0x01, 
	0x01, 
	0x12,
	0x04,  
	0x80
};

unsigned char const ucSetZipRateRcv [] = 
{
	0x76,
	0x00,
	0x31,
	0x00,
	0x00
};

//ֹͣ��������,���ķ��������뿪ʼ���յ���ͬ
unsigned char const ucStopPhotoCmd [] =
{
	0x56,
	0x00,
	0x36,
	0x01,
	0x03
};
//ֹͣ���շ�������,����ʼ���յ�һ��
unsigned char const ucStopPhotoRcv [] =
{
	0x76,
	0x00,
	0x36,
	0x00,
	0x00
};

//����ͼ���С����
unsigned char const ucPhotoSizeCmd [] =
{
	0x56,
	0x00, 
	0x31, 
	0x05,  
	0x04, 
	0x01, 
	0x00, 
	0x19,
	0x11,
	0x00
};
unsigned char const ucPhotoSizeRcv [] =
{
	0x76, 
	0x00, 
	0x31, 
	0x00, 
	0x00
};
unsigned char const ucSavePowerCmd [] =
{
	0x56, 
	0x00, 
	0x3E, 
	0x03, 
	0x00, 
	0x01, 
	0x00
};
unsigned char const ucSavePowerRcv [] =
{
	0x76, 
	0x00, 
	0x3E, 
	0x00, 
	0x00
};
unsigned char const ucSetBaudRateCmd [] = 
{
	0x56,
	0x00, 
	0x24, 
	0x03, 
	0x01, 
	0x0D,
	0xA6	
};
unsigned char const ucSetBaudRateRcv [] = 
{
	0x76, 
	0x00, 
	0x24, 
	0x00, 
	0x00
};


//*****************************************************************************
//
//! \brief Send a buffer to uart.
//!
//! \param ulBase is the base address of the UART port.
//! \param pucStr is the point of data will be sent.
//! \param ulLen is the lenth of data will be sent.
//!
//! Send a buffer to uart
//!
//! \return None
//
//*****************************************************************************
void
UARTBufferPut(unsigned long ulBase, unsigned char *pucStr, unsigned long ulLen)
{
    unsigned long i;
	  for(i=0; i<ulLen; i++)
    {
        xUARTCharPut(ulBase, *pucStr++);
    }	
}


//*****************************************************************************
//
//! \brief get data from UART.
//!
//! \param pucBuf is the point of data will be save.
//! \param ulLen is the lenth of data will be save.
//!
//! get data from UART.
//!
//! \return the lenth of data
//
//*****************************************************************************
unsigned long 
UARTBufferGet(unsigned long ulBase, unsigned char *pucBuf, unsigned long  ulLen)
{
	unsigned long i = 0;
	for (i = 0; i < ulLen; i++)
	{
		pucBuf[i] = UARTCharGet(ulBase);
	}	
	return ulLen;
}


//*****************************************************************************
//
//! \brief .
//!
//! \param .
//! \param .
//! \param .
//!
//! 
//!
//! \return 
//
//*****************************************************************************
//��������ͼ���С����,�˺������Բ�Ҫ
unsigned char 
PhotoSizeSend(void)
{
	  unsigned long i;//, count;
	  unsigned char tmp[5];
	
	  UARTBufferPut(sUART_BASE, (unsigned char *)&ucPhotoSizeCmd[0], 10);
	
	  if (!UARTBufferGet(sUART_BASE, tmp, 5)) return 0;
	
	//��������
	for (i = 0; i < 5; i++)
	{
		if (tmp[i] != ucPhotoSizeRcv[i]) return 0;
	}

	return 1;
}

//*****************************************************************************
//
//! \brief .
//!
//! \param .
//! \param .
//! \param .
//!
//! 
//!
//! \return 
//
//*****************************************************************************
//����ֹͣ���պ���
unsigned char PhotoStopSend(void)
{
	unsigned long i;//, count;
	unsigned char tmp[5];
	
	
	//����ֹͣ��������,��ʵ������ջ�����,������ʲôֹͣ����
	UARTBufferPut(sUART_BASE, (unsigned char *)&ucStopPhotoCmd[0], 5);
	
	if (!UARTBufferGet(sUART_BASE,tmp, 5)) return 0;
	
	//��������
	for (i = 0; i < 5; i++)
	{
		if (tmp[i] != ucStopPhotoRcv[i]) return 0;
	}
	
//	xSysCtlDelay(500);  //��ʱһ��,������ͷ��д�껺����
	
	return 1;
}


//*****************************************************************************
//
//! \brief .
//!
//! \param .
//! \param .
//! \param .
//!
//! 
//!
//! \return 
//
//*****************************************************************************
//���͸�λ�����
unsigned char PhotoResetSend(void)
{
	unsigned char tmp[4];
	unsigned long i;//, count;
	
	//while(1)
	UARTBufferPut(sUART_BASE, (unsigned char *)&ucResetCmd[0], 4);

	if (!UARTBufferGet(sUART_BASE,tmp, 4)) 
		 return 0;
	
	for (i = 0; i < 4; i++)
		if (tmp[i] != ucResetRcv[i]) return 0;
	
	xSysCtlDelay(300);  //��λ����ʱ,��Ϊֻ������֡ͷ,���������Ҫ����
	//���һ�������־
	
	return 1;
}

//*****************************************************************************
//
//! \brief .
//!
//! \param .
//! \param .
//! \param .
//!
//! 
//!
//! \return 
//
//*****************************************************************************
//��ͼ�����ݺ���������ͼ�����ݵĴ�С(���ֽ�Ϊ��λ),��ʧ���򷵻�0
unsigned long PhotoDataGet(unsigned char *ucBuf) 
{
	unsigned long i;//, count;
	unsigned long ulLeft;
	
	ulPhotoCount = 0;
	
	while(ulPhotoCount <= ulPhotoLen)
	{
		ulLeft = ulPhotoLen - ulPhotoCount;
    ucGetPhotoCmd[8] =ulPhotoCount/256;
		ucGetPhotoCmd[9] =ulPhotoCount%256;
		if(ulLeft <= 512)
		{
			ucGetPhotoCmd[12] =ulLeft/256;
		  ucGetPhotoCmd[13] =ulLeft%256;
			ulPhotoCount = ulPhotoCount + ulLeft;
			//���Ͷ�������
	    UARTBufferPut(sUART_BASE, (unsigned char *)&ucGetPhotoCmd[0], 16);
		  UARTBufferGet(sUART_BASE, ucBuf, ulLeft);
			UARTBufferPut(sUART_DBG_BASE, ucBuf, ulLeft);
    }
		else
		{
			ucGetPhotoCmd[12] =2;
		  ucGetPhotoCmd[13] =0;
			ulPhotoCount = ulPhotoCount + 512;
			//���Ͷ�������
	    UARTBufferPut(sUART_BASE, (unsigned char *)&ucGetPhotoCmd[0], 16);
		  UARTBufferGet(sUART_BASE, ucBuf, 512);
			UARTBufferPut(sUART_DBG_BASE, ucBuf, 512);
    }

  }
	
	/*//���Ͷ�������
	UARTBufferPut(sUART_BASE, (unsigned char *)&ucGetPhotoCmd[0], 16);
	
	if (!UARTBufferGet(sUART_BASE,ucBuf, 10 + ulPhotoLen)) return 0;
	
	//����֡ͷ
	for (i = 0; i < 5; i++)
	{
		if (ucBuf[i] != ucGetPhotoRcv[i]) return 0;
	}
	
	//����֡β
	for (i = 0; i < 5; i++)
	{
		if (ucBuf[i + 5 + ulPhotoLen] != ucGetPhotoRcv[i]) return 0;
	}
	
	//����ͼ��ĺϷ���,��0xFF,0xD8��ʼ,0xFF,0xD9����
	if (ucBuf[5] != 0xFF || ucBuf[6] != 0xD8 ||
		ucBuf[3 + ulPhotoLen] != 0xFF || ucBuf[4 + ulPhotoLen] != 0xD9)
	{
		return 0;
	}
	
	//��ͼ�������ƶ�������0��ʼ��
	for (i = 0; i < ulPhotoLen; i++)
		ucBuf[i] = ucBuf[i + 5];*/
	
	return ulPhotoLen;	//����ͼ�񳤶�
}


//*****************************************************************************
//
//! \brief .
//!
//! \param .
//! \param .
//! \param .
//!
//! 
//!
//! \return 
//
//*****************************************************************************
//�����Ⱥ���
unsigned char PhotoLenRead(void)
{
	unsigned long i;//, count;
	unsigned char tmp[9];
	
	//���Ͷ���������
	UARTBufferPut(sUART_BASE, (unsigned char *)&ucReadLenCmd[0], 5);
	
	if (!UARTBufferGet(sUART_BASE,tmp, 9)) return 0;
	
	//��������
	for (i = 0; i < 7; i++)
	{
		if (tmp[i] != ucReadLenRcv[i]) return 0;
	}
	
	//PhotoLenSet(tmp[7], tmp[8]);  //���ö���������
	ulPhotoLen = tmp[7] * 0x100 + tmp[8];
	PhotoLenSet(0x3, 0xE8);
	
	return 1;
}

//*****************************************************************************
//
//! \brief .
//!
//! \param .
//! \param .
//! \param .
//!
//! 
//!
//! \return 
//
//*****************************************************************************
void PhotoLenSet(unsigned char length1, unsigned char length2)
{
//	setlocale(LC_ALL, "C");
	ucGetPhotoCmd[12] = length1;
	ucGetPhotoCmd[13] = length2;
	//ulPhotoLen = length1 * 0x100 + length2;
}


//*****************************************************************************
//
//! \brief .
//!
//! \param .
//! \param .
//! \param .
//!
//! 
//!
//! \return 
//
//*****************************************************************************
//����ѹ���ʺ���
unsigned char PhotoZipRateSet(void)
{
	unsigned long i;//, count;
	unsigned char tmp[5];
	
	//����ѹ����Ϊ80:1
	UARTBufferPut(sUART_BASE, (unsigned char *)&ucSetZipRateCmd[0], 9);

	if (!UARTBufferGet(sUART_BASE,tmp, 5)) return 0;
	
	//��������
	for (i = 0; i < 5; i++)
	{
		if (tmp[i] != ucSetZipRateRcv[i]) return 0;
	}
	
	return 1;
}

//*****************************************************************************
//
//! \brief .
//!
//! \param .
//! \param .
//! \param .
//!
//! 
//!
//! \return 
//
//*****************************************************************************
unsigned char PhotoStartSend(void)
{	
	unsigned long i;//, count;
	unsigned char tmp[5];
	
	//������ͷ������������
	UARTBufferPut(sUART_BASE, (unsigned char *)&ucStartPhotoCmd[0], 5);

	if (!UARTBufferGet(sUART_BASE,tmp, 5)) return 0;
	
	//��������
	for (i = 0; i < 5; i++)
	{
		if (tmp[i] != ucStartPhotoRcv[i]) return 0;
	}
	
	return 1;
}

//*****************************************************************************
//
//! \brief .
//!
//! \param .
//! \param .
//! \param .
//!
//! 
//!
//! \return 
//
//*****************************************************************************
//���ճ�ʼ������
unsigned char PhotoInit(void)
{
	//camera_power_on();
		
	
	xSysCtlDelay(2500000);    //���ϵ縴λ��Ҫ��2.5������ͷ���ܽ�������
	if (!PhotoResetSend()) goto error_exit;	
//	if (!PhotoSizeSend()) goto error_exit;   //����ͼ���С,���ɲ�Ҫ
	if (!PhotoStopSend()) goto error_exit;  //�������ͷ������	
	if (!PhotoZipRateSet()) goto error_exit;    //����ѹ����Ϊ0x80:1
	xSysCtlDelay(10);   //����Ҫע��,����ѹ���ʺ�Ҫ��ʱ
	if (!PhotoSavePower()) goto error_exit;
	
	
	
	return 1;
error_exit:
  
    return 0;
}

//*****************************************************************************
//
//! \brief .
//!
//! \param .
//! \param .
//! \param .
//!
//! 
//!
//! \return 
//
//*****************************************************************************
unsigned char PhotoReset(void)
{
	if (!PhotoResetSend()) goto error_exit;	       //����ͼ���С��Ҫ����
	if (!PhotoInit()) goto error_exit;
	return 1;
error_exit:
	return 0;
}

//*****************************************************************************
//
//! \brief .
//!
//! \param .
//! \param .
//! \param .
//!
//! 
//!
//! \return 
//
//*****************************************************************************
unsigned char PhotoSavePower(void)
{
	unsigned long i;//, count;
	unsigned char tmp[5];
	
	//������ͷ�����˳�ʡ������
	UARTBufferPut(sUART_BASE, (unsigned char *)&ucSavePowerCmd[0], 7);

	if (!UARTBufferGet(sUART_BASE,tmp, 5)) return 0;
	
	//��������
	for (i = 0; i < 5; i++)
	{
		if (tmp[i] != ucSavePowerRcv[i]) return 0;
	}
	
	return 1;	
}

//*****************************************************************************
//
//! \brief .
//!
//! \param .
//! \param .
//! \param .
//!
//! 
//!
//! \return 
//
//*****************************************************************************
//���պ���,�����ĸ�����
unsigned long PhotoTake(unsigned char *ucBuf)
{
	unsigned long len;

   
	if(!PhotoInit()) goto error_exit;
	if (!PhotoStartSend()) goto error_exit;  //�Ϳ�ʼ��������
	if (!PhotoLenRead()) goto error_exit;   //������
		
	//len = PhotoDataGet(ucBuf);		//����
	//if (len == 0)  goto error_exit;

	//if (!PhotoStopSend()) goto error_exit;  //ֹͣ����,��ջ�����
	
	//return len;
	
error_exit:
  	return 0;
}

unsigned char ucUartTep[21]; 
unsigned char ucUartLen = 0;

//*****************************************************************************
//
//! \brief .
//!
//! \param .
//! \param .
//! \param .
//!
//! 
//!
//! \return 
//
//*****************************************************************************
unsigned long 
PhotoSendToComputer(void *pvCBData, unsigned long ulEvent,
                    unsigned long ulMsgParam, void *pvMsgData)
{
	  unsigned long i;
	  ucUartTep[ucUartLen] = xUARTCharGetNonBlocking(sUART_DBG_BASE);
	  ucUartLen++;

}

unsigned char
BufferLenGet(void)
{
    return ucUartLen;
}
void
BufferRemove(unsigned char ucLen)
{
    int i;
	  for(i=0; i< (20 - ucLen); i++)
	  {
        ucUartTep[i] = ucUartTep[i+ucLen];
    }
		ucUartLen = ucUartLen - ucLen;
}


int main()
{
	  int i,Index;
	  unsigned char ucRet = 0;
    xSysCtlClockSet(12000000, xSYSCTL_OSC_MAIN | xSYSCTL_XTAL_12MHZ);
    xSysCtlDelay(1000);
	
    xSysCtlPeripheralEnable(xSYSCTL_PERIPH_UART0);	
	  xSysCtlPeripheralEnable(xSYSCTL_PERIPH_UART1);
    xSysCtlPeripheralEnable(xGPIOSPinToPeripheralId(sD13));   
    xSysCtlPeripheralClockSourceSet(xSYSCTL_UART0_MAIN, 1);
	  xSysCtlPeripheralClockSourceSet(xSYSCTL_UART1_MAIN, 1);
	
    sPinTypeUART(sUART_BASE); 
	  sPinTypeUART(sUART_DBG_BASE);

    xUARTConfigSet(sUART_BASE, 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	  xUARTConfigSet(sUART_DBG_BASE, 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    xUARTEnable(sUART_BASE, (UART_BLOCK_UART | UART_BLOCK_TX | UART_BLOCK_RX));
	  xUARTEnable(sUART_DBG_BASE, (UART_BLOCK_UART | UART_BLOCK_TX | UART_BLOCK_RX));
	  xUARTIntCallbackInit(sUART_DBG_BASE, PhotoSendToComputer);
	  xUARTIntEnable(sUART_DBG_BASE, xUART_INT_RX);
	  xIntEnable(xINT_UART0);
	  UARTBufferPut(sUART_DBG_BASE, "adfdsgfh", 8);
	
	  //PhotoInit();
	  PhotoTake(&ucPhotoBuf[0]);

    while(1)
    {
			  ucRet = BufferLenGet();
			  if(ucRet <= 0) goto end;
        for(i=0; i<ucRet; i++)
			  {
					  if(ucUartTep[i] == 0x56)
						{
							  Index = i;
							  break;
						}
        }
				if(Index > 0)
				{
            BufferRemove(Index);
					  Index = 0;
        }
				ucRet = BufferLenGet();
				if( ucUartTep[0] == ucResetCmd[0] &&
					  ucUartTep[1] == ucResetCmd[1] &&
				    ucUartTep[2] == ucResetCmd[2] &&
				    ucUartTep[3] == ucResetCmd[3] 
					)
				{
					  UARTBufferPut(sUART_DBG_BASE, (unsigned char *)&ucResetRcv[0], 4);
					  BufferRemove(4);
        }
				if( ucUartTep[0] == ucStartPhotoCmd[0] &&
					  ucUartTep[1] == ucStartPhotoCmd[1] &&
				    ucUartTep[2] == ucStartPhotoCmd[2] &&
				    ucUartTep[3] == ucStartPhotoCmd[3] &&
				    ucUartTep[4] == ucStartPhotoCmd[4]
					)
				{
					  UARTBufferPut(sUART_DBG_BASE, (unsigned char *)&ucStartPhotoRcv[0], 5);
					  BufferRemove(5);
        }
				if( ucUartTep[0] == ucReadLenCmd[0] &&
					  ucUartTep[1] == ucReadLenCmd[1] &&
				    ucUartTep[2] == ucReadLenCmd[2] &&
				    ucUartTep[3] == ucReadLenCmd[3] &&
				    ucUartTep[4] == ucReadLenCmd[4]
					)
				{
					  UARTBufferPut(sUART_DBG_BASE, (unsigned char *)&ucReadLenRcv[0], 5);
					  xUARTCharPut(sUART_DBG_BASE, ulPhotoLen/256);
					  xUARTCharPut(sUART_DBG_BASE, ulPhotoLen%256);
					  BufferRemove(5);
        }
				if( ucUartTep[0] == ucSetZipRateCmd[0] &&
					  ucUartTep[1] == ucSetZipRateCmd[1] &&
				    ucUartTep[2] == ucSetZipRateCmd[2] &&
				    ucUartTep[3] == ucSetZipRateCmd[3] &&
				    ucUartTep[4] == ucSetZipRateCmd[4] &&
				    ucUartTep[5] == ucSetZipRateCmd[5] &&
				    ucUartTep[6] == ucSetZipRateCmd[6] &&
				    ucUartTep[7] == ucSetZipRateCmd[7] 
					)
				{
					  UARTBufferPut(sUART_DBG_BASE, (unsigned char *)&ucSetZipRateRcv[0], 5);
					  BufferRemove(9);
        }
				if( ucUartTep[0] == ucStopPhotoCmd[0] &&
					  ucUartTep[1] == ucStopPhotoCmd[1] &&
				    ucUartTep[2] == ucStopPhotoCmd[2] &&
				    ucUartTep[3] == ucStopPhotoCmd[3] &&
				    ucUartTep[4] == ucStopPhotoCmd[4]
					)
				{
					  UARTBufferPut(sUART_DBG_BASE, (unsigned char *)&ucStopPhotoRcv[0], 5);
					  BufferRemove(5);
        }
				if( ucUartTep[0] == ucPhotoSizeCmd[0] &&
					  ucUartTep[1] == ucPhotoSizeCmd[1] &&
				    ucUartTep[2] == ucPhotoSizeCmd[2] &&
				    ucUartTep[3] == ucPhotoSizeCmd[3] &&
				    ucUartTep[4] == ucPhotoSizeCmd[4] &&
				    ucUartTep[5] == ucPhotoSizeCmd[5] &&
				    ucUartTep[6] == ucPhotoSizeCmd[6] &&
			    	ucUartTep[7] == ucPhotoSizeCmd[7] 
					)
				{
					  UARTBufferPut(sUART_DBG_BASE, (unsigned char *)&ucPhotoSizeRcv[0], 5);
					  BufferRemove(9);
        }
				if( ucUartTep[0] == ucSavePowerCmd[0] &&
					  ucUartTep[1] == ucSavePowerCmd[1] &&
				    ucUartTep[2] == ucSavePowerCmd[2] &&
				    ucUartTep[3] == ucSavePowerCmd[3] &&
				    ucUartTep[4] == ucSavePowerCmd[4] &&
				    ucUartTep[5] == ucSavePowerCmd[5] 
					)
				{
					  UARTBufferPut(sUART_DBG_BASE, (unsigned char *)&ucSavePowerRcv[0], 5);
					  BufferRemove(7);
        }
				if( ucUartTep[0] == ucSetBaudRateCmd[0] &&
					  ucUartTep[1] == ucSetBaudRateCmd[1] &&
				    ucUartTep[2] == ucSetBaudRateCmd[2] &&
				    ucUartTep[3] == ucSetBaudRateCmd[3] &&
				    ucUartTep[4] == ucSetBaudRateCmd[4] &&
				    ucUartTep[5] == ucSetBaudRateCmd[5] 
					)
				{
					  UARTBufferPut(sUART_DBG_BASE, (unsigned char *)&ucSetBaudRateRcv[0], 5);
					  BufferRemove(7);
        }
				if( ucUartTep[0] == ucGetPhotoCmd[0] &&
					  ucUartTep[1] == ucGetPhotoCmd[1] &&
				    ucUartTep[2] == ucGetPhotoCmd[2] &&
				    ucUartTep[3] == ucGetPhotoCmd[3] &&
				    ucUartTep[4] == ucGetPhotoCmd[4] &&
				    ucUartTep[5] == ucGetPhotoCmd[5] 
					)
				{
					  BufferRemove(16);
					  PhotoDataGet(&ucPhotoBuf[0]);
					  
        }
				
end:
    }
}
