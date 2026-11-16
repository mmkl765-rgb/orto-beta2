// FILE CON LE FUNZIONI OPERATIVE RICHIAMATE NEL LOOP

// SCRIVI SU LCD: scrive le righe del display impostando X,Y,msg. Prima riempi il buffer cosi strcpy_P(W_BUFF, PSTR("messaggio..."))
// poi effettui la scrittura con  SCRIVI_LCD(0,1,W_BUFF). Implementa l'autoriempimento con spazi fino al ventesimo carattere per 
// riempire tutta la riga del lcd. Esegue la scrittura dello stesso msg in seriale per debug.
void SCRIVI_LCD(uint8_t colonna, uint8_t riga, const char* TXT) {
  char riga_pulita[21]; // Buffer locale di sicurezza (20 char + terminatore)
  snprintf(riga_pulita, sizeof(riga_pulita), "%-20s", TXT); // formatta a sx  riempiendo con spazi

  // COMMENTATO RIGHE SOTTO PER VEDERE DI RISOLVERE IL PROBLEMA DELLA LENTEZZA DEL FW - ok !
  // lcd.setCursor(colonna, riga);
  // lcd.print(riga_pulita);
  // COMMENTATO RIGHE SOPRA PER VEDERE DI RISOLVERE IL PROBLEMA DELLA LENTEZZA DEL FW

  if (Serial) { Serial.println(riga_pulita); }
}


// FUNZIONE MOSTRA SCHERMATA ZONA (Display LCD 20x4)
// Passi il numero della zona (es. 1, 2 o 3) e lui aggiorna il display con i dati freschi
// integrazione della MOSTRA_DATI_SISTEMA() 
void SCHERMATA_ZONA(uint8_t zona_idx) {
    lcd.clear();   
    // riga 0 - nome zona, da impostare a mano ! NB max 20 caratteri totali
    if      (zona_idx == 1) { strcpy_P(W_BUFF, PSTR("ZONA 1: ZUCCHINE")); }
    else if (zona_idx == 2) { strcpy_P(W_BUFF, PSTR("ZONA 2: CUDUMARU")); } 
    else if (zona_idx == 3) { strcpy_P(W_BUFF, PSTR("ZONA 3: VERDURA") ); }
    else                    { strcpy_P(W_BUFF, PSTR("ZONA x: N.D.")   ); }
    SCRIVI_LCD(0, 0, W_BUFF);
    // riga 1 - umidita e soglia specifica
    // es. output "HR: 70(50) => LIMITE" con stato piu lungo da scrivere esattamente 20 caratteri
    const char* stato_str = "N.D."; // parto nettendo un Not Define....
    if      (ZONA[zona_idx].STORICO_UMIDITA[0] >= ZONA[zona_idx].SOGLIA + 5) { stato_str = "OK";     } // poi cambio...
    else if (ZONA[zona_idx].STORICO_UMIDITA[0] >= ZONA[zona_idx].SOGLIA)     { stato_str = "LIMITE"; }
    else                                                                     { stato_str = "SECCO";  }
    snprintf_P(W_BUFF, sizeof(W_BUFF), PSTR("HR: %2d(%2d) => %s"), ZONA[zona_idx].STORICO_UMIDITA[0], ZONA[zona_idx].SOGLIA, stato_str);
    SCRIVI_LCD(0, 1, W_BUFF);
    // riga 2 - storico -3h, -6h, -10h, -15h (valori interi formattati a 2 caratteri, ogni ora => 2 misure)
    // CORRETTO: aggiunte le virgole separate per passare correttamente i 4 argomenti a snprintf
    snprintf_P(W_BUFF, sizeof(W_BUFF), PSTR("STORICO:%2d-%2d-%2d-%2d"), 
              ZONA[zona_idx].STORICO_UMIDITA[6],
              ZONA[zona_idx].STORICO_UMIDITA[12],
              ZONA[zona_idx].STORICO_UMIDITA[20], 
              ZONA[zona_idx].STORICO_UMIDITA[30]);
    SCRIVI_LCD(0, 2, W_BUFF);
    // riga 3 - utilizzo acqua: ultima irrigazione / litri totali
    // vedere se togliere il "4" dalla formattazione testo.... forse non bello da vedere- cosi tot 17 caratteri
    snprintf_P(W_BUFF, sizeof(W_BUFF), PSTR("ACQUA: %2u/%4u LT"), ZONA[zona_idx].LITRI_ULTIMA_IRRIGAZIONE, ZONA[zona_idx].LITRI_EROGATI_TOTALI);
    SCRIVI_LCD(0, 3, W_BUFF);
}


