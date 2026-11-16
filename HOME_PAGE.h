const char HOME_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="it">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Orto 2027 - Controllo Remoto</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #eef3f7;
      margin: 20px;
    }

    /* Contenitore principale centrato */
    .container {
      max-width: 600px;
      margin: 0 auto;
    }

    /* Stile base per i vari riquadri (header e sezioni dati) */
    .box {
      background-color: #ffffff;
      border-radius: 10px;
      padding: 20px;
      margin-bottom: 20px;
      box-shadow: 0 2px 5px rgba(0,0,0,0.1);
      text-align: center;
    }

    /* Allineamento orizzontale tra SVG e Titolo */
    .header {
      display: flex;
      align-items: center;
      gap: 20px;
    }

    .header-titolo {
      text-align: center;
      flex-grow: 1;
      color: #00796b;
    }

    .header-titolo h1 {
      margin: 0 0 8px 0;
      font-size: 22px;
    }

    .header-titolo p {
      margin: 4px 0;
      font-weight: bold;
      font-size: 14px;
    }

    .tabella-centrata {
      margin: 0 auto;
    }

    .tabella-centrata td, .tabella-centrata th {
      text-align: center;
    }

    /* MODALE A SCOMPARSA BLOCCANTE */
    .modal-overlay {
      /* Modalita overlay gestita da Arduino (%STATO_MODALE_WW%), se bagna => flex altrimenti => none */
      display: %STATO_MODALE_WW%;
      position: fixed;
      top: 0;
      left: 0;
      width: 100vw;
      height: 100vh;
      background-color: rgba(245, 247, 250, 0.85);
      z-index: 9999;
      justify-content: center;
      align-items: center;
      box-sizing: border-box;
    }

    .modal-box {
      background: white;
      padding: 25px;
      border-radius: 10px;
      box-shadow: 0 8px 24px rgba(0,0,0,0.8);
      text-align: center;
      max-width: 320px;
      box-sizing: border-box;
    }
  </style>
