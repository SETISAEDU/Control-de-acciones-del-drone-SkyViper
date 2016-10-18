/* --------------------------------------------------------------------
    SETISAEDU
    Codigo para el control del Drone SkyViper con
    la intervencion de las señales del mando.
   --------------------------------------------------------------------
*/

//Declaracion de los pines de control a usar
// Pin_ascenso = "DAC0", Pin_yaw = "DAC1" Solo arduino Due, para arduinos sin DAC usar salidas PWM

int Pin_ascenso = 3, Pin_yaw = 5;
//Declaracion de variables
int last_set = 0, set = 0;

//Funcion de configuracion de potencia de ascenso
void set_ascenso(int now)
{
  Serial.println("*******************************");
  if (last_set < now)
  {
    for (int x = last_set; x <= now; x++)
    {
      //analogWrite(DAC0, x * (4095 / 100)); // Escalado de valor de porcentaje a valor de salida del DAC
      analogWrite(Pin_ascenso, x * (255 / 100)); // Escalado de valor de porcentaje a valor de salida PWM 8 bits
      Serial.print("Potencia de ascenso en ");
      Serial.print(x); Serial.println("%");
      delay(100);
    }
    last_set = now;
  } else if (last_set > now)
  {
    for (int x = last_set; x >= now; x--)
    {
      //analogWrite(DAC0, x * (4095 / 100));
      analogWrite(Pin_ascenso, x * (255 / 100)); // Escalado de valor de porcentaje a valor de salida PWM 8 bits
      Serial.print("Potencia de ascenso en ");
      Serial.print(x); Serial.println("%");
      delay(100);
    }
    last_set = now;
  } else
  {
    Serial.print("Potencia de ascenso en ");
    Serial.print(now); Serial.println("%");
  }
  Serial.println("Valor de ascenso alcanzado");
  Serial.println("*******************************");

}

//Funcion de giro a la izquierda
void set_izquierda(int set_time)
{
  Serial.println("*******************************");
  for (int x = set_time;  x > 0; x--)
  {
      //analogWrite(DAC1, 4095);   //En el caso de usar DAC
      analogWrite(Pin_yaw, 230);   //PWM casi al maximo para realizar giro a la izquierda
      Serial.print("Girando a la izquierda, ");
      Serial.print(x); Serial.println(" segundos restantes");
      delay(1000);
  }
  //Control de Yaw, retorno a la posicion central
  //analogWrite(DAC1, 1500);  //Solo con DAC
  analogWrite(Pin_yaw, 85); // Para PWM de 8 bits
  Serial.println("Giro a la izquierda terminado");
  Serial.println("*******************************");

}

//Funcion de giro a la derecha
void set_derecha(int set_time)
{
  Serial.println("*******************************");
  for (int x = set_time;  x > 0; x--)
  {
      //analogWrite(DAC1, 0);   //En el caso de usar DAC
      analogWrite(Pin_yaw, 0);  //PWM al minimo para realizar giro a la derecha
      Serial.print("Girando a la derecha, ");
      Serial.print(x); Serial.println(" segundos restantes");
      delay(1000);
  }
  //Control de Yaw, retorno a la posicion central
  //analogWrite(DAC1, 2100);  //solo con DAC
  analogWrite(Pin_yaw, 85);   // Para PWM de 8 bits
  Serial.println("Giro a la derecha terminado");
  Serial.println("*******************************");

}

