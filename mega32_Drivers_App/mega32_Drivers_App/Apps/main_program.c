/*
 * main_program.c
 * Created: 29/01/2016 04:46:11 م
 *  Author: Muhammad Asem
 */ 
#define F_CPU 1000000u
#define PASSWORD_LENGTH 6
#define PASSWORD_START_ADRS 3 //address in EEPROM
#define PASS_CHANGE_FLAG_ADRS 1 //Address in EEPROM
#define ERROR_COUNT_ADRS 2 //Address in EEPROM
#define allowed 2 //flags for allowed state machine of keypad
#define not_allowed 3 //flag for not allowed state machine of keypad
#include "lcd.h"
#include <stdlib.h>
#include <util/delay.h>
#include "LCD_API_utilizing.h"
#include "EEPROM.h"
#include "ADC.h"
#include "SPI.h"
#include "i2c.h"
#include "KEYPAD.h"
#include "UART.h"
unsigned char compare_pass (unsigned char *arr_a,unsigned char *arr_b);// prototype of the password compare function

//main function
int main(void)
{
	/************************************************************************/
	/* variables and arrays definitions &declaration                                                                       */
	/************************************************************************/
	char str[]= "Hello";// character array that will be used in welcome screeen 
	unsigned char E_flag;//this is flag to detect weather the default pass changed and saved to EEPROM or not 
	unsigned char uart_counter=0;//counter to count the no of entered char to be compared to password length
	unsigned char flag;// flag to detect mismatch of password default set=1
	unsigned char x=0;//this for keypad value
	unsigned char x2;
	 char str1[10];///this is character array to store the characters of the spi tc72 temp sensor 
	unsigned char UART_data;// to store the data from UART terminal
	unsigned char pass_a[]={'1','2','3','4','5','6' }; //the real password array and initalized with default pass !! and must be stored at the E2PROM not here 
		//BIG noooooooooote the array nust be in ascii as we compare it to other array of asciiiii
	unsigned char pass_b[PASSWORD_LENGTH]; //array to store entered password in
	unsigned char i=0; // pass array counter 
	unsigned char error_counter;
	unsigned char data; //ADC data variable of LM35
	float Voltage,temp;//for use with ADC
	int lsb_temp=0;//in SPI part
	unsigned char wel_flag=0;//welcome screen flag that changed to one when a button is pressed on keypad   
	/************************************************************************/
	/* init part                                                                     */
	/************************************************************************/
	unsigned char kpd_s=allowed; //init the state machine flag
	KPD_Init();
	lcd_init();
	ADC_Init();
	I2C_master_init();
	USART_Init(9600);
	SetTimeDate(3,1,10,1,3,0);//mm,dd,yy--h,m,s
	/************************************************************************/
	/* SPI part                                                                     */
	/************************************************************************/
	SpiConfigType z; // we chamged it to z not x due to conflict
	z.DataOrder = 0;
	z.Mode = 1;
	z.PreScaler = 0;
	SPI_MasterInit(&z);
	SLAVE_SELCET_INIT();
	SLAVE_SELECT_ACTIVATE();
	//here i need to write so start with chip select and then put your commands and then end  
	SPI_Trsfr(0x80);// write control address
	SPI_Trsfr(0x14);//write data to control address control byte
	SLAVE_SELECT_DEACTIVATE();	
/************************************************************************/
/*  START WELCOME SCREEN SAVER                                                                     */
/************************************************************************/
	//lcd_writeString("     hello"); 
    while(1)
    {
		
		MoveStrinrgRight(str,sizeof(str),&wel_flag);
		if(wel_flag==1)break;
		MoveStrinrgLeft(str,sizeof(str),&wel_flag);
		if(wel_flag==1) break;
		
    }// end of while
	
/************************************************************************/
/* BEGINING OF THE PROGRAM                                                                     */
/************************************************************************/	
	
	enter_pass://it is a label for goto command if password is wrong it will come to this line to iterate the process again 
	lcd_writeString("Enter Password");// Call for entering Password
	//-------------------------------------------------------------------------------
	 while(1)//loop to take input from keypad and store it inside EEPROM 
	{
		LCD_GoToXy(0,1);//enter a new line to print the characters from keypad on LCD  
		KPD_ReadVal(&x); //continuous scan on keypad and return the ASCII value of pressed button to x 
		
		//-----------if # is pressed we will exit to password checking---------------------------
		if ((x=='#') & (i==PASSWORD_LENGTH))//checking x i is the array of password counter
		{
			lcd_clear();
			break;// break to the next phase of program of password checking
		}
		//--------------------entering the loop of number recording--------------------------
		if ((x!='n') & (kpd_s==allowed) & (x!='#')& ((i<PASSWORD_LENGTH)|(x=='*'))) // if a key is pressed and satisfy these conditions->   
		//->these instructions will be done 
		{	
			 kpd_s=not_allowed;//state machine flag
			if((x =='*') & (i>0)) //Erasing the mistake done by user 
			{
				//i put i>0 condition to prevent i-- from giving -ve no of i==0 
				i--;
				LCD_GoToXy(i,1);
				lcd_writeChar(' ');
				
			}
			else if (x !='*') //i didnt write else only and put x!='*' bec if i=0 so upper condition wont done and by (else) only-> 
			//->this code part will be executed  so a '*' character will be wrote if you pressed the erase (*) button and this is fault
			{
				LCD_GoToXy(i,1);
				//lcd_writeChar(x);
				lcd_writeChar('*');// for password hiding 
				pass_b[i]=x; //storing password array 
				 i++;//number of captured buttons
			} 
				
		}
		else if((x=='n')) // ie i didnt press any key or removed my finger out of key
		{
			kpd_s= allowed;//state machine flag 
		}
	}
	
/************************************************************************/
/* checking E2prom                                                                     */
/************************************************************************/	
	E_flag=EEPROM_read(PASS_CHANGE_FLAG_ADRS);// 1 is the address of the flag 
	if(1==E_flag) // there is a password written before 
	{
		for(int s=0;s<PASSWORD_LENGTH;s++)
		{
			// i will load the address with 
		pass_a[s]=EEPROM_read(PASSWORD_START_ADRS+s); // i will start the adresss of saving with 3	
		}
		
	}	
/************************************************************************/
/* PASSWORD VALIDATION and DECESION MAKING                              */
/************************************************************************/	
flag=compare_pass(pass_a,pass_b); 
	 
if (flag=='0')// wrong case password
{
	error_counter=EEPROM_read(ERROR_COUNT_ADRS); // to read the no of wrong counts 
	error_counter++; //increase it by1 
	EEPROM_write(ERROR_COUNT_ADRS,error_counter); //write back the value in the E2PROM
	if (error_counter>=3)// blocking policy
	{
		//_delay_ms(1000*(error_counter-3+1)*5);
		lcd_clear();
		LCD_GoToXy(0,0);
		lcd_writeString("block");
		unsigned char blk=(error_counter-3+1)*5;
		lcd_writeChar(blk);
		_delay_ms(5000);
		
	}	
	lcd_clear();
	lcd_writeString("wrong pass"); //i need delay here 
	LCD_GoToXy(0,1);
	lcd_writeString("enter pass again");// it must enter infinte loop here to wait for input
	lcd_clear();
	i=0;//in order not to expand the array as it will be increases if not reseted
	goto enter_pass;// this is new command to deal with its function is to go to the label of enter_pass 
	
}

else if (flag=='1')// password match
{
	//make the value in the EEPROM of Error counter to be 0
	EEPROM_write(ERROR_COUNT_ADRS,0);
	lcd_clear();
	lcd_writeString("correct pass");// delay 100 ms
	_delay_ms(500);
	lcd_clear();
	lcd_writeString("for new pass: 1");
	LCD_GoToXy(0,1);
	lcd_writeString("Or Press # to go");
	while (1)
	{
		KPD_ReadVal(&x2);
		if (x2=='#')// break to the main core of the program 
		{
			cont_to_program:// label after uart finish writing six pass
			break;
		}
		else if (x2=='1') //UART section -----------------------------------------------------
		{
			EEPROM_write(PASS_CHANGE_FLAG_ADRS,1);//set flag to 1 to inform the next loop that the defult password changed
			//write 3 in address no 2
			lcd_clear();
			LCD_GoToXy(0,0);
			lcd_writeString("Use UART screen");
			while(1)//loop to recive the uart characters 
			{
				if (uart_counter==(PASSWORD_LENGTH)) 
				{
				
				USART_Transmit('F');
				goto cont_to_program;	
				}
					
			UART_data=USART_Receive();
			lcd_clear();
			lcd_writeChar(UART_data);
			
			//condition to check wheather the data is numbers or not 
			if ((UART_data >= '0') && (UART_data <= '9') && uart_counter<PASSWORD_LENGTH)// problem is here ..0&8 have problems 
			//if (UART_data=='8' )// problem is here .. 
			{
				//write password to eeprom and set flag to declare that defult pass has no need 
				EEPROM_write(PASSWORD_START_ADRS+uart_counter,UART_data);//write each character of uart in a memory locations 
				//that is dedicated for saving password in 
				
				uart_counter ++;
			}
			
			else // it isnt valid character    
			{
				lcd_clear();
				LCD_GoToXy(0,0);
				lcd_writeString("enter valid no.");
				
			}
			
			}
		}
	}
} // end of true if flag

/************************************************************************/
/* LM35 section & TC72 algorithm                                                                     */
/************************************************************************/
lcd_clear();
LCD_GoToXy(0,0);
//lcd_writeString("Temp from LM35=");


while(1)
{
	//LM35 section 
	data = ADC_Read(POT0_ID);
	Voltage = (data * 5) / 255.0;//mapping 5 volt 
	temp=Voltage*100;
	data = (int) temp;
	itoa(data,str,10);// int to array 
	LCD_GoToXy(0,0);
	lcd_writeString("LM ");
	lcd_writeString(str);
	data = (int)((temp - (float) data)*100);
	lcd_writeChar('.');
	itoa(data,str,10);
	lcd_writeString(str);
	
	///////SPI//////////////
	SLAVE_SELECT_ACTIVATE();
	
	//SPI_Trsfr(0x01);// address of LSB reading
	//SPI_Trsfr(0x02);//the address of MSB
	SPI_Trsfr(0x02);//address byte
	signed char MSB=SPI_Trsfr(0x00);
	unsigned char LSB=SPI_Trsfr(0x00);
	 //char ctrl_r=SPI_Trsfr(0x00);
	
	//MaID = SPI_Trsfr(0x00);// any garbage just to push the data transfer
	
	SLAVE_SELECT_DEACTIVATE();
	
	switch(LSB)
	{
		case(0x80):
		{
			 lsb_temp=5;
			break;
		}
		case(0xc0):
		{
			 lsb_temp=75;
			 break;
		}
		case(0x00):
		{
			lsb_temp=0;
			break;
		}
	}
	
	// float t_data=LSB |MSB<<8;
	//itoa(MaID,str,10);
	itoa(MSB,str1,10);
	//ftoa(t_data,str1,2);
	
	//LCD_GoToXy(0,0);
	lcd_writeString("/TC");
	lcd_writeString(str1);
	lcd_writeChar('.');
	itoa(lsb_temp,str1,10);
	lcd_writeString(str1);
	/////////////////////// clock part
	LCD_GoToXy(0,1);
	lcd_writeString("Time: ");
	WriteTime();
}// end of while and program



}	 
	 
	 
// function of password comparison 
unsigned char compare_pass (unsigned char *arr_a,unsigned char *arr_b)
{ unsigned char flag='1';
	unsigned char y;
	for (y=0;y<PASSWORD_LENGTH;y++)
	{
		if(*(arr_a+y) != *(arr_b+y))
		{
			flag='0';//flag to detect mismatch in password
		}
	}
	return flag;
}
