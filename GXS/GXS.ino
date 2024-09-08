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
#define WHITE   0xFFFF
//定义AB相编码器的引脚
#define A_A 18 //电机A的A相
#define B_A 19 //电机A的B相
#define A_B 10 //电机B的A相
#define B_B 11 //电机B的B相

LCDWIKI_SPI mylcd(MODEL,CS,CD,RST,LED); //model,cs,dc,reset,led
Servo myservo;
//定义全局变量
volatile long count_A = 0; //记录电机A的转动次数
volatile long count_B = 0; //记录电机B的转动次数
int speed_A = 0; //记录电机A的速度（0~255）
int speed_B = 0; //记录电机B的速度（0~255）
int dir_A = 0; //记录电机A的方向（0或1）
int dir_B = 0; //记录电机B的方向（0或1）
int RUN;
int pos=0;
SoftwareSerial mySerial1(14, 15); // RX->14 TX->15
SoftwareSerial mySerial2(16, 17); // RX->16 TX->17
int C1,C2;
int D;
int T=0;
void dipan();
void receive_and_print(SoftwareSerial& mySerial);
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

  //设置串口通信波特率为9600bps
  Serial.begin(9600);
 // 开启软件串口，波特率为9600bps
  mySerial1.begin(9600);
  mySerial2.begin(9600);
  //设置AB相中断服务函数，并设置触发模式为变化沿
  attachInterrupt(digitalPinToInterrupt(A_A), countA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(A_B), countB, CHANGE);
//设置舵机输出引脚
  myservo.attach(10);
}

void loop() {
  //从串口读取用户输入的指令，并解析成速度和方向
  RUN=1111;
  dipan();



}
//中断服务函数，用于计算电机A的转动次数
void countA() {
  if (digitalRead(A_A) == digitalRead(B_A)) { //如果A相和B相同步变化，则说明正转
    count_A++; //次数加一
  } else { //否则反转
    count_A--; //次数减一
  }
}

//中断服务函数，用于计算电机B的转动次数
void countB() {
  if (digitalRead(A_B) == digitalRead(B_B)) { //如果A相和B相同步变化，则说明正转
    count_B++; //次数加一
  } else { //否则反转
    count_B--; //次数减一
  }
}

void dipan(){
  if(RUN==2222)//快速正向平动
  {
    analogWrite(IN1,0);
    analogWrite(IN2,110);
    analogWrite(IN3,0);
    analogWrite(IN4,110);
    analogWrite(IN5,0);
    analogWrite(IN6,110);
    analogWrite(IN7,0);
    analogWrite(IN8,110);
  }
    else if(RUN==4444)//快速反向平动
  {
    analogWrite(IN1,110);
    analogWrite(IN2,0);
    analogWrite(IN3,110);
    analogWrite(IN4,0);
    analogWrite(IN5,110);
    analogWrite(IN6,0);
    analogWrite(IN7,110);
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
    analogWrite(IN6,85);
    analogWrite(IN7,85);
    analogWrite(IN8,0);
  }
      else if(RUN==1331)//右移
  {
    analogWrite(IN1,0);
    analogWrite(IN2,85);
    analogWrite(IN3,85);
    analogWrite(IN4,0);
    analogWrite(IN5,85);
    analogWrite(IN6,0);
    analogWrite(IN7,0);
    analogWrite(IN8,85);
  }
      else if(RUN==3131)//左转
  {
    analogWrite(IN1,85);
    analogWrite(IN2,0);
    analogWrite(IN3,0);
    analogWrite(IN4,85);
    analogWrite(IN5,85);
    analogWrite(IN6,0);
    analogWrite(IN7,0);
    analogWrite(IN8,85);
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
void receive_and_print(SoftwareSerial& mySerial){
  // 如果有数据可读
  if (mySerial.available()){
    // 读取一行数据，以换行符为结束符
    String data = mySerial.readStringUntil('\n');
    // 打印数据到串口监视器
    Serial.println(data);
  }
}
void quanshu()
{
 if(T%9600==0)
  {
  Serial.print("Motor A: ");
  Serial.print(count_A); //打印次数
  Serial.print(" pulses, ");
  Serial.print(count_A / 780.0); //打印圈数（假设每圈有20个脉冲）
  Serial.println(" rounds");
  }
}
void duoji()//舵机投放件运动90° 
    { 
      if(D==1)
   { for (pos = 90; pos <= 150; pos += 1) { //从0度递增到180度
    myservo.write(pos); //设置舵机的目标角度为pos
    delay(7); //等待15毫秒，让舵机有时间到达目标位置
   }
   }
      if(D==2)
   { for (pos = 90; pos >=20; pos -= 1) { //从0度递增到180度
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
