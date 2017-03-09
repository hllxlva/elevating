boolean flag = true;
int PhotoPin = 10;
volatile int PhotoValue = 0;
volatile int pre_PhotoValue = 0;
int Height;
int Initial_Height = 200;
int V_out;
int V_out_max=255;
int V_out_min=10;
int dh = 0;
int prog_num = 0;

void setup() {
  pinMode(PhotoPin,INPUT) ;
  pinMode(12,OUTPUT);
  Height = Initial_Height;
  Serial.begin(250000);
}

void slit_count(int sign){//1か-1を引数に
  PhotoValue = digitalRead(PhotoPin);
  if(pre_PhotoValue != PhotoValue){
    dh += sign*3;//スリットが3mm刻みである故
    pre_PhotoValue = PhotoValue;
  }
}

void elevating(int value){
  int sign = value/abs(value);
  slit_count(sign);
  V_out = sign*255*sin(PI/float(value)*float(dh));
  if(abs(V_out) < abs(V_out_min)) V_out = sign*V_out_min;
  if(abs(dh) >= abs(value)){
    dh = 0;
    prog_num++;
    V_out = 0;
  }
}

void loop() {
  digitalWrite(12,HIGH);
  switch(prog_num){
    case 0:
      elevating(201);
      break;
    case 1:
      delay(1000);
      prog_num = 2;
      break;
    case 2:
      elevating(-201);
      break;
  }
  Serial.print(V_out);
  Serial.print(" | ");
  Serial.print(prog_num);
  Serial.print(" | ");
  Serial.println(dh);
}
