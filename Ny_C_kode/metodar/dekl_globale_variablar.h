//
// Fil: dekl_globale_variablar.h
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
volatile uint8_t gyldig_trykk_av_USERbrytar = 0; // For å unngå at optimaliseringa fjernar tilhøyrande kode
uint8_t tilstand = 1;

// For realisering av avprelling av brytar i programvare
//------------------------------------------------------
uint8_t brytar_var_nettopp_slept = 1;
uint8_t nedtrykt_er_naa_USERbrytar = 0;
uint8_t sleppteljar_USERbrytar = 0;
uint8_t klar_for_nytt_trykk_av_USERbrytar = 1;
uint8_t trykkteljar_USERbrytar = 0;


//////////////////////////////////////////////////////////////////////////////////////
uint32_t telling_avbrot = 0;

int32_t avstand_raa = 0;
int32_t avstand_filter_1;
int32_t avstand_filter_2 = 0;



//////////////////////////////////////////////////////////////////////////////////////


// For realisering av brytarsjekk mm i SysTick-metoden
//------------------------------------------------------
uint8_t  tikkteljar_avprelling = 0;
uint16_t tikkteljar_diodar = 0;
volatile uint8_t oppdater_diodar = 0; // For å unngå at optimaliseringa fjernar tilhøyrande kode

volatile uint8_t send_ny_tidsserie = 0; // For å unngå at optimaliseringa fjernar tilhøyrande kode
uint16_t maaling_teljar = 1;            // Startar på måling nr.1
uint16_t testmaaling = 0;
uint16_t testteljar = 0;

int8_t teikn = 0x30;

uint8_t hex2ascii_tabell[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

// Eksempel på variabeloppsett for TIM2, kanal 2 som er sett opp som PWM-modul
//----------------------------------------------------

uint16_t PWM_periode = 99; //Gir periodetid på 100 teljingar
                           //dvs.100/72MHz = 1.39usek
                           // Forklaring i TIM_oppstart()
uint16_t PWM_vidde = 25; // Startverdi for på-tida til pulsbreiddesignalet,
                        //dvs. 25 teljingar som gir 25% PÅ-tid
uint16_t PWM_preskalering = 0;

// For realisering av sensoravlesing og tidsreferering av måledata
//----------------------------------------------------------------
uint16_t tikkteljar_sampling = 0;
uint8_t samplenr = 0;
volatile uint8_t ny_maaling = 0; // For å unngå at optimaliseringa fjernar tilhøyrande kode

int16_t loggeverdi = 0;
uint8_t diode_moenster = 0;
int8_t temperatur = 0;

// Akselerometerspesifikke variablar
//-------------------------------------------------------
uint8_t buffer[6];

int16_t a = 0;

int16_t a_x = 0;
int16_t a_y = 0;
int16_t a_z = 0;

int32_t a1 = 76;  //1.ordens filter basert på 100Hz sampling og
int32_t b1 = 24;  //nedsampling til 10Hz => bandbreidde pi*5Hz
int32_t a_xf_k;
int32_t a_xf_k_1 = 0; //Foerre filterverdi
int32_t a_yf_k;
int32_t a_yf_k_1 = 0; //Foerre filterverdi
int32_t a_zf_k;
int32_t a_zf_k_1 = 1000; //Foerre filterverdi, a_z startar ofte med +1g.

// For realisering av UART-kommunikasjon
//------------------------------------------------------
#define STX 0x02   // ASCII-koden for teiknet "Start of TeXt", meldingshovud her.
#define ETX 0x03   // ASCII-koden for teiknet "End of TeXt", meldingshale her.

//////////////////////////////////////////////////




uint8_t send_maalingar_til_loggar = 0;
uint8_t send_maaling = 0;
uint8_t legg_til_meldingshovud = 0;
uint8_t legg_til_meldingshale = 0;
volatile uint8_t tikkteljar_uartkomm = 0;
uint8_t send_ei_brytarmelding_via_uart = 0;
uint8_t  mottatt_data;
// Definisjonar og variablar for utskrift til LCD-skjerm
//-----------------------------------------------
#define LCD_Enable 0x8 //E=1
#define LCD_Disable 0x0 //E=0
#define LCD_Read 0x4 // R/*W=1
#define LCD_Write 0x0 // R/*W=0,
#define LCD_RS_data 0x2 // RS=1
#define LCD_RS_kommando 0x0 // RS=0

uint8_t skriv_til_LCD = 1;