void setup()
{
  //Serial.begin(250000); //Solo arduino Due
  Serial.begin(115200);  //Inicializacion del Puerto 
  /*Solo arduinos con microcontrolador ARM
  pinMode(DAC0, OUTPUT);
  pinMode(DAC1, OUTPUT);
  analogWriteResolution(12); // Solo placas con microcontrolador ARM
  */
  
  //Configuracion de pin de control de ascenso
  pinMode(Pin_ascenso, OUTPUT);//Usar pines PWM
  analogWrite(Pin_ascenso, 0); //Iniciacion en 0
  //analogWrite(DAC0, 0);      //para arduino con DAC
  
  //Configuracion de pin de control de Yaw
  pinMode(Pin_yaw, OUTPUT);    //Usar pines PWM 
  analogWrite(Pin_yaw, 85);   //Control de Yaw, deber iniciar a la mitad (0xF4)
  //analogWrite(DAC1, 2047);   //para arduino con DAC
  
  //Mensaje inicial y explicacion del menu
  Serial.println("Control de drone SkyViper");
  Serial.println("Para controlar la potencia de ascenso, presione 'A' seguido de el valor de potencia deseado");
  Serial.println("Para realizar un giro a la izquierda, presione 'I' seguido de el valor de tiempo de giro");
  Serial.println("Para realizar un giro a la derecha, presione 'D' seguido de el valor de tiempo de giro");
  Serial.println("Para realizar un demo de vuelo autonomo (modo Crucero), presione 'C'");
}

void loop()
{
  if (Serial.available()) //Espera de recepcion de datos
  {
    char op = Serial.read(); //Lectura de la letra inicial
    int valor = Serial.parseInt(); //Lectura del valor numero ingresado
    
    //Comparacion para comprobar la operacion solicitada
    if (op == 'a' || op == 'A') 
    {
      if(valor <= 100) //Comprobacion si el nivel esta en rango 
      {
      Serial.print("Configuracion de potencia de ascenso a ");
      Serial.print(valor);Serial.println("%");
      delay(1000);
      set_ascenso(valor); //llamado de funcion de conficuracion de ascenso
      }else
      {  //En el caso de tener un valor invalido
        Serial.println("Configuracion de potencia invalido!");
        Serial.println("Ingresar un valor entre 0 y 100 ");
      }
    }else if(op == 'i' || op == 'I')
    {
      if(valor <= 10) //Comprobacion si el nivel esta en rango 
      {
      Serial.print("Secuencia de giro a la izquierda por ");
      Serial.print(valor);Serial.println(" segundos");
      delay(1000);
      set_izquierda(valor); //llamado de funcion de giro a la izquierda
      }else
      {  //En el caso de tener un valor invalido
        Serial.println("Configuracion de giro a la izquierda invalido");
        Serial.println("Ingresar un valor entre 0 y 10 ");
      }
    }else if(op == 'd' || op == 'D')
    {
      if(valor <= 10) //Comprobacion si el nivel esta en rango 
      {
      Serial.print("Secuencia de giro a la derecha por ");
      Serial.print(valor);Serial.println(" segundos");
      delay(1000);
      set_derecha(valor); //llamado de funcion de giro a la derecha
      }else
      {  //En el caso de tener un valor invalido
        Serial.println("Configuracion de giro a la derecha invalido");
        Serial.println("Ingresar un valor entre 0 y 10 ");
      }
    }else if(op == 'c' || op == 'C')
    {
      Serial.print("Modo crucero, se realizara un demo de vuelo autonomo ");
      Serial.println("Iniciando ascenso hasta el 60%");
      set_ascenso(60); //llamado de funcion de conficuracion de ascenso
      delay(500);
      Serial.println("Realizando giro a la izquierda por 6 segundos");
      set_izquierda(6); //llamado de funcion de giro a la izquierda
      delay(500);
      Serial.println("cambiando altura, ascenso hasta el 80%");
      set_ascenso(80); //llamado de funcion de conficuracion de ascenso
      delay(500);
      Serial.println("Realizando giro a la derecha por 8 segundos");
      set_derecha(8); //llamado de funcion de giro a la derecha
      delay(500);
      Serial.println("Iniciando descenso");
      set_ascenso(70); //llamado de funcion de conficuracion de ascenso
      delay(2000);
      set_ascenso(60);
      delay(1500);
      set_ascenso(50);
      delay(1500);
      set_ascenso(40);
      delay(1500);
      set_ascenso(30);
      delay(1000);
      set_ascenso(20);
      delay(1000);
      set_ascenso(0);
      Serial.println("Aterrizaje completado, motores al 0%");  
    }else
    { //En el caso de seleccion de de opcion invalida
      Serial.println("Opcion invalida");  
    }
  }
}
