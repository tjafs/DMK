//
// Fil: extern_dekl_globale_variablar.h
// m.t.240615

// Globale variablar kan bare deklarerast ein plass i programmet.
// Denne fila kan inkluderast i feks. main-fila.
// I dei andre filene må ein då inkludera såkalla extern-deklarasjonane
// som ein finn i fila extern_dekl_globale_variablar.h
//---------------------------------------------------------------------

//---------------------------------------
// Globale variablar
//---------------------------------------

// For tilstandsmaskinen
//------------------------------------------------------
extern volatile uint8_t gyldig_trykk_av_USERbrytar;
extern uint8_t tilstand;

// For realisering av avprelling av brytar i programvare
//------------------------------------------------------
extern uint8_t brytar_var_nettopp_slept;
extern uint8_t nedtrykt_er_naa_USERbrytar;
extern uint8_t sleppteljar_USERbrytar;
extern uint8_t klar_for_nytt_trykk_av_USERbrytar;
extern uint8_t trykkteljar_USERbrytar;

// For realisering av brytarsjekk mm i SysTick-metoden
//------------------------------------------------------
extern uint8_t  tikkteljar_avprelling;
extern uint16_t tikkteljar_diodar;
extern volatile uint8_t oppdater_diodar;

extern volatile uint8_t send_ny_tidsserie; // For å unngå at optimaliseringa fjernar tilhøyrande kode
extern uint16_t maaling_teljar;
extern uint16_t testmaaling;
extern uint16_t testteljar;

extern int8_t teikn;
extern uint8_t hex2ascii_tabell[16];

// Eksempel på variabeloppsett for ein PWM-kanal
//----------------------------------------------------

extern uint16_t PWM_periode; //Sjå dekl-fila
extern uint16_t PWM_vidde;
extern uint16_t PWM_preskalering;

// For realisering av sensoravlesing og tidsreferering av måledata
//----------------------------------------------------------------
extern uint16_t tikkteljar_sampling;
extern uint8_t  samplenr;
extern volatile uint8_t ny_maaling;

extern int16_t loggeverdi;
extern uint8_t diode_moenster;
extern int8_t  temperatur;

// Akselerometerspesifikke variablar
//-------------------------------------------------------
extern uint8_t buffer[6];

extern int16_t a_x;
extern int16_t a_y;
extern int16_t a_z;

extern int32_t a1;  //1.ordens filter basert på 100Hz sampling og
extern int32_t b1;  //nedsampling til 10Hz => bandbreidde pi*5Hz
extern int32_t a_xf_k;
extern int32_t a_xf_k_1; //Foerre filterverdi
extern int32_t a_yf_k;
extern int32_t a_yf_k_1; //Foerre filterverdi
extern int32_t a_zf_k;
extern int32_t a_zf_k_1; //Foerre filterverdi

// For realisering av UART-kommunikasjon
//------------------------------------------------------
#define STX 0x02   // ASCII-koden for teiknet "Start of TeXt", meldingshovud her.
#define ETX 0x03   // ASCII-koden for teiknet "End of TeXt", meldingshale her.

extern uint8_t send_maalingar_til_loggar;
extern uint8_t send_maaling;
extern volatile uint8_t tikkteljar_uartkomm;
extern uint8_t legg_til_meldingshovud;
extern uint8_t legg_til_meldingshale;

extern uint8_t send_ei_brytarmelding_via_uart;
extern uint8_t  mottatt_data;
// Definisjonar og variablar for utskrift til LCD-skjerm
//-----------------------------------------------
#define LCD_Enable 0x8 //E=1
#define LCD_Disable 0x0 //E=0
#define LCD_Read 0x4 // R/*W=1
#define LCD_Write 0x0 // R/*W=0,
#define LCD_RS_data 0x2 // RS=1
#define LCD_RS_kommando 0x0 // RS=0

extern uint8_t skriv_til_LCD;



