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
SoftwareSerial mySerial1(14, 15); // RX->14 TX->15
SoftwareSerial mySerial2(16, 17); // RX->16 TX->17
SoftwareSerial mySerial(A4,A5); //创建一个软串口的类，模拟引脚4,5分别代表 RX, TX引脚 AR多功能板
char cmd_return[100];//这里的cmd_return的字符长度要足够的大，根据舵机个数定，大小 = 15*个数+10
//char* name; //显示屏
char* mark1;//openmv1对arduino的指示
char* mark2;//openmv2对arduino的指示
const char * name = NULL; 
const char * work = NULL; 
int name0 = 0;//总线舵机0转动角度
int name1=name0+135;  //总线舵机1转动角度
int zero0=1415;//总线舵机0抬平角度
int zero1=zero0+135;//总线舵机1抬平角度
int up0=1100;//总线舵机0监视角度
int up1=up0+135;//总线舵机1监视角度
int C1,C2;
int b_1=0;
int b_2=0;
int T=0;
int left1=4*200;//左移圈数，暂定为2
int left2=4*160;//左转时长
int st1=3*165;//前进到二维码区圈数
int st2=3*165;//前进至转盘区圈数
int st3=3*165;//前进到转弯1圈数
String device_A_String = "";
String device_B_String = "";
void dipan(int RUN);
void communicate_b2(int x,int y);
void rot1(int a);
void rot0(int a);
void display();
//void receive_and_print(SoftwareSerial& mySerial);
void duoji();
void LCD();
void quanshu();
void setup() {
  //设置L298N引脚为输出模式
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //设置AB相引脚为输入模式，并启用内部上拉电阻
  pinMode(A_A, INPUT_PULLUP);
  pinMode(B_A, INPUT_PULLUP);
  pinMode(A_B, INPUT_PULLUP);
  pinMode(B_B, INPUT_PULLUP);
//LCD启动
  mylcd.Init_LCD();
  mylcd.Fill_Screen(BLACK);

  Serial2.begin(115200);           //硬件串口
  Serial.begin(115200);           //硬件串口
  Serial1.begin(115200);           //硬件串口
  mySerial.begin(115200);
 // 开启软件串口，波特率为9600bps
  mySerial1.begin(115200);
  mySerial2.begin(115200);
  //设置AB相中断服务函数，并设置触发模式为变化沿
  attachInterrupt(digitalPinToInterrupt(A_A), countA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(A_B), countB, CHANGE);
//设置舵机输出引脚
  myservo.attach(10);

  //正文
  //左移80mm
  while(left1>count_A){
    dipan(3131);
  }
  count_A=0;
  dipan(0000);
  //展开
  rot1(up1);
  rot0(up0);
  //行至二维码

  while(st1>count_A){
    dipan(2222);
  }
  dipan(0000);
  count_A=0;
  //读取openmv1传来的任务
  int i=0;
  while(strlen(name)<7&&!i<100){
    communicate_b2(1,7);
    i++;
   // if(i>200)//暂定，若等待时间过长，则调整车位置
 
  }
  while(strlen(name)<7)
  {
    communicate_b2(1,7);
    i++;
    if(i>700){//暂定，若等待时间过长，则调整车位置
    //前进一点
      while(!strlen(name)<7&&!count_A<5*165){
        dipan(1111);
      }
      count_A=0;
      dipan(0000);
    //后退一点
      while(!strlen(name)<7&&!count_A<10*165){
        dipan(3333);
      }
      count_A=0;
      dipan(0000);
    }
  }
  //显示
  work=name;
  display();
  name=NULL;
  //前进至转盘区并开启openmv2颜色形状识别
  Serial2.write(1);
  while(st2>count_A)){
    dipan(2222);
    communicate_b2(2,2);}
  while()
  
  //前进至转弯1
  while(st3>count_A){
    dipan(2222);
  }
  count_A=0;
  dipan(0000);
  //左转
  while(left2>count_A){
    dipan(3113);}
  count_A=0;
  dipan(0000);


 

}

