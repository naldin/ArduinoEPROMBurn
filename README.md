# ArduinoEPROMBurn
Arduino MEGA leitor e gravador de EPROM

* EPROM_v01 - Leitor de EPROM que mostra os valores em BIN e HEX de cada posição de memoria pela serial
* EPROM_v02 - Leitor de EPROM que mostra se a memoria está limpa ou a quantidade de posições ocupadas
* EPROM_Burn_v01 - Gravador de EPROM

Instruções de ligações das pinagens estão no sketch.

Para adicionar o código compilado no sketch Arduino, use o convbin, no Windows basta arrastar o arquivo .BIN para cima do convbin.exe e no Linux use o comando:

`./convbin ARQUIVO.bin`

Será gerado um arquivo out.txt, dentro dele terão os bytes para serem adicionados no array valByteROM[] da linha 54 do sketch Arduino.

Caso tenha compilado um formato .hex, será preciso converter primeiro para .bin, o pacote hexstuff.zip tem o BIN2HEX.EXE para fazer a conversão. O procedimento é o mesmo do convbin.

<img src="https://github.com/naldin/ArduinoEPROMBurn/raw/master/ArduinoEprom.jpg" />

