/*
				_______________________________________
							WARNING!!!						
				_______________________________________

 THIS CODE ISN'T WRITE BY ME, I JUST MADE COMMENTS IN PORTUGUESE TO HELP
THOSE WHO ARE BEGGINNERS IN ARDUINO. ALL CREDITS GOES TO: http://www.instructables.com/id/Electronic-Safe-With-Arduino/
*/






#include <Liquid.Crystal.h> // Biblioteca que mexe no LCD

#include <EEPROM.h> // Biblioteca que mexe no chip de memória

#include <Servo.h> // Biblioteca que controla o motorzinho

int address = 0;

static unsigned long SaveTimer;                

static unsigned long SaveDelay = (30 * 1000); 

char CODE[10] = "123142E";  // A senha é mudada nessa linha, suporte de até 10 caracteres.

char Str[10]; 

char CodeLength = 6; // Define o tamanho do código. (Sinceramente, não sei por qual razão fizeram isso ao invés de usarem strlen)

int Pos = 0;

bool Unlocked; /* Booleana para verificar se cofre abrirá. True para aberto, False para fechado*/

static unsigned long DisplayTimer; // Apenas uma declaração normal

static unsigned long DisplayDelay = 200; // Apenas uma declaração normal

LiquidCrystal lcd(12, 11, 9, 8, 7, 6);  /* Cria variável do tipo "LiquidCrystal", provinda da biblioteca <LiquidCrystal.h>. A estrutura
desta função é feita da seguinte maneira: LiquidCrystal(rs,rw, enable, d0,d1,...,d8). Rs é o número que está ligado ao pino
Rs do arduíno, ou seja, o pino que controla onde e como a informação será passada para o LCD. R/W é o número do pin que define o modo de leitura ou
escrita, esta opção é totalmente opcional. enable é o número do pino que permite a conexão aos registradores, isto é, memória RAM diretamente da CPU do arduíno.
Os pins de d0 a d8 são os lugares em que a informação será escrita ao registrador. d0 a d3 é opcional, se omitido, a função define d4 a d7 como
argumentos */

/*Resumindo e explicando o que a linha 27 faz: Define o RS no pin 12, define o enable em 11, define  9 como d4, 8 como d5, 7 como d6, 6 como d7*/

int buttonPin1 = 2;   // Define o pin do botão 1

int buttonPin2 = 3;

int buttonPin3 = 4;

int buttonPin4 = 5;

int enterbutton = 10; // Define o pin do botão de entrar

int clearlockbutton = 13; // Define o botão de limpar

Servo myServo;   // Variável que declara o servo (motorzinho). Incluída na biblioteca <Servo.h>

// Tudo dentro de void setup() roda apenas uma vez a cada vez que reset é apertado
void setup() {

  myServo.attach(A1);  // Conecta o mortozinho a um pin. No caso, A1 

  Lock(); // Se refere a uma função descrita na linha 335. Nesse caso, a função determina o ângulo do motor como 26 graus.
  // Para mais detalhes, confira a descrição da função.

  int EEPROMCodeOK = true; // integer importante para verificar se o código é válido

  /* Esse loop lerá o endereço de cada char de Str, importante para conferir se valores são váçodps para o código */
  for (Pos = 0; Pos <= (CodeLength); Pos++) {

    Str[Pos] =  EEPROM.read(Pos); // EEPROM.read lê a memória.

    if (!(strrchr("1123456789", Str[Pos]))) { /* Aqui está invertido? Caso não seja achado o caractere "1123456789", o 
código será dado como inválido. Não sei o propósito disso.	*/

      EEPROMCodeOK = false;

    }

  }

  Pos++;

  Str[Pos] =  EEPROM.read(Pos); 

  if (Str[CodeLength + 1] != 'E') EEPROMCodeOK = false; // Se o último caractere do código não for E, será inválido

  if (EEPROMCodeOK) { // loop é ativado quando o código é válido

    Str[CodeLength + 2] = '\0'; // Coloca um espaço no último caractere da string

    strncpy(CODE, Str, CodeLength + 1); // Copia os caracteres do código para CODE. 

  }

  ClearCode(); // vide linha 314. Reseta-se tudo.

  pinMode(buttonPin1, INPUT_PULLUP); // Pinmode serve para defenir se o pin de determinado será um imput ou output

  pinMode(buttonPin2, INPUT_PULLUP);

  pinMode(buttonPin3, INPUT_PULLUP);

  pinMode(buttonPin4, INPUT_PULLUP);

  pinMode(enterbutton, INPUT_PULLUP);

  pinMode(clearlockbutton, INPUT_PULLUP);

  lcd.begin(16, 2); // Define quantas linhas e colunas a tela terá

  lcd.setCursor(0, 0);

  lcd.print("Hello.");

  delay(2000); // Delay é um espaço de tempo até a próxima linha ser executada. A cada 1000, equivale 1 segundo;

  lcd.clear(); // limpa a tela

  lcd.setCursor(0, 0);// seta o cursor no canto superior esquerdo

  lcd.print("Password:"); // Printa a palavra password

  DisplayTimer = millis() + 200; // millis = tempo que a placa arduíno foi ligada em milézimos

}

