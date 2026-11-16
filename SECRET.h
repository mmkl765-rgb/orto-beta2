#include "api/IPAddress.h"
// FILE CONTENENTE LE VARIE CONFIGURAZIONI DELLE RETI

  // nei test impostando ssid/pwd del router di casa si è connesso al primo tentativo e la pagina funziona
  const char ssid_router[] = "-VDP-36-";       // al momento non arriva il segnale
  const char pass_router[] = "157@6638!!";     // al momento non arriva il segnale
  const char ssid_AP[]     = "Rete_Orto_AP";   // nome della rete dell'orto in AP
  const char pass_AP[]     = "12345678";       // pwd rete in AP (almeno 8 caratteri) (metti una + difficile poi !!!!)

  // --- IMPOSTAZIONI IP FISSO PER ROUTER (station STA) ---
  IPAddress sta_IP(192, 168, 1, 247);      // IP fisso che vuoi dare ad Arduino
  IPAddress sta_gateway(192, 168, 1, 1);   // IP del tuo router di casa
  IPAddress sta_subnet(255, 255, 255, 0);  // Subnet mask standard
  IPAddress sta_dns(192, 168, 1, 1);       // Server DNS (pari al router)

  // --- IMPOSTAZIONI PER L'ACCESS POINT (AP) ---
  IPAddress ap_IP(192, 168, 4, 1);
  IPAddress ap_gateway(192, 168, 4, 1);
  IPAddress ap_subnet(255, 255, 255, 0);
  IPAddress ap_Dns(192, 168, 4, 1);