void loop() {
  //从串口读取用户输入的指令，并解析成速度和方向
  

}
//中断服务函数，用于计算电机A的转动次数
void countA() {
  if (digitalRead(A_A) == digitalRead(B_A)) { //如果A相和B相同步变化，则说明正转
    count_A++; //次数加一
  } else { //否则反转
    count_A--; //次数减一
  }
  quanshu();
}

//中断服务函数，用于计算电机B的转动次数
void countB() {
  if (digitalRead(A_B) == digitalRead(B_B)) { //如果A相和B相同步变化，则说明正转
    count_B++; //次数加一
  } else { //否则反转
    count_B--; //次数减一
  }
}
//2222 快速正向平动
//4444 快速反向平动
//1111 慢速正向平动
//3333 慢速反向平动
//1331 右转
//3113 左转
//1313 右移
//3131 左移
//0000 停
void dipan(int RUN){
  if(RUN==2222)//快速正向平动
  {
    analogWrite(IN1,0);
    analogWrite(IN2,100);
    analogWrite(IN3,0);
    analogWrite(IN4,100);
    analogWrite(IN5,0);
    analogWrite(IN6,110);
    analogWrite(IN7,0);
    analogWrite(IN8,110);
  }
    else if(RUN==4444)//快速反向平动
  {
    analogWrite(IN1,95);
    analogWrite(IN2,0);
    analogWrite(IN3,100);
    analogWrite(IN4,0);
    analogWrite(IN5,100);
    analogWrite(IN6,0);
    analogWrite(IN7,98);
    analogWrite(IN8,0);
  }
    else if(RUN==3333)//慢速反向平动
  {
    analogWrite(IN1,85);
    analogWrite(IN2,0);
    analogWrite(IN3,85);
    analogWrite(IN4,0);
    analogWrite(IN5,85);
    analogWrite(IN6,0);
    analogWrite(IN7,85);
    analogWrite(IN8,0);
  }
    else if(RUN==1111)//慢速正向平动
  {
    analogWrite(IN1,0);
    analogWrite(IN2,85);
    analogWrite(IN3,0);
    analogWrite(IN4,85);
    analogWrite(IN5,0);
    analogWrite(IN6,85);
    analogWrite(IN7,0);
    analogWrite(IN8,85);
  }
    else if(RUN==3113)//左移
  {
    analogWrite(IN1,85);
    analogWrite(IN2,0);
    analogWrite(IN3,0);
    analogWrite(IN4,85);
    analogWrite(IN5,0);
    analogWrite(IN6,100);
    analogWrite(IN7,85);
    analogWrite(IN8,0);
  }
      else if(RUN==1331)//右移
  {
    analogWrite(IN1,0);
    analogWrite(IN2,85);
    analogWrite(IN3,100);
    analogWrite(IN4,0);
    analogWrite(IN5,100);
    analogWrite(IN6,0);
    analogWrite(IN7,0);
    analogWrite(IN8,85);
  }
      else if(RUN==3131)//左转
  {
    analogWrite(IN1,110);
    analogWrite(IN2,0);
    analogWrite(IN3,0);
    analogWrite(IN4,110);
    analogWrite(IN5,110);
    analogWrite(IN6,0);
    analogWrite(IN7,0);
    analogWrite(IN8,110);
  }
      else if(RUN==1313)//右转
  {
    analogWrite(IN1,0);
    analogWrite(IN2,85);
    analogWrite(IN3,85);
    analogWrite(IN4,0);
    analogWrite(IN5,0);
    analogWrite(IN6,85);
    analogWrite(IN7,85);
    analogWrite(IN8,0);
  }
      else if(RUN==0000)//停转
  {
    analogWrite(IN1,0);
    analogWrite(IN2,0);
    analogWrite(IN3,0);
    analogWrite(IN4,0);
    analogWrite(IN5,0);
    analogWrite(IN6,0);
    analogWrite(IN7,0);
    analogWrite(IN8,0);
  }
  countA();

}

