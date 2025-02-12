# Pipeline

Pipeline ist die Bibliothek um mit Proto zu interagieren. Um mehr über die einzelnen Funktionen zu erfahren existieren in `Pipeline.h` Kommentare zu jeweils jeder Funktion.

## Beispiel Projekt

This code will try to connect with proto. 

```c++
void loop() {
  // tryOpen versucht die Pipe zu öffnen
  // gibt true zurück wenn proto zuhört und false wenn nicht
  // Ausgabe == pipeline.isOpen()
  if (pipeline.tryOpen()) {
    // Eine Debugnachricht welche von Proto ausgegeben wird
    // Wenn die Pipe nicht offen ist, wird die Nachricht als
    // Rohtext an Serial geschickt
    pipeline.println("Verbindung hergestellt!");
    // Ein Paket an das Frontend senden
    // Geht nur wenn die Pipe offen ist
    const char* text = "Hallo Frontend";
    pipeline.send(101, text, strlen(text));
    // Mit einem Ping kann gefragt werden ob die Pipe
    // immer noch offen ist. So hat man einen TCP like effekt
    // um sicher zu gehen, dass ein Paket wirklich gesendet wurde
    if (pipeline.ping(200)) {
      pipeline.println("Ping erhalten");
    } else {
      pipeline.println("Ping nicht erhalten");
    }
  } else {
    pipeline.println("Keine Verbindung möglich");
  }
  // Proto bekommt den Status nicht mehr zuzuhören.
  // Das Programm läuft weiter und die Verbindung kann mit tryOpen()
  // wieder aufgenommen werden
  pipeline.close();
  delay(5000);
}
```