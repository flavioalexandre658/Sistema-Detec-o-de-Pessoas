/*
* Para sensores ultrasonicos com distancia entre eles de 0,13 m ocorrerá interferencia destrutiva na altura de 2,1 m e intereferencia construtira na altura de 1,0 m.
*/
#define trigger1 2
#define echo1 3

#define trigger2 4
#define echo2 5

#define trigger3 6
#define echo3 7

boolean contabilizado = false;
boolean detect = false;
boolean fora = false;
boolean dentro = false;

int cont = 0;
float dec_dist = 1.50;
unsigned long timer = 500;
unsigned long timer_pass = 1000;
int velocidade_som = 319;
float altura_porta=2.10;
/*
int buzzer = 9;

int A = 2;
int B = 3;
int C = 4;
int D = 5;
int E = 6;
int F = 7;
int G = 8;

byte seven_seg_digits[10][7] = { { 1,1,1,1,1,1,0 },  // = 0
                                 { 0,1,1,0,0,0,0 },  // = 1
                                 { 1,1,0,1,1,0,1 },  // = 2
                                 { 1,1,1,1,0,0,1 },  // = 3
                                 { 0,1,1,0,0,1,1 },  // = 4
                                 { 1,0,1,1,0,1,1 },  // = 5
                                 { 1,0,1,1,1,1,1 },  // = 6
                                 { 1,1,1,0,0,0,0 },  // = 7
                                 { 1,1,1,1,1,1,1 },  // = 8
                                 { 1,1,1,0,0,1,1 }   // = 9
                               };*/
                               
void setup() {     
  Serial.begin(9600);          
   /*
  pinMode(A, OUTPUT);   
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  
  pinMode(buzzer,OUTPUT);
  */
  pinMode(trigger1, OUTPUT);
  digitalWrite(trigger1, LOW);
  pinMode(echo1, INPUT);

  pinMode(trigger2, OUTPUT);
  digitalWrite(trigger2, LOW);
  pinMode(echo2, INPUT);

  pinMode(trigger3, OUTPUT);
  digitalWrite(trigger3, LOW);
  pinMode(echo3, INPUT);
} 
 
void loop() {
  delay(timer);
  PassOnDoor();
  Serial.print("Detectado: ");
  Serial.println(BoolToString(detect));
  Serial.print("Entrou: ");
  Serial.println(BoolToString(fora));
  Serial.print("Saiu: ");
  Serial.println(BoolToString(dentro));
  Serial.print("Contabilizado: ");
  Serial.println(BoolToString(contabilizado));
  Serial.print("Contagem: ");
  Serial.println(cont);
  Serial.println("################################################");
}

/*
* Função que vai escrever um numero no display de 7 segmento
*/

/*void numberWrite(byte digit) {
  byte pin = 2;
  for (byte contDetect = 0; contDetect < 8; ++contDetect) {
     digitalWrite(pin, seven_seg_digits[digit][contDetect]);
     ++pin;   
   } 
}*/

/*
* Função que converte variavel booleana para String.
* @return String
*/

String BoolToString(boolean boole){
  if(boole == 1){
    return "Sim";
  }else{
    return "Não";
  }
}

/*
* Função que vai varrer a area da porta e estabelecer
* os parametros para detectar se a pessoa esta
* entrando ou saindo do ambiente.
*/

void PassOnDoor(){ 
  if(fora == false || dentro == false){
    detect = isPerson(trigger1, echo1, 10); //leitura Ultrasonic
    if(detect == true){
      delay(timer_pass);
      fora = isPerson(trigger2, echo2, 10); // leitura Ultrasonic
      //dentro = isPerson(trigger3, echo3, 10); // leitura Ultrasonic    
    }else{
      contabilizado = detect;
    }
  }else{
    if((fora == true || dentro == true) && contabilizado == true){
      fora = false;
      dentro = false;
    }
  }
  Contabilize();
}

/*
* Função que vai gerenciar a contagem de pessoas
* presente no ambiente.
*/

void Contabilize(){
    if(detect == true){
      if(fora == true && dentro == false && contabilizado == false){ // Incrementa a contagem
        //numberWrite(cont);
        contabilizado = true;
        cont++;    
        //tone(buzzer,1500);
        //delay(500);
        //noTone(buzzer);
      }else if(dentro == true && fora == false && contabilizado == false){ // Decrementa a contagem 
        //numberWrite(cont);
        contabilizado = true;
        cont--;
        //tone(buzzer,1500);
        //delay(500);
        //noTone(buzzer);
      }else{ // Não faz nada
        //numberWrite(cont);
        cont;
      }
  }
}

/*
* Função que vai varrer emitir pulso dos sensores Ultrasônicos
* e retornar se foi detectado um objeto ou não.
* @return boolean
*/

boolean isPerson(int trigger, int echo, int cd){
  digitalWrite(trigger, HIGH);
  delayMicroseconds(cd);
  digitalWrite(trigger, LOW);
  float tempo = pulseIn(echo, HIGH);                              // Lê o tempo em que echo permanece em nivel alto, tempo dado em microsegundos
  tempo = tempo/1000000;                                    // Converte o tempo para segundos
  float distancia = (tempo*velocidade_som)/2;
  Serial.print("Distancia do tigger ");
  switch(trigger){
    case trigger1:
      Serial.print("Meio");
      Serial.print(": ");
      break;
    case trigger2:
      Serial.print("fora");
      Serial.print(": ");
      break;
    case trigger3:
      Serial.print("Dentro");
      Serial.print(": ");
      break;
  }
  if(distancia >= altura_porta){
    distancia = altura_porta;
  }else{
    distancia = 1.00;
  }
  Serial.println(distancia);
  if(distancia >= dec_dist){ // Se a distancia for 2.10 m ( altura portas padrão ), retorna false
    return false;
  }else{ // Caso contrario, se a distancia for menor, retorna true
    return true;
  }
}
