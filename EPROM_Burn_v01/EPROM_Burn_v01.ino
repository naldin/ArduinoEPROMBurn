/*
    Gravador de EPROM v01
    Grava dados na EEPROM 27C256
    Autor: Ronaldo Rezende Junior
    Data: 01/2021
    Arduino: Mega 2560

    Pinos de endereço:
    ADD | PIN
    A0  | 22
    A1  | 24
    A2  | 26
    ~   | ~
    A13 | 46
    A14 | 48
    A15 | 50

    Pinos de dados:
    Bit | PIN
    0   | 23
    1   | 25
    2   | 27
    3   | 29
    4   | 31
    5   | 33
    6   | 37
    7   | 39

    Pino 53 - CE (pulso de 100uS)

    Pino 11 - Botão de gravação:

     (+)--------
               |
               |
               \  Chave
                \
               |
               |
      |--------------- recPin (Pino 11)
      |
     | | 10k
     | | ohms
      |
      |
      |
     (-)

*/

// Array dos dados que serão gravados
// O arduino tem um limite de 16384 posições de array, o que daria 14 pinos de endereços

int valByteROM[] = {0x3E, 0x80, 0xD3, 0xFF, 0x0F, 0xFE, 0x01, 0x28, 0x02, 0x18, 0xF7, 0xD3, 0xFF, 0x07, 0xFE, 0x80, 0x28, 0xF0, 0x18, 0xF7};

int pinCE = 53;       //Pino do chip enable
int pinAdd = 22;      //Porta inicial de endereço
int portasAdd = 15;   //Qtd de portas de endereço
int somaPinAdd = 2;   //Soma para os pinos de endereço
int pinDados = 23;    //Porta inicial de dados
int portasDados = 8;  //Qtd de portas de dados
int somaPinDados = 2; //Soma para os pinos de dados

uint16_t bitsAdd = 32767; // 15 bits de endereço (2^15)

int recPin = 11;  //Pino que inicia a gravação
int pinLed = 13; //Pino do Led

void setup() {
  Serial.begin(57600);

  // Seta pino de gravação como entrada
  pinMode(recPin, INPUT);

  // Seta pino do led como saída
  pinMode(pinLed, OUTPUT);

  // Seta pino CE como saída
  pinMode(pinCE, OUTPUT);
  digitalWrite(pinCE, HIGH);

  // Seta portas de endereço como saída
  for (int i = 0; i < (portasAdd + 1); i++) {
    pinMode(pinAdd, OUTPUT);
    digitalWrite(pinAdd, LOW);
    pinAdd = pinAdd + somaPinAdd;
  }
  // Seta portas de dados como saída
  for (int i = 0; i < (portasDados + 1); i++) {
    pinMode(pinDados, OUTPUT);
    digitalWrite(pinDados, HIGH);
    pinDados = pinDados + somaPinDados;
  }

  Serial.println("");
  Serial.println("");
}

void loop() {

  Serial.println("Pressione o botão de gravação para iniciar\n");

  // Em loop até o botão de gravação ser pressionado
  while (digitalRead(recPin) == 0) {

  }

  Serial.println("Aguarde...");
  uint16_t mask = 0b0000000000000001; // Mascara para o "and"
  pinAdd = 22;        //Porta inicial de endereço ref A0
  pinDados = 23;      //Porta inicial de dados
  int arrayCount = 0; //Contador para o array dos dados

  //Roda todos os bits de endereço 2^15
  for (uint16_t i = 0; i < (bitsAdd + 1); i++) {

    // Roda os 16 pinos de endereço A0~A15
    for (int j = 0; j < (portasAdd + 1); j++) {

      //Se o "and" do bit com a mascara retornar 1 escreve 1 na porta
      if (i & mask) {
        digitalWrite(pinAdd, HIGH);
      }
      else {
        digitalWrite(pinAdd, LOW);
      }

      pinAdd = pinAdd + somaPinAdd; //Soma para ir ao proximo pino de endereço
      mask = mask << 1; // Bit shift da mascara

    }

    pinAdd = 22; //Retorna para porta inicial ref A0
    mask = 0b0000000000000001; //Retorna para mascara inicial

    // Roda os 8 pinos de dados para cada posição de dados do array
    for (int i = 0; i < (portasDados); i++) {

      // Pega o bit de 0 a 7 e escreve na porta
      if ((valByteROM[arrayCount] ^ 0xFF) & mask) {
        digitalWrite(pinDados, LOW);
        //Serial.print(0);
      }
      else {
        digitalWrite(pinDados, HIGH);
        //Serial.print(1);
        }
      
      pinDados = pinDados + somaPinDados; //Soma para ir ao proximo pino de dados
      mask = mask << 1; // Bit shift da mascara
    }
    //Serial.println();

    arrayCount++;  //Soma para proxima posição do array
    mask = 0b0000000000000001; //Retorna para mascara inicial
    pinDados = 23;  // Retorna para porta inicial ref O0

    // Envia o pulso de 100us para o chip enable
    digitalWrite(pinCE, LOW);
    delayMicroseconds(92);
    digitalWrite(pinCE, HIGH);

    // Encerra o processo depois do ultimo dado do array
    if (arrayCount == (sizeof(valByteROM) / 2)) {
      break;
    }
  }

  Serial.println("EPROM gravada\n");
  Serial.print("Total de ");
  Serial.print(sizeof(valByteROM) / 2);
  Serial.print(" endereços gravados");

  while (1) { //loop eterno
    digitalWrite(pinLed, HIGH);
  }
}
