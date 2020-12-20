    #define POMPA 4 
    #define IN 5 
    #define LIMIT 6
    #define LEDERR 7
    #define WATERR 3
    
    #define POMPA_TIME_MAX 3000   //maksymalny czas pracy pompy 3 s = 3000 ms 
    
    //zmienne przedstawiające stan pompy 
    int pompa=0;  //0 - wyłączona   1 - włączona  2 - error (awaria) 
    #define OFF 0 
    #define ON  1 
    #define ERR 2 

    //zmienne dla czujnika poziomu wody
    #define trigPin 12
    #define echoPin 11
    
    //zmienne zawierające czas 
    unsigned long time_on = 0;       //czas włączenia pompy 
    unsigned long aktualny_czas=0;   //aktualny czas 

   void setup() 
    { 
      Serial.begin (9600); 
      pinMode(POMPA, OUTPUT);    // Włączenie przekaźnika 
      pinMode(IN, INPUT_PULLUP); // IN jako stan wody w misce 
      pinMode(LIMIT, INPUT_PULLUP); // wskaznik konca miski - gdyby sie mialo przelac
      pinMode(LEDERR, OUTPUT); // Czerwony led alarmu
      pinMode(WATERR, OUTPUT); // Czerwony led alarmu
      pinMode(trigPin, OUTPUT); //Pin, do którego podłączymy trig jako wyjście
      pinMode(echoPin, INPUT); //a echo, jako wejście
      
      pompa = 0; 
      time_on = 0; 
    } 
      
    void loop() {
    //czytanie odleglosci / poziomu wody
        Serial.print(zmierzOdleglosc());
        Serial.println(" cm");
        zakres(10, 25);
        delay(500);
       
        

        //KONIEC czytanie odleglosci / poziomu wody
  
      //Pobierz liczbe milisekund od startu 
      aktualny_czas = millis(); 

      //w pętli switch w zależności od wartości zmiennej pompa wykonujemy nasz program 
      switch(pompa) 
      { 
        //---------------------------------------------------------------------------- 
        case OFF: 
          //pompa wyłączona więc sprawdzimy czy jest woda 
          if(digitalRead(IN)==LOW)             //jeśli brak wody 
            { 
              time_on=aktualny_czas;             //zapamiętaj czas włączenia 
              digitalWrite(POMPA, HIGH);        //włącz pompę 

              pompa=ON; 
            } 
          break;    //wyjście ze switch 
         //---------------------------------------------------------------------------- 
         case ON: 

           //pompa włączona więc sprawdzimy czy jest woda 
          digitalWrite(POMPA, HIGH);   //jeśli jest wode 
           delay(3000);
          digitalWrite(POMPA, LOW);         //wyłącz pompę 
           delay(1000);
           pompa=OFF; 

          if(digitalRead(LIMIT)==LOW)    //wody nie ma więc sprawdzimy czy pompa nie pracuje za długo 
            { //czas minął więc jest awaria! 
               digitalWrite(POMPA, LOW);       //wyłącz pompę na wszelki wypadek 
               pompa=ERR; 
            } 
           break;   //wyjście ze switch 
         //---------------------------------------------------------------------------- 
         case ERR: 
               digitalWrite(POMPA, LOW);       //wyłącz pompę na wszelki wypadek
               digitalWrite(LEDERR, HIGH);      
         defalut: 
          //jeśli pompa jest różna od ON, OFF i ERR to nie rób nic - wyjdź z pętli switch 
          break; 
      } //koniec switch 
} 
  
  int zmierzOdleglosc() {
  long czas, dystans;
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  czas = pulseIn(echoPin, HIGH);
  dystans = czas / 58;
 
  return dystans;
  }
     
    void zamigajLED() {
  digitalWrite(WATERR, HIGH); //Włączenie diody
  delay(500); //Odczekanie 0,5 sekundy
  digitalWrite(WATERR, LOW); //Wyłączenie diody
  delay(500); //Odczekanie 0,5 sekundy
}
   void zakres(int a, int b) {
    int jakDaleko = zmierzOdleglosc();
    if ((jakDaleko > a) && (jakDaleko < b)) {
      zamigajLED();
  }
}