// MOSTRA DATI SISTEMA : genera in output la visualizzazione dei dati del sistema a lcd suddivisa in pagine tematiche
// su lcd vengono messi i dati più importanti, non tutti. per la visualizzazione completa di tutti i dati collegarsi
// alla parte web e controllare i grafici (ancora da fare !!!) Le pagine vengono cambiate ed agguirnate ogni 7 secondi,
// automaticamente, da valutare se questo tempo sia sifficiente oppure troppo.
void MOSTRA_DATI_SISTEMA() {
  // Variabili che MANTENGONO il valore tra un giro del loop e l'altro
  static uint8_t paginaCorrente = 1;
  static unsigned long timerPagina = 0;
  static bool paginaDisegnata = false; 
  const uint8_t TOTALE_PAGINE = 6; // Imposta quante pagine sono da visualizzare
  // --- DISEGNO DELLA PAGINA (Eseguito UNA SOLA VOLTA per pagina) ---
  if (!paginaDisegnata) {
    lcd.clear();
    switch (paginaCorrente) {
      case 1: {
        // prima schermata -- versioni e sistema
        snprintf_P(W_BUFF, sizeof(W_BUFF), PSTR("ORTO'27-WEB v%s"), VERSIONE); SCRIVI_LCD(0, 0, W_BUFF);  
        unsigned long t = millis()/1000UL; // t in secondi
        snprintf_P(W_BUFF, sizeof(W_BUFF), PSTR("ATTIVO DA %02dg %02d:%02d"), t/86400UL, (t%86400UL)/3600, (t%3600)/60);
        SCRIVI_LCD (0,1, W_BUFF);
        // OPERATORE TERNARIO per scegliere al volo cosa mettere nella stringa in base all' ACQUA.STATO_CISTERNA
        strcpy_P(W_BUFF, ACQUA.STATO_CISTERNA ? PSTR("ACQUA: OK") : PSTR("ACQUA: ERRORE"));
        SCRIVI_LCD(0, 2, W_BUFF);
        strcpy_P(W_BUFF, ACQUA.STATO_CISTERNA ? PSTR("SISTEMA: ATTIVATO") : PSTR("SISTEMA: DISATTIVATO"));
        SCRIVI_LCD(0, 3, W_BUFF);
        break;
      } // Chiusura graffa case 1
      case 2: {
        // seconda schermata : servizi web, opzioni di visualizzazione con OPRETATORE TERNARIO
        strcpy_P(W_BUFF, PSTR("SERVIZIO WEB")); SCRIVI_LCD(0, 0, W_BUFF);
        if (WifiOK) {  
          // operatore ternario per gestire al volo il tipo di connessione
          strcpy_P(W_BUFF, WIFINET_AP ? PSTR("CONNESSO: A.P.") : PSTR("CONNESSO: STATION "));
          SCRIVI_LCD(0, 1, W_BUFF);
          strcpy_P(W_BUFF, WIFINET_AP ? PSTR("IP: 192.168.4.1") : PSTR("IP: 192.168.1.247"));
          SCRIVI_LCD(0, 2, W_BUFF);
        }
        else { 
          strcpy_P(W_BUFF, PSTR("SCHEDA ASSENTE"));    SCRIVI_LCD(0, 1, W_BUFF);
          strcpy_P(W_BUFF, PSTR("O NON FUNZIONANTE")); SCRIVI_LCD(0, 2, W_BUFF);
        }
        // operatore ternario per gestire al volo la presenza del client
        strcpy_P (W_BUFF, CLIENT_PRESENTE ?  PSTR("CLIENT CONNESSO") : PSTR("CLIENT ASSENTE"));
        SCRIVI_LCD(0, 3, W_BUFF);
        break;
      } // Chiusura graffa case 2
      case 3: {
        // Terza schermata: tempistiche controlli
        strcpy_P(W_BUFF, PSTR("PROSSIMI CONTROLLI:")); SCRIVI_LCD(0, 0, W_BUFF);
        // --- 1. LETTURE (Usiamo 'diff' e 'tra') come variabili di funzione
        unsigned long diff = millis() - timer_letture;
        unsigned long tra = (diff < INTERVALLO_LETTURE) ? (INTERVALLO_LETTURE - diff) / 1000UL : 0;
        snprintf_P(W_BUFF, sizeof(W_BUFF), PSTR("TRA: %02lu min %02lu sec"), (tra / 60), (tra % 60)); 
        SCRIVI_LCD(0, 1, W_BUFF);
        // --- 2. IRRIGAZIONE (Riusiamo le STESSE variabili senza crearne altre!) ---
        // OPERATORE TERNARIO : (condizione)?(se è vera fai questo):(altrimenti questo); (in if senza usare if)
        diff = millis() - timer_irrigazione;
        // operatore ternario per gestire i valori in modo che non vengano negativi
        tra = (diff < INTERVALLO_IRRIGAZIONI) ? (INTERVALLO_IRRIGAZIONI - diff) / 1000UL : 0;
        snprintf_P(W_BUFF, sizeof(W_BUFF), PSTR("IRRIGAZIONE %01lu:%02lu:%02lu"), (tra / 3600), ((tra % 3600) / 60), (tra % 60));
        SCRIVI_LCD(0, 2, W_BUFF);
        break; 
      } // Chiusura graffa case 3  
      // le ultime pagine gestiscono le zone (usando la funzione sopra), le metto tutte una dietro l'altra.   
      case 4: SCHERMATA_ZONA(1); break;
      case 5: SCHERMATA_ZONA(2); break;
      case 6: SCHERMATA_ZONA(3); break;
    }
    timerPagina = millis();   // Fai partire il cronometro visualizzazione pagina
    paginaDisegnata = true;   // Dice ad Arduino: "Ho scritto su LCD, ora fermati e aspetta!"
  }
  // --- TIMER UNICO DI ATTESA E CAMBIO PAGINA ---
  if ((millis()-timerPagina)>=7000UL) {  // <-- RIMESSO IL MENO E AGGIUNTO UL
    paginaCorrente++;         // Passa alla pagina successiva (+1)
    paginaDisegnata=false;  // Sblocca il disegno per la nuova pagina!
    // SE ABBIAMO FINITO TUTTE LE PAGINE: reset variabili e reinizio...
    if (paginaCorrente > TOTALE_PAGINE) {
      paginaCorrente = 1;     // Resetta la prima pagina per la prossima volta
      paginaDisegnata=false;
    }
  }
}

