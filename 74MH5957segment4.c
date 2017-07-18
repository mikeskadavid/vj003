#define   HC_DATA_H digitalWrite(data, HIGH)   // data line output high
#define   HC_DATA_L digitalWrite(data, LOW)   //date line output low
#define   HC_RCK_H digitalWrite(rck, HIGH)   // rck output high
#define   HC_RCK_L digitalWrite(rck, LOW)  // rck output low
#define   HC_SCK_H digitalWrite(sck, HIGH)   // sck output high
#define   HC_SCK_L digitalWrite(sck, LOW)  // sck output low


    
unsigned char LED_BCD[16] ={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e };  //common  anode digital tube BCD code
//Define the pin
int data =2;  
int rck =3;
int sck =5;
// the setup routine runs once when you press reset:
void setup() {               
  // initialize the digital pin as an output.
  pinMode(data, OUTPUT);
  pinMode(rck, OUTPUT);
  pinMode(sck, OUTPUT);
  pinMode(sclr, OUTPUT);

 
}

// the loop routine runs over and over again forever:
void loop() {
 
  unsigned char dopp =0;
  for( unsigned char i=0; i < 4; ++i)
  {
  if(i ==3) dopp =1;
else dopp =0;
  HC_data_analyze(i,i,dopp);  //Nixie Tube display
  }



}

void HC_data_analyze(char LED_number,unsigned char LED_display,unsigned char LED_dp)
{
  // data analyse
  unsigned int HC_DISP = 0,HC_LEDCODE,HC_LEDCODE_temp=0;
 
  if(LED_display > 15) LED_display = 0;
  HC_LEDCODE = LED_BCD[LED_display] ;  //get BCD code
  for(unsigned char i=0; i < 8;++i)
   {
     HC_LEDCODE_temp <<=1;
     if(HC_LEDCODE&0x01) HC_LEDCODE_temp |= 0x01;
     HC_LEDCODE >>=1;
    
   }
    if(LED_dp)  HC_LEDCODE_temp &= 0xfe;
   HC_DISP = HC_LEDCODE_temp;
 
  switch(LED_number)
  {
     case 0: HC_DISP |= 0x8000;break;
     case 1: HC_DISP |= 0x4000;break;
     case 2: HC_DISP |= 0x2000;break;
     case 3: HC_DISP |= 0x1000;break;
  }
 
  write_74HC595(HC_DISP);  //74HC595 shifting register  data transfer
 
 
}


void write_74HC595( unsigned int data_a)  //communication with 74HC595
{
  char look =0;
  HC_RCK_L;  //latch open
  HC_SCK_L;


 
  for (;look < 16; ++look)
  {
    if(data_a&0x0001) {HC_DATA_H;}
    else {HC_DATA_L;}
    HC_SCK_H;

    HC_SCK_L;
    data_a >>= 1;
  }
  HC_RCK_H;
}