void duoji(int pos1,int pos2)//pos1为原角度，pos2为目标角度
    { 
    int pos;
      if(pos1<pos2)
   { for (pos = pos1; pos <= pos2; pos += 1) { //从0度递增到180度
    myservo.write(pos); //设置舵机的目标角度为pos
    delay(7); //等待15毫秒，让舵机有时间到达目标位置
   }
   }
      if(pos1>pos2)
   { for (pos = pos1; pos >=pos2; pos -= 1) { //从0度递增到180度
    myservo.write(pos); //设置舵机的目标角度为pos
    delay(7); //等待15毫秒，让舵机有时间到达目标位置
   }
   }
    }
void LCD()
{ int name[10]={1,2,3,2,3,1};
  int key=name[0]*100+name[1]*10+name[2];
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(12);
  mylcd.Print_Number_Int(name[0]*100+name[1]*10+name[2], 0, 0, 0, ' ',10);
  mylcd.Print_String("+", 0, 100);
  mylcd.Print_Number_Int(name[3]*100+name[4]*10+name[5], 0, 200, 0, ' ',10);
  delay(3000);
}
void communicate_b2(int x,int y)
{
  for(;strlen(name)<y;){
  name = device_B_String.c_str();  //让指针指向s1的位置
 //读取从设备B传入的数据，并在串口监视器中显示
  if(x==2){
  if(Serial2. available() >0){
  if(Serial2. peek() != '\n')
  {
    device_B_String +=(char)Serial2. read();
    Serial2. read();
    //Serial. println( device_B_String);
    name=device_B_String.c_str();
  Serial. println(name);
  Serial.print(strlen(name));
  }
  else
  {
  Serial2. read();

  }}}
  if(x==1){
  if(Serial1. available() >0){
  if(Serial1. peek() != '\n')
  {
    device_B_String +=(char)Serial1. read();
    Serial1. read();
    //Serial. println( device_B_String);
    name=device_B_String.c_str();
    Serial.println(name);
      Serial.print(strlen(name));
  //Serial. println(name);
  }
  else
  {
  Serial1. read();

  }}}
  }
}
void rot1(int a){
  Serial.print("#001P");    //硬件串口 串口1 让0号舵机旋转到0500的位置 舵机的位置范围 500~2500
  Serial.print(a);
  Serial.print("T1000!");
  delay(1200);
}
void rot0(int a){
  Serial.print("#000P");    //硬件串口 串口1 让0号舵机旋转到0500的位置 舵机的位置范围 500~2500
  Serial.print(a);
  Serial.print("T1000!");
  delay(1200);
}
void communicate_b()
{

 name = device_B_String.c_str();  //让指针指向s1的位置
 //读取从设备B传入的数据，并在串口监视器中显示
  if(Serial1. available() >0){
  if(Serial1. peek() != '\n')
  {
  device_B_String +=(char)Serial1. read();
  Serial1. read();
  Serial. println( device_B_String);
  name=device_B_String.c_str();
  Serial. println(name);
  b_1++;}
  else
  {
  Serial1. read();
  Serial. println( device_B_String);
  //Serial. println("read");
  //name=device_B_String.c_str();
  //Serial. println(name);
  device_B_String = "";

  }}
}
void quanshu()
{
 if(T%115200==0)
  {
  Serial.print("Motor A: ");
  Serial.print(count_A); //打印次数
  Serial.print(" pulses, ");
  Serial.print(count_A / 165); //打印圈数（假设每圈有20个脉冲）
  Serial.println(" rounds");
  }
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
 //mylcd.Print_String(name_1.substr(4,3), 0, 0);
 delay(3000);}

