
#include <OneWire.h>
#include <EEPROM.h>
OneWire iButton(9); // создаём объект 1-wire на 10 выводе

void setup() {
  Serial.begin(9600);
  pinMode(12 , OUTPUT);//наш ключ 
  pinMode(11 , OUTPUT);//включается когда не наш ключ 
  pinMode(10, OUTPUT);//кол-во включений
}

void loop() {
  delay(500);                         
  
  byte addr[8]; // массив для хранения данных ключа
  
  byte dataArray[12];
  
  if ( !iButton.search(addr) ) { // если ключ не приложен
      Serial.println("No key connected...");
      return;
  }
  
  Serial.print("Key : ");
  for(int i=0; i<12; i++) {
    Serial.print(addr[i], HEX); //побайтно данные ключа
    EEPROM.update(i, addr[i]);//                                      clear it up 
    EEPROM.update(i+12, addr[i]);
    Serial.print(" ");
  }
  Serial.println();
  iButton.reset(); // сброс ключа
  
  //СМОТРИМ НАШ ЛИ ЭТО КЛЮЧ
  byte dataArray_read[12];
  int count;//считает кол-во входов нашего ключа за сутки 
  EEPROM.get(50, count);
  Serial.println("Выводим что в памяти:");
  int tmp = 0;
  for (int i=0; i<12; i++){
      if ( EEPROM.get(i, dataArray_read[i]) == EEPROM.get(i+12, dataArray_read[i+12]) ){
         tmp++;
      }
  }
  if (tmp == 12){
    Serial.println("наш ключ");
    EEPROM.update(50, count+1);
    digitalWrite(11, HIGH); 
    delay(5000); 
    digitalWrite(11, LOW);
  } else {
    digitalWrite(12, HIGH); 
    delay(5000); 
    digitalWrite(12, LOW);
    Serial.println(tmp);
    Serial.println("Не наш ключ");
  }
  
  //ПРОВЕРЯЕМ ВРЕМЯ после запуска проги
  uint32_t sec = millis() / 1000ul;      // полное количество секунд
  int timeHours = (sec / 3600ul);        // часы
  if (timeHours >= 24){
    EEPROM.update(50, 0);//раз в сутки обнуляем время
  }
  
  Serial.print("время: "); 
  Serial.println(sec);
  
  int i=count;
  while (i>0){
    digitalWrite(10, HIGH); 
    delay(500); 
    digitalWrite(10, LOW);
    delay(500);
    i--;
  }
  Serial.print("кол-во подключений: "); 
  Serial.println(count);
  
  delay(5000);
}