// GESTIONE PULSANTE
// funzione che gestisce la pressione del pulsante attivando le funzioni aggiuntive tramite flag
void GESTIONE_PULSANTE() {
  static bool           pulsantePremuto = false;
  static unsigned long  inizioPressione = 0;
  static uint8_t        scritto = 0;  // Spostata qui per renderla visibile a tutto il blocco pulsante
  // associa al bool il valore del confronto fra il digitalread ed il valore high...
  // se la lettura è HIGH è true (1) altrimenti false
  bool statoAttuale = (digitalRead(PULSANTE) == HIGH);  // PULL-DOWN
  // PRESSIONE (Fronte di salita)
  // NOTA: equivale a (statoAttuale == true && pulsantePremuto == false) ==>> pressione pulsante
  if (statoAttuale && !pulsantePremuto) {
    pulsantePremuto = true;
    inizioPressione = millis();
    lcd.clear(); strcpy_P(W_BUFF, PSTR("PULSANTE PREMUTO")); SCRIVI_LCD(0, 0, W_BUFF);
  }
  // MENTRE TIENI PREMUTO (Aggiornamento dinamico display)
  // NOTA: equivale a (statoAttuale == true && pulsantePremuto == true) ==>> mantenimento pressione pulsante
  if (statoAttuale && pulsantePremuto) {
    unsigned long durata = millis() - inizioPressione;
    // uso un uint8_t come contatore, parto con scritto=0, poi arrivo a t_op1 con scritto <1
    // entro nell'if e metto subito scritto =1 (perciò non è piu < 0 ). facendo cosi scrivo una volta sola
    // in quanto l'if è vero una volta sola !!!!!!
    if (durata >= TEMPO_MAX && scritto < 4) {  // tempo massimo superato
      scritto = 4; lcd.clear();
      strcpy_P(W_BUFF, PSTR("T-MAX SUPERATO")); SCRIVI_LCD(0, 1, W_BUFF);
      strcpy_P(W_BUFF, PSTR("MOLLA PER RIPARTIRE")); SCRIVI_LCD(0, 2, W_BUFF);
      strcpy_P(W_BUFF, PSTR("SENZA ATTIVARE NULLA"));  SCRIVI_LCD(0, 3, W_BUFF);
    }
    else if (durata >= TEMPO_OP3 && scritto < 3) { // tempo per reboot
      scritto = 3; lcd.clear();
      strcpy_P(W_BUFF, PSTR("REBOOT SISTEMA")); SCRIVI_LCD(0, 1, W_BUFF);
    }
    else if (durata >= TEMPO_OP2 && scritto < 2) {  // tempo per irrigazione
      scritto = 2; lcd.clear(); 
      strcpy_P(W_BUFF, PSTR("IRRIGAZIONE FORZATA")); SCRIVI_LCD(0, 1, W_BUFF);
    } 
  }
  // RILASCIO (Fronte di discesa -> Esecuzione comando)
  // NOTA: equivale a (statoAttuale == false && pulsantePremuto == true) ==>> momento preciso del rilascio pulsante
  if (!statoAttuale && pulsantePremuto) {
    unsigned long durataTotale = millis() - inizioPressione;
    if (durataTotale >= TEMPO_OP3) { 
      // se hai scelto di riavviare, fallo subito senza bool !
      lcd.clear();SCRIVI_LCD(0, 1, "RIAVVIO SISTEMA ..."); SCRIVI_LCD(0, 2, " ATTENDERE ...");
      delay(1000); NVIC_SystemReset();   // Reset software nativo ARM Cortex-M4
    }  
    else if (durataTotale >= TEMPO_OP2) { 
      if (ACQUA.STATO_CISTERNA) { ATTIVA_IRRIGAZIONE_FORZATA = true; }  // Impostiamo il flag dedicato!
      else { strcpy_P(W_BUFF, PSTR(" SISTEMA IN BLOCCO")); SCRIVI_LCD(0, 1, W_BUFF); }
    }
    else if (durataTotale <  TEMPO_OP1) {  // attiva retroilluminazione lcd, senza flag 
      //ATTENZIONE : da riattivare quando colleghi l'lcd !!!!!!!!
      // lcd.backlight(); timer_retroilluminazione = millis(); // AZZERI IL TIMER: riparte il minuto da capo!
    }
    // Resetti finali
    pulsantePremuto = false;  // non piu
    scritto = 0;
  }
}



