// -----------------------------------------------------------
// Software:         GW302 - McHale V6 Fixed Chamber Round Baler
// C Source File:    functions.c
// Description:      Graphic Wizard Display Related File
// -----------------------------------------------------------


// Include Standard files
#include "Board.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"


#ifdef WINDOWS
 // function not available in borland builder
 float cosf( float x ) { return cos( x ); }
 float sinf( float x ) { return sin( x ); }
 float atoff( const char *s ) { return atof( s ); }
#endif

// global variables
// -----------------------------------------------------------
// constants
unsigned char const binHex[16] =
 { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
unsigned char const * const DayofWeek[7] =
 { "Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat" };
unsigned char const * const MonthofYear[13] =
 { "---", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
unsigned char const Store[10] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' };
unsigned char const UCkey[] = " ÀÁÂÃÄÅÆABCÇDEÈÉÊËFGHIÌÍÎÏÐJKLMNÑOÒÓÔÕÖØPQRSTUÙÚÛÜVWXYÝßZÞ";
unsigned char const LCkey[] = " aàáâãäåæbcçdeèéêëfghiìíîïðjklmnñoòóôõöøpqrstuùúûüvwxyýÿzþ";
unsigned char const NCkey[] = "0123456789.+-±!#¢£$";
/*    
    "!\"#$%&'()*+,-./:;<=>?@[\\]^_`"
    "{|}~ ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿"
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß"
    "àáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
*/
//unsigned char const * const RotateShaft[4] = { SeedshaftOff, SEEDSHAFT, SEEDSHAFT2, SEEDSHAFT1 };
unsigned char const * const ip_status[2] = { SWOPEN, SWCLOSED };
unsigned char const * const op_status[2] = { BUTTON04, BUTTON01 };


// Programmable Factors (Array)                                          

// Machine Menu                                                 Step size,      Minimum,      Maximum,      Default
unsigned char const PF_GW302_BaleProfile[PF_DIM]            = {         1,  BP_PRESET_A,  BP_PRESET_E,  BP_PRESET_A }; // Preset A to E
unsigned char const PF_GW302_BaleDiameter[PF_DIM]           = {         1,           60,          168,       PF_cNA }; // cm     -- Preset Default (char) -- Minimum Bale Diameter is limited to Current Core Diameter
unsigned char const PF_GW302_CoreDiameter[PF_DIM]           = {         1,           60,          130,       PF_cNA }; // cm     -- Preset Default (char)
unsigned char const PF_GW302_BaleDensity[PF_DIM]            = {         1,            1,           10,       PF_cNA }; // ID     -- Preset Default (char)
unsigned char const PF_GW302_CoreDensity[PF_DIM]            = {         1,            1,           10,       PF_cNA }; // ID     -- Preset Default (char)
float const         PF_GW302_NetLayers[PF_DIM]              = {       0.1,          1.5,          6.0,       PF_fNA }; // layers -- Preset Default (float)
unsigned long int const PF_GW302_BaleTotal[PF_DIM]          = {      1UL,          0UL,        9999UL,          0UL }; // Bales  -- Bale Totals (in Bale Stores A to J)
unsigned char const PF_GW302_BaleStore[PF_DIM]              = {        1,            0,  MAX_STORES-1,            0 }; // Bale Store ID: A - J
// LubeCounter      // Moved to preserved run-time totals   = PF_GW302_LubeAlarm[PF_DIM] for range of values with the exception that minimum can go negative
unsigned long int const PF_GW302_GrandBaleTotal[PF_DIM]     = {       1UL,          0UL,     999999UL,          0UL }; // Bales  -- Moved to early position in memory since value is to be preserved between successive software versions 
float const         PF_GW302_DiaPotZero[PF_DIM]             = {    PF_fNA,       PF_fNA,       PF_fNA,          1.0 }; // V      -- Value fixed -- 'zero' bale diameter voltage when baler door closed & no bale in chamber
signed char const   PF_GW302_DiameterCorrection[PF_DIM]     = {         1,          -20,          +20,            0 }; // cm
unsigned char const PF_GW302_BaleShapeIndicator[PF_DIM]     = {         1,      BSI_OFF,     BSI_BEEP,      BSI_OFF }; // Off/On/Beep
float const         PF_GW302_BaleShapeIndicatorZero[PF_DIM] = {    PF_fNA,          0.0,          9.0,          2.5 }; // V      -- 'zero' deviation from uniform cylinder shape
float const         PF_GW302_TotalNetMetres[PF_DIM]         = {    PF_fNA,          0.0,       9999.0,          0.0 }; // m      -- Incremented by NetMetresAppliedForCount; Display range = 0 - 9999 in increments of 1
unsigned char const PF_GW302_PreWarningBeep[PF_DIM]         = {         1,            1,          100,           90 }; // %
float const         PF_GW302_NetDelay[PF_DIM]               = {       0.1,          0.0,          5.0,          2.0 }; // s
unsigned char const PF_GW302_NetTwineOption[PF_DIM]         = {         1,       TO_NET,     TO_TWINE,        TO_NET}; // Off/On/Beep
  
// Technician Menu - Baler Full Page 1/2                        Step size,   Minimum,   Maximum,   Default
unsigned short int const PF_GW302_LubeAlarm[PF_DIM]         = {         1,         0,       999,       300 }; // Bales
float const              PF_GW302_PresDeadband[PF_DIM]      = {      0.01,      0.00,      9.99,      0.00 }; // V     -- Deadband on AnIP2
float const              PF_GW302_NetSpeed[PF_DIM]          = {       0.1,       0.0,      99.0,      50.0 }; // Hz    -- OP2 switches off if frequency on IP4 reaches this setting
float const              PF_GW302_PresSwDelay[PF_DIM]       = {       0.1,       0.0,       9.9,       0.2 }; // s     -- Delay between target signal on AnIP2 and OP8 reacting
float const              PF_GW302_DoorClosed[PF_DIM]        = {       0.1,       0.0,       9.9,       1.5 }; // s     -- Time IP3 must be closed for once the bale has been ejected
unsigned char const      PF_GW302_BaleSizeFactor[PF_DIM]    = {         1,       100,       250,       221 }; //       -- Factor to determine the maximum diameter voltage    

// Technician Menu - Baler Full Page 2/2                  Step size,   Minimum,   Maximum,   Default
float const         PF_GW302_FillPotRange[PF_DIM]     = {      0.01,      0.00,      9.99,      1.00 }; // V          -- Range of AnIP3
float const         PF_GW302_FillPotIgnore[PF_DIM]    = {      0.01,      0.00,      9.99,      0.25 }; // V          -- Ignore range of AnIP3
unsigned char const PF_GW302_EngageDiameter[PF_DIM]   = {         1,         0,       100,        95 }; // %          -- % of target bale diameter when OP5 switches on
unsigned char const PF_GW302_BaleIndicator[PF_DIM]    = {         1,  BI_STEER,   BI_FILL,  BI_STEER }; // Steer/Fill -- Inverts the direction of the driving arrows on the main screen 
float const         PF_GW302_RollerDelay[PF_DIM]      = {       0.1,       0.0,       9.9,       0.5 }; // s          -- Delay between OP5 and OP2 switching on if 'Net System' is set to 'Roller'
  
// Technician Menu - Netting Setup Page 1/2               Step size,   Minimum,   Maximum,   Default
float const         PF_GW302_NetOn[PF_DIM]            = {       0.1,       0.0,       9.9,       3.0 }; // s            -- Duration that OP2 switches on for (t2)
unsigned char const PF_GW302_NetCutPulse[PF_DIM]      = {         1,         0,        99,        40 }; // pulses       -- Used in the calculation of the target net pulses
float const         PF_GW302_NetCutTime[PF_DIM]       = {       0.1,       0.0,       9.9,       6.0 }; // s            -- Duration that OP7 switches on for to cut the net (t5)
float const         PF_GW302_MinDiaPulsPerLay[PF_DIM] = {       0.1,       0.0,     999.9,      38.0 }; // pulses/layer -- Net pulses per layer at minimum diameter setting
float const         PF_GW302_MaxDiaPulsPerLay[PF_DIM] = {       0.1,       0.0,     999.9,     108.0 }; // pulses/layer -- Net pulses per layer at maximum diameter setting
float const         PF_GW302_NetFactor[PF_DIM]        = {       0.1,       0.0,       9.9,       1.0 }; //              -- Used in the calculation of the target net pulses
float const         PF_GW302_NoNetPulse[PF_DIM]       = {       0.1,       0.0,       9.9,       4.0 }; // s            -- No net warning appears if there are no pulses on IP4 within this time
unsigned char const PF_GW302_PreNetPulse[PF_DIM]      = {         1,         0,        99,         2 }; // pulses       -- Target pulses on IP4 to pre-feed the net (n2)

// Technician Menu - Netting Setup Page 2/2               Step size,   Minimum,   Maximum,   Default
unsigned char const PF_GW302_NetSystem[PF_DIM]        = {         1,   NS_BEAK, NS_ROLLER, NS_ROLLER }; // Beak/Roller -- Type of netting system fitted to the machine  
unsigned char const PF_GW302_BeakEngage[PF_DIM]       = {         1,         0,        99,         6 }; // pulses      -- Number of pulses on IP4 before OP5 switches on (only if Net System is set to Beak)
unsigned char const PF_GW302_TwineOption[PF_DIM]      = {         1,   TO_OFF,  	TO_ON, 	  TO_OFF }; // Twine Option-- Type of netting system fitted to the machine

// Technician Menu - Diameter Setup                                 Step size,   Minimum,   Maximum,   Default
unsigned char const PF_GW302_DiameterMin[PF_DIM]                = {         1,         0,       100,         8 }; // % -- Target diameter at setting 1
unsigned char const PF_GW302_DiameterMax[PF_DIM]                = {         1,         0,       100,       100 }; // % -- Target diameter at setting 109
float const         PF_GW302_DiaCorrection[PF_DIM]              = {      0.01,      0.00,      0.20,      0.01 }; // V -- Amount the target diameter voltage changes for each 'Diameter correction' step
unsigned char const PF_GW302_BaleDiaSmoothingFactor[PF_DIM]     = {         1,         1,       100,        50 }; // % -- Bale Diameter (AnIP1) Smoothing Factor
unsigned char const PF_GW302_DensityPresSmoothingFactor[PF_DIM] = {         1,         1,       100,       100 }; // % -- (Bale) Density Pressure (AnIP2) Smoothing Factor
unsigned char const PF_GW302_BaleShapeSmoothingFactor[PF_DIM]   = {         1,         1,       100,        50 }; // % -- Bale Shape Indicator (AnIP3) Smoothing Factor
unsigned char const PF_GW302_KnifePresSmoothingFactor[PF_DIM]   = {         1,         1,       100,        50 }; // % -- Knife Pressure (AnIP4) Smoothing Factor
 
// Technician Menu - Density Setup Page 1/2    Step size,   Minimum,   Maximum,   Default
float const PF_GW302_Density1[PF_DIM]      = {      0.01,      0.00,     10.00,      0.00 }; // V -- Target voltage on AnIP2 if target density is 1 -- Equivalent Pressure:   0 Bar
float const PF_GW302_Density2[PF_DIM]      = {      0.01,      0.00,     10.00,      0.80 }; // V -- Target voltage on AnIP2 if target density is 2 -- Equivalent Pressure:  40 Bar
float const PF_GW302_Density3[PF_DIM]      = {      0.01,      0.00,     10.00,      1.20 }; // V -- Target voltage on AnIP2 if target density is 3 -- Equivalent Pressure:  60 Bar
float const PF_GW302_Density4[PF_DIM]      = {      0.01,      0.00,     10.00,      1.60 }; // V -- Target voltage on AnIP2 if target density is 4 -- Equivalent Pressure:  80 Bar
float const PF_GW302_Density5[PF_DIM]      = {      0.01,      0.00,     10.00,      2.00 }; // V -- Target voltage on AnIP2 if target density is 5 -- Equivalent Pressure: 100 Bar
float const PF_GW302_Density6[PF_DIM]      = {      0.01,      0.00,     10.00,      2.40 }; // V -- Target voltage on AnIP2 if target density is 6 -- Equivalent Pressure: 120 Bar

// Technician Menu - Density Setup Page 2/2    Step size,   Minimum,   Maximum,   Default
float const PF_GW302_Density7[PF_DIM]      = {      0.01,      0.00,     10.00,      2.80 }; // V -- Target voltage on AnIP2 if target density is 7  -- Equivalent Pressure: 140 Bar
float const PF_GW302_Density8[PF_DIM]      = {      0.01,      0.00,     10.00,      3.20 }; // V -- Target voltage on AnIP2 if target density is 8  -- Equivalent Pressure: 160 Bar
float const PF_GW302_Density9[PF_DIM]      = {      0.01,      0.00,     10.00,      3.60 }; // V -- Target voltage on AnIP2 if target density is 9  -- Equivalent Pressure: 180 Bar
float const PF_GW302_Density10[PF_DIM]     = {      0.01,      0.00,     10.00,      4.00 }; // V -- Target voltage on AnIP2 if target density is 10 -- Equivalent Pressure: 200 Bar

// Technician Menu - Knives Setup              Step size,   Minimum,   Maximum,   Default
float const PF_GW302_KnifePressure[PF_DIM] = {      0.01,      0.00,      9.99,      0.90 }; // V
float const PF_GW302_KnifePresWarn[PF_DIM] = {      0.01,      0.00,      9.99,      1.30 }; // V -- Knife pressure warning appears if AnIP4 reaches this value
float const PF_GW302_DiverterDelay[PF_DIM] = {       0.1,       0.0,       9.9,       0.2 }; // s -- Delay to expire while key pressed before turning OP on
  
// Technician Menu - Formation 2                            Step size,   Minimum,   Maximum,   Default
unsigned char const PF_GW302_FormationDensity[PF_DIM]   = {         1,         1,        10,    PF_cNA }; // ID -- Preset Default (char) -- Target density during the 'Formation' stage
unsigned char const PF_GW302_FormationDiameter[PF_DIM]  = {         1,         0,       100,    PF_cNA }; // %  -- Preset Default (char) -- Upper diameter limit of 'Formation' stage
unsigned char const PF_GW302_Formation2Density[PF_DIM]  = {         1,         1,        10,    PF_cNA }; // ID -- Preset Default (char) -- Target density during the 'Formation2' stage
unsigned char const PF_GW302_Formation2Diameter[PF_DIM] = {         1,         0,       100,    PF_cNA }; // %  -- Preset Default (char) -- Upper diameter limit of 'Formation2' stage

// Diagnostics - Digital Inputs                               Step size,      Minimum,     Maximum,      Default
unsigned char const PF_GW302_ChoppingKnivesFitted[PF_DIM] = {         1, CKF_DISABLED, CKF_ENABLED, CKF_DISABLED }; // Disabled/Enabled


// Bale Profile defaults                             Preset =     A    B    C    D    E
unsigned char const PresetCoreDiameter[BP_PRESET_MAX]       = {  70,  70,  70,  90,  90 }; // Core Diameter [cm]
unsigned char const PresetCoreDensity[BP_PRESET_MAX]        = {   9,   8,   7,   3,   1 }; // Core Density [ID]  
unsigned char const PresetBaleDiameter[BP_PRESET_MAX]       = { 125, 125, 125, 125, 125 }; // Bale Diameter [cm]
unsigned char const PresetBaleDensity[BP_PRESET_MAX]        = {  10,   9,   8,   4,   2 }; // Bale Density [ID]
unsigned char const PresetFormationDiameter[BP_PRESET_MAX]  = {   0,   0,   0,   0,   0 }; // Formation Diameter [%]
unsigned char const PresetFormationDensity[BP_PRESET_MAX]   = {  10,  10,  10,   5,   5 }; // Formation Density [ID] 
unsigned char const PresetFormation2Diameter[BP_PRESET_MAX] = {   0,   0,   0,   0,   0 }; // Formation 2 Diameter [%] 
unsigned char const PresetFormation2Density[BP_PRESET_MAX]  = {  10,  10,  10,   5,   5 }; // Formation 2 Density [ID] 
float const PresetNetLayers[BP_PRESET_MAX]                  = { 3.5, 3.0, 2.5, 4.2, 4.2 }; // Net Layers [layers] 


// Bale/Core Density ID     =    1    2    3    4    5    6    7    8    9    10
unsigned char const Bar[10] = {  0,  40,  60,  80, 100, 120, 140, 160, 180,  200 };


/*
struct PinWin {
  unsigned short int PinNumber;
  void (*PinWindow)(unsigned int);
  };
*/
struct PinWin const PinOption = {
       PINTECH1, screen141
};

// language string pointer
char const * const *ptText;
 

// Zeroed
unsigned char buffer[GRAPHICS_SIZE+128];
unsigned char adm[GRAPHICS_SIZE];
unsigned char startup, selection, escape, leaf, windowdepth, nextwindow;
unsigned char deftime, reverttime, looptime, offtime, half_second, AutoMan;
unsigned char swrel;
unsigned char DisableChoppingKnivesWarnings;
unsigned char ContinueOnKeyRelease;
unsigned char StartAutoNet;

float BaleDiaVolts;
float DensityPresVolts;
float BaleShapeVolts;
float KnifePresVolts;

float SupplyVolts;

unsigned char Kniv1_IPClosed;

unsigned int SeqOpOn, SeqOpOff, KeyOpOn, KeyOpOff;

float DiaTargetVolts;    // V

float DiaMinVolts;       // V
float DiaMaxVolts;       // V

float DiaMinExtDisplayPercent;    // 0.0 - 100.0 % of extended display range from PotZero to DiaMax
float DiaTargetExtDisplayPercent; // 0.0 - 100.0 % of extended display range from PotZero to DiaMax
float BaleDiaExtDisplayPercent;   // 0.0 - 100.0 % of extended display range from PotZero to DiaMax

float DiaFormationVolts;                  // V
float DiaFormation2Volts;                 // V
float DiaCoreVolts;                       // V
float DiaEngageVolts;                     // V
float DiaPreWarnVolts;                    // V

float DensityBaleVolts;                   // V
float DensityCoreVolts;                   // V
float DensityFormation2Volts;             // V
float DensityFormationVolts;              // V

unsigned char DensityPressureTooHigh;     // 0 / 1

void (*window)( unsigned int );

struct sdPreviousWindow PreviousWindow[MAX_WINDOWDEPTH];

unsigned int key;
unsigned char savecal, editing, valuechanged, number[16];
unsigned char SavecalDelayWithinMenus;
unsigned char SavecalDelayOnMainDisplay;

unsigned char ClockRam[16];
unsigned char RunFactorStatus, PowerSwitchedOff;
unsigned char buzz_count;

unsigned long int BaleDiaVoltsTotAccumMultSamples;     // V
unsigned long int DensityPresVoltsTotAccumMultSamples; // V
unsigned long int BaleShapeVoltsTotAccumMultSamples;   // V
unsigned long int KnifePresVoltsTotAccumMultSamples;   // V

unsigned int TC1_interrupt_count;
unsigned int EOC7_interrupt_count;
unsigned short int f0count, f6count;
unsigned int f0time, f6time;
unsigned char newf6; 
unsigned int TC1_millisec_count;
unsigned int buzz_length, off_length;
unsigned char AllTime;
unsigned int TC1_count0;
unsigned char Stage, DelayTime, BeepDelay;
unsigned char stagecomp, knivesdelay, cont_beep;
unsigned char target_bale_dia_reached_cont_beep;
unsigned char pre_warning_cont_beep;
unsigned char start_chamber_open_delay_cont_beep;
unsigned char end_chamber_open_delay_cont_beep;

unsigned int cal_speed_pulses;
unsigned long int NetPulseCount; // Pulses
#ifdef DEVELOP // Testing pulse counting
unsigned long int Saved_NetPulseCount; // Pulses
unsigned long int SavedPreNetPulseCount; // Pulses
unsigned long int TargetPreNetPulseCount; // Pulses
unsigned long int SavedBeakEngagePulseCount; // Pulses
#endif
unsigned long int BeakEngagePulseCount; // Pulses
unsigned long int NetPulseCountPrev; // Pulses
unsigned long int NewNetPulses; // Pulses
unsigned long int NetMetresPulseCount; // Pulses
float NetMetresAppliedForCount; // m
float NetPulseFrequency; // Hz
float NetPulsesPerLayer; // Pulses/layer
unsigned short int TargetNetPulses; // Pulses

unsigned char SequenceInProgress; 

unsigned char OP2OnDelayTime, OP2OffDelayTime;
unsigned char OP7OffDelayTime;
unsigned char OP8OnDelayTime, OP8OffDelayTime;
unsigned char NetWarningDelayTime;

unsigned char NetWarningOn;  // Check Net Error (AKA Net Warning) On/Off switch

unsigned char RollEngageTurnedOn; // OP5 Roller Engage Solenoid (AKA Pre Net Position) turned on
unsigned char SavedRollEngageTurnedOn; // Saved when IP3 opens in AUTO mode
unsigned char NetWarningDisabledWhileIP3OpenInAutoMode;
unsigned char NettingStartBeepsDisabledWhileIP3OpenInAutoMode; // Including Pre Warning & Net Delay beep
unsigned char RollerTimer = 255;      // OP5 Roller Engage Solenoid (AKA Pre Net Position) turned on for 3 seconds

unsigned char RollEngageTurnedOnInManMode; // OP5 Roller Engage Solenoid (AKA Pre Net Position) turned on in MAN mode

unsigned char ChoppingKnives1WarningOn; // Chopping knives position 1 Warning On/Off switch

unsigned char ChamberClosedDelayTime; // Door Closed

unsigned char BalerFullDelayTime;     // applies to MAN mode only -- Set to Net Delay

unsigned char ChamberOpenDelayTime;   // applies to MAN mode only

unsigned char ChamberOpenBeepDelay;   // applies to AUTO mode only

unsigned char BaleKickerStatus;                // Bale Kicker Down/Up flags
unsigned char BaleDiameterStatusInManMode;     // Bale Diameter Below/Above Engage Diameter (in MAN mode) flags
unsigned char ChamberStatus;                   // Chamber Closed/Open flags

unsigned char OP8OnCount;             // Count for Density Release Solenoid (OP8) on after Chamber (IP3) closes then opens (MAN/AUTO)

unsigned char IP4PulsesStoppedCount;        // Count for Net Pulses (on IP4) to stop after Target Pulses reached for Net Cut (OP7) to be turned off 
unsigned long int NetPulseCountAtLastCheck; // Pulses -- Similar NetPulseCountPrev to but used in auto sequence loop 

unsigned char DrivingArrowBeepsDisabled;    // Driving Arrow Beeps turned off/on

unsigned char DensityControlFunctionActive;                 // Density Control Function (based on Target Bale Density) active
unsigned char NetPulseCheckingEnabled;                      // Checking for no pulses on IP4 (for NetWarningDelayTime) enabled
unsigned char OP5TurnedOnAfterBeakEngagePulsesEnabled;      // Turning on Roller Engage Solenoid (OP5) after 'Beak Engage' pulses counted on IP4
unsigned char SavedOP5TurnedOnAfterBeakEngagePulsesEnabled; // Saved when IP3 opens in AUTO mode

#ifdef DEVELOP // Testing memory
unsigned char TestOutputReadMemAllocFail;  // flag set in SPI1FlashRead() -- brought out for testing
unsigned char TestOutputWriteMemAllocFail; // flag set in SPI1FlashWrite() -- brought out for testing
#endif

//unsigned long int forward_speed_buffer[6]; // removed on removing contents of functions in CANtest.c & CAN_mcm.c
//float mcm_forward_speed, local_forward_speed; // removed on removing contents of functions in CANtest.c & CAN_mcm.c

//unsigned char use_simulated_speed;  // removed on removing contents of functions in CANtest.c
unsigned char tl_target_count[2], tl_confimation_delay[2]; // TC4_irq_handler
//unsigned char prestart_status;  // removed on removing contents of functions in CANtest.c

// to be placed in non volatile storage - check factor/runtime split
struct mcm_struct mcm[MAX_MCM];
#if 0 // Removed
struct prodt product[MAX_MCM][MAX_PRODUCTS];
#endif

#ifdef WINDOWS
 unsigned char Sam7A3sfr[0x4000];
#endif

// runtime variables for serial flash save held in this structure
#ifndef WINDOWS
 union PreservedRuntimeToChar pRun __attribute__((section(".IRamNV")));
#else 
 union PreservedRuntimeToChar pRun;
#endif

// factors for serial flash save held in this structure
#ifndef WINDOWS
 union PreservedFactorsToChar pFac __attribute__((section(".IRamNV")));
#else
 union PreservedFactorsToChar pFac;
#endif 
// -----------------------------------------------------------

#ifdef SHOW_TIME
// -----------------------------------------------------------
// Function Name       : ClockDisplay
// -----------------------------------------------------------
void ClockDisplay( unsigned char icon  )
{
	unsigned char xp, yp, tstring[32];

	if( icon == CL0CK_ICON_ON )
	{
		xp = write_bitmap( adm, F1_CLK, 2, 2, OR ) + 3;
		yp = 2;
	}
	else // icon == CL0CK_ICON_OFF
	{
		xp = 3;
		yp = 3;
	}

	sprintf( tstring, "%02X:%02X", ClockRam[2], ClockRam[1] );
	display_f1_text( tstring, TS5, xp, yp, FIXED ); // 5 wide


	if( half_second )
	{
		draw_box( adm, xp + 12, yp, xp + 13, yp + 6,  CLEAR );
	}

}
// -----------------------------------------------------------
#endif

// -----------------------------------------------------------
// Function Name       : CheckSumIt
// -----------------------------------------------------------
unsigned long int CheckSumIt( unsigned char *data, unsigned char *end )
{
	unsigned long int ul;

	for( ul = 0; data < end; data++ )
	{
		ul += *data;
	}

	ul = ~ul + 1;

	return( ul );
}
// -----------------------------------------------------------

#if 0 // mcF[MAX_MCM] & prF[MAX_MCM][MAX_PRODUCTS] removed from pFac.pf
// -----------------------------------------------------------
// Function Name       : SetFactorPointers
// Object              : set preserved non volatile memory pointers
// -----------------------------------------------------------
void SetFactorPointers( void )
{
	struct mcm_struct *ptm;
	struct prodt *ptp;
	unsigned int i, j;

	for( i = 0; i < MAX_MCM; i++ )
	{
		// set MCM defaults - for rarely changed non volatile factors
		ptm = &mcm[i];
		ptm->pF = &(pFac.pf.mcF[i]);
		ptm->pR = &(pRun.pr.mcR[i]);

		// set Product defaults - for rarely changed non volatile factors
		for( j = 0; j < MAX_PRODUCTS; j++ )
		{
			ptp = &product[i][j];
			ptp->pF = &(pFac.pf.prF[i][j]);
			ptp->pR = &(pRun.pr.prR[i][j]);
		}
	}

}
// -----------------------------------------------------------
#endif

// -----------------------------------------------------------
// Function Name       : SetDefaultFactors
// Object              : reset power on defaults & save to flash
// -----------------------------------------------------------
void SetDefaultFactors( void )
{
	char c;

	ptText = TextString[pFac.pf.Lang];


	// set default Programmable Factors

	if( pFac.pf.software_type != SOFTWARE ) // Value preserved between successive software versions
		pFac.pf.GrandBaleTotal = PF_GW302_GrandBaleTotal[PF_DEF]; // bales

	pFac.pf.software_type = SOFTWARE;
	pFac.pf.software_issue = ISSUE;
	pFac.pf.software_revision = REVISION;

	pFac.pf.Lang = 0;                  // Language set to English

	for( c = 0; c < MAX_STORES; c++ )  // Bale Totals (in Bale Stores A to J) set to 0
		pFac.pf.BaleTotal[c] = PF_GW302_BaleTotal[PF_DEF]; // bales

	pFac.pf.BaleStore = PF_GW302_BaleStore[PF_DEF]; // Bale Store ID set to A

	// pFac.pf.BaudRate -- Not set/used

	// Machine Menu
	pFac.pf.GW302_BaleProfile = PF_GW302_BaleProfile[PF_DEF]; // Preset A to E
	for( c = 0; c < BP_PRESET_MAX; c++ )  // Bale Profile defaults set for Presets A to E
	{
		pFac.pf.GW302_BaleDiameter[c] =  PresetBaleDiameter[c]; // cm
		pFac.pf.GW302_CoreDiameter[c] =  PresetCoreDiameter[c]; // cm
		pFac.pf.GW302_BaleDensity[c]  =  PresetBaleDensity[c]; // ID
		pFac.pf.GW302_CoreDensity[c]  =  PresetCoreDensity[c]; // ID
		pFac.pf.GW302_NetLayers[c]    =  PresetNetLayers[c]; // layers
	}
	// LubeCounter -- Moved to preserved run-time totals
	// GrandBaleTotal -- Moved to early position in memory since value is to be preserved between successive software versions
	pFac.pf.GW302_DiaPotZero = PF_GW302_DiaPotZero[PF_DEF]; // V
	pFac.pf.GW302_DiameterCorrection = PF_GW302_DiameterCorrection[PF_DEF]; // cm
	pFac.pf.GW302_BaleShapeIndicator = PF_GW302_BaleShapeIndicator[PF_DEF]; // Off/On/Beep
	pFac.pf.GW302_BaleShapeIndicatorZero = PF_GW302_BaleShapeIndicatorZero[PF_DEF]; // V
	pFac.pf.GW302_TotalNetMetres = PF_GW302_TotalNetMetres[PF_DEF]; // m
	pFac.pf.GW302_PreWarningBeep = PF_GW302_PreWarningBeep[PF_DEF]; // %
	pFac.pf.GW302_NetDelay = PF_GW302_NetDelay[PF_DEF]; // s

	// Technician Menu - Baler Full
	pFac.pf.GW302_LubeAlarm = PF_GW302_LubeAlarm[PF_DEF]; // bales
	pFac.pf.GW302_PresDeadband = PF_GW302_PresDeadband[PF_DEF]; // V
	pFac.pf.GW302_NetSpeed = PF_GW302_NetSpeed[PF_DEF]; // Hz
	pFac.pf.GW302_PresSwDelay = PF_GW302_PresSwDelay[PF_DEF]; // s
	pFac.pf.GW302_DoorClosed = PF_GW302_DoorClosed[PF_DEF]; // s
	pFac.pf.GW302_BaleSizeFactor = PF_GW302_BaleSizeFactor[PF_DEF];
	pFac.pf.GW302_FillPotRange = PF_GW302_FillPotRange[PF_DEF]; // V
	pFac.pf.GW302_FillPotIgnore = PF_GW302_FillPotIgnore[PF_DEF]; // V
	pFac.pf.GW302_EngageDiameter = PF_GW302_EngageDiameter[PF_DEF]; // %
	pFac.pf.GW302_BaleIndicator = PF_GW302_BaleIndicator[PF_DEF]; // Steer/Fill
	pFac.pf.GW302_RollerDelay = PF_GW302_RollerDelay[PF_DEF]; // s

	// Technician Menu - Netting Setup
	pFac.pf.GW302_NetOn = PF_GW302_NetOn[PF_DEF]; // s
	pFac.pf.GW302_NetCutPulse = PF_GW302_NetCutPulse[PF_DEF]; // pulses
	pFac.pf.GW302_NetCutTime = PF_GW302_NetCutTime[PF_DEF]; // s
	pFac.pf.GW302_MinDiaPulsPerLay = PF_GW302_MinDiaPulsPerLay[PF_DEF]; // pulses/layer
	pFac.pf.GW302_MaxDiaPulsPerLay = PF_GW302_MaxDiaPulsPerLay[PF_DEF]; // pulses/layer
	pFac.pf.GW302_NetFactor = PF_GW302_NetFactor[PF_DEF];
	pFac.pf.GW302_NoNetPulse = PF_GW302_NoNetPulse[PF_DEF]; // s
	pFac.pf.GW302_PreNetPulse = PF_GW302_PreNetPulse[PF_DEF]; // pulses
	pFac.pf.GW302_NetSystem = PF_GW302_NetSystem[PF_DEF]; // Beak/Roller
	pFac.pf.GW302_BeakEngage = PF_GW302_BeakEngage[PF_DEF]; // pulses
	pFac.pf.GW302_TwineOption = PF_GW302_TwineOption[PF_DEF]; // Net option on or off
	pFac.pf.GW302_NetTwineOption = PF_GW302_NetTwineOption[PF_DEF]; // Net or twine

	// Technician Menu - Diameter Setup
	pFac.pf.GW302_DiameterMin = PF_GW302_DiameterMin[PF_DEF]; // %
	pFac.pf.GW302_DiameterMax = PF_GW302_DiameterMax[PF_DEF]; // %
	pFac.pf.GW302_DiaCorrection = PF_GW302_DiaCorrection[PF_DEF]; // V
	pFac.pf.GW302_BaleDiaSmoothingFactor = PF_GW302_BaleDiaSmoothingFactor[PF_DEF]; // %
	pFac.pf.GW302_DensityPresSmoothingFactor = PF_GW302_DensityPresSmoothingFactor[PF_DEF]; // %
	pFac.pf.GW302_BaleShapeSmoothingFactor = PF_GW302_BaleShapeSmoothingFactor[PF_DEF]; // %
	pFac.pf.GW302_KnifePresSmoothingFactor = PF_GW302_KnifePresSmoothingFactor[PF_DEF]; // %

	// Technician Menu - Density Setup
	pFac.pf.GW302_Density1 = PF_GW302_Density1[PF_DEF]; // V
	pFac.pf.GW302_Density2 = PF_GW302_Density2[PF_DEF]; // V
	pFac.pf.GW302_Density3 = PF_GW302_Density3[PF_DEF]; // V
	pFac.pf.GW302_Density4 = PF_GW302_Density4[PF_DEF]; // V
	pFac.pf.GW302_Density5 = PF_GW302_Density5[PF_DEF]; // V
	pFac.pf.GW302_Density6 = PF_GW302_Density6[PF_DEF]; // V
	pFac.pf.GW302_Density7 = PF_GW302_Density7[PF_DEF]; // V
	pFac.pf.GW302_Density8 = PF_GW302_Density8[PF_DEF]; // V
	pFac.pf.GW302_Density9 = PF_GW302_Density9[PF_DEF]; // V
	pFac.pf.GW302_Density10 = PF_GW302_Density10[PF_DEF]; // V

	// Technician Menu - Knives Setup
	pFac.pf.GW302_KnifePressure = PF_GW302_KnifePressure[PF_DEF]; // V
	pFac.pf.GW302_KnifePresWarn = PF_GW302_KnifePresWarn[PF_DEF]; // V
	pFac.pf.GW302_DiverterDelay = PF_GW302_DiverterDelay[PF_DEF]; // s

	// Technician Menu - Formation 2
	for( c = 0; c < BP_PRESET_MAX; c++ )   // Bale Profile defaults set for Presets A to E
	{
		pFac.pf.GW302_FormationDensity[c]   = PresetFormationDensity[c]; // ID
		pFac.pf.GW302_FormationDiameter[c]  = PresetFormationDiameter[c]; // %
		pFac.pf.GW302_Formation2Density[c]  = PresetFormation2Density[c]; // ID
		pFac.pf.GW302_Formation2Diameter[c] = PresetFormation2Diameter[c]; // %
	}

	// Diagnostics - Digital Inputs
	pFac.pf.GW302_ChoppingKnivesFitted = PF_GW302_ChoppingKnivesFitted[PF_DEF]; // Disabled/Enabled

	pFac.pf.CheckSum = CheckSumIt( &(pFac.ch[0]), (unsigned char *)&(pFac.pf.CheckSum) );

#ifndef WINDOWS
	AT91C_BASE_PIOA->PIO_CODR = BUZZER;
	//AT91C_BASE_PIOA->PIO_CODR = OUT5;

	// write main factors to SPI1 Flash @ 0x000000
	SPI1FlashWrite( SFC_PW, pFac_BASEADDRESS, &(pFac.ch[0]), sizeof(pFac.pf) );

	// write backup factors to SPI1 Flash @ 0x000000 + 800h
	SPI1FlashWrite( SFC_PW, pFac_BASEADDRESS+ 0x800, &(pFac.ch[0]), sizeof(pFac.pf) );

	AT91C_BASE_PIOA->PIO_SODR = BUZZER;
	//AT91C_BASE_PIOA->PIO_SODR = OUT5;
#endif
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : SetDefaultTotals
// Object              : reset power on defaults
// -----------------------------------------------------------
void SetDefaultTotals( void )
{
	pRun.pr.Brightness[0] = DISPLAY_DEFAULT_DAY_BRIGHTNESS;
	pRun.pr.Contrast[0] = DISPLAY_DEFAULT_DAY_CONTRAST;

	pRun.pr.Brightness[1] = DISPLAY_DEFAULT_BRIGHTNESS;
	pRun.pr.Contrast[1] = DISPLAY_DEFAULT_CONTRAST;

	pRun.pr.DayNight = 0;

	pRun.pr.LubeCounter = PF_GW302_LubeAlarm[PF_DEF]; // bales

	pRun.pr.PowerCount = 0;

	pRun.pr.CheckSum = CheckSumIt( &(pRun.ch[0]), (unsigned char *)&(pRun.pr.CheckSum) );

#ifndef WINDOWS
	AT91C_BASE_PIOA->PIO_CODR = BUZZER;
	//AT91C_BASE_PIOA->PIO_CODR = OUT5;

	// write runtime factors and totals to SPI1 Flash @ 0x001000
	SPI1FlashWrite( SFC_PW, pRun_BASEADDRESS, &(pRun.ch[0]), sizeof(pRun.pr) );

	// write runtime factors and totals to SPI1 Flash @ 0x001800
	SPI1FlashWrite( SFC_PW, pRun_BASEADDRESS + 0x800, &(pRun.ch[0]), sizeof(pRun.pr) );

	AT91C_BASE_PIOA->PIO_SODR = BUZZER;
	//AT91C_BASE_PIOA->PIO_SODR = OUT5;
#endif

}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : SetStartupDefaults
// Object              : set global variables to power on values
// -----------------------------------------------------------
void SetStartupDefaults( void )
{    
	ptText = TextString[pFac.pf.Lang];
	key = (~AT91C_BASE_PIOB->PIO_PDSR) & SW_MASK;
	if( (key & ~(SW_NEW|SW_REP)) == SW_CU )
		window = screen147;  // Technician Menu - Reset Defaults
	else
		window = screen00;   // Startup Splash Screen

	looptime = 0;

	AutoMan = 0;
	SecondScreen = 0;
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : btobcd
// -----------------------------------------------------------
unsigned char btobcd( unsigned char bin )
{
	return( ((bin / 10) * 0x10) + (bin % 10) );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : bcdtob
// -----------------------------------------------------------
unsigned char bcdtob( unsigned char bcd )
{
	return( ((bcd / 0x10) * 10) + (bcd % 0x10) );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : SetOutputPWM
// -----------------------------------------------------------
void SetOutputPWM( unsigned char output, unsigned short int freq, unsigned char duty )
{
#ifndef WINDOWS    
	unsigned int const portpin[8] = { AT91C_PA18_PWM0, AT91C_PA19_PWM1, AT91C_PA20_PWM2, AT91C_PA21_PWM3, AT91C_PA22_PWM4, AT91C_PA23_PWM5, AT91C_PA24_PWM6, AT91C_PA25_PWM7 };
#endif

	static int status;
	static unsigned char dutyLast[8];
	static unsigned short int freqLast[8];

	unsigned int mode, mul, period, channel;

	channel = ((unsigned int)0x1 << output);

	if( duty && freq )
	{
		// reading ISR clears all bits, bits indicate new channel period has been achieved
		status |= AT91C_BASE_PWMC->PWMC_ISR;

		// if not running or frequency change, calculate clock settings
		if( !(AT91C_BASE_PWMC->PWMC_SR & channel) || (freqLast[output] != freq) || (dutyLast[output] != duty) )
		{
			// MCK = 47923200Hz
			// prescaler x CPRD = MCK / freq
			period = MCK / freq;
			mode = (period / 1048576); // 20 bit count max
			if( mode )
			{
				period = 0;
				mul = 1;
				while( mode )
				{
					period++;
					mode /= 2;
					mul *= 2;
				}
				mode = period; // 2 to the power of period is divider value
				period = MCK / freq / mul;
			}
			/*mode |= AT91C_PWMC_CPOL;*/ // output starts high if set
		}
		else
		{
			status &= ~channel; // clear status for this channel if no change required
		}

		// if PWM not running
		if( !(AT91C_BASE_PWMC->PWMC_SR & channel) )
		{
			freqLast[output] = freq;
			dutyLast[output] = duty;

			// AT91F_PWMC_CfgChannel()
			AT91C_BASE_PWMC->PWMC_CH[output].PWMC_CMR = mode;
			AT91C_BASE_PWMC->PWMC_CH[output].PWMC_CPRDR = period;
			AT91C_BASE_PWMC->PWMC_CH[output].PWMC_CDTYR = period * duty / 100;

			// AT91F_PWMC_StartChannel()
			AT91C_BASE_PWMC->PWMC_ENA = channel;

			// disable PIO mode
#ifndef WINDOWS
			AT91F_PIO_Disable( AT91C_BASE_PIOA, portpin[output] );
#endif
		}
		// channel count must be > 1, else if 0 or 1 CUPDR changes CPRDR/CDTYR directly (see Errata)
		else if( (status & channel) && (AT91C_BASE_PWMC->PWMC_CH[output].PWMC_CCNTR > 1) )
		{
			// if frequency changed then both period and duty will need updating
			// if period > current duty, set new period - else set new duty first
			if( (freqLast[output] != freq) && (period > AT91C_BASE_PWMC->PWMC_CH[output].PWMC_CDTYR) )
			{
				freqLast[output] = freq;
				dutyLast[output] = 0; // force duty to updated next time
				status &= ~channel;
				// set to update period
				AT91C_BASE_PWMC->PWMC_CH[output].PWMC_CMR = mode | AT91C_PWMC_CPD;
				// set update register for duty
				AT91C_BASE_PWMC->PWMC_CH[output].PWMC_CUPDR = period;
			}
			// if duty or frequency changed, duty needs updating
			else if( (dutyLast[output] != duty) || (freqLast[output] != freq) )
			{
				dutyLast[output] = duty;
				status &= ~channel;
				// set to update duty
				AT91C_BASE_PWMC->PWMC_CH[output].PWMC_CMR = mode;
				// set update register for duty
				AT91C_BASE_PWMC->PWMC_CH[output].PWMC_CUPDR = period * duty / 100;
			}
		}
	}
	else
	{
#ifndef WINDOWS
		// turn off output
		AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, portpin[output] );
		AT91F_PIO_SetOutput( AT91C_BASE_PIOA, portpin[output] );
#endif
		// Stop PWM
		AT91C_BASE_PWMC->PWMC_DIS = channel;
	}

}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : ResetLubeCount
// -----------------------------------------------------------
void ResetLubeCount( void )
{

	if( pRun.pr.LubeCounter < 0 )
	{
		pRun.pr.LubeCounter += pFac.pf.GW302_LubeAlarm; // Reset to Lube Alarm (in bales) minus the number of bales since alarm first appeared
	}
	else // No extra bales since alarm first appeared
	{
		pRun.pr.LubeCounter = pFac.pf.GW302_LubeAlarm; // Reset to Lube Alarm (in bales)
	}

}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : Inputs
// Object              : Calculates inputs
// Output Parameters   : DiaTargetVolts
//                       DiaMinExtDisplayPercent     (0.0 - 100.0 % of extended display range from PotZero to DiaMax)
//                       DiaTargetExtDisplayPercent  (0.0 - 100.0 % of extended display range from PotZero to DiaMax)
//                       BaleDiaExtDisplayPercent    (0.0 - 100.0 % of extended display range from PotZero to DiaMax)
//                       DiaMinVolts
//                       DiaMaxVolts
// -----------------------------------------------------------
void Inputs( void )
{    
	float DiaVoltsPer1PercentFullRange;       // V
	float DiaCorrectionNudgeVolts;            // V
	float DiaZeroVolts;                       // V

	float DiaVoltsPerSetting;                 // V where Setting = 1 to 109 (which corresponds to 60 to 168cm)

#ifdef DEVELOP
	float DiaVoltsPer1PercentDisplayRange;    // V where DisplayRange = 0 to 100 % (which corresponds to 60 to 168cm)

	unsigned char tstring[32];                // used in printing out values
#endif

	// Volts per 1% of full range = Voltage Range / 100  where Voltage Range = Bale Size Factor x Fixed Multiplier
	DiaVoltsPer1PercentFullRange = ((float)pFac.pf.GW302_BaleSizeFactor * DIA_FIXED_MULTIPLIER) / 100.0;

	// Correction nudge voltage = Units nudged x Size of unit
	DiaCorrectionNudgeVolts = (float)pFac.pf.GW302_DiameterCorrection * pFac.pf.GW302_DiaCorrection;

	// Diameter zero voltage corresponding to 0cm = Pot Zero Voltage + Correction nudge voltage
	DiaZeroVolts = pFac.pf.GW302_DiaPotZero + DiaCorrectionNudgeVolts;

	// Minimum voltage = (% diameter at setting 1 x Volts per 1%) + Diameter zero voltage corresponding to 0cm
	DiaMinVolts = ((float)pFac.pf.GW302_DiameterMin * DiaVoltsPer1PercentFullRange) + DiaZeroVolts;

	// Maximum voltage = (% diameter at setting 109 x Volts per 1%) + Diameter zero voltage corresponding to 0cm
	DiaMaxVolts = ((float)pFac.pf.GW302_DiameterMax * DiaVoltsPer1PercentFullRange) + DiaZeroVolts;

	// Volts per setting division = (Maximum voltage - Minimum voltage) / (109 - 1) where setting is 1 to 109 corresponding to 60cm to 168cm
	DiaVoltsPerSetting = (DiaMaxVolts - DiaMinVolts) / 108.0;

	// Target bale diameter voltage = ( (Target setting in cm - 60cm) x Volts per setting division  ) + Minimum voltage
	// Note: Target bale diameter voltage = Minimum voltage when Target setting is 60cm
	DiaTargetVolts = ((float)(pFac.pf.GW302_BaleDiameter[pFac.pf.GW302_BaleProfile] - 60) * DiaVoltsPerSetting) + DiaMinVolts;

#ifdef DEVELOP
	// Volts per 1% of display range (from 0 to 100 % corresponding to 60 to 168cm and setting 1 to 109)
	// Not used in any calculations
	DiaVoltsPer1PercentDisplayRange = (DiaMaxVolts - DiaMinVolts) / 100.0;
#endif

	// Current bale diameter -- shown on scale with target & minimum bale diameters indicated

	// Minimum bale diameter extended display percentage (0 - 100 % of extended display range from PotZero to DiaMax)
	if(DiaMinVolts > DiaZeroVolts)
		DiaMinExtDisplayPercent = ( (DiaMinVolts - DiaZeroVolts) / (DiaMaxVolts - DiaZeroVolts) ) * 100.0;
	else
		DiaMinExtDisplayPercent = 0.0; // Limiting to 0.0%
	if( DiaMinExtDisplayPercent > 100.0)
		DiaMinExtDisplayPercent = 100.0; // Limiting to 100.0%

	// Target bale diameter extended display percentage (0 - 100 % of extended display range from PotZero to DiaMax)
	if(DiaTargetVolts > DiaZeroVolts)
		DiaTargetExtDisplayPercent = ( (DiaTargetVolts - DiaZeroVolts) / (DiaMaxVolts - DiaZeroVolts) ) * 100.0;
	else
		DiaTargetExtDisplayPercent = 0.0; // Limiting to 0.0%
	if( DiaTargetExtDisplayPercent > 100.0)
		DiaTargetExtDisplayPercent = 100.0; // Limiting to 100.0%

	// Current bale diameter extended display percentage (0 - 100 %) = (Current bale diameter voltage minus Diameter zero voltage / Extended display voltage range) x 100 to convert to % value
	if(BaleDiaVolts > DiaZeroVolts)
		// Multiplying (BaleDiaVolts - DiaZeroVolts) by 100.0 before division for accuracy
		BaleDiaExtDisplayPercent = ( (BaleDiaVolts - DiaZeroVolts) * 100.0 ) / ( DiaMaxVolts - DiaZeroVolts );
	else
		BaleDiaExtDisplayPercent = 0.0; // Limiting to 0.0%
	if( BaleDiaExtDisplayPercent > 100.0)
		BaleDiaExtDisplayPercent = 100.0; // Limiting to 100.0%


	// Formation diameter voltage = % Upper diameter limit of 'Formation' stage * 100 % range from 0cm (zero voltage) to 168cm diameter (maximum voltage)
	DiaFormationVolts = ((float)pFac.pf.GW302_FormationDiameter[pFac.pf.GW302_BaleProfile] * ((DiaMaxVolts - DiaZeroVolts) / 100.0)) + DiaZeroVolts;

	// Formation2 diameter voltage = % Upper diameter limit of 'Formation2' stage * 100 % range from 0cm (zero voltage) to 168cm diameter (maximum voltage)
	DiaFormation2Volts = ((float)pFac.pf.GW302_Formation2Diameter[pFac.pf.GW302_BaleProfile] * ((DiaMaxVolts - DiaZeroVolts) / 100.0)) + DiaZeroVolts;

	// Core diameter voltage = ( (Core setting in cm - 60cm) x Volts per setting division  ) + Minimum voltage corresponding to 60cm
	DiaCoreVolts = ((float)(pFac.pf.GW302_CoreDiameter[pFac.pf.GW302_BaleProfile] - 60) * DiaVoltsPerSetting) + DiaMinVolts;

	// Engage diameter voltage = Engage % setting x Target diameter voltage (from Zero pot voltage)
	DiaEngageVolts = ((float)pFac.pf.GW302_EngageDiameter * ((DiaTargetVolts - DiaZeroVolts) / 100.0)) + DiaZeroVolts;

	// Pre warning beep diameter voltage = Pre Warning Beep % setting x Target diameter voltage (from Zero pot voltage)
	DiaPreWarnVolts = ((float)pFac.pf.GW302_PreWarningBeep * ((DiaTargetVolts - DiaZeroVolts) / 100.0)) + DiaZeroVolts;


	// Target density voltage during Main stage
	switch( pFac.pf.GW302_BaleDensity[pFac.pf.GW302_BaleProfile] )
	{
	case 1:
		DensityBaleVolts = pFac.pf.GW302_Density1;
		break;
	case 2:
		DensityBaleVolts = pFac.pf.GW302_Density2;
		break;
	case 3:
		DensityBaleVolts = pFac.pf.GW302_Density3;
		break;
	case 4:
		DensityBaleVolts = pFac.pf.GW302_Density4;
		break;
	case 5:
		DensityBaleVolts = pFac.pf.GW302_Density5;
		break;
	case 6:
		DensityBaleVolts = pFac.pf.GW302_Density6;
		break;
	case 7:
		DensityBaleVolts = pFac.pf.GW302_Density7;
		break;
	case 8:
		DensityBaleVolts = pFac.pf.GW302_Density8;
		break;
	case 9:
		DensityBaleVolts = pFac.pf.GW302_Density9;
		break;
	default: // 10
	DensityBaleVolts = pFac.pf.GW302_Density10;
	break;
	}

	// Target density voltage during Core stage
	switch( pFac.pf.GW302_CoreDensity[pFac.pf.GW302_BaleProfile] )
	{
	case 1:
		DensityCoreVolts = pFac.pf.GW302_Density1;
		break;
	case 2:
		DensityCoreVolts = pFac.pf.GW302_Density2;
		break;
	case 3:
		DensityCoreVolts = pFac.pf.GW302_Density3;
		break;
	case 4:
		DensityCoreVolts = pFac.pf.GW302_Density4;
		break;
	case 5:
		DensityCoreVolts = pFac.pf.GW302_Density5;
		break;
	case 6:
		DensityCoreVolts = pFac.pf.GW302_Density6;
		break;
	case 7:
		DensityCoreVolts = pFac.pf.GW302_Density7;
		break;
	case 8:
		DensityCoreVolts = pFac.pf.GW302_Density8;
		break;
	case 9:
		DensityCoreVolts = pFac.pf.GW302_Density9;
		break;
	default: // 10
	DensityCoreVolts = pFac.pf.GW302_Density10;
	break;
	}

	// Target density voltage during Formation2 stage
	switch( pFac.pf.GW302_Formation2Density[pFac.pf.GW302_BaleProfile] )
	{
	case 1:
		DensityFormation2Volts = pFac.pf.GW302_Density1;
		break;
	case 2:
		DensityFormation2Volts = pFac.pf.GW302_Density2;
		break;
	case 3:
		DensityFormation2Volts = pFac.pf.GW302_Density3;
		break;
	case 4:
		DensityFormation2Volts = pFac.pf.GW302_Density4;
		break;
	case 5:
		DensityFormation2Volts = pFac.pf.GW302_Density5;
		break;
	case 6:
		DensityFormation2Volts = pFac.pf.GW302_Density6;
		break;
	case 7:
		DensityFormation2Volts = pFac.pf.GW302_Density7;
		break;
	case 8:
		DensityFormation2Volts = pFac.pf.GW302_Density8;
		break;
	case 9:
		DensityFormation2Volts = pFac.pf.GW302_Density9;
		break;
	default: // 10
	DensityFormation2Volts = pFac.pf.GW302_Density10;
	break;
	}

	// Target density voltage during Formation stage
	switch( pFac.pf.GW302_FormationDensity[pFac.pf.GW302_BaleProfile] )
	{
	case 1:
		DensityFormationVolts = pFac.pf.GW302_Density1;
		break;
	case 2:
		DensityFormationVolts = pFac.pf.GW302_Density2;
		break;
	case 3:
		DensityFormationVolts = pFac.pf.GW302_Density3;
		break;
	case 4:
		DensityFormationVolts = pFac.pf.GW302_Density4;
		break;
	case 5:
		DensityFormationVolts = pFac.pf.GW302_Density5;
		break;
	case 6:
		DensityFormationVolts = pFac.pf.GW302_Density6;
		break;
	case 7:
		DensityFormationVolts = pFac.pf.GW302_Density7;
		break;
	case 8:
		DensityFormationVolts = pFac.pf.GW302_Density8;
		break;
	case 9:
		DensityFormationVolts = pFac.pf.GW302_Density9;
		break;
	default: // 10
	DensityFormationVolts = pFac.pf.GW302_Density10;
	break;
	}


#if 0 // Testing - intermediate inputs & outputs

	sprintf( tstring, "VP=%6.5fV", DiaVoltsPer1PercentFullRange ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 8, FIXED );
	sprintf( tstring, "CN=%6.5fV", DiaCorrectionNudgeVolts ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 18, FIXED );
	sprintf( tstring, "MI=%6.5fV", DiaMinVolts ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 28, FIXED );
	sprintf( tstring, "MA=%6.5fV", DiaMaxVolts ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 38, FIXED );
	sprintf( tstring, "PS=%6.5fV", DiaVoltsPerSetting ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 48, FIXED );
	sprintf( tstring, "PP=%6.5fV", DiaVoltsPer1PercentDisplayRange ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 58, FIXED );
#endif

#if 0 // Testing - intermediate inputs & outputs

	sprintf( tstring, "ZV=%6.5fV", DiaZeroVolts ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 8, FIXED );
	sprintf( tstring, "FV=%6.5fV", DiaFormationVolts ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 18, FIXED );
	sprintf( tstring, "F2=%6.5fV", DiaFormation2Volts ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 28, FIXED );
	sprintf( tstring, "CV=%6.5fV", DiaCoreVolts ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 38, FIXED );
	sprintf( tstring, "EV=%6.5fV", DiaEngageVolts ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 48, FIXED );
	sprintf( tstring, "PW=%6.5fV", DiaPreWarnVolts ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 58, FIXED );

#endif

#if 0 // Testing - intermediate inputs & outputs

	sprintf( tstring, "BD=%6.5fV", DensityBaleVolts ); // Set
	display_f1_text( tstring, TSMAX, 2, 8, FIXED );
	sprintf( tstring, "CD=%6.5fV", DensityCoreVolts ); // Set
	display_f1_text( tstring, TSMAX, 2, 18, FIXED );
	sprintf( tstring, "F2=%6.5fV", DensityFormation2Volts ); // Set
	display_f1_text( tstring, TSMAX, 2, 28, FIXED );
	sprintf( tstring, "FD=%6.5fV", DensityFormationVolts ); // Set
	display_f1_text( tstring, TSMAX, 2, 38, FIXED );
	sprintf( tstring, "On8=%02u Off8=%02u On2=%02u", OP8OnDelayTime, OP8OffDelayTime, OP2OnDelayTime); // Calculated
	display_f1_text( tstring, TSMAX, 2, 48, FIXED );
	sprintf( tstring, "DV=%6.5fV", DensityPresVolts ); // Input
	display_f1_text( tstring, TSMAX, 2, 58, FIXED );
#endif

#if 0 // Testing - main inputs & outputs

	//sprintf( tstring, "ST=%02X SC=%02X", Stage, stagecomp );
	//display_f1_text( tstring, TSMAX, 2, 68, FIXED );

	sprintf( tstring, "DiMaV=%3.2fV", DiaMaxVolts ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 8, FIXED );

	sprintf( tstring, "DTarV=%3.2fV", DiaTargetVolts );     // Calculated
	display_f1_text( tstring, TSMAX, 2, 18, FIXED );

	sprintf( tstring, "DiMiV=%3.2fV", DiaMinVolts ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 38, FIXED );

	sprintf( tstring, "MiEx%%=%3.2f%%", DiaMinExtDisplayPercent ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 48, FIXED );

	sprintf( tstring, "TaEx%%=%3.2f%%", DiaTargetExtDisplayPercent ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 58, FIXED );

	sprintf( tstring, "BDiE%%=%3.2f%%", BaleDiaExtDisplayPercent ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 68, FIXED );

	sprintf( tstring, "ZV=%6.5fV", DiaZeroVolts ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 88, FIXED );

	sprintf( tstring, "PP=%6.5fV", DiaVoltsPer1PercentDisplayRange ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 98, FIXED );

	sprintf( tstring, "PS=%6.5fV", DiaVoltsPerSetting ); // Calculated
	display_f1_text( tstring, TSMAX, 2, 108, FIXED );

#endif

#if 0 // Testing Analogue inputs
	sprintf( tstring, "KnifePresV=%4.3f", KnifePresVolts );
	display_f1_text( tstring, TSMAX, 2, 48, FIXED );

	sprintf( tstring, "BaleDiaV=%4.3f", BaleDiaVolts );
	display_f1_text( tstring, TSMAX, 2, 58, FIXED );

	sprintf( tstring, "DensityPresV=%4.3f", DensityPresVolts );
	display_f1_text( tstring, TSMAX, 2, 68, FIXED );

	sprintf( tstring, "BaleShapeV=%4.3f", BaleShapeVolts );
	display_f1_text( tstring, TSMAX, 2, 78, FIXED );
#endif

#if 0 // Testing - inputs & outputs
	sprintf( tstring, "%2.1fV", KnifePresVolts ); // Input Top - left
	display_f1_text( tstring, TSMAX, 42, 8, FIXED );

	sprintf( tstring, "%2.1fV", BaleShapeVolts ); // Input Top - right
	display_f1_text( tstring, TSMAX, 66, 8, FIXED );

	sprintf( tstring, "%2.1fV", DensityPresVolts ); // Input Bot - left
	display_f1_text( tstring, TSMAX, 42, 18, FIXED );

	sprintf( tstring, "%2.1fV", BaleDiaVolts ); // Input Bot - right
	display_f1_text( tstring, TSMAX, 66, 18, FIXED );
#endif

#if 0 // Testing
	sprintf( tstring, "%2.1fHz", NetPulseFrequency );
	display_f1_text( tstring, TSMAX, 2, 78, FIXED );

	sprintf( tstring, "%4u", TargetNetPulses ); // Input Bot - far right
	display_f1_text( tstring, TSMAX, 50, 98, FIXED );

	sprintf( tstring, "%4u", NetPulseCount ); // Input Bot - farthest far right
	display_f1_text( tstring, TSMAX, 84, 98, FIXED );

	//sprintf( tstring, "%4u", NetMetresPulseCount ); // Input Bot - farthest far right
	//display_f1_text( tstring, TSMAX, 118, 98, FIXED );
#endif

#if 0 // Testing pulse counting
	sprintf( tstring, "%2.1fHz", NetPulseFrequency );
	display_f1_text( tstring, TSMAX, 2, 78, FIXED );

	sprintf( tstring, "%4u", TargetNetPulses ); // Input Bot - far right
	display_f1_text( tstring, TSMAX, 50, 98, FIXED );

	sprintf( tstring, "%4u", NetPulseCount ); // Input Bot - farthest far right
	display_f1_text( tstring, TSMAX, 84, 98, FIXED );

	//sprintf( tstring, "%4u", NetMetresPulseCount ); // Input Bot - farthest far right
	//display_f1_text( tstring, TSMAX, 118, 98, FIXED );

	sprintf( tstring, "%4u", NewNetPulses ); // Input Bot - far right
	display_f1_text( tstring, TSMAX, 50, 88, FIXED );

	sprintf( tstring, "%4u", NetPulseCountPrev ); // Input Bot - farthest far right
	display_f1_text( tstring, TSMAX, 84, 88, FIXED );

	sprintf( tstring, "%4u", NetMetresPulseCount ); // Input Bot - farthest far right
	display_f1_text( tstring, TSMAX, 118, 88, FIXED );

#endif

#if 0 // Testing net metres, pulse count, target pulses
	sprintf( tstring, "MI=%2.1fV", DiaMinVolts ); // Calculated
	display_f1_text( tstring, TSMAX, 42, 18, FIXED );

	sprintf( tstring, "MA=%2.1fV", DiaMaxVolts ); // Calculated
	display_f1_text( tstring, TSMAX, 42, 28, FIXED );

	sprintf( tstring, "T=%2.1fV", DiaTargetVolts );     // Calculated
	display_f1_text( tstring, TSMAX, 42, 38, FIXED );

	sprintf( tstring, "ST=%02X", Stage );
	display_f1_text( tstring, TSMAX, 2, 58, FIXED );

	sprintf( tstring, "SC=%02X", stagecomp );
	display_f1_text( tstring, TSMAX, 2, 68, FIXED );

	/*sprintf( tstring, "%6u", TotalNetMetresLastSaved ); // NOT USED NOW
    display_f1_text( tstring, TSMAX, 2, 78, FIXED );*/

	sprintf( tstring, "%2.1fHz", NetPulseFrequency );
	display_f1_text( tstring, TSMAX, 2, 78, FIXED );

	sprintf( tstring, "%2.1fV", KnifePresVolts ); // Input Top - left
	display_f1_text( tstring, TSMAX, 2, 88, FIXED );

	sprintf( tstring, "%2.1fV", BaleShapeVolts ); // Input Top - right
	display_f1_text( tstring, TSMAX, 26, 88, FIXED );

	sprintf( tstring, "%2.1fp/l", NetPulsesPerLayer ); // Input Top - far right
	display_f1_text( tstring, TSMAX, 50, 88, FIXED );

	sprintf( tstring, "%2.1fV", DensityPresVolts ); // Input Bot - left
	display_f1_text( tstring, TSMAX, 2, 98, FIXED );

	sprintf( tstring, "%2.1fV", BaleDiaVolts ); // Input Bot - right
	display_f1_text( tstring, TSMAX, 26, 98, FIXED );

	sprintf( tstring, "%u p", TargetNetPulses ); // Input Bot - far right
	display_f1_text( tstring, TSMAX, 50, 98, FIXED );

	sprintf( tstring, "%u p", NetPulseCount ); // Input Bot - farthest far right
	display_f1_text( tstring, TSMAX, 74, 98, FIXED );
#endif

#if 0 // Testing Count for Net Pulses (on IP4) to stop after Target Pulses reached for Net Cut (OP7) to be turned off 
	sprintf( tstring, "NPC=%u", NetPulseCount );
	display_f1_text( tstring, TSMAX, 69,  8, FIXED );

	sprintf( tstring, "LST=%u", NetPulseCountAtLastCheck );
	display_f1_text( tstring, TSMAX, 69,  18, FIXED );

	sprintf( tstring, "OP7=%u", OP7OffDelayTime );
	display_f1_text( tstring, TSMAX, 69,  28, FIXED );

	sprintf( tstring, "STP=%u", IP4PulsesStoppedCount );
	display_f1_text( tstring, TSMAX, 69,  38, FIXED );
#endif

#if 0 // Testing Analogue inputs
	sprintf( tstring, "D1=%4.3f", BaleDiaVolts );
	display_f1_text( tstring, TSMAX, 69, 28, FIXED );

	sprintf( tstring, "P2=%4.3f", DensityPresVolts );
	display_f1_text( tstring, TSMAX, 69, 78, FIXED );

	sprintf( tstring, "S3=%4.3f", BaleShapeVolts );
	display_f1_text( tstring, TSMAX, 69, 88, FIXED );

	sprintf( tstring, "K4=%4.3f", KnifePresVolts );
	display_f1_text( tstring, TSMAX, 69, 98, FIXED );
#endif

#if 0 // Testing memory
	sprintf( tstring, "F=%3u", sizeof(pFac.pf) );
	display_f1_text( tstring, TSMAX, 127, 16, FIXED );

	sprintf( tstring, "R=%3u", sizeof(pRun.pr) );
	display_f1_text( tstring, TSMAX, 127, 26, FIXED );

	sprintf( tstring, "RF=%1u", TestOutputReadMemAllocFail);
	display_f1_text( tstring, TSMAX, 127, 48, FIXED );

	sprintf( tstring, "WF=%1u", TestOutputWriteMemAllocFail);
	display_f1_text( tstring, TSMAX, 127, 58, FIXED );
#endif

#if 0 // Testing BaleKickerStatus & BaleDiameterStatusInManMode & ChamberStatus
	sprintf( tstring, "BKS=%02X", BaleKickerStatus );
	display_f1_text( tstring, TSMAX, 3, 78, FIXED );

	sprintf( tstring, "BDSIMM=%02X", BaleDiameterStatusInManMode );
	display_f1_text( tstring, TSMAX, 3, 88, FIXED );

	sprintf( tstring, "CS=%02X", ChamberStatus );
	display_f1_text( tstring, TSMAX, 3, 98, FIXED );
#endif

#if 0 // Testing Net Pulses (on IP4)
	sprintf( tstring, "NPC=%3lu", NetPulseCount ); // added l
	display_f1_text( tstring, TSMAX, 69,  8, FIXED );

	sprintf( tstring, "TNP=%3u", TargetNetPulses );
	display_f1_text( tstring, TSMAX, 69,  18, FIXED );

	sprintf( tstring, "LST=%3u", NetPulseCountAtLastCheck );
	display_f1_text( tstring, TSMAX, 69,  28, FIXED );

	sprintf( tstring, "OP7=%3u", OP7OffDelayTime );
	display_f1_text( tstring, TSMAX, 69,  38, FIXED );

	sprintf( tstring, "STP=%3u", IP4PulsesStoppedCount );
	display_f1_text( tstring, TSMAX, 69,  48, FIXED );

	sprintf( tstring, "BEP=%3lu", BeakEngagePulseCount ); // added l
	display_f1_text( tstring, TSMAX, 69,  58, FIXED );
#endif

#if 0 //#ifdef DEVELOP // Testing pulse counting -- Incl. testing Pre Net Pulses & Beak Engage Pulses (on IP4)
	sprintf( tstring, "  NPC=%4lu", NetPulseCount );
	display_f1_text( tstring, TSMAX, 49,  8, FIXED );

	sprintf( tstring, " SNPC=%4lu", Saved_NetPulseCount );
	display_f1_text( tstring, TSMAX, 49,  18, FIXED );

	sprintf( tstring, " TNPC=%4u", TargetNetPulses );
	display_f1_text( tstring, TSMAX, 49,  28, FIXED );

	sprintf( tstring, "SPNPC=%4lu", SavedPreNetPulseCount );
	display_f1_text( tstring, TSMAX, 49,  38, FIXED );

	sprintf( tstring, "TPNPC=%4lu", TargetPreNetPulseCount );
	display_f1_text( tstring, TSMAX, 49,  48, FIXED );

	sprintf( tstring, " BEPC=%4lu", BeakEngagePulseCount );
	display_f1_text( tstring, TSMAX, 49,  58, FIXED );

	sprintf( tstring, "SBEPC=%4lu", SavedBeakEngagePulseCount );
	display_f1_text( tstring, TSMAX, 49,  68, FIXED );

	sprintf( tstring, "TBEPC=%4lu", (unsigned long int)pFac.pf.GW302_BeakEngage );
	display_f1_text( tstring, TSMAX, 49,  78, FIXED );
#endif

#ifdef DEVELOP // Testing state machine
	sprintf( tstring, "ST=%02X", Stage );
	display_f1_text( tstring, TSMAX, 3, 58, FIXED );

	sprintf( tstring, "SC=%02X", stagecomp );
	display_f1_text( tstring, TSMAX, 3, 68, FIXED );
#endif

}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : CheckAlarms
// -----------------------------------------------------------
void CheckAlarms( void )
{

	// -----------------------------------------------------------
	// Density Pressure Too High Warning

	if( DensityPresVolts > DPV_DENSITY_PRESSURE_TOO_HIGH_VOLTS )
	{
		if( !(MWarn & MW_DENSITYPRESS) )
		{
			MWarn |= MW_DENSITYPRESS;
			OKWarnBox = EXCL;

			// Sounding 1 beep when message first appears
			buzz_count = 1;
			buzz_length = 250;

			DensityPressureTooHigh = 1;
		}

		// Sounding warning every second
		if( !half_second )
		{
			buzz_count = 1;
			buzz_length = 250;
		}
	}
	else if( DensityPresVolts < (DPV_DENSITY_PRESSURE_TOO_HIGH_VOLTS - DPV_DENSITY_PRESSURE_TOO_HIGH_VOLTS_HYSTERESIS) )
	{
		if( MWarn & MW_DENSITYPRESS )
		{
			MWarn &= ~MW_DENSITYPRESS;
			OKWarnBox &= ~EXCL;

			DensityPressureTooHigh = 0;
		}
	}


	// -----------------------------------------------------------
	// Check Net Error (AKA Net Warning)
	if(pFac.pf.GW302_NetTwineOption == TO_NET) /**/
	{
		if( NetWarningOn )
		{
			if( !(MWarn & MW_CHECKNET) )
			{
				MWarn |= MW_CHECKNET;
				OKWarnBox = EXCL;

				// Sounding 1 beep when message first appears
				buzz_count = 1;
				buzz_length = 250;
			}
		}
		else // !NetWarningOn
		{
			if( MWarn & MW_CHECKNET )
			{
				MWarn &= ~MW_CHECKNET;
				OKWarnBox &= ~EXCL;
			}
		}
	}



	// -----------------------------------------------------------
	// Drop Floor (AKA Floor Position) Warning

	if( !FLOOR_WORKING_POS )
	{
		if( !(MWarn & MW_DROPFLOOR) )
		{
			MWarn |= MW_DROPFLOOR;
			OKWarnBox = EXCL;

			// Sounding 1 beep when message first appears
			buzz_count = 1;
			buzz_length = 250;

		}
	}
	else  // Floor (Position) Sensor closed
	{
		if( MWarn & MW_DROPFLOOR )
		{
			MWarn &= ~MW_DROPFLOOR;
			OKWarnBox &= ~EXCL;
		}
	}


	// -----------------------------------------------------------
	// Knife Maximum Pressure Warning
	// Inhibited when:
	// - 'Chopping Knives Fitted' set to disabled -- Chopping Knives not fitted
	// - Outside Right (OR) button pressed while first screen displayed

	if( pFac.pf.GW302_ChoppingKnivesFitted == CKF_ENABLED ) // 'Chopping Knives Fitted' set to enabled
	{
		if( (KnifePresVolts > pFac.pf.GW302_KnifePresWarn) &&
				( ((key & ~(SW_NEW | SW_REP)) != SW_OR) || SecondScreen ) )
		{
			if( !(MWarn & MW_KNIFEPRES) )
			{
				//buzz_count = 1;
				swrel = 0; /* search for if( looptime > 3 ) in screen.c */
				MWarn |= MW_KNIFEPRES;
				OKWarnBox = EXCL;
			}

			if( !(swrel % 30) )
			{
				//buzz_count = 1;  // 100ms beep every 3 seconds.
				if( !DelayTime )  // When other beeps are going on
				{
					buzz_length = 250;
				}
			}
		}
		else // (KnifePresVolts <= pFac.pf.GW302_KnifePresWarn) or OR button pressed
		{
			if( MWarn & MW_KNIFEPRES )
			{
				MWarn &= ~MW_KNIFEPRES;
				OKWarnBox &= ~EXCL;
			}
		}
	}


	// -----------------------------------------------------------
	// Chopping knives position Warning

	if( DisableChoppingKnivesWarnings ) // Disabling prevents chopping knives position warning from coming on
	{
		if( MWarn & MW_CHOPKNIVPOS1 )
		{
			// Both warnings disabled
			MWarn &= ~(MW_CHOPKNIVPOS1);
			OKWarnBox &= ~EXCL;
		}

		// Chopping knives position Warning turned off
		ChoppingKnives1WarningOn = 0;
	}
	else if( ChoppingKnives1WarningOn ) // Chopping knives position Warning on
	{
		if( ChoppingKnives1WarningOn )
		{
			if( !(MWarn & MW_CHOPKNIVPOS1) )
			{
				MWarn |= MW_CHOPKNIVPOS1;
				OKWarnBox = EXCL;

				// Sounding 1 beep when message first appears
				buzz_count = 1;
				buzz_length = 250;
			}
		}
	}


	// -----------------------------------------------------------
	// Low Battery (AKA Low voltage) Warning

	if( SupplyVolts < 10.5 )
	{
		if( !(MWarn & MW_LOWBAT) )
		{
			MWarn |= MW_LOWBAT;
			OKWarnBox = EXCL;

			// Sounding 1 beep when message first appears
			buzz_count = 1;
			buzz_length = 250;
		}
	}
	else // SupplyVolts >= 10.5
	{
		if( MWarn & MW_LOWBAT )
		{
			MWarn &= ~MW_LOWBAT;
			OKWarnBox &= ~EXCL;
		}
	}


	// -----------------------------------------------------------
	// Lube Count Alarm

	if( pRun.pr.LubeCounter <= 0 )
	{
		if( !MWarn && !(OKWarnBox & LUBE) ) // Lube count warning has lowest priority
		{
			// Oil can icon shown instead of showing total net applied in metres in screens.c

			OKWarnBox = LUBE;

			// Sounding 1 beep when message first appears
			buzz_count = 1;
			buzz_length = 250;

			BeepDelay = 0;
		}

		// Beeping sounds continuously
		if( BeepDelay )
		{
			BeepDelay--;
		}
		else
		{
			BeepDelay = 29;
			buzz_count = 1;
		}
	}
	else // pRun.pr.LubeCounter > 0
	{
		if( OKWarnBox == LUBE )
		{
			OKWarnBox &= ~LUBE;
			draw_box( adm, 2+2, 11+2, 30+2, 32+2, CLEAR );
		}
	}

}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : Sequence
// -----------------------------------------------------------
void Sequence( void )
{

	static unsigned char Kniv1_IPOpenTime;
	static unsigned long int LastNetPulseCount;


	if( AllTime ) // Incremented every 100 msec in TC1_irq_handler; so this routine is executed once every 100msec
	{
		AllTime--;

		if( AutoMan )
		{

			// -----------------------------------------------------------
			// State machine:
			// - only runs in AUTO mode -- this mode cannot be selected if chamber closed

			switch( Stage )
			{

			// -----------------------------------------------------------
			// State: 0

			case STAGE_START:

				if( StartAutoNet ) // Start Netting button pressed
				{
					Stage = STAGE_NETTING_START; // Go straight to Netting stage
				}
				else // Go to Formation stage
				{
					SequenceInProgress = 1;

					OP8OnDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
					OP8OffDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
					Stage = STAGE_FORMATION; // Go to Formation stage

				}
				break;


			// -----------------------------------------------------------
			// State: 1

			case STAGE_FORMATION:

				if( StartAutoNet ) // Start Netting button pressed
				{
					Stage = STAGE_NETTING_START; // Go straight to Netting stage
				}
				else // Continuing Formation stage
				{
					SequenceInProgress = 1;

					if( BaleDiaVolts > DiaFormationVolts )  // Formation diameter exceeded
					{
						OP8OnDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
						OP8OffDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
						Stage = STAGE_FORMATION2; // Go to Formation2 stage
					}
					else  // Formation diameter not yet reached
					{
						// -----------------------------------------------------------
						// Density Control Function -- based on Formation Density

						if( DensityPresVolts >= DensityFormationVolts ) // Target Formation density reached/exceeded
						{
							if( OP8OnDelayTime )
								OP8OnDelayTime--;
							else // Pres. Sw. delay has expired
							{
								SeqOpOn |= OP_DENSITY_REL; // OP8 ON: Density Release Solenoid (AKA Density Control)
								OP8OffDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
							}
						}
						else if( DensityPresVolts < (DensityFormationVolts - pFac.pf.GW302_PresDeadband) ) // Fallen below deadband below target Formation density
						{
							if( OP8OffDelayTime )
								OP8OffDelayTime--;
							else // Pres. Sw. delay has expired
							{
								AllSeqOpOff(); // All OPs OFF
								OP8OnDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
							}
						}
					}
				}
				break;


			// -----------------------------------------------------------
			// State: 2

			case STAGE_FORMATION2:

				if( StartAutoNet ) // Start Netting button pressed
				{
					Stage = STAGE_NETTING_START; // Go straight to Netting stage
				}
				else // Continuing Formation2 stage
				{
					SequenceInProgress = 1;

					if( BaleDiaVolts > DiaFormation2Volts )  // Formation2 diameter exceeded
					{
						OP8OnDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
						OP8OffDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
						Stage = STAGE_CORE; // Go to Core stage
					}
					else  // Formation2 diameter not yet reached
					{
						// -----------------------------------------------------------
						// Density Control Function -- based on Formation2 Density

						if( DensityPresVolts >= DensityFormation2Volts ) // Target Formation2 density reached/exceeded
						{
							if( OP8OnDelayTime )
								OP8OnDelayTime--;
							else // Pres. Sw. delay has expired
							{
								SeqOpOn |= OP_DENSITY_REL; // OP8 ON: Density Release Solenoid (AKA Density Control)
								OP8OffDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
							}
						}
						else if( DensityPresVolts < (DensityFormation2Volts - pFac.pf.GW302_PresDeadband) ) // Fallen below deadband below target Formation2 density
						{
							if( OP8OffDelayTime )
								OP8OffDelayTime--;
							else // Pres. Sw. delay has expired
							{
								AllSeqOpOff(); // All OPs OFF
								OP8OnDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
							}
						}
					}
				}
				break;


			// -----------------------------------------------------------
			// State: 3

			case STAGE_CORE:

				if( StartAutoNet ) // Start Netting button pressed
				{
					Stage = STAGE_NETTING_START; // Go straight to Netting stage
				}
				else // Continuing Core stage
				{
					SequenceInProgress = 1;

					if( BaleDiaVolts > DiaCoreVolts )  // Core diameter exceeded
					{
						OP8OnDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
						OP8OffDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
						Stage = STAGE_MAIN; // Go to Main stage
					}
					else  // Core diameter not yet reached
					{
						// -----------------------------------------------------------
						// Density Control Function -- based on Bale Core Density

						if( DensityPresVolts >= DensityCoreVolts ) // Target core density reached/exceeded
						{
							if( OP8OnDelayTime )
								OP8OnDelayTime--;
							else // Pres. Sw. delay has expired
							{
								SeqOpOn |= OP_DENSITY_REL; // OP8 ON: Density Release Solenoid (AKA Density Control)
								OP8OffDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
							}
						}
						else if( DensityPresVolts < (DensityCoreVolts - pFac.pf.GW302_PresDeadband) ) // Fallen below deadband below target core density
						{
							if( OP8OffDelayTime )
								OP8OffDelayTime--;
							else // Pres. Sw. delay has expired
							{
								AllSeqOpOff(); // All OPs OFF
								OP8OnDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
							}
						}
					}
				}
				break;


			// -----------------------------------------------------------
			// State: 4

			case STAGE_MAIN:

				if( StartAutoNet ) // Start Netting button pressed
				{
					Stage = STAGE_NETTING_START; // Go straight to Netting stage
				}
				else // Continuing Main stage
				{
					SequenceInProgress = 1;

					if( BaleDiaVolts > DiaTargetVolts )  // Target bale diameter exceeded
					{
						Stage = STAGE_NETTING_START; // Go to Netting stage
					}
					else  // Target bale diameter not yet reached
					{
						// Density Control Function (based on Target Bale Density) activated -- turned on
						DensityControlFunctionActive = 1;

					}

					// Roller Engage Solenoid (OP5) turned on now/later when Engage diameter reached
					if( BaleDiaVolts >= DiaEngageVolts ) // Engage diameter reached
					{
						if( pFac.pf.GW302_NetSystem == NS_ROLLER )
						{

							RollEngageTurnedOn = 1; // OP5 ON: Roller Engage Solenoid (AKA Pre Net Position)

						}
						else //  pFac.pf.GW302_NetSystem == NS_BEAK
						{
							// Roller Engage Solenoid (OP5) turned on after 'Beak Engage' pulses counted on IP4
							// - Counting starts from now but checking only starts after OP2 (Net On) has come on
							BeakEngagePulseCount = 0UL; // Resetting IP4 'Beak Engage' pulse count
							OP5TurnedOnAfterBeakEngagePulsesEnabled = 1;
						}
					}
				}
				break;


			// -----------------------------------------------------------
			// State: 5

			case STAGE_NETTING_START:

				if( StartAutoNet ) // Start Netting button pressed
				{
					// Pausing at start of Netting stage while button held

					// -----------------------------------------------------------
					// Roller Engage Solenoid (OP5) turned on now/later while Net On button 2 pressed

					if( pFac.pf.GW302_NetSystem == NS_ROLLER )
					{

						RollEngageTurnedOn = 1; // OP5 ON: Roller Engage Solenoid (AKA Pre Net Position)

					}
					else //  pFac.pf.GW302_NetSystem == NS_BEAK
					{
						// Roller Engage Solenoid (OP5) turned on after 'Beak Engage' pulses counted on IP4
						// - Counting starts from now but checking only starts after OP2 (Net On) has come on
						OP5TurnedOnAfterBeakEngagePulsesEnabled = 1;
					}
				}
				else // Continuing with this stage
				{
					SequenceInProgress = 1;

					// Density Control Function active/not according to Target Bale Density ID
					if( pFac.pf.GW302_BaleDensity[pFac.pf.GW302_BaleProfile] <= 5 ) // Target Bale Density ID is 1 - 5
					{
						DensityControlFunctionActive = 1; // Density Control Function (based on Target Bale Density) active / remains active
					}
					else // Target Bale Density ID is 6 - 10
					{
						DensityControlFunctionActive = 0; // Density Control Function (based on Target Bale Density) not active -- turned off

						// Ensuring OP8 is turned off
						SeqOpOn &= ~OP_DENSITY_REL; // OP8 OFF: Density Release Solenoid (AKA Density Control)
						SeqOpOff |= OP_DENSITY_REL;
					}

					// Next stage
					OP2OnDelayTime = (unsigned char)(pFac.pf.GW302_NetDelay * 10.0);
					Stage = STAGE_NETTING_NET_DELAY; // Go to Net Delay stage
				}
				break;


			// -----------------------------------------------------------
			// State: 6

			case STAGE_NETTING_NET_DELAY:

				if( StartAutoNet ) // Start Netting button pressed
				{
					// Pausing while button held
					OP2OnDelayTime = 0; // Net Delay (t1) set to 0; hence, on key release go to Netting stage -- Net On
				}
				else // Continuing with this stage
				{
					SequenceInProgress = 1;

					// Intermittent beep for duration of Net Delay time (t1) -- unless disabled by IP3 opening
					if( !NettingStartBeepsDisabledWhileIP3OpenInAutoMode )
					{
						// Intermittent beep for duration of Net Delay time (t1)
						buzz_count = 1;
					}

					if( OP2OnDelayTime )
						OP2OnDelayTime--;
					else // Net Delay (t1) has expired
					{
						SeqOpOn |= OP_NET_ON;   // OP2 ON: Net Clutch (AKA Net On)
						stagecomp |= NETFEEDON; // Set NET FEED ON stage completed

						// Roller Engage Solenoid (OP5) turned on after 'Beak Engage' pulses counted on IP4
						// - Counting starts from now but checking only starts after OP2 (Net On) has come on
						BeakEngagePulseCount = 0UL; // Resetting IP4 'Beak Engage' pulse count

#ifdef DEVELOP // Testing pulse counting
						SavedBeakEngagePulseCount = 0UL;  // Resetting
#endif

						// Continuous beep for 3s (when OP2 turns on) -- unless disabled by IP3 opening
						if( !NettingStartBeepsDisabledWhileIP3OpenInAutoMode )
						{
							if( !cont_beep )
							{
								buzz_length = NETTING_START_BEEP; // Long continuous beep when netting starts in AUTO mode
								cont_beep = 1;
							}
						}

						// Calculating Target Net Pulses for current bale diameter (expected to be just above Target Bale Diameter Voltage)
						CalculateTargetNetPulses();

						// Resetting IP4 net pulse count (when OP2 turns on)
						NetPulseCount = 0UL;

						// Start checking for no pulses detected on IP4 for NetWarningDelayTime since
						// OP2 turned on or since the last pulse counted
						if (pFac.pf.GW302_NetTwineOption == TO_NET)
							NetPulseCheckingEnabled = 1;

						LastNetPulseCount = NetPulseCount;
						NetWarningDelayTime = (unsigned char)(pFac.pf.GW302_NoNetPulse * 10.0); // Delay before Check Net Error (AKA Net Warning) appears

						// Next stage
						OP2OffDelayTime = (unsigned char)(pFac.pf.GW302_NetOn * 10.0); // Total amount of time OP2 is to remain on
						Stage = STAGE_NETTING_NET_ON; // Go to Netting stage -- Net On
					}
				}
				break;


			// -----------------------------------------------------------
			// State: 7

			case STAGE_NETTING_NET_ON:

				if( StartAutoNet ) // Start Netting button pressed
				{
					// Pausing while button held
					OP2OffDelayTime = 0;  // Net On (t2) set to 0; hence on key release turn off OP2 & go to STAGE_NETTING_WAIT_REQ_NET_PULSES
					NetWarningOn = 0;     // Check Net Error (AKA Net Warning) turned off -- if on
				}
				else // Continuing with this stage
				{
					SequenceInProgress = 1;

					if( OP2OffDelayTime )
					{
						if( NetPulseFrequency > pFac.pf.GW302_NetSpeed ) // Net Pulse Frequency exceeds Net Speed
						{
							SeqOpOn &= ~OP_NET_ON; // OP2 OFF: Net Clutch (AKA Net On)
							SeqOpOff |= OP_NET_ON;
							OP2OffDelayTime = 0; // Net On (t2) expires
						}
						else // Net Pulse Frequency at/below Net Speed
						{
							OP2OffDelayTime--;
						}
					}
					else // Net On (t2) has expired
					{
						SeqOpOn &= ~OP_NET_ON; // OP2 OFF: Net Clutch (AKA Net On)
						SeqOpOff |= OP_NET_ON;

						Stage = STAGE_NETTING_WAIT_REQ_NET_PULSES; // Go to Netting stage -- waiting for Target Net Pulses
					}
				}
				break;


			// -----------------------------------------------------------
			// State: 8

			case STAGE_NETTING_WAIT_REQ_NET_PULSES:

				if( StartAutoNet ) // Start Netting button pressed
				{
					// Pausing while button held
					// OP2 is turned on & kept on while key held down
					NetWarningOn = 0;     // Check Net Error (AKA Net Warning) turned off -- if on
				}
				else // Continuing with this stage
				{
					SequenceInProgress = 1;

					if(( NetPulseCount > (unsigned long int)TargetNetPulses ) || (pFac.pf.GW302_NetTwineOption == TO_TWINE)) // Target Net Pulses counted on IP4
					{
						// Density Control Function active/not according to Target Bale Density ID
						if( pFac.pf.GW302_BaleDensity[pFac.pf.GW302_BaleProfile] <= 5 ) // Target Bale Density ID is 1 - 5
						{
							DensityControlFunctionActive = 0; // Density Control Function (based on Target Bale Density) not active -- turned off

							// Ensuring OP8 is turned off
							SeqOpOn &= ~OP_DENSITY_REL; // OP8 OFF: Density Release Solenoid (AKA Density Control)
							SeqOpOff |= OP_DENSITY_REL;
						}

						if(pFac.pf.GW302_NetTwineOption == TO_NET)
							SeqOpOn |= OP_NET_CUT; // OP7 ON: Net Cut Motor (AKA Net Cut Trip)

						stagecomp |= NETCUTON; // Set NET CUT ON stage completed

#ifdef DEVELOP // Testing pulse counting
						Saved_NetPulseCount = NetPulseCount;
						SavedPreNetPulseCount = 0UL;   // Resetting
						TargetPreNetPulseCount = (unsigned long int)pFac.pf.GW302_PreNetPulse;
#endif

						// Next stage
						OP7OffDelayTime = (unsigned char)(pFac.pf.GW302_NetCutTime * 10.0);
						IP4PulsesStoppedCount = IP4_PULSES_STOPPED_COUNT_DURATION; // 1.5 s
						NetPulseCountAtLastCheck = NetPulseCount; // Pulse count saved
						Stage = STAGE_NETTING_NET_CUT_ON; // Go to Netting stage -- Net Cut On

						// Stop checking for no pulses detected on IP4 for NetWarningDelayTime
						NetPulseCheckingEnabled = 0;

						// Stop checking for 'Beak Engage' Pulses counted on IP4
						OP5TurnedOnAfterBeakEngagePulsesEnabled = 0;
					}
				}
				break;


			// -----------------------------------------------------------
			// State: 9

			case STAGE_NETTING_NET_CUT_ON:

				SequenceInProgress = 1;

				if( OP7OffDelayTime || IP4PulsesStoppedCount )
				{
					if( OP7OffDelayTime )
					{
						OP7OffDelayTime--;
					}

					if( IP4PulsesStoppedCount )
					{
						if( NetPulseCount == NetPulseCountAtLastCheck ) // Net Pulses cease to be detected on IP4 after Target Net Pulses reached
								{
							IP4PulsesStoppedCount--;
								}
						else // Net Pulses continue to be detected on IP4 after Target Net Pulses reached
						{
							IP4PulsesStoppedCount = IP4_PULSES_STOPPED_COUNT_DURATION; // Resetting to 1.5 s
						}
						NetPulseCountAtLastCheck = NetPulseCount; // Net Pulse count (for what will be the last check) saved
					}
				}

				// Net Cut Time (t5) has expired OR No Net Pulses detected on IP4 for > 1 s OR Net Cut Sensor closed (if fitted)
				if( !OP7OffDelayTime || !IP4PulsesStoppedCount || NET_CUT_SENSOR_CLOSED || (pFac.pf.GW302_NetTwineOption == TO_TWINE))
				{
					RollEngageTurnedOn = 0;     // OP5 Roller Engage Solenoid (AKA Pre Net Position) turned off
					AllSeqOpOff(); // All OPs OFF

					buzz_length = PROMPT_TO_OPEN_DOOR_BEEP; // Beep to prompt operator to open the chamber door & unload the bale

					Stage = STAGE_NETTING_END; // Go to end of Netting stage
				}
				break;


			// -----------------------------------------------------------
			// State: 10 (Hex A)

			case STAGE_NETTING_END:

				SequenceInProgress = 1;

				if( CHAMBER_OPEN ) // Chamber Locked Sensor open
				{
					stagecomp |= CHAMBOP;      // Set CHAMBER OPEN stage completed

					buzz_length = AUTO_CONFIRM_DOOR_OPENED_BEEP; // Beep to confirm chamber door has been opened

					Stage = STAGE_UNLOADING;   // Go to Unloading stage
				}
				break;


			// -----------------------------------------------------------
			// State: 11 (Hex B)

			case STAGE_UNLOADING:

				SequenceInProgress = 1;

				if( CHAMBER_CLOSED ) // Chamber Locked Sensor closed
				{
					stagecomp |= CHAMBCLO;  // Set CHAMBER CLOSED stage completed

					buzz_length = AUTO_CONFIRM_DOOR_CLOSED_BEEP; // Beep to confirm chamber door has been closed

					// Next stage
					ChamberClosedDelayTime = (unsigned char)(pFac.pf.GW302_DoorClosed * 10.0);
					Stage = STAGE_UNLOADING_WAIT;      // Go to Unloading Wait stage
				}
				else // Chamber Locked Sensor still open
				{
					// Looking for Bale Kicker to go down then up
					if( BALE_KICKER_DOWN ) // IP5: Bale Kicker Sensor open
					{
						BaleKickerStatus |= BKS_BALE_KICKER_DOWN; // Set BKS_BALE_KICKER_DOWN flag
						BaleKickerStatus &= ~(BKS_BALE_KICKER_UP); // Flag to be set next
					}
					else if( BALE_KICKER_UP ) // IP5: Bale Kicker Sensor closed
					{
						if( BaleKickerStatus == BKS_BALE_KICKER_DOWN ) // BKS_BALE_KICKER_DOWN flag already set
						{
							BaleKickerStatus |= BKS_BALE_KICKER_UP; // Set BKS_BALE_KICKER_UP flag

							buzz_length = AUTO_CONFIRM_BALE_UNLOADED_BEEP; // Beep to confirm bale has been unloaded
							// Image of bale shown on the display disappears
						}
					}
				}
				break;


			// -----------------------------------------------------------
			// State: 12 (Hex C)

			case STAGE_UNLOADING_WAIT:

				SequenceInProgress = 1;

				if( ChamberClosedDelayTime )
					ChamberClosedDelayTime--;
				else // Chamber/Door closed delay has expired
				{
					AllSeqOpOff();          // All OPs OFF

					Stage = STAGE_END;      // Go to End stage
				}
				break;


			// -----------------------------------------------------------
			// State: 13 (Hex D)

			case STAGE_END:

				SequenceInProgress = 1;

				if( stagecomp == (NETFEEDON | NETCUTON | CHAMBOP | CHAMBCLO) ) // all stages completed
						{
					stagecomp = SC_CLEAR; // Reset

					UpdatingAndSavingTotalsToMemory();

					buzz_length = ALL_STAGES_COMPLETE_BEEP; // Beep on completion of all stages
						}

				if( (pFac.pf.GW302_PreNetPulse > 0) && (pFac.pf.GW302_NetTwineOption == TO_NET))
				{
					SeqOpOn |= OP_NET_ON;         // OP2 ON: Net Clutch (AKA Net On)

					// Resetting IP4 net pulse count (to count Pre Net Pulses)
					NetPulseCount = 0UL;

					// Start checking for no pulses detected on IP4 for NetWarningDelayTime since
					// start of checking or since the last pulse counted
					NetPulseCheckingEnabled = 1;
					LastNetPulseCount = NetPulseCount;
					NetWarningDelayTime = (unsigned char)(pFac.pf.GW302_NoNetPulse * 10.0); // Delay before Check Net Error (AKA Net Warning) appears

					// Next stage
					Stage = STAGE_PRE_NET_PULSES; // Go to Pre Net Pulses stage
				}
				else // No Pre Net Pulses
				{
					// Finish sequence
					ResetSequenceFactors( SEQ_ALL ); // Turns off all OPs & goes to start of sequence
				}
				break;


			// -----------------------------------------------------------
			// State: 14 (Hex E)

			case STAGE_PRE_NET_PULSES:

				SequenceInProgress = 1;

				if( NetPulseCount >= (unsigned long int)pFac.pf.GW302_PreNetPulse ) // Sufficient Pre Net Pulses counted on IP4
				{
#ifdef DEVELOP // Testing pulse counting
					SavedPreNetPulseCount = NetPulseCount;
#endif
					// Stop checking for no pulses detected on IP4 for NetWarningDelayTime
					NetPulseCheckingEnabled = 0;

					// Finish sequence
					ResetSequenceFactors( SEQ_ALL );  // Turns off all OPs & goes to start of sequence
				}
				break;

			} // end switch( Stage )


			// -----------------------------------------------------------
			// Density Control Function -- based on Target Bale Density (AUTO)

			if( DensityControlFunctionActive )
			{
				if( DensityPresVolts >= DensityBaleVolts ) // Target bale density reached/exceeded
				{
					if( OP8OnDelayTime )
						OP8OnDelayTime--;
					else // Pres. Sw. delay has expired
					{
						SeqOpOn |= OP_DENSITY_REL; // OP8 ON: Density Release Solenoid (AKA Density Control)
						SeqOpOff &= ~OP_DENSITY_REL;
						OP8OffDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
					}
				}
				else if( DensityPresVolts < (DensityBaleVolts - pFac.pf.GW302_PresDeadband) ) // Fallen below deadband below target bale density
				{
					if( OP8OffDelayTime )
						OP8OffDelayTime--;
					else // Pres. Sw. delay has expired
					{
						SeqOpOn &= ~OP_DENSITY_REL; // OP8 OFF: Density Release Solenoid (AKA Density Control)
						SeqOpOff |= OP_DENSITY_REL;
						OP8OnDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
					}
				}
			} // end if( DensityControlFunctionActive )


			// -----------------------------------------------------------
			// Checking for no pulses detected on IP4 for NetWarningDelayTime (AUTO)

			if( (NetPulseCheckingEnabled) && (pFac.pf.GW302_NetTwineOption == TO_NET) ) // Flag set from when OP2 (Net Clutch) turned on up to when OP7 (Net Cut Motor) turned on
			{
				if( (NetPulseCount == 0UL) || (NetPulseCount == LastNetPulseCount) ) // No pulses detected on IP4 for NetWarningDelayTime
				{
					if( NetWarningDelayTime )
					{
						NetWarningDelayTime--;
					}
					else // NetWarningDelayTime expired
					{
						if(pFac.pf.GW302_NetTwineOption == TO_NET) /**/
							NetWarningOn = 1; // Check Net Error (AKA Net Warning) turned on
					}
				}
				else // Pulses detected on IP4
				{
					NetWarningOn = 0; // Check Net Error (AKA Net Warning) turned off
					LastNetPulseCount = NetPulseCount;
					NetWarningDelayTime = (unsigned char)(pFac.pf.GW302_NoNetPulse * 10.0); // Delay before Check Net Error (AKA Net Warning) appears
				}

				// Turning Net Warning off while IP3 is open
				if( NetWarningDisabledWhileIP3OpenInAutoMode )
				{
					NetWarningOn = 0; // Check Net Error (AKA Net Warning) turned off
				}

			} // end if( NetPulseCheckingEnabled )


			// -----------------------------------------------------------
			// Roller Engage Solenoid (OP5) turned on after 'Beak Engage' Pulses counted on IP4 -- Only applies if 'Net System' set to 'Beak' (AUTO)

			if( OP5TurnedOnAfterBeakEngagePulsesEnabled ) // Flag set from when Engage diameter reached (or Net On button 2 pressed) up to when OP7 (Net Cut Motor) turned on
			{
				if( !NetWarningOn ) // Disabled while Check Net Error (AKA Net Warning) active
				{
					if( pFac.pf.GW302_NetSystem == NS_BEAK ) // Only applies if 'Net System' set to 'Beak' (Double checking)
					{
						if( stagecomp == NETFEEDON ) // Only checks for 'Beak Engage' pulses after OP2 (Net On) has come on (Double checking -- Pulses not expected before)
						{
							if( BeakEngagePulseCount >= (unsigned long int)pFac.pf.GW302_BeakEngage ) // 'Beak Engage' Pulses counted on IP4
							{
#ifdef DEVELOP // Testing pulse counting
								if( !RollEngageTurnedOn )
									SavedBeakEngagePulseCount = BeakEngagePulseCount;
#endif
								RollEngageTurnedOn = 1; // OP5 ON: Roller Engage Solenoid (AKA Pre Net Position)

							}
						}
					}
				}
			} // end if( OP5TurnedOnAfterBeakEngagePulsesEnabled )



			// -----------------------------------------------------------
			// Roller Engage Solenoid (OP5) turned on/off (AUTO)

			if( NetWarningOn ) // Check Net Error (AKA Net Warning) active (AUTO)
			{
				SeqOpOn &= ~OP_ROLL_ENGAGE; // OP5 OFF: Roller Engage Solenoid (AKA Pre Net Position)
				SeqOpOff |= OP_ROLL_ENGAGE;
				RollerTimer = 0;
			}
			else if( RollEngageTurnedOn )
			{
				/*if(pFac.pf.GW302_NetTwineOption == TO_TWINE)
				{*/
				RollerTimer = OP_ROLL_ENGAGE_TIMER; /*3 seconds*/
				/*}
				else
				{
					SeqOpOn |= OP_ROLL_ENGAGE; // OP5 ON: Roller Engage Solenoid (AKA Pre Net Position)
					SeqOpOff &= ~OP_ROLL_ENGAGE;
				}*/

			}





			// -----------------------------------------------------------
			// Pre Warning beep (AUTO) -- unless disabled by IP3 opening

			if(  ( !NettingStartBeepsDisabledWhileIP3OpenInAutoMode )
			  && ( BaleDiaVolts > DiaPreWarnVolts ) // Pre warning beep bale diameter exceeded
			  && ( pFac.pf.GW302_PreWarningBeep > 0 ) // Pre Warning beep on (set to non-zero value)
			  && ( !pre_warning_cont_beep ) )
			{
				buzz_length = PRE_WARNING_BEEP; // Pre Warning beep
				pre_warning_cont_beep = 1;      // Only repeated after a sequence is completed

			}


			// -----------------------------------------------------------
			// Beeping if chamber open > 1 s outside unloading (IP3 opens & closes) (AUTO)

			if( (Stage != STAGE_NETTING_END)    &&    // Ignoring chamber opening between the start & end of unloading
					(Stage != STAGE_UNLOADING)      &&
					(Stage != STAGE_UNLOADING_WAIT)    )
			{
				if( CHAMBER_OPEN ) // Chamber Locked Sensor open
				{
					// Beeping
					if( ChamberOpenBeepDelay )
					{
						ChamberOpenBeepDelay--;
					}
					else
					{
						// Intermittent beeping while chamber open
						buzz_length = AUTO_CHAMBER_OPEN_BEEP;
						ChamberOpenBeepDelay = COBD_CHAMBER_OPEN_BEEP_DELAY;  // Reset chamber open beep delay to beep at 1 s intervals
					}
				}
				else // Chamber closed
				{
					ChamberOpenBeepDelay = COBD_CHAMBER_OPEN_BEEP_DELAY; // to ensure beeping starts after 1 s delay
				}
			}


			// -----------------------------------------------------------
			// Turning off OP2 & OP5 if chamber opens outside of unloading (AUTO)

			if( (Stage != STAGE_NETTING_END)    &&    // Ignoring chamber opening between the start & end of unloading
					(Stage != STAGE_UNLOADING)      &&
					(Stage != STAGE_UNLOADING_WAIT)    )
			{
				if( CHAMBER_OPEN ) // Chamber Locked Sensor open
				{
					// Saving OP5 flags if not yet saved
					if( !SavedRollEngageTurnedOn )
					{
						SavedRollEngageTurnedOn = RollEngageTurnedOn;
					}
					if( !SavedOP5TurnedOnAfterBeakEngagePulsesEnabled )
					{
						SavedOP5TurnedOnAfterBeakEngagePulsesEnabled = OP5TurnedOnAfterBeakEngagePulsesEnabled;
					}

					// Ensuring OP5 Roller Engage Solenoid is off while Chamber Locked Sensor open
					RollEngageTurnedOn = 0;                      // OP5 Roller Engage Solenoid (AKA Pre Net Position) turned off
					OP5TurnedOnAfterBeakEngagePulsesEnabled = 0; // Turning on Roller Engage Solenoid (OP5) after 'Beak Engage' pulses counted on IP4
					SeqOpOn &= ~OP_ROLL_ENGAGE; // OP5 OFF: Roller Engage Solenoid (AKA Pre Net Position)
					SeqOpOff |= OP_ROLL_ENGAGE;

					// Ensuring OP2 Net Clutch is off while Chamber Locked Sensor open
					SeqOpOn &= ~OP_NET_ON; // OP2 OFF: Net Clutch (AKA Net On)
					SeqOpOff |= OP_NET_ON;
				}
				else // Chamber closed
				{
					// Restoring OP5 flags if saved
					if( SavedRollEngageTurnedOn )
					{
						RollEngageTurnedOn = SavedRollEngageTurnedOn;
						SavedRollEngageTurnedOn = 0; // Reset
					}
					if( SavedOP5TurnedOnAfterBeakEngagePulsesEnabled )
					{
						OP5TurnedOnAfterBeakEngagePulsesEnabled = SavedOP5TurnedOnAfterBeakEngagePulsesEnabled;
						SavedOP5TurnedOnAfterBeakEngagePulsesEnabled = 0; // Reset
					}
				}
			}
			else // Between start & end of unloading -- Netting finished before unloading OR waiting for sufficient pre net pulses to be counted after unloading
			{
				RollEngageTurnedOn = 0; // Reset
				SavedRollEngageTurnedOn = 0; // Reset

				OP5TurnedOnAfterBeakEngagePulsesEnabled = 0; // Reset
				SavedOP5TurnedOnAfterBeakEngagePulsesEnabled = 0; // Reset
			}


			// -----------------------------------------------------------
			// Disabling Net Warning & Netting Start Beeps (incl. Pre Warning & Net Delay Beeps) if chamber opens outside of unloading (AUTO)

			if( (Stage != STAGE_NETTING_END)    &&    // Ignoring chamber opening between the start & end of unloading
					(Stage != STAGE_UNLOADING)      &&
					(Stage != STAGE_UNLOADING_WAIT)    )
			{
				if( CHAMBER_OPEN ) // Chamber Locked Sensor open
				{
					NetWarningDisabledWhileIP3OpenInAutoMode = 1;
					NettingStartBeepsDisabledWhileIP3OpenInAutoMode = 1; // Including Pre Warning & Net Delay beep
				}
				else // Chamber closed
				{
					NetWarningDisabledWhileIP3OpenInAutoMode = 0; // Reset
					NettingStartBeepsDisabledWhileIP3OpenInAutoMode = 0; // Reset
				}
			}
			else // Between start & end of unloading -- Netting finished before unloading OR waiting for sufficient pre net pulses to be counted after unloading
			{
				NetWarningDisabledWhileIP3OpenInAutoMode = 0; // Reset
				NettingStartBeepsDisabledWhileIP3OpenInAutoMode = 0; // Reset
			}

		}
		else // MAN mode
		{
			// -----------------------------------------------------------
			// Monitoring bale density (MAN)

			if( BaleDiaVolts > DiaCoreVolts )  // Core diameter exceeded
			{
				// -----------------------------------------------------------
				// Density Control Function -- based on Target Bale Density

				if( DensityPresVolts >= DensityBaleVolts ) // Target bale density reached/exceeded
				{
					if( OP8OnDelayTime )
						OP8OnDelayTime--;
					else // Pres. Sw. delay has expired
					{
						SeqOpOn |= OP_DENSITY_REL; // OP8 ON: Density Release Solenoid (AKA Density Control)
						SeqOpOff &= ~OP_DENSITY_REL;
						OP8OffDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
					}
				}
				else if( DensityPresVolts < (DensityBaleVolts - pFac.pf.GW302_PresDeadband) ) // Fallen below deadband below target bale density
				{
					if( OP8OffDelayTime )
						OP8OffDelayTime--;
					else // Pres. Sw. delay has expired
					{
						SeqOpOn &= ~OP_DENSITY_REL; // OP8 OFF: Density Release Solenoid (AKA Density Control)
						SeqOpOff |= OP_DENSITY_REL;
						OP8OnDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
					}
				}
			}
			else  // Core diameter not yet reached
			{
				// -----------------------------------------------------------
				// Density Control Function -- based on Bale Core Density

				if( DensityPresVolts >= DensityCoreVolts ) // Target core density reached/exceeded
				{
					if( OP8OnDelayTime )
						OP8OnDelayTime--;
					else // Pres. Sw. delay has expired
					{
						SeqOpOn |= OP_DENSITY_REL; // OP8 ON: Density Release Solenoid (AKA Density Control)
						SeqOpOff &= ~OP_DENSITY_REL;
						OP8OffDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
					}
				}
				else if( DensityPresVolts < (DensityCoreVolts - pFac.pf.GW302_PresDeadband) ) // Fallen below deadband below target core density
				{
					if( OP8OffDelayTime )
						OP8OffDelayTime--;
					else // Pres. Sw. delay has expired
					{
						SeqOpOn &= ~OP_DENSITY_REL; // OP8 OFF: Density Release Solenoid (AKA Density Control)
						SeqOpOff |= OP_DENSITY_REL;
						OP8OnDelayTime = (unsigned char)(pFac.pf.GW302_PresSwDelay * 10.0);
					}
				}
			}


			// -----------------------------------------------------------
			// Monitoring bale size (MAN)

			if( BaleDiaVolts > DiaTargetVolts )  // Target bale diameter exceeded
			{
				if( BalerFullDelayTime )
				{
					BalerFullDelayTime--;

					// Intermittent beep for duration of Net Delay time (t1)
					buzz_count = 1;

					// Resetting to enable target beeps
					target_bale_dia_reached_cont_beep = 0;
				}
				else // Baler Full / Net Delay (t1) has expired
				{
					// Continuous beep for 3s (when OP2 turns on)
					if( !target_bale_dia_reached_cont_beep )
					{
						buzz_length = NETTING_START_BEEP; // Long continuous beep when netting can be started in MAN mode
						target_bale_dia_reached_cont_beep = 1;
					}
				}
			}
			else // Target bale diameter has not yet been reached
			{
				if( BaleDiaVolts > DiaPreWarnVolts ) // Pre warning beep bale diameter exceeded
				{
					if( pFac.pf.GW302_PreWarningBeep > 0 ) // Pre Warning beep on (set to non-zero value)
					{
						if( !pre_warning_cont_beep )
						{
							buzz_length = PRE_WARNING_BEEP; // Pre Warning beep
							pre_warning_cont_beep = 1;      // Only repeated after a sequence is completed
						}
					}
				}

				// Resetting to allow baler full timer to be restarted & enable target beeps
				BalerFullDelayTime = (unsigned char)(pFac.pf.GW302_NetDelay * 10.0);
				target_bale_dia_reached_cont_beep = 0;
			}


			// -----------------------------------------------------------
			// Stage completion (MAN)

			// First 2 stages completed
			if( stagecomp == (NETFEEDON | NETCUTON) )
			{
				if( CHAMBER_OPEN ) // Chamber Locked Sensor open
				{
					if( ChamberOpenDelayTime )
					{
						ChamberOpenDelayTime--;

						if( !start_chamber_open_delay_cont_beep )
						{
							buzz_length = MAN_START_END_CHAMBER_OPEN_DELAY_BEEP; // Beep at start of chamber open delay
							start_chamber_open_delay_cont_beep = 1;
						}
					}
					else // Chamber open delay has expired
					{
						stagecomp |= CHAMBOP;     // Set CHAMBER OPEN stage completed
						stagecomp &= ~(CHAMBCLO); // This stage is to be completed next

						if( !end_chamber_open_delay_cont_beep )
						{
							buzz_length = MAN_START_END_CHAMBER_OPEN_DELAY_BEEP; // Beep at end of chamber open delay
							end_chamber_open_delay_cont_beep = 1;
						}
					}
				}

				if( CHAMBER_CLOSED) // Chamber Locked Sensor closed
				{
					ChamberOpenDelayTime = CODT_CHAMBER_OPEN_TIME;  // Reset Chamber open delay
					start_chamber_open_delay_cont_beep = 0;
				}
			}

			// First 3 stages completed
			if( stagecomp == (NETFEEDON | NETCUTON | CHAMBOP) )
			{
				if( CHAMBER_CLOSED) // Chamber Locked Sensor closed
				{
					stagecomp |= CHAMBCLO;  // Set CHAMBER CLOSED stage completed
				}
			}

			// all stages completed (in the order indicated left to right)
			if( stagecomp == (NETFEEDON | NETCUTON | CHAMBOP | CHAMBCLO) )
			{
				stagecomp = SC_CLEAR; // Reset

				UpdatingAndSavingTotalsToMemory();

				buzz_length = ALL_STAGES_COMPLETE_BEEP; // Beep on completion of all stages

				ResetSequenceFactors( SEQ_ALL );
			}


			// -----------------------------------------------------------
			// Roller Engage Solenoid (OP5) turned on/off (MAN)

			if( pFac.pf.GW302_NetSystem == NS_ROLLER ) // Roller Engage Solenoid (OP5) only turned on/off if 'Net System' set to 'Roller'
			{
				// Turning Roller Engage Solenoid (OP5) on if Bale Diameter goes from below to above Engage diameter
				if( BaleDiaVolts < DiaEngageVolts ) // below Engage diameter
				{
					BaleDiameterStatusInManMode |= BDSIMM_BELOW_ENGAGE_DIA; // Set BDSIMM_BELOW_ENGAGE_DIA flag
					BaleDiameterStatusInManMode &= ~(BDSIMM_ABOVE_ENGAGE_DIA); // Flag to be set next
					RollEngageTurnedOnInManMode = 0; // OP5 OFF: Roller Engage Solenoid (AKA Pre Net Position) -- turned off in MAN mode
				}
				else // above Engage diameter
				{
					if( BaleDiameterStatusInManMode == BDSIMM_BELOW_ENGAGE_DIA ) // BDSIMM_BELOW_ENGAGE_DIA flag already set
					{
						BaleDiameterStatusInManMode |= BDSIMM_ABOVE_ENGAGE_DIA; // Set BDSIMM_ABOVE_ENGAGE_DIA flag

						if ((RollerTimer == 255) && (RollEngageTurnedOnInManMode == 0) )
						{
							RollEngageTurnedOnInManMode = 1; // OP5 ON: Roller Engage Solenoid (AKA Pre Net Position) -- turned on in MAN mode
							RollerTimer = OP_ROLL_ENGAGE_TIMER; /*3 seconds*/
						}

					}
				}

				// Turning Roller Engage Solenoid (OP5) off if OP7 (Net Cut) comes on -- at any time
				// Done in Button 2 code

				// Turning Roller Engage Solenoid (OP5) off if chamber closes and opens -- at any time
				if( ChamberStatus == (CS_CLOSED | CS_OPEN) ) // Chamber was closed and now opened
				{
					RollEngageTurnedOnInManMode = 0; // OP5 OFF: Roller Engage Solenoid (AKA Pre Net Position) -- turned off in MAN mode
				}

				// Turning Roller Engage Solenoid (OP5) on/off
/*				if( RollEngageTurnedOnInManMode )
				{
					KeyOpOn |=  OP_ROLL_ENGAGE; // OP5 ON: Roller Engage Solenoid (AKA Pre Net Position)
				}*/

			} // end if( pFac.pf.GW302_NetSystem == NS_ROLLER )

		} // end MAN mode


		/*if(pFac.pf.GW302_NetTwineOption == TO_TWINE)
		{*/
			if (RollerTimer && !(RollerTimer == 255))
			{
				RollEngageTurnedOn = 0;
				SeqOpOn |= OP_ROLL_ENGAGE; // OP5 ON: Roller Engage Solenoid (AKA Pre Net Position)
				SeqOpOff &= ~OP_ROLL_ENGAGE;
				RollerTimer--;
			}
			else if(!RollerTimer)
			{
				SeqOpOn &= ~OP_ROLL_ENGAGE; // OP5 OFF: Roller Engage Solenoid (AKA Pre Net Position)
				SeqOpOff |= OP_ROLL_ENGAGE;
				RollerTimer = 255;
			}
		/* } */


		// -----------------------------------------------------------
		// Check Chopping knives position (MAN/AUTO)

		if( CHOPPING_KNIVES_ON && // IP2 open: 11 Knife Sensor (AKA Chopping Knives position / Knives Position Sensor 1) in the ‘ON’ position
				( (((key & ~(SW_NEW | SW_REP)) != SW_IR) && ((key & ~(SW_NEW | SW_REP)) != SW_OR)) || SecondScreen ) ) // Inhibited when IR or OR button pressed while first screen displayed
		{
			Kniv1_IPClosed = 1;
			Kniv1_IPOpenTime = 0;
		}
		else // IP2 closed: 11 Knife Sensor in the ‘OFF’ position
		{
			if( Kniv1_IPClosed )
			{
				if( Kniv1_IPOpenTime < 250 )
					Kniv1_IPOpenTime++;

				if( Kniv1_IPOpenTime > 10 )
					ChoppingKnives1WarningOn = 1; // Chopping knives position Warning on
			}
		}


		// -----------------------------------------------------------
		// Check Chamber Locked Sensor (MAN/AUTO)

		// Looking for Chamber to close then open
		if( CHAMBER_CLOSED ) // IP3: Chamber Locked Sensor closed
		{
			ChamberStatus |= CS_CLOSED; // Set CS_CLOSED flag
			ChamberStatus &= ~(CS_OPEN); // Flag to be set next
		}
		else // CHAMBER_OPEN -- IP3: Chamber Locked Sensor open
		{
			if( ChamberStatus == CS_CLOSED ) // CS_CLOSED flag already set
			{
				ChamberStatus |= CS_OPEN; // Set CS_OPEN flag
				OP8OnCount = OP8_ON_COUNT_DURATION; // 6s -- Set count (or reset, if count down already underway)
			}
		}

		// After Chamber closes then opens, turn on Density Release Solenoid for the count
		if( OP8OnCount )
		{
			KeyOpOn |= OP_DENSITY_REL; // OP8 ON: Density Release Solenoid (AKA Density Control)

			OP8OnCount--;
		}


		// -----------------------------------------------------------
		// Check Bale Diameter (MAN/AUTO)

		// Looking for whether Pre warning beep bale diameter exceeded OR Engage diameter reached
		if( (BaleDiaVolts > DiaPreWarnVolts) || (BaleDiaVolts >= DiaEngageVolts) )
		{
			DrivingArrowBeepsDisabled = 1; // Driving Arrow Beeps turned off
		}


		// -----------------------------------------------------------
		// Outputs (MAN/AUTO)

		SeqOpOn = SeqOpOn & ~SeqOpOff; /* If an Op is to be turned off, dont turn it on */

		OpOff = KeyOpOff | SeqOpOff;
		/*AT91C_BASE_PIOA->PIO_SODR = OpOff;*/

		OpOn = KeyOpOn | SeqOpOn;
		/*AT91C_BASE_PIOA->PIO_CODR = OpOn;*/


	} // end if( AllTime )

}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : ResetSequenceFactors
// -----------------------------------------------------------
void ResetSequenceFactors( unsigned char seq )
{
	SeqOpOff = SeqOpOn;
	OpOff = SeqOpOff;
	SeqOpOff = 0; /* Nothing off */
	SeqOpOn = 0;  /* Nothing on */

	ContinueOnKeyRelease = 0;
	StartAutoNet = 0;

	Stage = STAGE_START;
	stagecomp = SC_CLEAR;

	cont_beep = 0;
	target_bale_dia_reached_cont_beep = 0;
	pre_warning_cont_beep = 0;
	start_chamber_open_delay_cont_beep = 0;
	end_chamber_open_delay_cont_beep = 0;

	DensityPressureTooHigh = 0; // Density Pressure Too High Warning

	SequenceInProgress = 0;

	OP2OnDelayTime  = 0;        // Net Clutch (AKA Net On)
	OP2OffDelayTime = 0;
	OP7OffDelayTime = 0;        // Net Cut Motor (AKA Net Cut Trip)
	OP8OnDelayTime  = 0;        // Density Release Solenoid (AKA Density Control)
	OP8OffDelayTime = 0;

	NetPulseCount = 0UL;        // Resetting IP4 net pulse count
	BeakEngagePulseCount = 0UL; // Resetting IP4 'Beak Engage' pulse count
	NewNetPulses = 0UL;         // Resetting new IP4 pulses

#ifdef DEVELOP // Testing pulse counting
	//Saved_NetPulseCount = 0UL;        // Resetting
	//SavedBeakEngagePulseCount = 0UL;  // Resetting
#endif

	NetWarningDelayTime = 0;    // Check Net Error (AKA Net Warning)
	NetWarningOn = 0;           // Check Net Error (AKA Net Warning) turned off

	RollEngageTurnedOn = 0;       // OP5 Roller Engage Solenoid (AKA Pre Net Position) turned off
	SavedRollEngageTurnedOn = 0;  // Saved when IP3 opens in AUTO mode
	NetWarningDisabledWhileIP3OpenInAutoMode = 0;
	NettingStartBeepsDisabledWhileIP3OpenInAutoMode = 0; // Including Pre Warning & Net Delay beep

	ChamberClosedDelayTime = 0; // Door Closed

	ChamberOpenDelayTime = 0;   // applies to MAN mode only

	ChamberOpenBeepDelay = COBD_CHAMBER_OPEN_BEEP_DELAY; // to ensure beeping starts after 1 s delay -- applies to AUTO mode only

	BaleKickerStatus = BKS_CLEAR;               // Reset Bale Kicker Down/Up flags
	BaleDiameterStatusInManMode = BDSIMM_CLEAR; // Reset Bale Diameter Below/Above Engage Diameter (in MAN mode) flags
	ChamberStatus = CS_CLEAR;                   // Reset Chamber Closed/Open flags

	OP8OnCount = 0;                 // Reset count for Density Release Solenoid (OP8) on after Chamber (IP3) closes then opens (MAN/AUTO)

	DrivingArrowBeepsDisabled = 0;  // Driving Arrow Beeps turned on

	DensityControlFunctionActive = 0;                 // Disabling (de-activating) Density Control Function (based on Target Bale Density)
	NetPulseCheckingEnabled = 0;                      // Disabling checking for no pulses on IP4 (for NetWarningDelayTime)
	OP5TurnedOnAfterBeakEngagePulsesEnabled = 0;      // Disabling turning on Roller Engage Solenoid (OP5) after 'Beak Engage' pulses counted on IP4
	SavedOP5TurnedOnAfterBeakEngagePulsesEnabled = 0; // Saved when IP3 opens in AUTO mode


/*	pFac.pf.GW302_TwineOption = TO_OFF;
	pFac.pf.GW302_NetTwineOption = TO_NET;*/

}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : AllSeqOpOff 
// -----------------------------------------------------------
void AllSeqOpOff( void )
{
	SeqOpOff = SeqOpOn;
	OpOff = SeqOpOff;
	SeqOpOff = 0; /* Nothing off */
	SeqOpOn = 0;  /* Nothing on */
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : UpdatingAndSavingTotalsToMemory
// -----------------------------------------------------------
void UpdatingAndSavingTotalsToMemory( void ) 
{        
	pFac.pf.BaleTotal[pFac.pf.BaleStore] += PF_GW302_BaleTotal[PF_STEP];
	if( pFac.pf.BaleTotal[pFac.pf.BaleStore] > PF_GW302_BaleTotal[PF_MAX] ) // Rolling over at max number of digits displayed
	{
		pFac.pf.BaleTotal[pFac.pf.BaleStore] = PF_GW302_BaleTotal[PF_MIN]; // 0UL
	}

	pFac.pf.GrandBaleTotal += PF_GW302_GrandBaleTotal[PF_STEP];
	if( pFac.pf.GrandBaleTotal > PF_GW302_GrandBaleTotal[PF_MAX] ) // Rolling over at max number of digits displayed
	{
		pFac.pf.GrandBaleTotal = PF_GW302_GrandBaleTotal[PF_MIN]; // 0UL
	}

	// Saving Net applied indicated by pulses counted for Netting stage of current sequence
	// and unless instrument was powered up at the start of the sequence,
	// any pulses counted since last save from previous sequence (including any pulses counted during Pre-Net stage)
	pFac.pf.GW302_TotalNetMetres += NetMetresAppliedForCount;
	if( pFac.pf.GW302_TotalNetMetres > PF_GW302_TotalNetMetres[PF_MAX] ) // Freezing Total Net Metres at a maximum (when reached)
	{
		pFac.pf.GW302_TotalNetMetres = PF_GW302_TotalNetMetres[PF_MAX];
	}
	NetMetresPulseCount = 0UL; // Resetting IP4 pulse count for Net applied (in metres)

	pRun.pr.LubeCounter--; // pRun.pr.LubeCounter allowed to go negative

	SavecalDelayOnMainDisplay = 1;
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : CalculateNetPulseFrequency
// Object              : Calculates net pulse frequency (Hz)
// Output Parameters   : NetPulseFrequency
// -----------------------------------------------------------
void CalculateNetPulseFrequency( void )
{
	if( f6time )
		NetPulseFrequency = (float)f6count * (float)(MCK / 128) / (float)f6time;
	else
		NetPulseFrequency = 0.0;
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : CalculateNetPulsesPerLayer
// Object              : Calculates net pulses per layer
// Output Parameters   : NetPulsesPerLayer
// -----------------------------------------------------------
void CalculateNetPulsesPerLayer( void ) 
{
	float LinearCharacteristicGradient; // [m]
	float LinearCharacteristicOffset; // [c]

	// Net Pulses Per Layer [y] = function( Bale Diameter Voltage minus Minimum Bale Diameter Voltage [x] )
	// where function has a linear characteristic of the general form: y = mx + c

	LinearCharacteristicGradient = (pFac.pf.GW302_MaxDiaPulsPerLay - pFac.pf.GW302_MinDiaPulsPerLay) / (DiaMaxVolts - DiaMinVolts);
	LinearCharacteristicOffset = pFac.pf.GW302_MinDiaPulsPerLay;

	NetPulsesPerLayer = ( LinearCharacteristicGradient * (BaleDiaVolts - DiaMinVolts) ) + LinearCharacteristicOffset;
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : CalculateTargetNetPulses
// Object              : Calculates target net pulses 
// Output Parameters   : TargetNetPulses
// -----------------------------------------------------------
void CalculateTargetNetPulses( void )
{
	// Calculating Net Pulses Per Layer = function( Bale Diameter Voltage minus Minimum Bale Diameter Voltage )
	CalculateNetPulsesPerLayer();

	// Calculating Target Net Pulses = function( Net Pulses Per Layer )
	TargetNetPulses = (unsigned short int)(NetPulsesPerLayer * pFac.pf.GW302_NetLayers[pFac.pf.GW302_BaleProfile] * pFac.pf.GW302_NetFactor) - (unsigned short int)pFac.pf.GW302_NetCutPulse;
}
// -----------------------------------------------------------

#ifndef WINDOWS
// -----------------------------------------------------------
// Function Name       : CalculateAnalogInputRollingAverages 
// Object              : Calculates analogue input rolling averages
// Output Parameters   : BaleDiaVolts     - Bale Diameter (AnIP1)
//                       DensityPresVolts - (Bale) Density Pressure (AnIP2)
//                       BaleShapeVolts   - Bale Shape Indicator (AnIP3) 
//                       KnifePresVolts   - Knife Pressure (AnIP4)        
// -----------------------------------------------------------
void CalculateAnalogInputRollingAverages( void ) 
{
	// Average Sample
	float BaleDiaVoltsAvSample;     // V
	float DensityPresVoltsAvSample; // V
	float BaleShapeVoltsAvSample;   // V
	float KnifePresVoltsAvSample;   // V

	// Rolling Average Sample
	static float BaleDiaVoltsRollAvSample;     // V
	static float DensityPresVoltsRollAvSample; // V
	static float BaleShapeVoltsRollAvSample;   // V
	static float KnifePresVoltsRollAvSample;   // V

	// RollingAverageSample = ( [SmoothingFactor]% x AverageSample ) +
	//                        ( [100-SmoothingFactor]% x Previous_RollingAverageSample )
	// where AverageSample = Average of total NUM_ACCUMULATED_SAMPLES

	// AnIP1: Bale Diameter
	BaleDiaVoltsTotAccumMultSamples = (unsigned long int)( (float)BaleDiaVoltsTotAccumMultSamples * POTDIV * ATODSCALE * (float)INPUT_ACCURACY_MULTIPLIER );
	BaleDiaVoltsAvSample = (float)BaleDiaVoltsTotAccumMultSamples / (float)(NUM_ACCUMULATED_SAMPLES * INPUT_ACCURACY_MULTIPLIER);
	BaleDiaVoltsRollAvSample = ( ((float)pFac.pf.GW302_BaleDiaSmoothingFactor/100.0) * BaleDiaVoltsAvSample ) +
			( ((100.0 - (float)pFac.pf.GW302_BaleDiaSmoothingFactor)/100.0) * BaleDiaVoltsRollAvSample);
	BaleDiaVolts = BaleDiaVoltsRollAvSample;

	// AnIP2: (Bale) Density Pressure
	DensityPresVoltsTotAccumMultSamples = (unsigned long int)( (float)DensityPresVoltsTotAccumMultSamples * POTDIV * ATODSCALE * (float)INPUT_ACCURACY_MULTIPLIER );
	DensityPresVoltsAvSample = (float)DensityPresVoltsTotAccumMultSamples / (float)(NUM_ACCUMULATED_SAMPLES * INPUT_ACCURACY_MULTIPLIER);
	DensityPresVoltsRollAvSample = ( ((float)pFac.pf.GW302_DensityPresSmoothingFactor/100.0) * DensityPresVoltsAvSample ) +
			( ((100.0 - (float)pFac.pf.GW302_DensityPresSmoothingFactor)/100.0) * DensityPresVoltsRollAvSample);
	DensityPresVolts = DensityPresVoltsRollAvSample;

	// AnIP3: Bale Shape Indicator
	BaleShapeVoltsTotAccumMultSamples = (unsigned long int)( (float)BaleShapeVoltsTotAccumMultSamples * POTDIV * ATODSCALE * (float)INPUT_ACCURACY_MULTIPLIER );
	BaleShapeVoltsAvSample = (float)BaleShapeVoltsTotAccumMultSamples / (float)(NUM_ACCUMULATED_SAMPLES * INPUT_ACCURACY_MULTIPLIER);
	BaleShapeVoltsRollAvSample = ( ((float)pFac.pf.GW302_BaleShapeSmoothingFactor/100.0) * BaleShapeVoltsAvSample ) +
			( ((100.0 - (float)pFac.pf.GW302_BaleShapeSmoothingFactor)/100.0) * BaleShapeVoltsRollAvSample);
	BaleShapeVolts = BaleShapeVoltsRollAvSample;

	// AnIP4: Knife Pressure
	KnifePresVoltsTotAccumMultSamples = (unsigned long int)( (float)KnifePresVoltsTotAccumMultSamples * POTDIV * ATODSCALE * (float)INPUT_ACCURACY_MULTIPLIER );
	KnifePresVoltsAvSample = (float)KnifePresVoltsTotAccumMultSamples / (float)(NUM_ACCUMULATED_SAMPLES * INPUT_ACCURACY_MULTIPLIER);
	KnifePresVoltsRollAvSample = ( ((float)pFac.pf.GW302_KnifePresSmoothingFactor/100.0) * KnifePresVoltsAvSample ) +
			( ((100.0 - (float)pFac.pf.GW302_KnifePresSmoothingFactor)/100.0) * KnifePresVoltsRollAvSample);
	KnifePresVolts = KnifePresVoltsRollAvSample;
}
// -----------------------------------------------------------
#endif

#ifdef DEVELOP // Testing Stage Completed (stagecomp) Flags
// -----------------------------------------------------------
// Function Name       : DisplayStageCompletedIndicator
// -----------------------------------------------------------
void DisplayStageCompletedIndicator( void )
{
	static unsigned char AllStagesJustCompletedDelay;

	unsigned char tstring[32];

	if( AutoMan ) // AUTO mode
	{
		if( stagecomp == SC_CLEAR )
		{
			if( (Stage == (STAGE_FORMATION | STAGE_FORMATION2 | STAGE_CORE | STAGE_MAIN | STAGE_NETTING_START | STAGE_NETTING_NET_DELAY)) && half_second )
				sprintf( tstring, "0   " );
			else // Sequence not yet underway -- prior to reaching NETFEEDON stage completion
			{
				if( AllStagesJustCompletedDelay ) // flashing all 4 dots after completion
				{
					if( half_second )
						sprintf( tstring, "    " );
					else
					{
						sprintf( tstring, "0000" );
						AllStagesJustCompletedDelay = 0;
					}
				}
				else // !AllStagesJustCompletedDelay
					sprintf( tstring, "    " ); // 0 dots shown
			}
		}
		else if( stagecomp == NETFEEDON )
		{
			if( (NetPulseCount != 0UL) && half_second ) // flashing 2nd dot
				sprintf( tstring, "00  " );
			else // Pulses not yet being received on IP4 (since OP2 turned on)
				sprintf( tstring, "0   " );
		}
		else if( stagecomp == (NETFEEDON | NETCUTON) )
		{
			sprintf( tstring, "00  " ); // 2 dots shown
		}
		else if( stagecomp == (NETFEEDON | NETCUTON | CHAMBOP) )
		{
			sprintf( tstring, "000 " ); // 3 dots shown
		}
		else if( stagecomp == (NETFEEDON | NETCUTON | CHAMBOP | CHAMBCLO) )
		{
			sprintf( tstring, "0000" ); // 4 dots shown
			AllStagesJustCompletedDelay = 1;
		}
		else // Invalid setting for stagecomp flags
		{
			sprintf( tstring, "----" ); // 4 dashes shown
		}
	}
	else // MAN mode
	{
		if( stagecomp == SC_CLEAR )
		{
			if( AllStagesJustCompletedDelay ) // flashing all 4 dots after completion
			{
				if( half_second )
					sprintf( tstring, "    " );
				else
				{
					sprintf( tstring, "0000" );
					AllStagesJustCompletedDelay = 0;
				}
			}
			else // !AllStagesJustCompletedDelay
				sprintf( tstring, "    " ); // 0 dots shown
		}
		else if( stagecomp == NETFEEDON )
		{
			sprintf( tstring, "0   " ); // 1 dot shown
		}
		else if( stagecomp == (NETFEEDON | NETCUTON) )
		{
			if( CHAMBER_OPEN && half_second ) // flashing 3rd dot
				sprintf( tstring, "000 " );
			else // Chamber not yet opened
				sprintf( tstring, "00  " ); // 2 dots shown
		}
		else if( stagecomp == (NETFEEDON | NETCUTON | CHAMBOP) )
		{
			sprintf( tstring, "000 " );  // 3 dots shown
		}
		else if( stagecomp == (NETFEEDON | NETCUTON | CHAMBOP | CHAMBCLO) )
		{
			sprintf( tstring, "0000" );  // 4 dots shown
			AllStagesJustCompletedDelay = 1;
		}
		else // Invalid setting for stagecomp flags
		{
			sprintf( tstring, "----" ); // 4 dashes shown
		}
	}

	display_f1_text( tstring, TS4, 8, 96, FIXED ); // 4 chars wide
}
// -----------------------------------------------------------
#endif
