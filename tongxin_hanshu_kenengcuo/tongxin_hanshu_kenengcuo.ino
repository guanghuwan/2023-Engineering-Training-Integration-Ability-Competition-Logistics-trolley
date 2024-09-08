#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library
#include <String.h>
#define MODEL ILI9341
#define CS   A5    
#define CD   A3
#define RST  A4
#define LED  A0   //if you don't need to control the LED pin,you should set it to -1 and set it to 3.3V

//the definiens of hardware spi mode as follow:
//if the IC model is known or the modules is unreadable,you can use this constructed function
LCDWIKI_SPI mylcd(MODEL,CS,CD,RST,LED); //model,cs,dc,reset,led
//if the IC model is not known and the modules is readable,you can use this constructed function
//LCDWIKI_SPI mylcd(240,320,CS,CD,RST,LED); //model,cs,dc,reset,led

//define some colour values
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
String device_A_String = "";
String device_B_String = "";
 const char * work = NULL;	
void setup() {
//初始化 Serial，该串口用于与计算机连接通信,计算机是A，对应serial
Serial.begin(9600);
//初始化 Seriall，该串口用于与设备B连接通信，对应serial1
Serial1.begin(9600);
mylcd.Init_LCD();
mylcd.Fill_Screen(BLACK);
//两个字符串分别用于存储A、B两端传来的数据

}
void communicate_b()
{

 work = device_B_String.c_str();	//让指针指向s1的位置
 //读取从设备B传入的数据，并在串口监视器中显示
if(Serial1. available() >0){
if(Serial1. peek() != '\n')
{
device_B_String +=(char)Serial1. read();
Serial1. read();
Serial. println( device_B_String);
work=device_B_String.c_str();
Serial. println(work);
}
else
{
Serial1. read();
Serial. println( device_B_String);
//Serial. println("read");
//work=device_B_String.c_str();
//Serial. println(work);
device_B_String = "";

}}
}
void display()
 {
 char delims[] = "+";
    char *result = NULL;
    result = strtok( work, delims );
   

    
 mylcd.Set_Text_Mode(0);
 mylcd.Set_Text_colour(WHITE);
 mylcd.Set_Text_Back_colour(BLACK);
 mylcd.Set_Text_Size(12);
 mylcd.Print_String(result, 0, 0);
 mylcd.Print_String("+", 0, 100);
  result = strtok( NULL, delims );
  mylcd.Print_String(result, 0, 200);
 //mylcd.Print_String(work_1.substr(4,3), 0, 0);
 delay(3000);}

void loop() {


communicate_b();
if(strlen(work)>=7)
display();
}