/* Tudo o que estiver dentro de void loop() será executado várias vezes, de novo e de novo....*/
void loop() {

  Lock(); // vide linha 337. Trava a porta.

  Pos = constrain(Pos, 0, CodeLength); // Não deixa pos ter valor maior do que a int codeLenght

  int buttonState1 = digitalRead(buttonPin1);  /* Int é definida como o resultado da voltagem do botão do argumento. 
  No nosso caso, se pressionado, voltará como LOW.*/

  int buttonState2 = digitalRead(buttonPin2);

  int buttonState3 = digitalRead(buttonPin3);

  int buttonState4 = digitalRead(buttonPin4);

  int clButtonState = digitalRead(clearlockbutton);

  int enterButtonState = digitalRead(enterbutton);

  lcd.setCursor(9, 0); // Posição do cursor

  if (buttonState1 == LOW) { // Se botão for pressionado, retornará o valor 1. 

    Str[Pos] = '1'; // Coloca o valor 1 na posição atual

    Pos++; 

    Str[Pos] = '\0'; // Coloca valor vazio na próxima

    delay(250); //1/4 de segundo

    while (digitalRead(buttonPin1) == LOW); //Qual o propósito disso

  }

  else if (buttonState2 == LOW) { // Mesma coisa que a outra condição, mas agora como o segundo botão como 2

    Str[Pos] = '2';

    Pos++;

    Str[Pos] = '\0';

    delay(250); 

    while (digitalRead(buttonPin2) == LOW);

  }

  else if (buttonState3 == LOW) {

    Str[Pos] = '3';

    Pos++;

    Str[Pos] = '\0';

    delay(250); 

    while (digitalRead(buttonPin3) == LOW);

  }

  else if (buttonState4 == LOW) {

    Str[Pos] = '4';

    Pos++;

    Str[Pos] = '\0';

    delay(250); 

    while (digitalRead(buttonPin4) == LOW); 

  }

  else if (enterButtonState == LOW) {

    Str[Pos] = 'E';

    Pos++;

    Str[Pos] = '\0';

    delay(250);

    lcd.setCursor(15, 0);

    lcd.print("E");

    delay(400);

    lcd.setCursor(15, 0);

    lcd.print(" ");

    while (digitalRead(buttonPin1) == LOW); 

     if (strcmp (Str,CODE) == 0) { //Se os caracteres de str forem iguais ao de CODE, a trava será liberada

      Unlocked = true;

      lcd.setCursor(0, 0);

      lcd.print(" Access Granted");

      delay(2000); // 2 segundos

      lcd.clear();

      lcd.print("    Unlocked");

    } 

    else { // Caso contrário, o cofre não se abrirá

      lcd.clear();

      lcd.print(" Access Denied.");

      delay(2000);

      lcd.clear();

      lcd.print("Password:");

    }

    while (Unlocked) { // Enquanto a booleana unlocked for true

      Unlock();

      if (digitalRead(clearlockbutton) == LOW) { // Se o botão de limpar código for apertado, tudo volta à estaca zero no LCD

        delay(200);

        lcd.clear(); // Reseta tudo, coloca pos como 0 e o primeiro caractere como espaço

        lcd.print("     Locked");

        delay(2000);

        lcd.clear(); // Tela é limpada

        Unlocked = false;

        SaveTimer = millis() + 30000; // Tempo de referência é igual o tempo que a placa foi ligada + 30 segundos

      }

    }

    ClearCode(); // Reseta tudo, coloca pos como 0 e o primeiro caractere como espaço

  }

  else if (clButtonState == LOW) { // Se o botão de limpar for apertado, o código é limpo, complemento do estatamento anterior

    delay(500);

    while (clearlockbutton == LOW);

    if ((millis() - SaveTimer) > 4500) { // não sei para que isso serve

    }

    ClearCode();

  }

  if ( (long)( millis() - DisplayTimer ) >= 0) { /* Pelo que entendi, isso é feito para que a tela volte a mostrar "Password" 
  e colocar os cursores na estaca 0 */

    DisplayTimer += DisplayDelay;

    lcd.setCursor(9, 0); 

    lcd.print(Str);

    lcd.setCursor(15, 0);

    lcd.print(" ");

    if (clButtonState == LOW) {

      lcd.clear();

      lcd.print("Password:");

    }

  }

}

void ClearCode() { //Reseta a tudo, incluindo o valor de pos. A tela da senha fica em branco e o cursor vai para a primeira casa

  Pos = 0;

  Str[Pos] = '\0';

  lcd.setCursor(0, 0);

  lcd.print("Password:");

  lcd.setCursor(0, 1);

  lcd.print("          ");

}

void Unlock() { // Altera a angulação do motorzinho de maneira que ele destrave a porta (117 graus, no caso);

  myServo.write(117);

}

void Lock() { //Altera a angulação do motorzinho de maneira que ele trave a porta (26 graus, no caso);

  myServo.write(26);

}