//  Includes de bibliotecas que iremos utilizar. Uma do ultrassom, outra para o controle PID
#include <HCSR04.h>
#include <PID_v1.h>

// definicao das variáveis dos pinos do sonar
#define trigger 4
#define echo 6

// definicao das variaveis dos pinos do motor dc
#define motor_A 12
#define brake_A 9

//  Declaração das variáveis controladas
double objetivo, entrada, saida;

// Definição das constantes de controle
double kp = 25,
       ki = 0.2,
       kd = 0.02;

//  Instanciamento da função PID
PID myPID(&entrada, &saida, &objetivo, kp, ki, kd, DIRECT);

//  Instanciamento da medição de distancia com o ultrassom
UltraSonicDistanceSensor distanceSensor(trigger, echo);

void setup() {
//  Padrão de entrada nula e set do andar a qual se deseja ir
  entrada = 0;
//  Primeiro andar = 30 cm, Segundo andar = 60 cm, Terceiro andar = 90 cm, Quarto andar = 120 cm
  objetivo = 0;

//  Escolha do modo da função PID
  myPID.SetMode(AUTOMATIC);

//  Escolha da do tipo de informação dos principais pinos
  pinMode(echo, INPUT);
  pinMode(motor_A, OUTPUT);
  pinMode(brake_A, OUTPUT);
  
//  Limites de valores de pulso PWM para a função PID
  myPID.SetOutputLimits(0,255);

//  Função que habilita o comunicador serial
  Serial.begin(9600);
  
}

void loop() {
  // Obtenção da distancia atraves do sensor e estabecer como variável de entrada do PID
  float dist = distanceSensor.measureDistanceCm();
  entrada = dist;

//  Se a entrada for maior que o objetivo gire o motor em um sentido
  if (entrada > objetivo) {
    Serial.println("distancia em cm: ");
    Serial.println(dist);
//    Serial.println("entrada > obj");/
    digitalWrite(motor_A, HIGH); //Ativa o motor em stand-by
    digitalWrite(brake_A, LOW); //Desativa o freio do motor

//    Parte responsável pelo controle PID
    myPID.Compute();
    analogWrite(3, saida);
  }

//  Se a entrada for menor que o objetivo, gire o motor no outro sentido
  if (entrada < objetivo) {
    Serial.println("distancia em cm: ");
    Serial.println(dist); 
//    Serial.println("entrada < obj");/
    digitalWrite(motor_A, LOW); //Ativa o motor em stand-by
    digitalWrite(brake_A, LOW); //Desativa o freio do motor

//    Parte responsável pelo controle PID
    myPID.Compute();
    analogWrite(3, saida); 
    }
    
    delay(1);
  }