</head>
<body>

  <div class="container">
    <!-- RIQUADRO INTESTAZIONE (SVG a sx + Titolo a dx) -->
    <div class="box header">
      <svg class="header-logo" width="100" height="100" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 120 120">
        <g fill="none" stroke-linecap="round" stroke-linejoin="round">
          <!-- Goccia d'acqua (Blu) -->
          <g stroke="#0288D1">
            <path d="M 60,4.8 C 60,4.8 24,43.2 24,72 A 36,36 0 0,0 84,100.8" stroke-width="1.92" />
            <path d="M 55.2,15.6 C 55.2,15.6 31.2,48 31.2,72 A 28.8,28.8 0 0,0 52.8,98.88" stroke-width="1.44" />
            <!-- Riflesso interno -->
            <path d="M 48,31.2 C 40.8,48 36,60 36,72 C 36,79.2 38.4,84 40.8,86.4 C 38.4,81.6 38.4,74.4 43.2,60 C 46.8,49.2 
            51.6,38.4 48,31.2 Z" fill="#7FE3FE" stroke="none" />
          </g>
          <!-- Ingranaggio (Colore Mattone) -->
          <g stroke="#D2691E">
            <path d="M 75.6,27.6 L 81.6,27.6 L 82.8,33.6 L 87.6,35.52 L 92.4,31.2 L 97.2,34.8 L 95.52,40.8 L 99.6,44.4
             L 105.6,43.2 L 108,49.2 L 103.2,54 L 105.6,58.8 L 111.6,61.2 L 111.6,68.4 L 105.6,70.8 L 103.2,75.6 L 108,80.4
              L 105.6,86.4 L 99.6,85.2 L 95.52,88.8 L 97.2,94.8 L 92.4,98.4 L 87.6,94.08 L 82.8,96 L 81.6,102 L 75.6,102" stroke-width="1.92" />
            <path d="M 75.6,34.8 L 79.68,34.8 L 80.64,39.12 L 84.48,40.8 L 87.84,37.44 L 91.2,40.08 L 90,44.4 L 93.12,47.04
             L 97.44,46.08 L 99.12,50.4 L 95.52,54 L 97.44,57.6 L 101.76,59.52 L 101.76,64.8 L 97.44,66.72 L 95.52,70.32
              L 99.12,73.92 L 97.44,78.24 L 93.12,77.28 L 90,79.92 L 91.2,84.24 L 87.84,86.88 L 84.48,83.52 L 80.64,85.2
               L 79.68,89.52 L 75.6,89.52" stroke-width="1.2" />
          </g>
          <!-- Profilo del terreno (Marrone) -->
          <g stroke="#6D4C41">
            <path d="M 28.8,105.6 Q 60,86.4 91.2,105.6" stroke-width="2.4" />
            <path d="M 33.6,111.6 Q 60,93.6 86.4,111.6" stroke-width="1.92" />
          </g>
          <!-- Pianta e Foglie (Verde) -->
          <g stroke="#2E7D32">
            <path d="M 60,93.6 C 50.4,74.4 50.4,50.4 60,36 C 69.6,50.4 69.6,74.4 60,93.6 Z" stroke-width="1.92" />
            <path d="M 60,93.6 C 54,76.8 54,52.8 60,40.8 C 66,52.8 66,76.8 60,93.6 Z" stroke-width="1.2" />
            <line x1="60" y1="93.6" x2="60" y2="38.4" stroke-width="1.44" />
            <path d="M 56.4,86.4 C 38.4,84 28.8,69.6 28.8,64.8 C 36,60 52.8,64.8 57.6,79.2" stroke-width="1.92" />
            <path d="M 55.2,84 C 40.8,81.6 32.4,69.6 32.4,66 C 38.4,62.4 51.6,67.2 55.68,78" stroke-width="1.2" />
            <path d="M 28.8,64.8 C 40.8,68.4 50.4,75.6 56.4,86.4" stroke-width="1.2" />
            <path d="M 63.6,86.4 C 81.6,84 91.2,69.6 91.2,64.8 C 84,60 67.2,64.8 62.4,79.2" stroke-width="1.92" />
            <path d="M 64.8,84 C 79.2,81.6 87.6,69.6 87.6,66 C 81.6,62.4 68.4,67.2 64.32,78" stroke-width="1.2" />
            <path d="M 91.2,64.8 C 79.2,68.4 69.6,75.6 63.6,86.4" stroke-width="1.2" />
          </g>
        </g>
      </svg>
      <div class="header-titolo">
        <h1>ORTOMATIC 2027</h1>
        <p>Versione %VERSIONE_WW% - WIFI 📡</p>
        <p>Automazione Irrigazione Orto</p>
      </div>
    </div>

    <!-- SEZIONE 1: METEO -->
    <div class="box">
      <h3>🌤️ STORICO METEO 🌤️</h3>
      <table class="tabella-centrata" border="0" cellpadding="6" cellspacing="0">
        <tr>
          <td>&nbsp;</td><td>Attuale</td><td>-2h</td>
          <td>-5h</td><td>-10h</td><td>-12h</td>
        </tr>
        <tr>
          <td>Temperatura</td>
          <td><b>%TEMP_WW_0%</b></td>
          <td>%TEMP_WW_4%</td>
          <td>%TEMP_WW_10%</td>
          <td>%TEMP_WW_20%</td>
          <td>%TEMP_WW_24%</td>
        </tr>
        <tr>
          <td>Umidità</td>
          <td><b>%UMID_WW_0% &#37;</b></td>
          <td>%UMID_WW_4%</td>
          <td>%UMID_WW_10%</td>
          <td>%UMID_WW_20%</td>
          <td>%UMID_WW_24%</td>
        </tr>
      </table>
      <p>Errori nelle letture : %ERR_METEO%</p>
      <details>
        <summary>📈&nbsp;&nbsp; Andamento Meteo</summary>
        <svg width="320" height="100" viewBox="0 0 540 100" style="background: #fafafa; border-radius: 10px; border: 1.5px dotted #888;">
          <!-- Linee orizzontali di griglia (comuni a entrambi i dati) ed etichette sx e dx -->
          <line x1="0" y1="20" x2="540" y2="20" stroke="#ccc" stroke-dasharray="17" stroke-width="1.25"/>
          <text x="8" y="17" font-size="10" fill="#2196f3" font-family="sans-serif">40°</text>
          <text x="532" y="17" font-size="10" fill="#009688" font-family="sans-serif" text-anchor="end">90&#37;</text>
          <line x1="0" y1="40" x2="540" y2="40" stroke="#ccc" stroke-dasharray="17" stroke-width="1.25"/>
          <text x="8" y="37" font-size="10" fill="#2196f3" font-family="sans-serif">30°</text>
          <text x="532" y="37" font-size="10" fill="#009688" font-family="sans-serif" text-anchor="end">70&#37;</text>
          <line x1="0" y1="60" x2="540" y2="60" stroke="#ccc" stroke-dasharray="17" stroke-width="1.25"/>
          <text x="8" y="57" font-size="10" fill="#2196f3" font-family="sans-serif">20°</text>
          <text x="532" y="57" font-size="10" fill="#009688" font-family="sans-serif" text-anchor="end">50&#37;</text>
          <line x1="0" y1="80" x2="540" y2="80" stroke="#ccc" stroke-dasharray="17" stroke-width="1.25"/>
          <text x="8" y="77" font-size="10" fill="#2196f3" font-family="sans-serif">10°</text>
          <text x="532" y="77" font-size="10" fill="#009688" font-family="sans-serif" text-anchor="end">30&#37;</text>
          <!-- qui inserisco le polyline generate dinamicamente -->
          %GRAFICO_METEO_WW%
        </svg>
      </details>
    </div>

    <!-- SEZIONE 2: CISTERNA -->
    <div class="box">
      <h3>💧💧&nbsp;&nbsp;GESTIONE ACQUA&nbsp;&nbsp;💧💧</h3>
      <table class="tabella-centrata" border="0" cellpadding="6" cellspacing="0">
        <tr>
          <td>Cisterna: <b>%STATO_CISTERNA_WW%</b></td>
          <td>Restanti: <b>%LITRI_CISTERNA_WW%</b> litri (stima)</td>
        </tr>
      </table>
      <table class="tabella-centrata" border="0" cellpadding="6" cellspacing="0">
        <tr>
          <td><b>Zona</b></td><td><b>Ultima Irr.</b></td>
          <td><b>Litri Tot.</b></td><td><b>N. Irr.</b></td>
        </tr>
        <tr>
          <td><b>Zucchine</b></td><td><b>%LT_ULT_IRR_Z1_WW%</b></td>
          <td><b>%LT_TOT_IRR_Z1_WW%</b></td><td><b>%N_IRR_Z1_WW%</b></td>
        </tr>
        <tr>
          <td><b>Pomodori</b></td><td><b>%LT_ULT_IRR_Z2_WW%</b></td>
          <td><b>%LT_TOT_IRR_Z2_WW%</b></td><td><b>%N_IRR_Z2_WW%</b></td>
        </tr>
        <tr>
          <td><b>Verdura</b></td><td><b>%LT_ULT_IRR_Z3_WW%</b></td>
          <td><b>%LT_TOT_IRR_Z3_WW%</b></td><td><b>%N_IRR_Z3_WW%</b></td>
        </tr>
        <tr>
          <td colspan="4">
            <details><summary> 💡 Info Acqua e Riavvio </summary>
            <p>
            Se appare la dicitura <b>Acqua Finita</b>, il sistema è entrato in un "blocco"<br>
            nel quale la parte operativa (irrigazione) è bloccata per evitare la rottura e<br>
            la cavitazione della pompa. Se dovesse capitare che vada in blocco durante una<br>
            irrigazione, il sistema si mette in sicurezza da solo bloccando la pompa e <br>
            chiudendo le valvole. Per ristabilire la modalità ordinaria è richiesto la<br>
            presenza di un operatore che riempia la cisterna e poi prema il pulsante di riavvio.<br>
            Il sistema ripartirà dal punto dove si era fermato rileggendo prima di tutto il<br>
            livello dell'acqua in cisterna.
            </p>
            </details>
          </td>
        </tr>
      </table>
    </div>

    <!-- SEZIONE 3: ZONE -->
    <div class="box">
      <h3>🥗🌿&nbsp;&nbsp;STATO DELLE ZONE&nbsp;&nbsp;🌿🥗</h3>
      <table class="tabella-centrata" border="1" cellpadding="6" cellspacing="0">
        <tr>
          <td>&nbsp;</td><td>Umidità</td><td>Stato</td><td>Misure</td>
        </tr>
        <tr>
          <!-- zona1 nome ZUCCHINE -->
          <td>Zucchine</td><td>%UMIDITA_Z1_WW%</td><td>%STATO_Z1_WW%</td><td>%N_MIS_Z1_WW%</td>
        </tr>
        <tr>
          <!-- zona2 nome Cudumaru -->
          <td>Cudumaru</td><td>%UMIDITA_Z2_WW%</td><td>%STATO_Z2_WW%</td><td>%N_MIS_Z2_WW%</td>
        </tr>
        <tr>
          <!-- zona3 nome Verdure -->
          <td>Verdure</td><td>%UMIDITA_Z3_WW%</td><td>%STATO_Z3_WW%</td><td>%N_MIS_Z3_WW%</td>
        </tr>
      </table>
      <details>
        <summary>📈&nbsp;&nbsp; Storico Terreno e Grafico</summary>
        <br><!-- grafico aree dinamico %GRAFICO_AREE_WW% --><br>
        <svg width="320" height="100" viewBox="0 0 540 100" style="background: #fafafa; border-radius: 10px; border: 1.5px dotted #888;">
          <line x1="0" y1="50" x2="540" y2="50" stroke="#888" stroke-dasharray="17" stroke-width="1.25"/>
          <text x="5" y="47" fill="#888" font-size="9" font-family="sans-serif">Limite</text>
          <!-- qui inserisco le polyline generate dinamicamente -->
          %GRAFICO_AREE_WW%
        </svg>
        <p>Storico (circa 30 ore) dati memorizzati (grafico).<br>
           Il tratteggio indica il limite secco/bagnato.<br>
           Le varie zone hanno lo stesso colore usato nella tabella sopra. 
        </p>
      </details>
      <br>
      <a href="./bagna" style="display:inline-block;padding:12px 24px;background:#2e7d32;color:#fff;text-decoration:none;font-weight:bold;border-radius:8px;">
      AVVIA IRRIGAZIONE FORZATA</a>
    </div>

    <!-- SEZIONE 4: INFO E CHIUSURA PAGINA -->
    <div class="box">
      <center>
      <table class="tabella-centrata" border="0" cellpadding="6" cellspacing="0">
        <tr><td>🌿&nbsp; INFO -FW- ORTOMATIC 2027 &nbsp;🌿</td></tr>
        <tr><td>Versione SW: <strong>%VERSIONE_WW%</strong><br>
        <span>Acceso da: <strong>%UPTIME_WW%</strong></span></td></tr>
        <tr><td><span style="font-size: 15px; color: #aaa;">
        &copy; 2027 MIK. Tutti i diritti riservati. <br>
        Proprietà intellettuale Mik ed Edo</span></td></tr>
        <tr><td style="background-color: #f9f9f9; border: 1px solid #ccc;">
          <div style="display: inline-flex; align-items: center; gap: 8px;">
            <svg width="53" height="28" viewBox="0 0 100 48" xmlns="http://www.w3.org/2000/svg">
              <path d="M24,8 C13,8 6,15 6,24 C6,33 13,40 24,40 C33,40 41,32 50,24 C59,16 67,8 76,8 C87,8 94,15 94,24 C94,33 87,40 76,40 C67,40 
              59,32 50,24 C41,16 33,8 24,8 Z" fill="none" stroke="#333" stroke-width="7"/>
              <line x1="18" y1="24" x2="30" y2="24" stroke="#333" stroke-width="5" stroke-linecap="round"/>
              <line x1="70" y1="24" x2="82" y2="24" stroke="#333" stroke-width="5" stroke-linecap="round"/>
              <line x1="76" y1="18" x2="76" y2="30" stroke="#333" stroke-width="5" stroke-linecap="round"/>
            </svg>
            <span>Powered by <strong>Arduino</strong></span>
          </div>
        </td></tr>
      </table>
      </center>
    </div>

    <!-- FINESTRA MODALE IRRIGAZIONE IN ATTO (Bloccante e centrata) -->
    <div id="modalIrrigazione" class="modal-overlay">
      <div class="modal-box">
        <center>
        <svg width="100" height="100" viewBox="0 0 100 100" xmlns="http://www.w3.org/2000/svg">
          <polygon points="50,8 94,86 6,86" fill="#ffeb3b" stroke="#d32f2f" stroke-width="8" stroke-linejoin="round"/>
          <circle cx="43" cy="44" r="4.5" fill="#212121"/>
          <path d="M41,51 L37,65 L32,76 M41,51 L46,64 L49,76" stroke="#212121" stroke-width="4.5" stroke-linecap="round" stroke-linejoin="round" fill="none"/>
          <path d="M41,53 L52,58 L59,54" stroke="#212121" stroke-width="4" stroke-linecap="round" stroke-linejoin="round" fill="none"/>
          <line x1="47" y1="50" x2="62" y2="71" stroke="#212121" stroke-width="3" stroke-linecap="round"/>
          <path d="M55,77 Q65,64 76,77 Z" fill="#212121"/>
        </svg>
        <h3 style="color: #2e7d32; margin-top: 0; margin-bottom: 10px;"><b>IRRIGAZIONE IN ATTO</b></h3>
        <div>⛔&nbsp;&nbsp;&nbsp;💧🚿💧&nbsp;&nbsp;&nbsp;⛔</div>
        <p style="color: #555; font-size: 14px; margin-bottom: 0;">Il sistema sta bagnando le zone dell'orto in modalità automatica o forzata.<br>
        <strong>Attendere il completamento del ciclo.<br><font color="darkred">La pagina ritornerà operativa automaticamente.</font></strong>
        </p>
        </center>
      </div>
    </div>

  <!-- chiusura container -->
  </div>
</body>
</html>
)rawliteral";