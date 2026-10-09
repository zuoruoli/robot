#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include<math.h>

#define SERVOMIN 102
#define SERVOMAX 512
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//前后步差200mm，最远100mm，最近70mm
float farmax=60,farmin=40;
//上下最高40mm，最低60mm
float high=-20,low=-40;
float a=60,b=85;//a一级脚长，b二级脚长；
int t=10;
float footl=1;

enum channel{
  m1 =15,m2 =14,//右前
  m3 =13,m4=12,//左前
  m5=10,m6=11,//左后
  m7=9,m8=8//右后
};

struct leg{
  uint8_t leg1;
  uint8_t leg2;
  bool flip;
  float x;//mm
  float y;//mm
  int number;
};

leg legs[4]{
  {m1,m2,0,farmin,low,1},
  {m3,m4,1,farmin,low,2},
  {m5,m6,0,farmin,low,3},
  {m7,m8,1,farmin,low,4}
};
//驱动舵机
void mot(uint8_t channel ,int angle){
  pwm.setPWM(channel,0,map(angle,0,180,SERVOMIN,SERVOMAX));
}
//统一坐标
int toangle(){
  legs[2].x=legs[0].x;//mm
  legs[2].y=legs[0].y;

  legs[1].x=legs[0].x;

  legs[1].y=legs[0].y;//mm

  legs[3].x=legs[0].x;//mm
  legs[3].y=legs[1].y;
}


void upleg(leg l){
  float ang1;
  float ang2;
  float x,y;
  x=l.x;
  y=l.y;

//调整误差
  switch(l.number){
    case 2:
      break;
    
    case 3:
      y-=10;
      break;

    case 4:
    
      break;
    default: break;
  }


  //计算角度
  float zf;
  if(y>=0) zf=1;
  else zf=-1;
  
  float c2=x*x+y*y;
  float cosCang2 = (a*a+b*b-c2)/(2*a*b);
  float Cang2 = acos(cosCang2)/PI*180;
  ang2 = 180-Cang2;

  float r=atan2(abs(y),x)/PI*180;
  float coso=(a*a+c2-b*b)/(2*a*sqrt(c2));
  float o = acos(coso)/PI*180;
  ang1=(90+o+r*zf);

  if(l.flip){
    ang1=180-ang1;
    ang2=180-ang2;
  }

  ang1=round(ang1);
  ang2=round(ang2);

  mot(l.leg1,ang1);
  mot(l.leg2,ang2);
}


//驱动四个脚
void move(float x,float y){

  upleg(legs[0]);
  upleg(legs[1]);
  upleg(legs[2]);
  upleg(legs[3]);
}

//初始驱动化四个脚的坐标
void innit(){
  move(farmin,low);
}

void setup(){
  pwm.begin();
  pwm.setPWMFreq(50);
  Wire.begin();
  
  //pwm.setPWM(10,0,map(90,0,180,SERVOMIN,SERVOMAX));

  innit();
 

  Serial.begin(9600);
  delay(1000);

}

//整体上下移动
void up(float high){
 //high为一级舵机轴心到地面的距离
  if(legs[0].y>-high) for(legs[0].y;legs[0].y>=-high;legs[0].y-=footl){
    toangle();
    move(legs[0].x,legs[0].y);
    delay(t);
  }
  
  else for(legs[0].y;legs[0].y<-high;legs[0].y+=footl){
    toangle();
    move(legs[0].x,legs[0].y);
    delay(t);
  }

}

void loop(){

/*
  for(legs[0].y;legs[0].y<=high;legs[0].y+=footl){
    move(legs[0].x,legs[0].y);
    delay(t);
  }

  for(legs[0].x;legs[0].x<=farmax;legs[0].x+=footl){
    move(legs[0].x,legs[0].y);
    delay(t);
  }

  for(legs[0].y;legs[0].y>=low;legs[0].y-=footl){
    move(legs[0].x,legs[0].y);
    delay(t);
  }

for(legs[0].x;legs[0].x>=farmin;legs[0].x-=footl){
    move(legs[0].x,legs[0].y);
    delay(t);
  }*/

}











