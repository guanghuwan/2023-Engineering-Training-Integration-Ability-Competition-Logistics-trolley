/******************************************************************************************************
 * 单片机：mega328p-au 外部晶振：16M 
********************************************************************************************/
//调用舵机库
#include<Servo.h>
//调用软件串口库
#include <SoftwareSerial.h> 

#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library

//paramters define
#define MODEL ILI9341
#define CS   A5    
#define CD   A3
#define RST  A4
#define LED  A0   //if you don't need to control the LED pin,you should set it to -1 and set it to 3.3V  
LCDWIKI_SPI mylcd(MODEL,CS,CD,RST,LED); //model,cs,dc,reset,led
//定义L298N的引脚
#define IN1 3 //控制电机A的方向
#define IN2 2 //控制电机A的方向
#define IN3 4 //控制电机B的方向
#define IN4 5 //控制电机B的方向
#define IN7 8//控制电机A的方向
#define IN8 9 //控制电机A的方向
#define IN5 7 //控制电机B的方向
#define IN6 6 //控制电机B的方向
//屏幕颜色
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
//定义AB相编码器的引脚
#define A_A 18 //电机A的A相
#define B_A 19 //电机A的B相
#define A_B 10 //电机B的A相
#define B_B 11 //电机B的B相

Servo myservo;
//定义全局变量
volatile long count_A = 0; //记录电机A的转动次数
volatile long count_B = 0; //记录电机B的转动次数
int speed_A = 0; //记录电机A的速度（0~255）
int speed_B = 0; //记录电机B的速度（0~255）
int dir_A = 0; //记录电机A的方向（0或1）
int dir_B = 0; //记录电机B的方向（0或1）
//int RUN;
int pos=0;
SoftwareSerial mySerial1(14, 15); // RX->14 TX->15
SoftwareSerial mySerial2(16, 17); // RX->16 TX->17
SoftwareSerial mySerial(A4,A5); //创建一个软串口的类，模拟引脚4,5分别代表 RX, TX引脚 AR多功能板
char cmd_return[100];//这里的cmd_return的字符长度要足够的大，根据舵机个数定，大小 = 15*个数+10
//char* name;	//显示屏
char* mark1;//openmv1对arduino的指示
char* mark2;//openmv2对arduino的指示
const char * name = NULL;	
int name0 = 0;//总线舵机0转动角度
int name1=name0+135;  //总线舵机1转动角度
int zero0=1415;//总线舵机0抬平角度
int zero1=zero0+135;//总线舵机1抬平角度
int up0=1100;//总线舵机0监视角度
int up1=up0+135;//总线舵机1监视角度
int C1,C2;
int D;
int b_1=0;
int b_2=0;
int T=0;
int left1=4*200;//左移圈数，暂定为2
int left2=4*160;//左转时长
int st1=3*165;//前进到二维码区圈数
String device_A_String = "";
String device_B_String = "";


void display();
//void receive_and_print(SoftwareSerial& mySerial);

void setup() {
    Serial1.begin(115200);           //硬件串口
    Serial.begin(115200);           //硬件串口
    mySerial.begin(115200);
}

//这里我们实验软硬件串口 用户可根据实际情况定
void loop()
 {//Serial.println("tong");
        name="123+123";

        display();
    
}



void display()
 {
 char delims[] = "+";
    char *result = NULL;
    result = strtok( name, delims );
   

    
 mylcd.Set_Text_Mode(0);
 mylcd.Set_Text_colour(WHITE);
 mylcd.Set_Text_Back_colour(BLACK);
 mylcd.Set_Text_Size(12);
 mylcd.Print_String(result, 0, 0);
 mylcd.Print_String("+", 0, 100);
  result = strtok( NULL, delims );
  mylcd.Print_String(result, 0, 200);
 //mylcd.Print_String(name_1.substr(4,3), 0, 0);
 delay(3000);}


