#include "DHT.h"          // biblioteka DHT
#define DHTTYPE DHT11         
const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);
void setup()
{
  Serial.begin(115200);     // otworzenie portu szeregowego
  dht.begin();         // inicjalizacja czujnika
}
 
void loop()
{
  // Odczyt temperatury i wilgotności powietrza
  float t = dht.readTemperature();
  float h = dht.readHumidity();
 
  // Sprawdzamy czy są odczytane wartości
  if (isnan(t) || isnan(h))
  {
    // Jeśli nie, wyświetlamy informację o błędzie
    Serial.println("Blad odczytu danych z czujnika");
  } else
  {
    // Jeśli tak, wyświetlamy wyniki pomiaru
    Serial.print("Wilgotnosc: ");
    Serial.print(h);
    Serial.print(" % ");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
  }
}
