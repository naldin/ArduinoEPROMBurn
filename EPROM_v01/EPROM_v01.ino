 /*
    Leitor de EPROM
    Confere cada posição de memoria se está em FF
    Autor: Ronaldo Rezende Junior
    Data: 11/2020
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
*/

int pinAdd = 22; //Porta inicial de endereço
int portasAdd = 15; //Qtd de portas de endereço
int somaPinAdd = 2; //Soma para os pinos de endereço
int pinDados = 23; //Porta inicial de dados
int portasDados = 8; //Qtd de portas de dados
int somaPinDados = 2; //Soma para os pinos de dados
int saidaDados = 0; //Valor de saida dos dados
int countErro = 0;

uint16_t bitsAdd = 32767; // 15 bits de endereço (2^15)

void setup() {
  Serial.begin(57600);

  // Seta portas de endereço como saída
  for (int i = 0; i < (portasAdd + 1); i++) {
    pinMode(pinAdd, OUTPUT);
    digitalWrite(pinAdd, LOW);
    pinAdd = pinAdd + somaPinAdd;
  }
  // Seta portas de dados como entrada
  for (int i = 0; i < (portasDados + 1); i++) {
    pinMode(pinDados, INPUT);
    pinDados = pinDados + somaPinDados;
  }
  Serial.println("");
  Serial.println("");
  Serial.println("");
}

void loop() {
  uint16_t mask = 0b0000000000000001; // Mascara para o "and"
  pinAdd = 22; //Porta inicial de endereço ref A0
  pinDados = 23; //Porta inicial de dados

  //Roda todos os bits de endereço 2^15
  for (uint16_t i = 0; i < (bitsAdd + 1); i++) {
    Serial.print("Pos: ");
    Serial.print(i);
    Serial.print(" | ");
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
      //delay(1);
    }
    pinAdd = 22; //Retorna para porta inicial ref A0
    mask = 0b0000000000000001; //Retorna para mascara inicial

    // Roda os 8 pinos de dados
    for (int i = 0; i < (portasDados); i++) {

      // Se a porta for 1 adiciona o valor na variavel saidaDados
      if (digitalRead(pinDados) == 1) {
        saidaDados = (saidaDados | mask);
        Serial.print(1);
      }
      else{
        
        Serial.print(0);
        }
      pinDados = pinDados + somaPinDados; //Soma para ir ao proximo pino de dados
      mask = mask << 1; // Bit shift da mascara
      delayMicroseconds(1);
    }
    Serial.print(" | ");
    // Se o valor da saidaDados nao for FF imprime erro
    if (saidaDados != 0xFF) {
      Serial.print(saidaDados, HEX);
      Serial.println(" | Erro");
      countErro++;
    }
    else {
      Serial.print(saidaDados, HEX);
      Serial.println(" | Ok");
    }
    saidaDados = 0; // Retorna saida de dados para 0
    pinDados = 23;  // Retorna para porta inicial ref O0
    mask = 0b0000000000000001; //Retorna para mascara inicial
    //delay(1);
  }
  Serial.println();
  Serial.print("Pos com dados: ");
  Serial.println(countErro);
  while (1) {

  }
}