/* FUNZIONE CHE DIVIDE IL COMANDO ALLO SHIFT REGISTER IN 4 OTTETI DA INVIARE OGNIUNO AL SINGOLO CHIP
Ecco punto per punto di quello che fa il codice:
byteChip4 = (valore >> 24) & 0xFF; Prende i 32 bit, li fa scorrere a destra di 24 posizioni e isola i bit da 31 a 24 (l'ultimo chip della catena).
byteChip3 = (valore >> 16) & 0xFF; Fa scorrere di 16 posizioni e isola i bit da 23 a 16 (3° chip).
byteChip2 = (valore >> 8) & 0xFF;  Fa scorrere di 8 posizioni e isola i bit da 15 a 8 (2° chip).
byteChip1 = valore & 0xFF;         Prende direttamente i bit da 7 a 0 senza bisogno di shiftare (1° chip, quello direttamente collegato ad Arduino). 
La maschera finale ( &0xFF che è il numero 255 in hex >= 8 bit tutti a 1...) funziona come una maschera di photoshop: dove c'e'
lo 0 non passa nulla, dove c'e' l'1 passa il valore sopra, qualunque sia...
      10110110  (I tuoi 8 bit reali)
    & 11111111  (& 0xFF oppure & 255 oppure & 0b11111111)
      ----------
    = 10110110  <-- Tutti gli 8 bit passano perfetti e inalterati!

      10110110  (I tuoi 8 bit reali)
    & 10101010  (un & che non ha tutto 1)
      ----------
    = 10100010  <-- bit risultante!
Poi apre la comunicazione ed invia il tutto dall'ultimo al primo...
*/
void AggiornaShiftRegister(uint32_t valore) {
  // 1. Estraiamo i 4 singoli byte da 8 bit l'uno
  uint8_t byteChip4 = (valore >> 24) & 0xFF;    // I bit piu alti          (24..31) -> "high"
  uint8_t byteChip3 = (valore >> 16) & 0xFF;    // I bit medio alti        (23..16) -> "mid high"
  uint8_t byteChip2 = (valore >> 8)  & 0xFF;    // I bit medio bassi mezzo (15..8)  -> "mid low"
  uint8_t byteChip1 = valore & 0xFF;            // I bit più bassi       (7..0)   -> "Low"
  // 2. Inviamo i byte in sequenza
  digitalWrite(pinLatch, LOW);
  // non serve mettere nessun ritardo: il chip lavora a 100MHz mentre arduino lavora a 100kHz, infinitamente piu lento
  //shiftOut(pinData, pinClock, MSBFIRST, byteChip4); // Spinto fino al 4° chip, commenta la riga se hai solo 3 chip !
  shiftOut(pinData, pinClock, MSBFIRST, byteChip3); // Spinto fino al 3° chip
  shiftOut(pinData, pinClock, MSBFIRST, byteChip2); // Finisce nel 2° chip
  shiftOut(pinData, pinClock, MSBFIRST, byteChip1); // Resta nel 1° chip
  // Applica le modifiche
  digitalWrite(pinLatch, HIGH); // Applica tutte le uscite insieme
}

