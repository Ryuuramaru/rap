void loop() {
  calcularLinea(COLOR);
  correccion = calcCorreccion();

  //updatePID(cambiaTramo(COLOR));
  
  if(dR(BOT1) == 0){
    delay(500);
    calibrar(calVel, -calVel, 300);
    calibrar(-calVel, calVel, 400);
    calibrar(calVel, -calVel, 500);
    calibrar(-calVel, calVel, 400);
  }
  if(dR(BOT2) == 0){
    while(dR(BOT2) == 0){}
    start = 1;
    tramo = RECTA;
    last_mark = 0;
    ms[T_GENERAL] = 0;
  }
  if(start){
    updatePID(cambiaTramo(COLOR));
    
    if(correccion > 0){
      motorSpeed(maxVel, maxVel-correccion);
    }else{
      motorSpeed(maxVel+correccion, maxVel);
    }
  }
}

void calibrar(int m1, int m2, int timeCal){         //viteza motoarelor și timpul de calibrare
  ms[T_CALIBRACION] = 0;
  while(ms[T_CALIBRACION] < timeCal){
    if(ms[T_CALIBRACION]%100 >= 50){                //LED-ul clipește
      dW(LEDG, 1);
    }else{
      dW(LEDG, 0);
    }
    motorSpeed(m1, m2);
    int CNYread[CANT_SENS_MAX];
    for(int x = 0; x<=CANT_SENS_MAX-1; x++){
      CNYread[x] = analogRead(CNY[x]);
      CNYmin[x] = (CNYread[x] < CNYmin[x]) ? CNYread[x] : CNYmin[x];
      CNYmax[x] = (CNYread[x] > CNYmax[x]) ? CNYread[x] : CNYmax[x];
    }
  }
  motorSpeed(0, 0);
  dW(LEDG, 1);
}
