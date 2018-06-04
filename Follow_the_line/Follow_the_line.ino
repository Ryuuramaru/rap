 #include <TimerOne.h>

#define dW digitalWrite
#define dR digitalRead

#define BOT1  7
#define BOT2  6
#define LEDG  13

#define MOT1  2
#define MOT2  3
#define MOT3  4
#define MOT4  5

#define T_GENERAL         0
#define T_LEDS            (T_GENERAL+1)
#define T_CALIBRACION     (T_LEDS+1)
#define T_CANT            (T_CALIBRACION+1)

#define BLANCO  1   //alb
#define NEGRO   0   //negru

#define COLOR   NEGRO   //culoarea negru

#define RECTA   0    //drept
#define CURVA   1    //curba
#define FRENO   2    //frana

#define CANT_SENS     5   //val sens
#define CANT_SENS_MAX 6   //val sens max

int ms[T_CANT];

const int CNY[CANT_SENS_MAX]={ A0, A1, A2, A3, A4, A5 };              //pinii
int CNYmax[CANT_SENS_MAX] = { 0, 0, 0, 0, 0, 0 };                     //valoarea maximă care poate fi luată (calibrată)
int CNYmin[CANT_SENS_MAX] = { 1023, 1023, 1023, 1023, 1023, 1023 };   //valoarea minimă pe care o puteți lua (calibrează)
const int peso[CANT_SENS_MAX] = {0, 1000, 2000, 3000, 4000, 5000};    //greutate pentru media ponderată
int linea = 0;                                                        //poziția liniei de la -1000 la 1000
boolean last_dir  = 1;                                                //ultima adresa, 0 stanga, 1 dreapta
boolean hay_linea = 0;
int settle_time = 200;

#define POR_MIN   50        //procentul minim să fie luat în considerare deoarece există o linie
#define GIRO_MAX  1.25      //-GIRO_MAX * max viteza este cea mai mică valoare posibilă  

int maxVel = 600;         //viteza maximă
int calVel = 200;         //viteza de calibrare
float KP = 0.4;
float KD = 12;
float KI = 0;
float correccion = 0;
int proporcional = 0;
int derivada = 0;
int integral = 0;
int last_linea = 0;

int tramo = RECTA;
int last_mark = 0;
//                        drept curba frana
const int tramoVel[] =  { 800,  600,  100   };
const float tramoKp[] = { 0.5,  0.4,  0.1   };
const float tramoKd[] = { 15,   12,   5     };
const float tramoKi[] = { 0,    0,    0     };

int start = 0;

void setup() {
  Serial.begin(9600);
  Timer1.initialize(750);
  Timer1.attachInterrupt(contador);
  
  pinMode(BOT1, INPUT_PULLUP);
  pinMode(BOT2, INPUT_PULLUP);

  pinMode(LEDG, OUTPUT);

  pinMode(MOT1, OUTPUT);
  pinMode(MOT2, OUTPUT);
  pinMode(MOT3, OUTPUT);
  pinMode(MOT4, OUTPUT);

  for(int x = 0; x <= CANT_SENS_MAX; x++){
    pinMode(CNY[x], INPUT);
  }
}

void contador(){
  for(int x = 0; x < T_CANT; x++){
    ms[x]++;
  }
}

void calcularLinea(boolean color){    //1 pentru alb 0 pentru negru
  long CNYesc[CANT_SENS_MAX];         //pentru a o pune pe o scară de 1000
  hay_linea = 0;                      //în cazul în care am părăsit linia // trebuie să înceapă 0
  long numerador = 0;
  long denominador = 0;
  for(int x = 0; x<=CANT_SENS-1; x++){
    CNYesc[x] = analogRead(CNY[x]) - CNYmin[x];
    CNYesc[x] = CNYesc[x] * 1000;
    CNYesc[x] = CNYesc[x] / (CNYmax[x]-CNYmin[x]);
    if(color == 0){
      CNYesc[x] = 1000-CNYesc[x];     //Mă întorc dacă-l caut pe negru
    }
    if(CNYesc[x] > 10*POR_MIN){       //detectează dacă există o linie
      hay_linea = 1;
    }
    numerador += CNYesc[x]*peso[x];   //pentru media ponderată
    denominador += CNYesc[x];         //pentru media ponderată
  }
  if(hay_linea == 1){
    linea = numerador/denominador;    //pentru media ponderată
    linea = linea-(CANT_SENS-1)/2*1000;         //schimbați de la 0 la 4000 la eroare
    last_dir = (linea>=0) ? 1 : 0;    //acutalizează ultima adresă
  }else{
    linea = 0 + (CANT_SENS-1)*1000*last_dir;    //utilizați informațiile de la ultima adresă
    linea = linea-(CANT_SENS-1)/2*1000;         //schimbați de la 0 la 4000 la eroare

    //tramo = (start==1)? CURVA : RECTA;        //asigurare
  }

  //dW(LEDG, hay_linea);
}

int cambiaTramo(boolean color){
  color = !color;                   //în inserarea individuală valoarea este inversă
  long CNYesc;
  int x = CANT_SENS_MAX-1;
  CNYesc = analogRead(CNY[x]);/* - CNYmin[x];
  CNYesc = CNYesc * 1000;
  CNYesc = CNYesc / (CNYmax[x]-CNYmin[x]);*/
  if(color == NEGRO){
    CNYesc = 1000-CNYesc;           //Mă întorc dacă-l caut pe negru
  }
  if(CNYesc > 10*POR_MIN){          //detectează dacă există o linie
    last_mark = 1;
  }else{
    if(last_mark == 1){
      motorSpeed(-300, -300);
      delay(50);
      if(tramo == RECTA){
        tramo = CURVA;
      }else{
        if(tramo == CURVA){
          tramo = RECTA;
        }
      }
    }
    last_mark = 0;
  }
  Serial.println(CNYesc);
  dW(LEDG, (CNYesc > 10*POR_MIN));
  return tramo;
}

void updatePID(int t){
  maxVel = tramoVel[t];
  KP = tramoKp[t];
  KD = tramoKd[t];
  KI = tramoKi[t];
}

float calcCorreccion(void){
  proporcional = linea;
  derivada = linea - last_linea;
  integral = linea + last_linea;
  last_linea = linea;
  correccion = (KP*proporcional + KD*derivada + KI*integral);
  if(correccion > 0){
    correccion = (correccion > maxVel*GIRO_MAX)? maxVel*GIRO_MAX : correccion;
  }
  if(correccion < 0){
    correccion = (correccion < -maxVel*GIRO_MAX)? -maxVel*GIRO_MAX : correccion;
  }

  return (float)(KP*proporcional + KD*derivada + KI*integral);
}

void motorSpeed(int m1, int m2) { //intre 1000 si -1000
  int en1 = map(abs(m1), 0, 1000, 0, 255);
  int en2 = map(abs(m2), 0, 1000, 0, 255);
  en1 = (m1>0) ? 255-en1 : en1;
  en2 = (m2>=0) ? en2 : 255-en2; 
  analogWrite(MOT2, en1);
  analogWrite(MOT4, en2);
  digitalWrite(MOT1, (m1 > 0) ? HIGH : LOW);
  digitalWrite(MOT3, (m2 >= 0) ? LOW : HIGH);
}

