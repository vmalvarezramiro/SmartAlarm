/*
Programa: Smart Alarm Arduino
*/
#include <SoftwareSerial.h>
#include <Password.h> //Incluimos la libreria Password
#include <Keypad.h> //Incluimos la libreria Keypad

Password password = Password( "1234" ); //Definimos nuestro Password
SoftwareSerial SIM900(7, 8); // 10 y 11 para el Arduino Mega. Configura el puerto serie para el SIM900.

byte sensorpir = 10;
byte buzzer = 13;
byte led = 9;

boolean estado_alarma = false;
const byte ROWS = 4; // Cuatro Filas
const byte COLS = 3; // Cuatro Columnas

// Definimos el Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = { 12,11,6,5 };
byte colPins[COLS] = { 4,3,2};

// Creamos el Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
      keypad.addEventListener(keypadEvent);
      pinMode(sensorpir,INPUT);
      pinMode(buzzer, OUTPUT); 
      pinMode(led, OUTPUT); 
      password.reset();
      delay (5000);  //Nos damos un tiempo para encender el GPRS y la alimentación de la tarjeta
      SIM900.begin(19200);  //Configura velocidad del puerto serie para el SIM900
      Serial.begin(19200);  //Configura velocidad del puerto serie del Arduino
      Serial.println("OK");
      delay (1000);
      SIM900.println("AT + CPIN = \"2016\"");  //Comando AT para introducir el PIN de la tarjeta
      delay(25000);  //Tiempo para que encuentre una RED
}

void loop(){
  keypad.getKey();
  if (digitalRead(sensorpir)== HIGH && estado_alarma== true){
    detectado();
  }
}

void detectado(){
    digitalWrite(led, HIGH);
    mensaje_sms();
    //delay(1000);
    digitalWrite(led, LOW);
    Serial.println("Intruso detectado");
}
void llamar()
   {
      Serial.println("Realizando llamada...");
      SIM900.println("ATD606747597;");  //Comando AT para realizar una llamada
      delay(30000);  // Espera 30 segundos mientras realiza la llamada
      SIM900.println("ATH");  // Cuelga la llamada
      delay(1000);
      Serial.println("Llamada finalizada");
   }
void mensaje_sms()
  {
      Serial.println("Enviando SMS...");
      SIM900.print("AT+CMGF=1\r");  //Configura el modo texto para enviar o recibir mensajes
      delay(1000);
      SIM900.println("AT+CMGS=\"606747597\"");  //Numero al que vamos a enviar el mensaje
      delay(1000);
      SIM900.println("ALARMA HAY INTRUSOS EN SU HOGAR");// Texto del SMS
      delay(100);
      SIM900.println((char)26); //Comando de finalización ^Z
      delay(100);
      SIM900.println();
      delay(5000);  // Esperamos un tiempo para que envíe el SMS
      Serial.println("SMS enviado");
   }

void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
    case PRESSED:
	Serial.print("Pressed: ");
	Serial.println(eKey);
	switch (eKey){
	  case '*': checkPassword(); break;
	  case '#': password.reset(); break;
	  default: password.append(eKey);
    }
  }
}

void checkPassword(){
  if (password.evaluate()){
    
        if(estado_alarma == false){
              estado_alarma = true;
              digitalWrite(buzzer, HIGH );   
              Serial.println("Alarma Activada correctamente");
              password.reset();
          
          }else if (estado_alarma == true){
              // apagamos alarma al estar el sensor pir activado
              estado_alarma = false;
              digitalWrite(buzzer, LOW );
              Serial.println("Alarma desactivada  correctamente");
              password.reset();
            }  
  }else{
    Serial.println("Codigo erroneo");
    password.reset();  
  }
}
  
