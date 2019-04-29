// -----------------------------------------------------------
// Software:         GW302 - McHale V6 Fixed Chamber Round Baler
// C Header File:    board.h
// Description:      Include Standard file
//                   Contains Summary & Modification History 
// -----------------------------------------------------------


#define SOFTWARE 302
#define ISSUE 1
#define REVISION 23


//#define DEVELOP        // Uncomment to display diagnostics on screen
//#define WINDOWS        // Uncomment to run PC simulation
#define SHOW_TIME        // Uncomment to show time (format hh:mm) in top left corner of display

// Pin code
#define PINTECH1 3473    // Released version
//#define PINTECH1 0000  // Test version


/* -----------------------------------------------------------
 * Summary (start of)  
 * -----------------------------------------------------------

 * Updated:  15 August 2014

 * Compiler: GNUARM 4.7.1 -- See makefile.c for more information
 *           (Copy of compiler to be found at Q:\Software\Development Tools\GNUARM\GNUARM-471_EABI.7z)

 * RTOS:     FreeRTOS v6.1.0
 *           (Copy of original files to be found at Q:\Software\Software_Source_Files\FreeRTOS\SAM7-V6.1.0\)

 * IDE:      Eclipse 4.2
 *           (Copy of IDE to be found at Q:\Software\Development Tools)

 * PC Simulation: GW302.bpr (Borland C++ Builder 6 project)
 *           (Located at Q:\Software\Simulation_Software\G-Wiz Simulations\GW302 McHale V6 (formerly V660)\)

 * Specs:    S402-###.doc & W402-###.xls
 *           (Located at Q:\Product information\Current Products\OEM\McHale V640 G-Wiz Basic (402)\Development Documents\Specs\)

 * Origin:   This software was derived from software GW300-000 rev19
 *           (Located at Q:\Software\Software_Source_Files\Graphic Wizard\GW300 McHale F550 Baler\GW300000rev19 20120420.7z)
 *           Note potentially useful diagnostic code and details of its modification history (not carried over to 
 *           this software) may be found here.

 * Author:   Roslyn White

 * -----------------------------------------------------------
 * Analogue Inputs:

 * AnIP1 (0 - 5V) Bale Diameter           (AKA Diameter Pot / Bale Diameter Potentiometer)  
 * AnIP2 (0 - 5V) (Bale) Density Pressure (AKA Chamber Pressure)  
 * AnIP3 (0 - 5V) Bale Shape Indicator    (AKA Fill Potentiometer / Bale Shape Diameter)  
 * AnIP4 (0 - 5V) Knife Pressure 

 * -----------------------------------------------------------
 * Digital Inputs:

 * IP1: Floor Sensor          (AKA Floor Position Sensor)    
 * IP2: 11 Knife Sensor       (AKA Chopping Knives position / Knives Position Sensor 1)
 * IP3: Chamber Locked Sensor (AKA Chamber Open Sensor)
 * IP4: Net Pulse Sensor      (AKA Net Pulses)
 * IP5: Bale Kicker   
 * IP6: Net Cut Sensor (optionally fitted on machine)

 * -----------------------------------------------------------
 * Digital Outputs:
     
 * OP1: Master Diverter 
 * OP2: Net Clutch               (AKA Net On)
 * OP3: Knife Diverter           (AKA Knives Diverter)
 * OP4: Floor Diverter
 * OP5: Roller Engage Solenoid   (AKA Pre Net Position)
 * OP6: 12V Sensor Power         (AKA 12V supply)
 * OP7: Net Cut Motor            (AKA Net Cut Trip)
 * OP8: Density Release Solenoid (AKA Density Control)

 * -----------------------------------------------------------
 * Warnings (& alarms) in order of priority -- highest listed first: 

 * 1) Density Pressure Too High 
 * 2) Check Net Error           (AKA Net Warning)
 * 3) Drop Floor Warning        (AKA Floor Position)   
 * 4) Knife maximum pressure
 * 5) Chopping knives position  (Only for 11 Knife Sensor on IP2)
 * 6) Low battery warning       (AKA Low voltage)              
 * 7) Lube Count Alarm

 * -----------------------------------------------------------
 * Summary (end of)  
 * -----------------------------------------------------------


 * -----------------------------------------------------------
 * Modification History (start of)
 * -----------------------------------------------------------
 *
 *
 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 23  based on  ISS 001 REV 22	20160309	KE
 *
 * OP5 switches off after 3s if the bale diameter goes above 95% (Engage Diameter) rather than staying on until the bale diameter goes below 95%.
 *
 *
 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 22  based on  ISS 001 REV 21	20160308	KE
 *
 * OP5 now times out and switches off 3s after coming on regardless of whether net or twine is selected.
 * 		This happens in manual and auto mode.
 *
 *
 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 21  based on  ISS 001 REV 20	20160303	KE
 *
 * Fixed issue where bale count wouldn't increase when using the twine sequence.
 *
 *
 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 20  based on  ISS 001 REV 19	20160301	KE
 *
 * Twine option is now correctly remembered after powering off then on.
 * Net/Twine option is now correctly remembered after powering off then on.
 * Check net error is now disables when Twine is selected.
 * OP05 now turns on (if Twine is selected) when the target diameter is reached for 3 seconds instead of waiting for IP06 or OP07.
 *
 *
 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 19  based on  ISS 001 REV 17	20160222	KE
 *
 * Revert software revision from version 18 back to version 17.
 * 		"Rev18 had changes made to the screen refresh rate that made the screen flicker so the changes done in rev18 not be carried forward to the next version of software."
 *
 * Added a "Twine" Option setting to Technician menu can be turned ‘On’ or ‘Off’. Default = ‘Off’.
 * 		Turning this option off hides the "Twine/Net" option.
 * 		Changing this option sets "Twine/Net Option" to "Net".
 *
 * Added a "Twine/Net" Option setting to Machine Setup menu can be turned ‘Twine’ or ‘Net’. Default = ‘Net’.
 *		When Twine is selected:
 *			"Net feed warning" is disabled.
 *			"Net cut motor" (OP7) is  disabled.
 *			"Pre Net Pulse" is set to zero (restored when Net is selected or Twine option is turned off).
 *


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 17  based on  ISS 001 REV 16	20140911	RW

   Changes made according to CT GW302-001 rev16.xls:

   1) In AUTO mode, if 'Net System' is set to 'Roller' or 'Beak':
      OP5 (Roller Engage Solenoid) now comes on, in the case of the bale diameter (AnIP1) increasing very quickly
      past the  'Engage diameter' and the final target diameter.
      Note:
      - A further change was made such that if the instrument is switched from MAN to AUTO mode, when the bale diameter
        has already reached the target, OP5 now comes on -- whereas before it did not.

   Additional changes / work done:

   A) Warning removed: variable 'i' set but not used (function: TC1_irq_handler() file: interrupts.c) -- done by restoring some mcm code

   B) Tidied up, including removing conditionally compiled out bitmaps & other commented out redundant code.

   C) GW302001rev17 PC simulation generated:
      Located: Q:\Software\Simulation_Software\G-Wiz Simulations\GW302 McHale V6 (formerly V660)\GW302 McHale V6 (GW302001rev17)


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 16  based on  ISS 001 REV 15    20140828

   Changes made according to CT GW302-001 rev15.xls:

   1) Netting Setup sub-menu (page 1/2) in Technician menu:
      'Pre Net Pulse' default value changed from 4 to 2 pulses.
      Function: Target pulses on IP4 to pre-feed the net (n2)

   Additional changes / work done:

   A) GW302001rev16 PC simulation generated:
      Located: Q:\Software\Simulation_Software\G-Wiz Simulations\GW302 McHale V6 (formerly V660)\GW302 McHale V6 (GW302001rev16)


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 15  based on  ISS 001 REV 14    20140826    RW

   Changes made according to CT GW302-001 rev14.xls:

   1) In AUTO mode:
      If the chamber is open when bale diameter (AnIP1) reaches the target diameter:
      a) Bale full beeps (a continuous long beep) are disabled -- the intermittent 1 s beeps
         indicating that the chamber is open are sounded instead and;
      b) 'Check Net Error' no longer appears while the chamber is open.
      Note:
      - The pre-warning beeps &  Net Delay intermittent beeps are also disabled while the chamber is open --
        the chamber open (intermittent 1 s) beeps sounded instead.
      - The pre-warning beeps, Net Delay intermittent beeps, bale full beeps & 'Check Net Error' are all disabled outside of
        unloading -- same as OP2 & OP5.
      - The chamber open (intermittent 1 s) beeps are now sounded up until unloading -- but not during unloading.

   2) In MAN mode:
      Roller Engage Solenoid (OP5) now ONLY switches on, when bale diameter (AnIP1) reaches 'Engage diameter',
      if 'Net System' is set to 'Roller' -- if 'Net System' is set to 'Beak', OP5 does not switch on.
      In summary:   
      - If 'Net System' is set to 'Roller', when bale diameter goes from below to above the 'Engage Diameter' threshold, 
        Roller Engage Solenoid (OP5) turns on.
      - Any of the following happening subsequently (at any time / any point in the sequence) will turn OP5 off:
        a) OP7 (Net Cut) turning on -- done by pressing Button 2 on Screen 2.
        b) IP3 going from closed to open to signal that the chamber door has opened.
        c) Bale diameter dropping below the 'Engage Diameter' threshold.
        Note: OP5 does not come on again until bale diameter goes from below to above the 'Engage Diameter' threshold.

   3) In MAN mode, when bale diameter has not yet reached the core diameter:
      Density Release Solenoid (OP8) now automatically switches on, to reduce the density pressure, when 
      the Bale Density Pressure (AnIP2) exceeds the core bale density setting.
      Note:
      - When the core diameter is exceeded, OP8 is switched on when AnIP2 exceeds the target bale density.  
      - When below the core diameter, hysteresis is applied to AnIP2 according to 'Pres. Deadband' [V] and 
        'Pres. Sw. Delay' [s] (set in Baler Full sub-menu) -- the same hysteresis applied when the bale diameter is
        above the core diameter.

   Additional changes / work done:

   A) Ensured Lube Count Alarm (oil can icon) is completely cleared from the display when no longer active and
      bale density pressure & total net applied in metres are displayed in its place -- done by aligning borders of
      areas cleared.       

   B) Re-formatted displayed values to make them clearer in reset messages / reset screens and info panel on main screen:
      - 'Reset Lube Counter to Lube Alarm Confirm/Cancel' Message box              (moved value: pFac.pf.GW302_LubeAlarm)
      - Lube Count Reset Screen                                                    (moved value: pFac.pf.GW302_LubeAlarm)      
      - Bale Count Reset Screen                                                    (altered bitmap: ICON_BALE_COUNT_RESET)
      - Fill Pot Zero Reset Screen (AKA Bale Shape Indicator Zero)                 (altered bitmap: ICON_FILL_POT_ZERO_RESET)
      - Net Metres Reset Screen                                                    (altered bitmap: ICON_NET_METRES_RESET)
      - Target bale density displayed in bottom box of side panel on second screen (moved value: pFac.pf.GW302_BaleDensity)

   C) Development work: 
      - Redundant bitmaps conditionally compiled out of the source code to free up memory.
      - Source code files re-formatted to make the code more maintainable.

   D) GW302001rev15 PC simulation generated:
      Located: Q:\Software\Simulation_Software\G-Wiz Simulations\GW302 McHale V6 (formerly V660)\GW302 McHale V6 (GW302001rev15)


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 14  based on  ISS 001 REV 13    20140821    RW

   Further changes made following on from changes made according to CT GW302-001 rev11.xls:

   A) On first main screen,
      Side panel graphics changed to show the cm units for the bale diameter.
      Changed bitmap: SIDEDISPLAY_1

   B) On second main screen,
      Side panel graphics changed to show the voltage units (V) for the supply voltage.
      Changed bitmap: SIDEDISPLAY_2

   C) On second main screen,
      Changed graphics for tab indicating button used to cycle currently selected bale store such that it now shows a pile of
      bales in addition to the currently selected bale store (A - J).
      Done to make it clear that this button does not alter the Bale Profile (A-E).
      New bitmap: TABCYCLE_ATOJ (this replaces existing bitmap: TABATOJ)

   D) On first main screen, when Lube Count Alarm is active:
      When the Reset Lube Count Alarm button (with a tab showing an oil can) is pressed, the Lube Alarm Confirm/Cancel' message box
      that is shown is now iconized -- taking the same form as the graphics that appears in Lube Count Reset Screen (screen120),
      including showing the number of bales to which the Lube Count will be reset, if confirmed.
      New bitmap: LUBE_COUNT_RESET_PROMPT
      Note minor change:
      - Adjusted graphics for tab indicating button used to reset the Lube Count Alarm (bitmap: TABRESET_LUBECOUNT)

   E) On second main screen,
      When RESET tab is pressed & held, the bale count flashes for the currently selected bale store (A -J), to
      indicate that if the button continues to be pressed this bale count, if not already 0, will be reset to 0.
      Note:
      - The length of time that the RESET button has to be held, before the bale count is reset, has been increased.
      - If the bale count resets, a long beep is heard -- as in the previous version of software.

   F) GW302001rev14 PC simulation generated:
      Located: Q:\Software\Simulation_Software\G-Wiz Simulations\GW302 McHale V6 (formerly V660)\GW302 McHale V6 (GW302001rev14)


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 13  based on  ISS 001 REV 12    20140820    RW

   Changes made according to CT GW302-001 rev11.xls:

   1) In MAN or AUTO mode:
      The threshold for the bale density pressure voltage, above which the 'Density Pressure Too High' warning appears,
      has been changed from 4.5 to 4.8 V.
      Note:
      - The threshold (DPV_DENSITY_PRESSURE_TOO_HIGH_VOLTS) is fixed within the software.
      - Hysteresis of 0.2 V (DPV_DENSITY_PRESSURE_TOO_HIGH_VOLTS_HYSTERESIS) has been added to decrease flickering on/off of
        the warning when the bale density pressure voltage is around 4.8V; hence, the bale density pressure voltage has to
        fall to 4.6 V for the warning to disappear. The bale density pressure (AnIP2) smoothing Factor, 'DensPres ANIP2 SF'
        influences the amount of hysteresis required [See Item 11 Note g].

   2) Diameter Setup sub-menu in Technician menu:
      Bale Density Pressure (AnIP2) Smoothing Factor 'DensPres ANIP2 SF' default value changed from 95 to 100 %.

   3) & 4a) In MAN mode:
      When bale diameter goes from below to above the 'Engage Diameter' threshold, Roller Engage Solenoid (OP5) turns on.
      Any of the following happening subsequently (at any time / any point in the sequence) will turn OP5 off:
      a) OP7 (Net Cut) turning on -- done by pressing Button 2 on Screen 2.
      b) IP3 going from closed to open to signal that the chamber door has opened.
      c) Bale diameter dropping below the 'Engage Diameter' threshold.
      Note: OP5 does not come on again until bale diameter goes from below to above the 'Engage Diameter' threshold.

   4b) & 5) In AUTO mode, outside of netting starting (OP2 coming on) & unloading (IP3 opening & closing):
      If chamber opens > 1 s, the instrument beeps intermittently (every 1 s).
      Note:
      - The chamber does not have to be closed before opening to trigger the beeps.
      - The instrument beeps intermittently if the chamber opens while the bale is being formed up until netting starts with the
        exception that during the 'Net Delay' period (just before OP2 comes on), the beeps to indicate that netting has started
        are heard instead.
      - The instrument beeps intermittently if the chamber opens while waiting for sufficient pre net pulses to be counted on
        IP4 at the end of the sequence / start of the next sequence.

   6) In AUTO mode, outside of unloading:
      If chamber opens, OP2 (Net Clutch) & OP5 (Roller Engage Solenoid) are turned off.
      Note:
      a) OP5 flags are saved when the chamber opens and restored when the chamber closes to ensure OP5 comes back on when the
         chamber closes if it was turned off when the chamber opens, further note:
         - OP5 will only come back on if netting has not yet finished -- if it has finished, the saved copies of the OP5 flags
           are reset.
         - If 'Net System' is set to 'Beak', and 'Beak Engage' pulses have not yet been counted on IP4 to turn OP5 on when the
           chamber opens -- OP5 will still turn on after the required number of 'Beak Engage' pulses and the chamber is closed.
      b) OP2 will not turn back on when the chamber closes, if it was turned off when the chamber opened -- OP2 (net Clutch) can be
         turned on, as required, by pressing the Net On button (button 2 on Screen 1).

   7) In any mode, on main display:
      At the top of the display, the currently selected Bale Profile (A-E) is now displayed.
      Note:
      - Bale profile letter (A-E) appears in the same form used in the Bale Setup Menu.
      - The left & right driving arrows' position & form have been adjusted to make room for the display of
        the bale profile letter -- displayed when 'Bale Shape Indicator' option is ticked (or set to 'Beep').
      - Bale profile letter does not appear if the 'Density Pressure Too High' Warning is present.
      - This same feature has already been applied to EP660.

   8) Analogue Inputs screen in Diagnostics menu:
      Instead of displaying the 'live' analogue voltages directly, smoothing factors are now applied to these voltages.
      Note:
      - This removes fluctuations in the displayed voltage and flickering of the open/close switch.
      - Bale Diameter (AnIP1), Bale Density Pressure (AnIP2), Bale Shape Indicator (AnIP3) and Knife Pressure (AnIP4)
        are displayed, calculated by the CalculateAnalogInputRollingAverages function.

   9) Formation 2 sub-menu in Technician menu:
      Formation Diameter 'Formation Dia.' default value changed from 5 to 0 %.

   10)Formation 2 sub-menu in Technician menu:
      Formation 2 Diameter 'Formation2 Dia.' default value changed from 9 to 0 %.

   11)In any mode, on second main screen:
      In the top left hand corner of the display, underneath the displayed time, the bale density pressure (based on AnIP2)
      is displayed.
      Note:
      a) Displayed in bars, where 1 V equivalent to 50 bar --
         the same conversion ratio is used to display the knife pressure in bars in one of the side panels.
      b) Units are displayed after the value, in the same "BAR" form used in the side panels on the main screen.
      c) For completeness, metre units (m) are now displayed after the total net applied.
      d) When Lube Count Alarm is active: Bale density pressure is NOT shown on the second screen --
         the oil can icon being displayed in its place.
      e) When 'Density Pressure Too High' Warning is active: Bale density pressure is shown on the warning screen.
      f) Just to confirm, the bale and core densities follow the same conversion of 1 V being equivalent to 50 bar,
         where they are set as follows:
         - Density Index:                  1,     2,     3,     4,     5,     6,    7,     8,     9,    10
         - Density voltage, AnIP2 (V):   0.0,  0.80,  1.20,  1.60,  2.00,  2.40, 2.80,  3.20,  3.60,  4.00
         - Density Pressure (bar):         0,    40,    60,    80,   100,   120,  140,   160,   180,   200
      g) Note that bale density pressure (based on AnIP2) now has a smoothing Factor, 'DensPres ANIP2 SF' with a default value
         of 100 %; hence, the voltage is averaged but there is minimal smoothing -- and the bale density pressure value can be seen
         to jump about when increasing/decreasing the value. This compares to knife pressure (based on AnIP4) which has a smoothing
         factor, 'KnifePresAnIP4 SF' with a default value of 50% -- and the value does not jump about when being increased/decreased.

   Additional changes / work done:

   A) In working on Lube Count Alarm & 'Density Pressure Too High' Warning:
      - Corrected partial clearing of Lube Count Alarm (oil can icon) when 'Density Pressure Too High' Warning is
        active at the same time.
      - Removed MAN/AUTO from the display when 'Density Pressure Too High' Warning active --
        done to remove redundant information & meet the specification (W402-008.xls).
      - Replaced RESET tab (TABRESET) used to reset the Lube Count Alarm with a tab containing a picture of an
        oil can (TABRESET_LUBECOUNT) in IL tab position on Screen 1 when Lube Count Alarm is active -- done to avoid having
        two RESET tabs at the same time (one on Screen 1 & the other on Screen 2) each with different functions.

   B) GW302001rev13 PC simulation generated:
      Located: Q:\Software\Simulation_Software\G-Wiz Simulations\GW302 McHale V6 (formerly V660)\GW302 McHale V6 (GW302001rev13)


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 12  based on  ISS 001 REV 11    20140815    RW

   A) Changed environment to match latest GW300000rev26 20140514 environment --
      previously modified to match GW100004rev18 - Test Software 20111122 environment.
      This involved:

      A-1) Moving all relevant folders into GW302 project workspace:
           - include
           - New bitmaps
           - Old bitmaps
           - resource
           Note:
           - Subsequently updated path specification in makefile, External Tools options for JTAG (& files used) and
             Borland C++ builder paths.

      A-2) Updating compiler from GNUARM 4.2.2 (GNUARM-422_ELF) to 4.7.1 (GNUARM-471_EABI).
           Note:
           - Subsequently updated makefile and added 'C:\GNUARM-471_EABI\bin' to Environment variable PATH in Properties.
           Note:
           - Replaced Paths and Symbols (for all configurations & languages) with:
             C:\GNUARM-471_EABI
             C:\GNUARM-471_EABI\lib\gcc\arm-none-eabi\4.7.1\include
             Workspace path: /GW302/include

      A-3) Installing MinGW (downloaded 4.8.1 - Basic Setup) and using this -- instead of cygwin.
           Note:
           - Subsequently updated makefile and added 'C:\MinGW\bin' to Environment variable PATH in Properties.

      A-4) Changing FreeRTOS from version 5.4.2 to 6.1.0.
           Note:
           - Replacing, removing & adding files in include directory and main source directory

      A-5) Corrected casting within function: vWriteTraceToBuffer() to remove -Wcast-align warning.

      A-6) Set up remaining Project environment as follows:
           Note:
           - Added 'C:\msys\1.0\bin;C:\H8\IAR\EXE' to Environment variable PATH in Properties.
           - Created TMP Environment variable PATH in Properties: C:\Temp
           Note:
           - Instead of ticking 'Append variables to native environment'; ticked 'Replace native environment with specified one' instead.

   B) GW302001rev12 PC simulation generated:
      Located: Q:\Software\Simulation_Software\G-Wiz Simulations\GW302 McHale V6 (formerly V660)\GW302 McHale V6 (GW302001rev12)

   c) Updated Summary at the start of board.h

   d) Put software under git version control


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 11  based on  ISS 001 REV 10    20131213    RW

   Changes made according to CT GW302-001 rev10.xls:

   1) Density voltage (AnIP2) defaults of the bale and core, changed:
      From:
      - Density Index:                  1,     2,     3,     4,     5,     6,    7,     8,     9,    10
      - Density voltage, AnIP2 (V):   0.0,  0.64,  0.96,  1.28,  1.60,  1.92, 2.24,  2.56,  2.88,  3.20
      To:
      - Density Index:                  1,     2,     3,     4,     5,     6,    7,     8,     9,    10
      - Density voltage, AnIP2 (V):   0.0,  0.80,  1.20,  1.60,  2.00,  2.40, 2.80,  3.20,  3.60,  4.00
      Note:
      - Equiv. Density Pressure (bar):  0,    40,    60,    80,   100,   120,  140,   160,   180,   200
      - When Density default is set to Index = 1 (AnIP2 = 0.0 V & Pressure = 0 bar), when OP8 can be on/off:
        Density Control (OP8) is permanently on.

   2) Machine menu:
      'Bale Diameter' maximum value (that can be set by the operator via the menu) changed
      back from 165 to 168 cm.


   Additional changes / work done:

   a) In Technician menu, Baler Full sub-menu (page 1/2):
      'Net Speed' option now decrements down to 0.0 Hz correctly without jumping to the maximum value of 99.0 Hz

   b) PC Simulation updated for GW302001rev11:
      The link to the PC simulation is:
      Q:\Software\Simulation_Software\G-Wiz Simulations\GW302 McHale V6 (formerly V660)\GW302 McHale V6 (GW302001rev11)


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 10  based on  ISS 001 REV 09    20131024    RW
 
   Further changes made according to email:
   From: John Warren [mailto:jwarren@mchale.net]
   Sent: 18 October 2013 16:26
   To: Jon Pocock
   Subject: RE: New version (Iss#1 Rev#8) of McHale V6 G-Wiz (GW302) software

   1) Responsiveness of pulse counting in interrupt has been increased by x 5
      to reduce the lag on counting the target number of pre net pulses, 
      beak engage pulses & also net pulses.

   2) In AUTO mode, if 'Net System' is set to 'Beak':
      'Beak Engage' pulses count on IP4, is now reset to 0, when OP2 (Net On) is turned on 
      at the start of netting -- this makes the 'Beak Engage' feature easier to test.

   Following extra changes have been made:

   a) PC Simulation updated:
      - Dot (that appears when factors are being written to flash) has been removed.
      - Menus are accessible & softkey functions available.
      - No digital/analogue inputs or digital outputs are present.
      - Chamber door is permanently closed & 
        net roll continuously rotates while the simulation runs.
      The link to the PC simulation is:
      Q:\Software\Simulation_Software\G-Wiz Simulations\GW302 McHale V6 (formerly V660)\GW302 McHale V6 (GW302001rev10)
   
   b) The following items in the interface have been tidied up:
      - Bale Count Menu: tick both indicating the current store and appearing in
        the tab, made clearer -- with necessary adjustments to offsets and addition of
        deletion code.
      - All reset screens: tick & cross used in tabs made clearer.
      - Bale Count for currently selected store (A-J) appearing in top info box 
        on second screen, reformatted to ensure the maximum 4 digits fitted within
        the box.
   

 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 09  based on  ISS 001 REV 08    20131022    RW
 
   Changes made according to email:
   From: John Warren [mailto:jwarren@mchale.net]
   Sent: 18 October 2013 16:26
   To: Jon Pocock
   Subject: RE: New version (Iss#1 Rev#8) of McHale V6 G-Wiz (GW302) software

   1) Time in format hh:mm is now displayed in top left corner of the instrument's screen and
      Time & Date Adjust Menu (accessed via Operator Setup Menu) now works correctly and can
      be used to set the time & date.
      Note:
      - In previous versions of the software, the displayed time was being corrupted.
        To remove this problem:
        Calculations required to determine the analogue input values previously done within 
        the interrupt (ADC0_irq_function) were moved to higher level code (into function, 
        CalculateAnalogInputRollingAverages).
 
   2) In Technician menu:
      a) Diameter Setup sub-menu: 'DensPres ANIP2 SF' default value changed from 50 to 95 %
      b) Netting Setup sub-menu (page 1/2): 'Net On' default value changed from 6.0 to 3.0 s
      c) Netting Setup sub-menu (page 1/2): 'Pre Net Pulse' default value changed from 5 to 4 pulses

   3) 'Pre-Net' feature (OP2 turned off after sufficient pre net pulses have been counted on IP4) has been
      speeded up by 100 ms (one step increment of the state machine).
      Note:
      - The test for sufficient pre net pulses counted on IP4 is now equal as well as over the required
        pre net pulses (according to 'Pre Net Pulse' setting).        

   Following extra changes have been made:
   
   a) Knife pressure information in lower info box on first screen of main display moved up 2 pixels to
      fit properly within box.
      Note:
      - Knife pressure information only displayed in info box if 'Chopping Knives' set to
        'Disabled' (Chopping Knives NOT fitted to the machine).
 

 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 08  based on  ISS 001 REV 07    20131016    RW

   Changes made according to email:
   From: John Warren [mailto:jwarren@mchale.net]
   Sent: 10 October 2013 14:39
   To: Jon Pocock
   Subject: V6 G-Wiz software
   Where the following assumptions are made:
   - IP2 (mentioned in the email) is Net Pulse Sensor input (IP4 in the software).
   - IP8 (mentioned in the email) is Floor Position Sensor input (IP1 in the software).
   - OP1 (mentioned in the email) is Roller Engage Solenoid output (OP5 in the software).

   1) In AUTO mode, if 'Net System' is set to 'Roller':
      On pressing Net Feed button 2 to begin netting early, OP5 (Roller Engage Solenoid) now comes on 
      as soon as the button is pressed and remains on until the net has been cut (or a Net Warning is activated).
      Note:
      - OP7 goes off when net has been cut.
      - See item 3 concerning OP5 being turned off when Net Warning is activated.
      - OP5 (Roller Engage Solenoid) still comes on immediately Engage diameter is reached
        and remains on until the net has been cut (or a Net Warning is activated).

   2) In AUTO mode, if 'Net System' is set to 'Beak':
      After pressing Net Feed button 2 to begin netting early, OP5 (Roller Engage Solenoid) now comes on
      when 'Beak Engage' pulses are seen on IP4 (Net Pulse Sensor) and remains on until the net has been cut (or a Net Warning is activated).
      Note:
      - OP7 goes off when net has been cut.
      - See item 3 concerning OP5 being turned off when Net Warning is activated.
      - After Engage diameter is reached and then 'Net Delay' time expires, OP5 (Roller Engage Solenoid) still comes on
        when 'Beak Engage' pulses are seen on IP4 (Net Pulse Sensor) and remains on until the net has been cut (or a Net Warning is activated).

   3) In AUTO mode, if 'Net System' is set to either 'Roller' or 'Beak':
      If no pulses are detected on IP4 (Net Pulse Sensor) for 'No Net Pulse' time, now 
      OP5 (Roller Engage Solenoid) is turned off when Net Warning (AKA Check Net Error) is activated.
      Note:
      - When pulses are detected again on IP4, and if the net has not yet been cut, OP5 comes back on.
 
   4) In any mode: 
      'Floor Down' warning is now always displayed while IP1 (Floor Position Sensor) is open -- 
      this warning is no longer inhibited when the Floor Diverter button is pressed.
      Note:
      - 'Floor Down' warning is the Drop Floor (AKA Floor Position) warning.   
      - Floor Diverter button is the outside-right button while instrument is displaying the first screen.

   5) Diagnostics menu:
      Resolution of voltages displayed on Analogue Inputs screen increased from 0.1 to 0.01 V.  
 
   6) Machine menu:
      Default value for 'Bale Shape Indicator' sensor zero point voltage changed from 3.0 to 2.5 V.
 
   7) In Technician menu:
      a) Netting Setup sub-menu (page 1/2): 'Net Cut Pulse' default value changed from 24 to 40 pulses
      b) Netting Setup sub-menu (page 1/2): 'MinDia puls/lay' (Dia1) default value changed from 46.2 to 38.0 pulses/layer
      c) Netting Setup sub-menu (page 1/2): 'MaxDia puls/lay' (Dia50) default value changed from 110.4 to 108.0 pulses/layer
      d) Netting Setup sub-menu (page 2/2): 'Beak Engage' default value changed from 12 to 6 pulses
 
   Following extra changes have been made:

   1) Machine menu:
      Scrolling in 'S' pattern in both up & down directions added --
      as can already be done in both left & right directions.
      Note:
      - This allows the operator to keep their finger on just one 
        navigation key and move through all options in the Machine menu in
        any direction.

   2) PC Simulation updated.

   Software development:
   a) For all programmable factors (and preserved totals), the step size, minimum value, maximum value & default value 
      have been gathered into arrays, located in the same place in the source code. 
   b) New iconized menu function names made consistent with other menu function names.
   c) Removed commented out blocks of redundant code.


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 07  based on  ISS 001 REV 06    20130905    RW

   Changes made according to CT GW302-001 rev06 V2.xls:

   1) In Technician menu:
      a) Baler Full sub-menu (page 1/2): 'Net Speed' default value changed from 20.0 Hz to 50.0 Hz
      b) Netting Setup sub-menu (page 1/2): 'Pre Net Pulse' default value changed from 0 to 5 pulses
      c) Netting Setup sub-menu (page 2/2): 'Net System' default value changed from 'Beak' to 'Roller'
      d) Knives Setup sub-menu: 'Knife Pressure' default value changed from 1.05 V to 0.90 V
      Note:
      - Baler Full sub-menu (page 1/2): 'Net Speed' range changed from 0.0 - 40.0 Hz to 0.0 - 99.0 Hz --
        matching the 'Net Speed' range employed in the EP660 software.

   2&3) In Technician menu, Netting Setup sub-menu (page 2/2): 
        a) Incorrect spelling of 'Net System' factor option, 'Break' corrected to 'Beak'.
        b) Incorrect spelling of 'Break Engage' factor, corrected to 'Beak Engage'.
        Note:
        - All instances of 'Break' within the source code -- used in variable names and
          appearing within comments -- corrected to 'Beak'.

   4) In any mode, on main display, if 'Chopping Knives' is set to 'Enabled':
      'Floor position' (IP1) warning is no longer disabled when button 3 (Knife diverter) is pressed.
      Note:
      - 'Chopping knives position' warning is still disabled while button 3 is pressed.
      - 'Chopping Knives' is 'Enabled' via the first diagnostic screen -- on enabling the 
        chopping knives function, the 'Chopping Knives Up/Down' icon appears in button 3 tab 
        on first screen of main display.

   5) In any mode, while within the main display:
      If both 'Floor position' warning and 'Knife pressure' warning are active, the 
      'Floor position' warning has priority.
      Note:
      - If 'Chopping Knives' is set to 'Enabled', and if both 'Floor position' warning and 'Chopping knives position' warning 
        are active, the 'Floor position' warning has priority.   
      - 'Net Error' warning (AKA Net Warning) has been set to a higher priority than the 
        'Floor position', 'Knife pressure' & 'Chopping knives position' warnings.
      - In summary, the warnings in order of priority -- highest listed first:
        1) Density Pressure Too High 
        2) Check Net Error           (AKA Net Warning)
        3) Drop Floor Warning        (AKA Floor Position)   
        4) Knife maximum pressure
        5) Chopping knives position  (Only for 11 Knife Sensor on IP2)
        6) Low battery warning       (AKA Low voltage)              
        7) Lube Count Alarm

   6) In Technician menu, Baler Full sub-menu (page 2/2): 
      New programmable factor added, called 'Roller Delay'
      Default: 0.5 s 
      Range: 0 - 9.9 s 
      Function: Delay between OP5 (Roller Engage) & OP2 (Net Clutch) switching on, if 'Net System' is set to 'Roller'.

   7) In MAN mode, if 'Net System' is set to 'Roller':
      On pressing Net Feed button 2, OP5 (Roller Engage) comes on immediately and 
      OP2 (Net clutch) comes on after 'Roller Delay'.
      Subsequently on releasing button 2, OP5 & OP2 go off immediately -- if button released before
      OP5 comes on, OP5 does not come on.

   8) Density voltage (AnIP2) defaults & corresponding Density Pressure ('bar' values) of the bale and core,
      changed:
      From Index (1-10) / AnIP2 (1.1-4.0) / 'bar' values (55-200) based on the following conversion:
      - Density Index:                 1,     2,     3,     4,     5,     6,    7,     8,     9,    10
      - Density voltage, AnIP2 (V):  1.1,   1.4,   1.7,   2.1,   2.4,   2.7,  3.1,   3.4,   3.7,   4.0
      - Density Pressure (bar):       55,    70,    85,   105,   120,   135,  155,   170,   185,   200
      To Index (1-10) / AnIP2 (0.0-3.20) / 'bar' values (0-200) based on the following conversion:
      - Density Index:                 1,     2,     3,     4,     5,     6,    7,     8,     9,    10
      - Density voltage, AnIP2 (V):  0.0,  0.64,  0.96,  1.28,  1.60,  1.92, 2.24,  2.56,  2.88,  3.20
      - Density Pressure (bar):        0,    40,    60,    80,   100,   120,  140,   160,   180,   200
      Note:
      - When Density default is set to Index = 1 (AnIP2 = 0.0 V & Pressure = 0 bar), when OP8 can be on/off:
        Density Control (OP8) is permanently on.
      - Display of Density voltage, AnIP2 (V) has been increased from 1 to 2 decimal places.

   9) In AUTO mode:
      After netting starts up to when cutting starts -- OP2 (Net Clutch) turned on up to when OP7 (Net Cut Motor) turned on, 
      if pulses on IP4 stop for 'No Net Pulse' time, the 'Net Error' warning now appears.
      Note:
      - At the start of netting, if no pulses are counted on IP4 for 'No Net Pulse' time, the 'Net Error' warning appears as before.
      - At the end of netting (when 'Pre Net Pulse'[s] are expected), if no pulses are counted on IP4 for 'No Net Pulse' time, 
        'Net Error' warning appears as before; and if after some of the expected 'Pre Net Pulse'[s] are counted, pulses on IP4
        stop for 'No Net Pulse' time, the 'Net Error' warning now appears. 
      - Checking for an absence of pulses on IP4 (leading to a 'Net Error' warning) is turned off when the Target net pulses is reached.
      - The same block of 'Pulse Checking' code is used for all the pulse checking functions above -- a flag is now set to call this 
        single block of code. 

   10) In AUTO mode:
       If the target bale density index is between 1-5, the density control feature (OP8 switching on/off) now
       remains active until the net is cut (OP7 switches OFF).
       Note:  
       - If the target bale density index is 6-10, the density control feature stops when the bale full beeps start, as in
         the previous version of software.
       - While the density control feature is active, OP8 is NOT turned off if a Net Warning occurs -- this is different
         behaviour compared to the EP660 software.

   11) Day Total has been removed from this version of the software.
       Note:
       - This was a bale count which was incremented with each bale -- 
         in addition to the bale total in the current bale store, A-J.

   12) Added the following icon based menu structure:
       - Machine Menu
       - Bale Setup Menu (accessed via Machine Menu)
       - Machine Setup Menu (accessed via Machine Menu)
       - Bale Count Menu (accessed via Machine Menu)
       - Operator Setup Menu (accessed via Machine Menu)
       - Brightness & Contrast Adjust Menu (accessed via Operator Setup Menu)
       - Time & Date Adjust Menu (accessed via Operator Setup Menu)
       - Pin Entry Display [Screen] for Technician Menu (accessed via Operator Setup Menu)
       - Diagnostics (hidden menu accessed via Operator Setup Menu) 
       Note:
       a) Technician Menu remains text based.
       b) Matches icon based menu structure used in EP660 software (rev07), with the following exceptions:
          - No Volume option in Operator Setup Menu.
          - 4-Way Navigation button, rather than a single Cycle button, used to move around the menus.
          - In Bale Setup Menu, if the operator tries to set the bale diameter lower then the core
            diameter, the bale diameter & core diameter values both flash and the bale diameter is blocked from
            being reduced any further.
       c) Changed to match EP660 software (rev07), as follows:
          - Grand Bale Total roll over value increased from 99,999 to 999,999.
          - All reset screen images for: Lube Count Reset Screen; Bale Count Reset Screen; 
            Fill Pot Zero (AKA Bale Shape Indicator Zero) Reset Screen; Net Metres Reset Screen.
          - In Machine Setup Menu, +/- sign added to 'Diameter Correction'.
          - In Machine Setup Menu, the current voltage setting is now displayed for 
            the 'Bale Shape Indicator Zero' rather than '= 0'. 
          - On main screen, BAR unit added to bale density icon.
          - At the end of the Operator Setup Menu, the Diagnostics option appears after, pressing and 
            holding the DOWN arrow for 5 seconds to display -- this is longer than in the previous GW302 
            version of software.
       d) Further note:
          - The same bale count icon, as used in Bale Count menu, is used in side panel on the main display.
          - In Bale Count Menu, the ENTER icon, on button 2 tab, is replaced with a TICK icon.

   13) In Machine Setup Menu:
       Added 'Beep' option (displayed as a small speaker icon) to the range of 'Bale Shape Indicator' settings. 

   14) In Machine Setup Menu, if 'Bale Shape Indicator' is set to 'Beep':
       The instrument emits a short beep, to accompany each driving arrow displayed, as follows:
       a) When 4 driving arrows are displayed:
          - Beep length (for each of 4 beeps):       50 ms
          - Delay (before first beep of 4 starts):   1.5 s
       b) When 3 driving arrows are displayed:
          - Beep length (for each of 3 beeps):      100 ms
          - Delay (before first beep of 3 starts):  2.0 s
       c) When 2 driving arrows are displayed:
          - Beep length (for each of 2 beeps):      200 ms
          - Delay (before first beep of 2 starts):  2.5 s
       d) When 1 driving arrow is displayed:
          - Beep length:                            400 ms
          - Delay (before beep starts):             3.0 s
       Note:
       - Beeps occur with increasing frequency as the voltage on AnIP3 deviates further from 'Fill pot zero' --
         subjectively the beeps become more insistent.
       - The delay acts as a debounce delay: such that if the driving improves within the delay the 
         beep does not sound. Hence, if the driving keeps improving -- nothing is heard -- the beep is 
         only heard if the driving gets worst or remains off to the left or right after the given delay.
       - Bale full beeps have priority over the driving arrow beeps. To achieve this: when either
         the Engage diameter is reached OR the Pre warning beep bale diameter exceeded, the driving arrow beeps
         are disabled -- the driving arrow beeps are re-enabled when the sequence resets. It is expected that the
         Engage diameter will be set below the Pre warning beep bale diameter; hence, the pre-warning beep is likely
         to be heard clearly.
       - Warning beeps are heard over the driving arrow beeps -- at present the driving arrow beeps are turn turned off
         while a warning is present.

   15) Five pre-set bale profiles (A - E) added:
       Where pre-set default values exist for each bale profile, including:
       In the Bale Setup Menu:       
       - Core Diameter (cm):          70,   70,   70,   90,   90
       - Core Density (bar):           9,    8,    7,    3,    1
       - Bale Diameter (cm):         125,  125,  125,  125,  125
       - Bale Density (bar):          10,    9,    8,    4,    2
       - Net Layers                  3.5,  3.0,  2.5,  4.2,  4.2
       In the Formation2 Menu (hidden menu accessed via the Technician Menu):
       - Formation Density (Index):   10,   10,   10,    5,    5
       - Formation Diameter (%):       5,    5,    5,    5,    5
       - Formation2 Density (Index):  10,   10,   10,    5,    5
       - Formation2 Diameter (%):      9,    9,    9,    9,    9
                             Preset =  A     B     C     D     E
       Note:
       - On resetting factory defaults: the above pre-set default values are restored -- the current
         bale profile is reset to A and hence, the above factors are reset to the Preset A defaults.
       - On changing the current bale profile (A - E), the factors are changed to the settings for the
         selected bale profile.
       - Any value for any factor for any bale profile can be changed and the change is remembered -- 
         including, on changing the selected bale profile and subsequently returning to.

   16) In MAN and AUTO mode, if 'Net System' is set to 'Beak':
       When 'Beak Engage' pulses have been received via IP4, OP5 (Roller Engage) now switch on without
       a significant delay.
       Note:
       - At higher pulse frequencies, the interrupt counting pulses on IP4 was previously counting in blocks
         of around 5 pulses. Since, the number of 'Beak Engage' pulses is around 10, this interrupt has been 
         adjusted to count in around blocks of 1 -- with an occasional block of 2 occurring.
       - 'Beak Engage' pulse counting code has been simplified such that a single block of code is called when a  
         flag is set -- this flag is set from when Engage diameter is reached up to when OP7 (Net Cut Motor) is 
         switched on.

   Extra changes:

   A) Automatic return to main display from screens after period of inactivity (20 s) turned on/off, as follows:
      i) Automatic return, from the following screens, turned on: 
         - Machine Menu
         - Bale Setup Menu
         - Machine Setup Menu
         - Bale Count Menu
         - Operator Setup Menu
         - Brightness & Contrast Adjust Menu
         - Time & Date Adjust Menu
         - Pin Entry Display [Screen] for Technician Menu
      ii) Automatic return, from the following screens, turned off:
         - Lube Count Reset Screen
         - Bale Count Reset Screen
         - Fill Pot Zero (AKA Bale Shape Indicator Zero) Reset Screen  
         - Net Metres Reset Screen
         - Technician Menu
         - Technician Menu - Baler Full
         - Technician Menu - Netting Setup
         - Technician Menu - Diameter Setup    
         - Technician Menu - Density Setup
         - Technician Menu - Knives Setup
         - Technician Menu - Reset Defaults
         - Technician Menu - Formation 2
         - Diagnostics - Digital Inputs
         - Diagnostics - Analogue Inputs
         - Diagnostics - Outputs

   B) Power off button disabled for the following extra screens:
         - Bale Setup Menu
         - Machine Setup Menu
         - Bale Count Menu
         - Operator Setup Menu

   C) Technician menu:
      - Values made clearer by adding space between value & units.
      - Tidied up formatting.

   D) Added same splash screen as used for EP660.


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 06  based on  ISS 001 REV 05    20130724    RW
 
   Changes made according to CT GW302-001 rev05.xls:
   
   1) In any mode:
      The down-arrow centre-navigation button now only needs to be pressed briefly
      to access the menu structure -- previously the button needed to be held for
      ~1 second to access the menus. 

   2) Technician menu, Baler Full sub-menu (page 1/2):
      'Bale Size Factor' default value changed from 194 to 221.
      Note:
      - Calculated 'max dia voltage' (displayed underneath in brackets) has changed
        from 3.52 V to 3.87 V.

   3) Machine menu:
      'Bale Diameter' maximum value (that can be set by the operator via the menu) reduced
      from 168 to 165 cm.

   4) In MAN or AUTO mode:    
      Pre-warning beep duration halved -- changed from ~2 s to ~1s.

   5&6) A new 'Chopping Knives' programmable factor has been introduced.
        Default: 'Disabled'. Range: 'Disabled' / 'Enabled'.
        Located in keytab 3 on the first diagnostic screen --
        cross indicates disabled; tick indicates enabled.
        Function: Selects whether the machine is fitted with Chopping Knives or not.

        If 'Chopping Knives' set to 'Enabled' (Chopping Knives fitted to the machine),
        the software functions as before.

        If 'Chopping Knives' set to 'Disabled' (Chopping Knives NOT fitted to the machine),
        the software has been changed as follows:
        a) Tab 3 blanked and functionality removed from button 3 (Knife Diverter) on
           first screen of main display -- OP1 & OP3 no longer come on, when button 3 is pressed.
        b) Knife pressure information removed from lower info box on first screen of main display --
           the lower info box is empty.
        c) 'Maximum Knife Pressure' warning removed -- warning no longer activated when 
           Knife Pressure (AnIP4) goes above 'Knife Pres. Warn' threshold [V] setting.

   7) Technician menu, Netting Setup sub-menu (page 2/2): 
      New programmable factor added, called 'Net System'
      Default: 'Beak'. Range: 'Beak' / 'Roller' 
      Function: Selects type of netting system fitted to the machine.
      Note: 
      - When 'Roller' is selected the software functions as before.
      - A second page for the Netting Setup sub-menu had to be created.

   8) Technician menu, Netting Setup sub-menu (page 2/2): 
      New programmable factor added, called 'Beak Engage'
      Default: 12 pulses. Range: 0 - 99 pulses 
      Function: Number of pulses on IP4 before OP5 switches on (if 'Net System' set to 'Beak')
      Note: Same notes as item 7.

   9) In AUTO mode, if 'Net System' is set to 'Beak':
      When netting starts, OP5 (Roller Engage Solenoid) only switches on when there 
      have been 'Beak Engage' pulses on IP4 (Net Pulse Sensor). 
      Note:
      - Pulses are counted on IP4 from when OP2 (Net Clutch) comes on -- which can happen
        after 'Engage Diameter' has been reached.
      - It is possible for OP2 to switch off before the required 'Beak Engage' number of pulses have 
        been counted; hence, OP5 can come on after OP2 has switched off. 
      - If 'Check Net Error' warning occurs (no pulses detected on IP4 for 'No net pulse' time since 
        OP2 on) and button 2 is subsequently pressed: OP5 comes on as soon as 'Beak Engage' number of
        pulses have been counted, whether the button is still pressed or has been released. 
      - If 'Net System' is set to 'Roller': OP5 (Roller Engage Solenoid) switches on when 
        the bale diameter reaches 'Engage Diameter' -- as in the previous software.

   10) In MAN mode, if 'Net System' is set to 'Beak':
       When button 2 is pressed to start netting, 
       OP2 (Net Clutch) switches on immediately and 
       OP5 (Roller Engage Solenoid) switches on only after 
       'Beak Engage' pulses have been counted on IP4 (Net Pulse Sensor).
       Note:
       - Pulses are counted on IP4 from when button 2 is pressed.
       - On releasing button 2, the outputs (OP2 and OP5 if on) switch off immediately.
       - The software assumes that the 'NET FEED ON' stage has been completed (even if OP5 does
         not come on) in determining that all the necessary stages have been completed to increment
         the bale count.
       - If 'Net System' is set to 'Roller': On pressing button 2, OP2 (Net Clutch) &
         OP5 (Roller Engage Solenoid) both switch on immediately and on releasing
         the button, both OP2 & OP5 switch off immediately -- as in the previous software.

   11) In MAN mode:
       When Bale Density Pressure (AnIP2) exceeds the target bale density setting,
       Density Release Solenoid (OP8) automatically switches on to reduce the density pressure.
       Note:
       - This feature (applied to several density thresholds) is already provided in AUTO mode.
       - OP8 now also comes on while PRESSURE TOO HIGH warning is active (AnIP2 > 4.5 V) -- 
         same as in AUTO mode.
       - Hysteresis is applied to AnIP2 according to 'Pres. Deadband' [V] and
         'Pres. Sw. Delay' [s] set in Baler Full sub-menu -- same as in AUTO mode. 
 
   A) To keep the software consistent with the V660 (software EP660):
      - The pin entry window for the Technician menu has been 'iconised' --
        "Technician Menu" title changed to the icon for the Technician menu (picture of a spanner).
      - 'Formation2' sub-menu in the Technician menu is now hidden -- only appearing after 
        pressing & holding the down arrow for approx. 6 s (while at the bottom of the Technician menu).

   B) General tidying of software files done -- including removal of redundant code.


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 05  based on  ISS 001 REV 04    20130620    RW

   Changes made according to CT GW302-001 rev02 V2.xls:
   
   1) Operator menu:
      'Pre-warning beep' default value changed from 99 to 90 %

   2) Operator menu:
      'Net delay' default value changed from 0.0 to 2.0 s 

   3) Technician menu, Baler Full sub-menu (page 1/2):
      'Door Closed' default value changed from 0.5 to 1.5 s

   4) In MAN or AUTO mode:
      When the 'Density Pressure Too High' warning is active, the warning message now fills the 
      entire screen (except for the tabs & MAN/AUTO) and 3 warning symbols flash. As before, while 
      this warning is active, there is continuous beeping -- this is the highest priority warning which 
      cancels itself when the pressure falls below the alarm threshold (fixed within the software at 4.5 V).   
      Note:
      - While the alarm is active: tabs relating to the current MAN/AUTO mode (and the currently   
        selected screen) are still visible and; outputs can be turned on/off. 
      - While in the menus, the alarm can still be activated (& cancelled): no warning message is shown but 
        continuous beeping occurs.
      - Density Release Solenoid (OP8) is not turned on automatically when the condition that activates the 
        'Density Pressure Too High' warning occurs (as in the previous version of the software).

   5) In both MAN and AUTO modes:
      Density Release Solenoid (OP8) switches on for 6 seconds any time that
      Chamber Locked Sensor (IP3) changes from closed to open.
      Note:
      - Density Release Solenoid (OP8) no longer switches on after netting when Chamber Locked Sensor (IP3) opens -- 
        previously remaining on until Chamber Locked Sensor (IP3) closes.
      - Density Release Solenoid (OP8) is turned on and off during the sequence and all outputs are turned off at 
        the end of the sequence -- Density Release Solenoid (OP8) switching on for 6 seconds happens independently.
      - If the Chamber Locked Sensor (IP3) closes & opens repeatedly, the 6 second count is reset each time this happens.

   6) In any mode:
      'Knife Pressure' info moved from bottom of side display on second page to bottom of side display on first page.
      'Bale Diameter' info moved from bottom to middle of side display on first page.
      'Bale Density' info moved from middle of side display on first page to bottom of side display on second page.

   7) In any mode:
      Bale oversize sensor removed from IP5 and 'Bale Oversize' warning removed.
      Note:
      - Bale oversize sensor is no longer fitted

  8) In AUTO mode: 
     When the target pulses have been counted on IP4 and OP7 switches on for 'Net cut time' to 
     cut the net, the pulses on IP4 continue to be monitored and if the pulses stop for > 1 s,
     OP7 switches off (and the instrument beeps) -- the net presumed cut.
     Note:
     - A stopping period of 1.5 s is used -- fixed within the software.
     - If there has not been a period of 1.5 s without pulses, OP7 still switches off and 
       the instrument beeps after the usual 'Net cut time'.
     - See item 19 for an additional way in which OP7 is switched off.

   9) Startup screen:
      On powering up the instrument, the start-up screen now shows
      'McHale V6' rather than 'McHale V660'. 

   11) Technician menu, Baler Full sub-menu (page 2/2):
       'Fill Pot Range' default value changed from 0.50 to 1.00 V

   12) Technician menu, Baler Full sub-menu (page 2/2):
       'Fill Pot Ignore' default value changed from 0.10 to 0.25 V

   13) In any mode, with Bale Indicator turned on:
       Logic of the driving arrows inverted, such that when:
       a) 'Bale Indicator' set to 'Steer', increasing the input voltage displays the 
          right side arrows, showing ". >>>" at maximum voltage.
       b) 'Bale Indicator' set to 'Fill', increasing the input voltage displays the 
          left side arrows, showing "<<<< ." at maximum voltage.
       Note:
       - By default 'Bale Indicator' remains set to 'Steer'.

   14) In any mode:
       'Chopping Knives Position' warning function for IP6 has been removed -- 
       Previously activated when IP6 goes from closed to open.  
       The knife icon on the main screen that indicates the status of IP6 has also been removed.
       Note:
       - IP6 is no longer used for the 12 knife sensor.
       - 'Chopping Knives Position' warning function for IP2 remains -- 
         activated when IP2 goes from closed to open.
       - The knife icon on the main screen that indicates the status of IP2 is still shown but
         moved down to where IP6 had previously been shown.   

   15) In any mode:
       IP5 is now designated 'Bale kicker' and the up/down status of the bale kicker icon (part of 
       the machine picture on the main screen) indicates the live status of IP5, where:  
       - If IP5 is closed, the bale kicker icon is up (shown horizontal), 
       - If IP5 is open, the bale kicker icon is down (shown diagonal).

   16) In AUTO mode:
       When the chamber door opens and the bale is ejected, the image of the 
       bale disappears once the Bale Kicker (IP5) opens and then closes again.  At the same time 
       the instrument emits a short beep to confirm that the bale has been unloaded.
       Note:
       - It is assumed that the Bale Kicker will go down & then up (IP5 opening & then 
         closing) as the bale unloads, after the chamber opens and before the chamber closes --
         Door Closed period ignored.

   17) In any mode:
       'Dia Pot. Zero' factor fixed at 1.0 V. 
       'Dia Pot. Zero' Reset Option removed from the Operator menu.
       Note:
       - Option is blanked out in the Operator menu.
       - Selection pointer skips this option's position.

   18) Technician menu, Baler Full sub-menu (page 1/2):
       'Bale Size Factor' default value changed from 85 to 194 (2.20 to 3.52 V)
       'Bale Size Factor' range of values changed from 0-150 (1.1-3.05 V) to 100-250 (2.30-4.25 V)
       Note: 
       - The voltage displayed in brackets (appearing underneath the 'Bale Size Factor' value in 
         the menu) is the calculated 'max dia voltage' for the current 'Bale Size Factor'. 

   19) In AUTO mode, if net cut sensor fitted on machine to IP6:
       When the target pulses have been counted on IP4 and OP7 switches on for 'Net cut time' to 
       cut the net, if IP6 closes to 0V, OP7 switches off (and the instrument beeps) -- 
       the net having been cut.
       Note:
       - See item 8 for the other ways in which OP7 is switched off.


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 04  based on  ISS 001 REV 03    20121217    RW

   Changes as part of the initial development of GW302:

   PC Simulation added:
      - #define WINDOWS needs to be uncommented to bring in PC simulation code (& remove unwanted code)
      - To run PC simulation: Borland C++ Builder 6 project (GW302.bpr) built & run
      - PC simulation only includes softkey options & menu -- no digital/analogue inputs or 
        digital outputs present. Note: chamber door is permanently closed & net roll continuously rotates.   


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 03  based on  ISS 001 REV 02    20121105    RW

   Changes as part of the initial development of GW302:
   
   a) Time in format hh:mm is still not shown in top left corner of the display and the 
      Clock Adjust screen has been temporarily fixed to start up showing the software release date with time 00:00.

   b) Fixed problem spotted: Screen buffer no longer becomes corrupted while instrument 
      is showing the Analogue Inputs screen in Diagnostics mode.

   c) When the software is updated from previous revisions out in the field the grand total bale count is now only 
      reset if the software ID number (e.g. GW302) changes. 
      Note: To achieve this the 'grand total bale count' permanent factor is kept in the same place in serial flash 
      between successive revisions of software -- 'grand total bale count' from the previous revision (Rev#2) onwards 
      maintains the same position in flash memory.

   d) Added Summary information to main header file (board.h).

   e) Tidied diagnostics printed out when DEVELOP uncommented.


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 02  based on  ISS 001 REV 01    20121101    RW

   Changes made according to CT GW302-001 rev01.xls

   1) AnIP1 (which was previously used for  'Knife Pressure') 
      is now used for 'Bale Diameter'

   2) AnIP2 (which was previously used for 'Bale Diameter') 
      is now used for 'Density Pressure'

   3) AnIP3 (which was previously used for 'Density Pressure') 
      is now used for 'Bale Shape Indicator'

   4) AnIP4 (which was previously used for 'Bale Shape Indicator') 
      is now used for 'Knife Pressure'

   5) In the 'Diameter Setup' section of the Technician Setup menu the
      smoothing factors have been renamed to reflect the revised analogue inputs:
      - Bale Diameter (AnIP1) 
      - (Bale) Density Pressure (AnIP2) 
      - Bale Shape Indicator (AnIP3)
      - Knife Pressure (AnIP4)

   6) Changed default smoothing factors (for the analogue inputs listed in item 5) from 10 to 50%

   7) Potential divider (POTDIV) factor of 1.568 used to scale up the analogue voltage levels within the software. 
    Note:
    - The pressure sensors used by McHale give a voltage output in the range 0 - 5 V; 
      however, the instrument can only receive a voltage input in the range 0 - 3.2 V (max: 3.2 or 3.3 V)
    - The hardware is modified using a potential divider made up of a 2K2 ohm resistance in series & 
      3K9 ohm resistance to ground, giving the multiplying factor of 1.568.
    - The analogue voltage values are now displayed correctly on the instrument where:
      a) Maximum analogue voltage values displayed in Diagnostics mode has increased from 3.2 V to 5.1 V. 
      b) Maximum knife pressure displayed on the main screen has increased from 161 BAR to 252 BAR.
    - The analogue voltage values, used internally by instrument, have now been corrected. 
    - Most of the voltage threshold settings did not require scaling up; however, some of the fixed/programmable settings were changed:
      a) 'Density Pressure Too High Voltage' (DPV_DENSITY_PRESSURE_TOO_HIGH_VOLTS) setting (fixed within the software) changed from 3.0 to 4.5 V
      b) 'Bale Shape Uniform voltage' (DEFAULT_BALE_SHAPE_INDICATOR_ZERO) programmable setting default value changed from 1.5 to 3.0 V
      c) Density voltages for setting 1 to 10 (DEFAULT_DENSITY_1 to DEFAULT_DENSITY_10) changed from the range 1.1 - 3.2 V to the range 1.1 - 4.0 V, used by:
         - Target density voltage during Main stage (DensityBaleVolts)
         - Target density voltage during Core stage (DensityCoreVolts)
         - Target density voltage during Formation2 stage (DensityFormation2Volts)    
         - Target density voltage during Formation stage (DensityFormationVolts)
    - Bale Diameter (AnIP1), (Bale) Density Pressure (AnIP2), Bale Shape Indicator (AnIP3) and Knife Pressure (AnIP4) are scaled up.
    - The open/close threshold of the analogue switches (1.70V and 1.60V) displayed in Diagnostics mode were scaled up.
    - SupplyVolts displayed on the main screen and in Diagnostics mode did not require scaling up -- and was not scaled up. 

   Extra changes as part of the initial development of GW302:

   a) Removed CAN task and associated code. 
      Note: commented out the following line in main.c:
      vStartCANTasks( mainCAN_TEST_PRIORITY );

   b) Removed Build Warning: cast increases required alignment of target type 
      (function 'vTaskSwitchContext'; file tasks.c; line 1550)
      Turning TRACE FACILITY off in FreeRTOSConfig.h: #define configUSE_TRACE_FACILITY    0

   c) Display Stage Completed Indicator/stagecomp Flags when diagnostics are displayed on the screen (#define DEVELOP uncommented)
      Note: as each flag is set a non-flashing 0 is shown 
      if there is an invalid setting for the stagecomp flags, 4 dashes are shown   

   d) In MAN/AUTO mode: the driving arrows disappear from the display once OP2 (Net On) comes on --
      they do not appear again until a new sequence starts (such as after a completed sequence or on resetting the
      instrument to its starting state by toggling between MAN/AUTO mode) 

   e) In MAN mode: if button 2 in screen 2 (Net Cut) is pressed after completing a valid sequence, but before button 2 in screen 1 (Net On)
      has been pressed, OP7 (Net Cut Motor) will come on but the flag is not set for this to count as a stage in a valid sequence -- 
      button 2 in screen 1 (Net On) has to be pressed before button 2 in screen 2 (Net Cut) is pressed.

   f) Incorporated feature previously added to GW300 rev22 in Knives Setup submenu (in Technician Menu):
      i) Knife Pressure default value (DEFAULT_KNIFE_PRESSURE) changed from 0.95 V to 1.05 V
      ii)New programmable factor added called 'Diverter Delay'
         Default: 0.2 s  
         Range: 0.0 - 9.9 s 

   g) Incorporated feature previously added to GW300 rev22 in any mode:
      On pressing button 3 (Knives Up/Down) or button 4 (Floor Up/Down) to 
      operate the knife or floor functions, instead of a fixed 0.5s delay between 
      OP1 switching on and OP3/OP4 switching on, this delay is now the 
      programmable 'Diverter Delay'.
      Note: 
      - Delay between the outputs switching off is fixed at 0.5s --
        in the previously version of the software this was 0.4s.
      - Additional behaviour on pressing button 3 (Knives Up/Down) to operate 
        the knife function, remains unchanged: 
        where if AnIP2 reaches > 'Knife pressure' OP3 turns off and if it 
        subsequently falls below it comes back on again; however, if the button is 
        released & pressed again while off it comes back on & stays on. 

   h) Fixed problem spotted: Lube Alarm icon which was being partially overdrawn

   i) Fixed problem spotted: Escaping Chopping Knives warning now works from both screens 1 & 2 -- 
      via button 2 while tab shows 'ESC'
      
   j) The programmable factors have been moved around such that the Grand Bale Total has been moved to an early position in 
      memory. This value is to be preserved between successive software versions and has been moved here to make the code 
      easier to maintain; However, downloading the new version of GW302/V660 to an instrument loaded with an old version of 
      the GW302/V660 software will result in an odd value for the Grand Bale Total. To solve this can download another software
      such as GW300/f5500 before downloading a new version of the GW302/V660 software.  


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 01  based on  ISS 001 REV 00    20121026    RW

   Changes made according to TR GW302-001 rev00.xls


 * -----------------------------------------------------------
 * SW 302 ISS 001 REV 00  based on  SW 300 ISS 000 REV 19    20121017    RW
 
   Derived from software GW300-000 rev19 modified according to S402-002.doc & W402-002.xls
   

 * -----------------------------------------------------------
 * Modification History (end of)
 * ----------------------------------------------------------- */


#ifndef Board_h
#define Board_h

#include "AT91SAM7A3.h"

#ifndef WINDOWS
 #define __inline static inline
 #include "lib_AT91SAM7A3.h"

 // Scheduler includes
 #include "FreeRTOS.h"
 #include "task.h"
 #include "queue.h"
 #include "semphr.h"
#endif

#define true    -1
#define false    0

#define POWEROFF 1
#define POWERON  0

// -----------------------------------------------------------
// SAM7 Memory Definitions
// -----------------------------------------------------------
// The AT91SAM7A3 embeds a 32-Kbyte SRAM bank, and 256 K-Byte Flash

#define SHUTDOWN_KEY ((unsigned int)0xa5 << 24)
#define RESET_KEY    ((unsigned int)0xa5 << 24)
#define WATCHDOG_KEY ((unsigned int)0xa5 << 24)
#define FLASH_KEY    ((unsigned int)0x5a << 24)

// Output Definition
#define TESTPIN      (1 <<  6) // PA06 - AT91C_PIO_PA6
#define BUZZER       (1 << 14) // PA14
#define BACKLIGHT    (1 << 18) // PA18
#define ILLUMINATION (1 << 19) // PA19
#define OUT1         (1 << 20) // PA20
#define OUT2         (1 << 21) // PA21
#define OUT3         (1 << 22) // PA22
#define OUT4         (1 << 23) // PA23
#define OUT5         (1 << 24) // PA24
#define OUT6         (1 << 25) // PA25

// For new Hardware
/*
#define INHIBIT1     (1 << 13) // PA13
#define INHIBIT2     (1 << 15) // PA15
*/

#define OUT7        (1 << 13) // PA13
#define OUT8        (1 << 15) // PA15

#define OpOff   AT91C_BASE_PIOA->PIO_SODR
#define OpOn    AT91C_BASE_PIOA->PIO_CODR

//#define OUTPUT_MASK_PIOA (BUZZER | BACKLIGHT | ILLUMINATION | OUT1 | OUT2 | OUT3 | OUT4 | OUT5 | OUT6)
// For new Hardware
#define OUTPUT_MASK_PIOA (BUZZER | BACKLIGHT | ILLUMINATION | OUT1 | OUT2 | OUT3 | OUT4 | OUT5 | OUT6 | OUT7 | OUT8)

#define IE_IP01   (1 << 10) // PB10
#define IE_IP02   (1 << 11) // PB11
#define IE_IP03   (1 << 13) // PB13
#define IE_IP04   (1 << 23) // PB23
#define LED_GREEN (1 << 25) // PB25
#define LED_AMBER (1 << 27) // PB27
#define LED_RED   (1 << 29) // PB29
#define OUTPUT_MASK_PIOB (IE_IP01 | IE_IP02 | IE_IP03 | IE_IP04 | LED_GREEN | LED_AMBER | LED_RED)

// PWM channel
#define BACK 0
#define ILLU 1
#define OP01 2
#define OP02 3
#define OP03 4
#define OP04 5
#define OP05 6
#define OP06 7

// INPUTS

// Analogue Input Definition
#define IP01      (1 << 14) // PB14 - ADC0_AD0
#define IP02      (1 << 15) // PB15 - ADC0_AD1
#define IP03      (1 << 16) // PB16 - ADC0_AD2
#define IP04      (1 << 17) // PB17 - ADC0_AD3
#define IP05      (1 << 18) // PB18 - ADC0_AD4
#define IP06      (1 << 19) // PB19 - ADC0_AD5
#define IPVOUT    (1 << 21) // PB21 - ADC0_AD7

// Digital Input Definition
#define PFAIL     (1 <<  0) // PB00
#define ATODRDY   (1 <<  1) // PB01

#define KEY2      (1 <<  2) // PB02
#define KEY3      (1 <<  3) // PB03
#define KEY7      (1 <<  4) // PB04
#define KEY5      (1 <<  5) // PB05
#define KEY6      (1 <<  6) // PB06
#define KEY8      (1 <<  7) // PB07
#define KEY4      (1 <<  8) // PB08
#define KEY1      (1 <<  9) // PB09

#define IP07      (1 << 12) // PB12
#define IP08      (1 << 20) // PB20
#define IP09      (1 << 22) // PB22
#define IP10      (1 << 24) // PB24
#define IP11      (1 << 26) // PB26
#define IP12      (1 << 28) // PB28

/* G-Wiz Keys */
#define SW_CU KEY5
#define SW_CL KEY7
#define SW_CR KEY8
#define SW_CD KEY6
#define SW_IL KEY2
#define SW_OL KEY1
#define SW_IR KEY3
#define SW_OR KEY4

#define SW_NEW (1<<31)
#define SW_REP (1<<30)

#define SW_MASK (KEY1 | KEY2 | KEY3 | KEY4 | KEY5 | KEY6 | KEY7 | KEY8)

#define PIOA_PULLUP_OFF ((1 << 2) | (1 << 8) | (1 << 30))
#define PIOB_PULLUP_OFF (IP01 | IP02 | IP03 | IP04 | IP05 | IP06 | IP07 | IP08 | IP09 | IP10 | IP11 | IP12)
#define ATOD_CHANNELS (AT91C_ADC_CH0 | AT91C_ADC_CH1 | AT91C_ADC_CH2 | AT91C_ADC_CH3 | AT91C_ADC_CH4 | AT91C_ADC_CH5 | AT91C_ADC_CH7)

// -----------------------------------------------------------
// Master Clock
// -----------------------------------------------------------
#define EXT_OC 18432000   // External oscillator MAINCK
#define MCK    47923200   // MCK (PLLRC divide by 2)
#define SCK    32768      // slow clock
#define MCKKHz (MCK/1000)
#define ADCCK  5990400

#define ATODVREF         3.30
#define ATODSCALE       (ATODVREF / 1023.0)
#define ATODIPVOUTSCALE (ATODSCALE * ((220.0 + 22.0) / 22.0)) 
#define ATODIPVOUTDROP  1.0
#define ATODVMIN        (7.2 / ATODIPVOUTSCALE) 

#define POTDIV   1.568  // Potential divider scaling factor applied to analogue input voltages

#define PWMC_MCK_OVER_1    0x00
#define PWMC_MCK_OVER_2    0x01
#define PWMC_MCK_OVER_4    0x02
#define PWMC_MCK_OVER_8    0x03
#define PWMC_MCK_OVER_16   0x04
#define PWMC_MCK_OVER_32   0x05
#define PWMC_MCK_OVER_64   0x06
#define PWMC_MCK_OVER_128  0x07
#define PWMC_MCK_OVER_256  0x08
#define PWMC_MCK_OVER_512  0x09
#define PWMC_MCK_OVER_1024 0x0a
// TC_CLKS_MCK1024 0x4

// application specific defines
#define PIO_INTERRUPT_LEVEL  6
#define IRQ0_INTERRUPT_LEVEL 2
#define SOFT_INTERRUPT_LEVEL 5
#define FIQ_INTERRUPT_LEVEL  0

// FreeRTOS stuff
// Default Task Priorities (configMAX_PRIORITIES = 5)
#define mainPOWER_TASK_PRIORITY    (tskIDLE_PRIORITY + 4)
#define mainTWI_TASK_SET_PRIORITY  (tskIDLE_PRIORITY + 3)
#define mainTWI_TASK_READ_PRIORITY (tskIDLE_PRIORITY + 1)
#define mainDISPLAY_TASK_PRIORITY  (tskIDLE_PRIORITY + 2)
#define mainCOM_TEST_PRIORITY      (tskIDLE_PRIORITY + 2)
#define mainCAN_TEST_PRIORITY      (tskIDLE_PRIORITY + 3) // Task removed
#define mainOPERATE_TASK_PRIORITY  (tskIDLE_PRIORITY + 2)

#define IRamNV(var) var __attribute__ ((section(".IRamNV")))
#define RAMFUNC __attribute__ ((long_call, section (".ramsection")))
// -----------------------------------------------------------

// -----------------------------------------------------------
// External Function Prototypes
// -----------------------------------------------------------

// -----------------------------------------------------------
// main.c
// -----------------------------------------------------------
// defines
#define TC_CLKS         0x7
#define TC_CLKS_MCK2    0x0
#define TC_CLKS_MCK8    0x1
#define TC_CLKS_MCK32   0x2
#define TC_CLKS_MCK128  0x3
#define TC_CLKS_MCK1024 0x4

#define INTERRUPT_PRIORITY0  0
#define INTERRUPT_PRIORITY1  1
#define INTERRUPT_PRIORITY2  2
#define INTERRUPT_PRIORITY3  3
#define INTERRUPT_PRIORITY4  4
#define INTERRUPT_PRIORITY5  5
#define INTERRUPT_PRIORITY6  6
#define INTERRUPT_PRIORITY7  7

// variables
// function prototypes
// -----------------------------------------------------------


// -----------------------------------------------------------
// Bm_num.c
// -----------------------------------------------------------
// defines

// variables
struct NumericFont {
       unsigned char gap;
       unsigned char height;
       const unsigned char *bitmap[14];
       };

extern struct NumericFont const RDSfnt0;
extern struct NumericFont const RDSfnt1;
extern struct NumericFont const RDSfnt2;
extern struct NumericFont const RDSfnt3;
extern struct NumericFont const RDS7x11;
extern struct NumericFont const ComicSansBold12;
extern struct NumericFont const ComicSansBold10;
extern struct NumericFont const ComicSans26;
extern struct NumericFont const ComicSans28;

// function prototypes
// -----------------------------------------------------------


// -----------------------------------------------------------
// bm_f1all.c
// -----------------------------------------------------------
#include "bm_f1all.h"

// defines
#define MAXLANGUAGES 2

// variables
struct fn1 {
       unsigned char spc;
       const unsigned char *f1[256];
       };
extern struct fn1 const fnt1_EUR;
extern struct fn1 const fnt1_CYR;
extern struct fn1 const * const fnt1[MAXLANGUAGES];

// function prototypes
// -----------------------------------------------------------


// -----------------------------------------------------------
// Bm_sym.c
// -----------------------------------------------------------
// defines

// variables
extern const unsigned char TAB_PAGE[82];
extern const unsigned char TABENT[86]; 
extern const unsigned char TABESC[86];

extern const unsigned char WARNING_CHOP_KNIV_POS[530];

extern const unsigned char SWCLOSED[16];
extern const unsigned char SWOPEN[16];

extern const unsigned char WARNING_KNIFEPRES[530];

extern const unsigned char Tick[20]; 

// ---------------McHale Bitmaps-----------------------
extern const unsigned char McHale_V6[769];
extern const unsigned char MODEAUTO[56];
extern const unsigned char MODEMAN[47];

extern const unsigned char BalerHitch[66];

extern const unsigned char Balerdoorclosed_withoutkicker[342];
extern const unsigned char Balerdooropen_withoutkicker[372];

extern const unsigned char BalerCenter[416];
extern const unsigned char ROLL01[53];
extern const unsigned char ROLL02[53];
extern const unsigned char ROLL03[53];
extern const unsigned char ROLL04[53];
extern const unsigned char ROLL05[53];
extern const unsigned char ROLL06[53];
extern const unsigned char ROLL07[53];
extern const unsigned char ROLL08[53];

extern unsigned char const * const ROLLER[8];
extern const unsigned char NETROLL_OUTLINE[53];
extern const unsigned char OILCAN[90];
extern const unsigned char GRASS01[30];
extern const unsigned char GRASS02[30];
extern const unsigned char GRASS03[30];
extern const unsigned char GRASS04[30];
extern const unsigned char GRASS05[30];
extern const unsigned char GRASS06[30];

extern unsigned char const * const CROPFEED[12];
extern const unsigned char SIDEDISPLAY_1[342];
extern const unsigned char SIDEDISPLAY_2[342];

extern const unsigned char TABNET[82];
extern const unsigned char TABNAV01[122];
extern const unsigned char TABKNFON[82]; 
extern const unsigned char TABKNFOFF[82];
extern const unsigned char TABKNFDIV[82];
extern const unsigned char TABDIVERT[82];
extern const unsigned char TABAUTOMAN[82];
extern const unsigned char TAB[82];
extern const unsigned char TABMAN[82];
extern const unsigned char TABNAV00[122];
extern const unsigned char TABNAV_UDLR[122];
extern const unsigned char TABNAV_UDL[122];
extern const unsigned char TABNAV_DLR[122];
extern const unsigned char TABNAV_ULR[122];
extern const unsigned char TABNAV_DL[122];
extern const unsigned char TABNAV_UL[122];
extern const unsigned char TABNAV_LR[122];
extern const unsigned char TABNAV_L[122];
extern const unsigned char TABNAV_R[122];
extern const unsigned char TABNAV_[122];
extern const unsigned char TABNAV_MENU_DL[122];
extern const unsigned char KICK[12];
extern const unsigned char KICKER_UP[26];
extern const unsigned char KICKER_DOWN[26];
extern const unsigned char KNIFE[26];
extern const unsigned char TABRESET[82];
extern const unsigned char BRIGHTN1[24];
extern const unsigned char CONTRST1[20];
extern const unsigned char DAYSYM[106];
extern const unsigned char NIGHTSYM[62];
extern const unsigned char TABPLUS[82];
extern const unsigned char TABMINUS[82];
extern const unsigned char BUTTON01[22];
extern const unsigned char BUTTON03[22];
extern const unsigned char BUTTON04[22];
extern const unsigned char BATTSLIM[18];
extern const unsigned char TABLEFT[82];
extern const unsigned char TABRIGHT[82];  
extern const unsigned char TABTICK[82];
extern const unsigned char TABCROSS[82];
extern const unsigned char TAB_DENSITY_RELEASE[82];
extern const unsigned char TAB_NET_CUT[82];
extern const unsigned char WARNINGBOX[102];
extern const unsigned char BAR_UNIT[16];
extern const unsigned char WARNING_DROPFLOOR[530];
extern const unsigned char Bale[254];
extern const unsigned char WARNING_LOWBATT[530];
extern const unsigned char WARNING_CHECKNET[530];
extern const unsigned char ENT[24];
extern const unsigned char POWER[24];
extern const unsigned char BRICK1[56];
extern const unsigned char BAR[145];
extern const unsigned char DENSITY[26];

// --------------Iconized menu Bitmaps (start of)------

extern const unsigned char ICON_BALE_COUNT_MENU[219];
extern const unsigned char ICON_BALE_COUNT_MENU_SELECTED[219];
extern const unsigned char ICON_BALE_SETUP_MENU[219];
extern const unsigned char ICON_BALE_SETUP_MENU_SELECTED[219];
extern const unsigned char ICON_MACHINE_SETUP_MENU[219];
extern const unsigned char ICON_MACHINE_SETUP_MENU_SELECTED[219];
extern const unsigned char ICON_OPERATOR_SETUP_MENU[219];
extern const unsigned char ICON_OPERATOR_SETUP_MENU_SELECTED[219];

// ----------------------------------------------------

extern const unsigned char ICON_BALE_COUNT[41];
extern const unsigned char ICON_BALE_DENSITY[173];
extern const unsigned char ICON_BALE_DENSITY_SELECTED[173];
extern const unsigned char ICON_BALE_DIAMETER_SELECTED[740];
extern const unsigned char ICON_BALE_INDICATOR[138];
extern const unsigned char ICON_BALE_INDICATOR_BEEP[38];
extern const unsigned char ICON_BALE_INDICATOR_OFF[38];
extern const unsigned char ICON_BALE_INDICATOR_ON[38];
extern const unsigned char ICON_BALE_INDICATOR_SELECTED[138];
extern const unsigned char ICON_BALE_PROFILE_A[36];
extern const unsigned char ICON_BALE_PROFILE_B[36];
extern const unsigned char ICON_BALE_PROFILE_C[36];
extern const unsigned char ICON_BALE_PROFILE_CORE_DIAMETER_BALE_DIAMETER[740];
extern const unsigned char ICON_BALE_PROFILE_D[36];
extern const unsigned char ICON_BALE_PROFILE_E[36];
extern const unsigned char ICON_BALE_PROFILE_SELECTED[740];
extern const unsigned char ICON_CORE_DENSITY[154];
extern const unsigned char ICON_CORE_DENSITY_SELECTED[154];
extern const unsigned char ICON_CORE_DIAMETER_SELECTED[740];
extern const unsigned char ICON_CURRENT_SELECTION_INDICATOR[28];
extern const unsigned char ICON_DIAMETER_CORRECTION[288];
extern const unsigned char ICON_DIAMETER_CORRECTION_SELECTED[288];
extern const unsigned char ICON_FILL_POT_ZERO[206];
extern const unsigned char ICON_FILL_POT_ZERO_SELECTED[206];
extern const unsigned char ICON_GRAND_TOTAL_BALES[41];
extern const unsigned char ICON_LUBE_COUNT[41];
extern const unsigned char ICON_NET_DELAY[194];
extern const unsigned char ICON_NET_DELAY_SELECTED[194];
extern const unsigned char ICON_NET_LAYERS[82];
extern const unsigned char ICON_NET_LAYERS_SELECTED[82];
extern const unsigned char ICON_NET_METRES[254];
extern const unsigned char ICON_NET_METRES_SELECTED[254];
extern const unsigned char ICON_PRE_WARNING_BEEP[230];
extern const unsigned char ICON_PRE_WARNING_BEEP_SELECTED[230];
extern const unsigned char ICON_SCROLL_BAR_PAGE_1_OF_2[170];
extern const unsigned char ICON_SCROLL_BAR_PAGE_2_OF_2[170];
extern const unsigned char ICON_SELECTION_POINTER[15];

extern const unsigned char ICON_NET_TWINE_WITH_TWINE_HIGHLIGHTED[277];
extern const unsigned char ICON_NET_TWINE_WITH_NET_HIGHLIGHTED[277];
extern const unsigned char ICON_NET_TWINE_SELECTION_POINTER[52];

// ----------------------------------------------------

extern const unsigned char ICON_BRIGHTNESS_AND_CONTRAST_MENU[100];
extern const unsigned char ICON_BRIGHTNESS_AND_CONTRAST_MENU_SELECTED[100];
extern const unsigned char ICON_DIAGNOSTICS_SCREENS[100];
extern const unsigned char ICON_DIAGNOSTICS_SCREENS_SELECTED[100];
extern const unsigned char ICON_TECHNICIAN_MENU[65];
extern const unsigned char ICON_TECHNICIAN_MENU_SELECTED[65];
extern const unsigned char ICON_TIME_AND_DATE_ADJUST_MENU[107];
extern const unsigned char ICON_TIME_AND_DATE_ADJUST_MENU_SELECTED[107];

// ----------------------------------------------------

extern const unsigned char TABNAV_DR[122];
extern const unsigned char TABNAV_U[122];
extern const unsigned char TABNAV_UDR[122];
extern const unsigned char TABNAV_UR[122];
extern const unsigned char TABNAV_D[122];
extern const unsigned char TABNAV_UD[122];

// ----------------------------------------------------

extern const unsigned char ICON_BALE_COUNT_RESET[233];
extern const unsigned char ICON_FILL_POT_ZERO_RESET[189];
extern const unsigned char ICON_LUBE_COUNT_RESET[233];
extern const unsigned char ICON_NET_METRES_RESET[233];

// ----------------------------------------------------

extern const unsigned char ICON_CLOCK_AT_TEN_TO_TWO[32];
extern const unsigned char UNDERLINE_TWO_DIGIT[6];
extern const unsigned char UNDERLINE_FOUR_DIGIT[8];

// ----------------------------------------------------

extern const unsigned char TAB_MAKE_SELECTION_CURRENT[82];
extern const unsigned char TABRESET_LUBECOUNT[82];

// ----------------------------------------------------

extern const unsigned char LUBE_COUNT_RESET_PROMPT[514];
extern const unsigned char TABCYCLE_ATOJ[82];


// --------------Iconized menu Bitmaps (end of)--------

extern const unsigned char CROSS[59];
extern const unsigned char TICK[59];
extern const unsigned char SPEAKER[59];

extern const unsigned char Clock[32];
extern const unsigned char DIASCALE[83];
extern const unsigned char DIASCALE_FROM_ZERO[83];
extern const unsigned char DIAMIND[7];
extern const unsigned char DRIVING_ARROWS_NONE[101];
extern const unsigned char DRIVING_ARROWS_X1_LEFT[101];
extern const unsigned char DRIVING_ARROWS_X1_RIGHT[101];
extern const unsigned char DRIVING_ARROWS_X2_LEFT[101];
extern const unsigned char DRIVING_ARROWS_X2_RIGHT[101];
extern const unsigned char DRIVING_ARROWS_X3_LEFT[101];
extern const unsigned char DRIVING_ARROWS_X3_RIGHT[101];
extern const unsigned char DRIVING_ARROWS_X4_LEFT[101];
extern const unsigned char DRIVING_ARROWS_X4_RIGHT[101];
extern const unsigned char WARNING_DENSITY_PRESSURE_SCREEN_IMAGE[1161];
// -----------------------------------------------------------
                             
// function prototypes
// -----------------------------------------------------------


// -----------------------------------------------------------
// display.c
// -----------------------------------------------------------
// defines
#define configDISPLAY_STACK_SIZE (configMINIMAL_STACK_SIZE + 600) // uses sprintf and other stuff

#define DISPLAY_DEFAULT_PANEL_LOADING 0x02  // 20nF-56nF
#define DISPLAY_DEFAULT_BIAS_RATIO    0x01  // 0x00 5, 0x01 10, 0x02 11, 0x03 12
#define DISPLAY_DEFAULT_CHARGE_PUMP   0x03
#define DISPLAY_DEFAULT_GRAY          0x00
#define DISPLAY_DEFAULT_CONTRAST      0x20

#define DISPLAY_DEFAULT_DAY_CONTRAST 0x28

#define DISPLAY_DEFAULT_TEMPCOMP   0x00
#define DISPLAY_DEFAULT_PARTIAL    0x02
#define DISPLAY_DEFAULT_GAIN       0x80    // 0x00 0, 0x40 1, 0x80 2, 0xc0 3
#define DISPLAY_DEFAULT_COLUMNS    0x07
#define DISPLAY_DEFAULT_LINE_RATE  0x02    // 0x00 20.0kHz, 0x01 23.2kHz, 0x02 27.2kHz, 0x03 32.0kHz
#define DISPLAY_DEFAULT_GRAY_SCALE 0x00
// start on page 16 (line 32) for PM5718 240x128
#define DISPLAY_DEFAULT_STARTLINE  32
#define DISPLAY_DEFAULT_ENDLINE    159
#define DISPLAY_DEFAULT_COMEND     159
#define DISPLAY_DEFAULT_LINERATE   0x02
#define DISPLAY_DEFAULT_MAPPING    0x00
#define DISPLAY_DEFAULT_RAMCONTROL 0x01
#define DISPLAY_DEFAULT_MAXCOLUMN  199

#define DISPLAY_CONTRAST_MIN  0x00
#define DISPLAY_CONTRAST_MAX  0x3f
#define DISPLAY_DEFAULT_MASK  0xff

#define DISPLAY_DEFAULT_BRIGHTNESS 76 // multiple of 4 needed as step size is 4

#define DISPLAY_DEFAULT_DAY_BRIGHTNESS 100 // multiple of 4 needed as step size is 4

#define AND    0x0001
#define OR     0x0002
#define INV    0x0004
#define XOR    0x0008
#define EMPTY  0x0010
#define FULL   0x0020
#define CLEAR  0x0040
#define GREY   0x0080
#define INVCLP 0x0100
/*#define EMPTYCLP 0x0200
#define FULLCLP 0x0400*/

// display number/test
#define FIXED      0x00
#define EDIT       0x01
#define CENTER     0x02
//#define INV      0x04
#define UNDERLINED 0x08
#define dn_JUST    0x10
#define LEFT       0x00
#define RIGHT      0x10
#define dn_SIGN    0x20
#define NSIGN      0x20
#define EDSCAPE    0x40
//#define GREY     0x80

#define GRAPHICS_SIZE (160 * 16)
#define COLUMNS 20 // 20 columns of 8 bytes for 160 pixel wide display

#define TS4    4
#define TS5    5
#define TS10   10
#define TS15   15
#define TSNAME 19
#define TSMAX  26

#define FARRIGHT 156

#define DP0 0
#define DP1 1
#define DP2 2
#define DP3 3

#define DW1 6
#define DW2 12
#define DW3 18
#define DW4 24
#define DW5 30
#define DW6 36

#define DM_COMMAND 1
#define DM_DATA    0

/* number program options */
#define PMODE  0x08
#define PINC   0x08
#define PENT   0x00

#define PTYPE  0x07
#define PFLOAT 4
#define PLONG  3
#define PSHORT 2
#define PCHAR  1
#define PNULL  0x00

/* text program options */
#define EDIT_UPPER   0x01
#define EDIT_LOWER   0x02
#define EDIT_NUMERIC 0x03

// function prototypes
extern void setPixel( unsigned char *buff, unsigned char x, unsigned char y, unsigned int method );
extern void circlePoints( unsigned char *buff, unsigned char cx, unsigned char cy, unsigned char x, unsigned char y, unsigned int method );
extern void circleMidpoint( unsigned char *buff, unsigned char xCenter, unsigned char yCenter, unsigned char radius, unsigned int method );
extern unsigned char display_f1_text( const unsigned char *data, unsigned char max, unsigned char co, unsigned char ro, unsigned char prog );
extern unsigned char display_number( float data, float min, float max, float con,
  unsigned char dp, unsigned char xc, unsigned char yc, unsigned char width, unsigned char just,
  struct NumericFont const *font, void *factor, unsigned char ftype );
extern unsigned char greatest( unsigned char *a, unsigned char *b );
extern void draw_line( unsigned char *buff, unsigned char xp1, unsigned char yp1, unsigned char xp2, unsigned char yp2, unsigned int method );
extern void draw_box( unsigned char *buff, unsigned char xp1, unsigned char yp1,unsigned char xp2, unsigned char yp2, unsigned int method );
extern void clip_box( unsigned char xp1, unsigned char yp1, unsigned char xp2, unsigned char yp2 );
extern unsigned char write_bitmap( unsigned char *ptd, const unsigned char *pts, unsigned char xpos, unsigned char ypos, unsigned int method );
extern void draw_frame( unsigned char *pt );

#ifndef WINDOWS
 extern void SendToDisplay( unsigned char *pbuffer, int count, char mode );
 extern void WriteDisplay( unsigned char *pts );
 extern void SPI0Setup( void );
 extern void InitilizeDisplay( void );
 // static void vDisplayTask( void *pvParameters );
 extern void vStartDisplayTask( unsigned portBASE_TYPE uxPriority );
#else
 /*extern void sim_main( int );*/
 /*extern void WriteToScreen( void );*/
#endif
// -----------------------------------------------------------

#ifndef WINDOWS
// -----------------------------------------------------------
// timers.c - thumb
// -----------------------------------------------------------
// function prototypes
extern void AT91F_TC_Open( AT91PS_TC TC_pt, unsigned int Mode, unsigned int TimerId );
// -----------------------------------------------------------


// -----------------------------------------------------------
// interrupts.c - arm
// -----------------------------------------------------------
// defines

// Reading analogue inputs: Bale Diameter (AnIP1); (Bale) Density Pressure (AnIP2); Bale Shape Indicator (AnIP3); Knife Pressure (AnIP4)
#define NUM_ACCUMULATED_SAMPLES       50  // Accumulating 50 samples to be averaged -- collected in 50 ms (subsequently used in Rolling Average Calculation called every 100 ms)
#define INPUT_ACCURACY_MULTIPLIER 100000  // Reading, storing & accumulating inputs at a resolution of 1/100000 = 0.00001 V

// function prototypes
// The following functions must be written in ARM mode this function called directly by exception vector
extern void TC0_irq_handler( void ) __attribute__((interrupt("IRQ")));
extern void TC1_irq_handler( void ) __attribute__((interrupt("IRQ")));
extern void TC4_irq_handler( void ) __attribute__((interrupt("IRQ")));
extern void TC5_irq_handler( void ) __attribute__((interrupt("IRQ")));
extern void TC6_irq_handler( void ) __attribute__((interrupt("IRQ")));
// any which can cause switch need naked attribute + interrupt function call
// PIOB0 can cause switch so use naked attribute
extern void PIOB_irq_handler( void ) __attribute__ ((naked));
// ADC0 can cause switch so use naked attribute
extern void ADC0_irq_handler( void ) __attribute__ ((naked));

//extern void CAN0_irq_handler( void ) __attribute__((naked)); // Removed
//void CAN0_irq_function( void );

extern void debug_serial_setup( void );
extern void debug_message_wait( void );
extern void debug_message( unsigned char *pt );
extern unsigned char *IntToString( unsigned long int i );
extern void DebugAbortMessage( unsigned char messageID, int *lnk_ptr ) __attribute__ ((noreturn));
extern void AT91F_Undefined_Instruction_handler( void ) __attribute__ ((naked,noreturn)); // undefvec - 0x04 Undefined Instruction
extern void AT91F_Prefetch_Abort_handler( void ) __attribute__((naked,noreturn)); // pabtvec - 0x0C Prefetch Abort
extern void AT91F_Data_Abort_handler( void ) __attribute__((naked,noreturn)); // dabtvec - 0x10 Data Abort
extern void AT91F_Reserved_handler( void ) __attribute__((naked,noreturn)); // rsvdvec - 0x14 reserved 
extern void AT91F_Default_FIQ_handler( void ) __attribute__((naked,noreturn));
extern void AT91F_Default_IRQ_handler( void ) __attribute__((naked,noreturn));
extern void AT91F_Spurious_handler( void ) __attribute__((interrupt("IRQ")));
// -----------------------------------------------------------


// -----------------------------------------------------------
// serial.c
// -----------------------------------------------------------
// defines

/* Constants to setup and access the UART. */
#define serINVALID_QUEUE   ( ( xQueueHandle ) 0 )
#define serHANDLE          ( ( xComPortHandle ) 1 )

#define serBLOCK_TIME      ( (portTickType)250 )

#define BAUD9600    ( (configCPU_CLOCK_HZ / 16) / 9600 )   /* Baud Rate */
#define BAUD19200   ( (configCPU_CLOCK_HZ / 16) / 19200 )  /* Baud Rate */
#define BAUD38400   ( (configCPU_CLOCK_HZ / 16) / 38400 )  /* Baud Rate */
#define BAUD115200  ( (configCPU_CLOCK_HZ / 16) / 115200 ) /* Baud Rate */

// variables
typedef void *xComPortHandle;

extern xQueueHandle RxCOM0;
extern xQueueHandle TxCOM0;

// function prototypes
extern void vSerialISRCreateQueues( unsigned portBASE_TYPE uxQueueLength, xQueueHandle *pRxCOM0, xQueueHandle *pTxCOM0 );
extern xComPortHandle xSerialPortInit( unsigned portLONG ulWantedBaud, unsigned portBASE_TYPE uxQueueLength );
extern signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed portCHAR *pcRxedChar, portTickType xBlockTime );
extern void vSerialPutString( xComPortHandle pxPort, const signed portCHAR * const pcString, unsigned portSHORT usStringLength );
extern signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed portCHAR cOutChar, portTickType xBlockTime );
extern void vSerialClose( xComPortHandle xPort );
// -----------------------------------------------------------


// -----------------------------------------------------------
// comtest.c
// -----------------------------------------------------------
// defines
#define comTx_STACK_SIZE (configMINIMAL_STACK_SIZE + 480) // uses sprintf
#define comRx_STACK_SIZE configMINIMAL_STACK_SIZE

/* The Tx task will transmit the sequence of characters at a pseudo random
interval.  This is the maximum and minimum block time between sends. */
#define comTX_MAX_BLOCK_TIME ((portTickType)800) //0x96)
#define comTX_MIN_BLOCK_TIME ((portTickType)200) //0x32)
#define comOFFSET_TIME       ((portTickType)55)  //3)

/* We should find that each character can be queued for Tx immediately and we
don't have to block to send. */
#define comNO_BLOCK ((portTickType)0)

/* The Rx task will block on the Rx queue for a long period. */
#define comRX_BLOCK_TIME ((portTickType)(50 / portTICK_RATE_MS))   // 50ms
#define comTX_BLOCK_TIME ((portTickType)(250 / portTICK_RATE_MS))  // 250ms
#define comRX_ACK_TIME   ((portTickType)(1000 / portTICK_RATE_MS)) // 1s

#define comBUFFER_LEN (unsigned portBASE_TYPE)128

// variables
extern xComPortHandle xPort;

// function prototypes
// static void vComTxTask( void *pvParameters );
// static void vComRxTask( void *pvParameters );
extern void vStartComTasks( unsigned portBASE_TYPE uxPriority, unsigned portLONG ulBaudRate );
// -----------------------------------------------------------


// -----------------------------------------------------------
// TWI_task.c
// -----------------------------------------------------------
// defines
#define twiSTACK_SIZE configMINIMAL_STACK_SIZE

// variables
// preserved

// zeroed
xSemaphoreHandle SetTime;
xTaskHandle TWItaskhandle;

// function prototypes
// static void vTWI_task( void *pvParameters )
extern void vStartTWITask( unsigned portBASE_TYPE uxPriority );
// -----------------------------------------------------------


// -----------------------------------------------------------
// power.c
// -----------------------------------------------------------
// defines
#define configPOWER_STACK_SIZE configMINIMAL_STACK_SIZE

// CS0 = FLASH
#define SPI1_FLASH_MODE (AT91C_SPI_MSTR | AT91C_SPI_PS_FIXED | (AT91C_SPI_PCS & ((unsigned int)0 << 16)))
// CS1 == ATOD
#define SPI1_ATOD_MODE (AT91C_SPI_MSTR | AT91C_SPI_PS_FIXED | (AT91C_SPI_PCS & ((unsigned int)1 << 16)))

// M25PE40 serial flash stuff
#define pFac_BASEADDRESS 0x00000
#define pRun_BASEADDRESS 0x01000

#define RFS_CLEAR    0
#define RFS_CS1FAIL  1
#define RFS_CS2FAIL  2
#define RFS_SW1FAIL  3
#define RFS_IS1FAIL  4
#define RFS_RV1FAIL  5
#define RFS_SW2FAIL  6
#define RFS_IS2FAIL  7
#define RFS_RV2FAIL  8

#define SFS_WEL  0x02
#define SFS_WIP  0x01

#define SFC_WREN 0x06
#define SFC_WRDI 0x04
#define SFC_RDID 0x9f
#define SFC_RDSR 0x05
#define SFC_READ 0x03
#define SFC_FAST 0x0b
#define SFC_PW   0x0a
#define SFC_PP   0x02
#define SFC_PE   0xdb
#define SFC_SE   0xd8
#define SFC_DP   0xb9
#define SFC_RDP  0xab

#define ADC_WAKEUP1  0x00
#define ADC_RDATA    0x01
#define ADC_RDATAC   0x03
#define ADC_SDATAC   0x0f
#define ADC_RREG     0x10
#define ADC_WREG     0x50
#define ADC_SELFCAL  0xf0
#define ADC_SELFOCAL 0xf1
#define ADC_SELFGCAL 0xf2
#define ADC_SYSOCAL  0xf3
#define ADC_SYSGCAL  0xf4
#define ADC_SYNC     0xfc
#define ADC_STANDBY  0xfd
#define ADC_RESET    0xfe
#define ADC_WAKEUP2  0xff

#define ADR_STATUS 0x00
#define ADR_MUX    0x01
#define ADR_ADCON  0x02
#define ADR_DRATE  0x03
#define ADR_IO     0x04
#define ADR_OFC0   0x05
#define ADR_OFC1   0x06
#define ADR_OFC2   0x07
#define ADR_FSC0   0x08
#define ADR_FSC1   0x09
#define ADR_FSC2   0x0a

// variables
// preserved
// zeroed
xTaskHandle POWERtaskhandle;

// function prototypes
extern void SPI1Setup( void );
extern void SPI1TransferData( unsigned int device, unsigned char *data, unsigned short int length );
extern void SPI1FlashRead( unsigned long int address, unsigned char *data, unsigned short int length );
extern void SPI1FlashWrite( unsigned char command, unsigned long int address, unsigned char *data, unsigned short int length );
// static void vPowerTask( void *pvParameters )
extern void vStartPowerTask( unsigned portBASE_TYPE uxPriority );
// -----------------------------------------------------------


// -----------------------------------------------------------
// Operate.c
// -----------------------------------------------------------
// defines
#define configOPERATE_STACK_SIZE configMINIMAL_STACK_SIZE

// variables
// - initilised
// - preserved
// - zeroed

// function prototypes
// static void vOperateTask( void *pvParameters )
extern void vStartOperateTask( unsigned portBASE_TYPE uxPriority );
// -----------------------------------------------------------


// -----------------------------------------------------------
// CANtest.c
// -----------------------------------------------------------
// defines
//#define canTx_STACK_SIZE configMINIMAL_STACK_SIZE // Removed
//#define canRx_STACK_SIZE configMINIMAL_STACK_SIZE // Removed

//#define CAN_MAXRX 20 // Removed
//#define CAN_MAXTX 10 // Removed

#if 0  // Removed
union CanData {
  unsigned int  l[2]; // ints are 4 bytes long
  unsigned char c[8];
  };
#endif

#if 0  // Removed
// must we align word data for interrupt else we get a prefetch abort?
// struct CanMessage CanM __attribute__ ((aligned (16)));
struct CanMessage {
  unsigned int Id;
  union CanData Data;
  };
//  } __attribute__ ((aligned(16)));
#endif

// variables
//extern xQueueHandle RxCAN_MCM, TxCAN; // Removed

// function prototypes
//extern void InitialiseCAN( void );
//static void vCANTxTask( void *pvParameters );
//static void vCANRxTask( void *pvParameters );
//extern void vStartCANTasks( unsigned portBASE_TYPE uxPriority ); // Removed
// -----------------------------------------------------------
#endif /* ifndef WINDOWS */


// -----------------------------------------------------------
// CAN_mcm.c
// -----------------------------------------------------------
//#define CAN_MAXRX 20 // Removed
//#define CAN_MAXTX 10 // Removed

//#define CAN_TIMEOUT 250 // 250ms // Removed

//#define CAN_RXID_MCM ((0x084 << 18) & AT91C_CAN_MIDvA) // Removed
//#define CAN_TXID_MCM ((0x085 << 18) & AT91C_CAN_MIDvA) // Removed
//#define CAN_IDMASK_MCM (~(0x0f << 21) & AT91C_CAN_MIDvA) // Removed
//#define CAN_RXID_ACM ((0x204 << 18) & AT91C_CAN_MIDvA)
//#define CAN_TXID_ACM ((0x205 << 18) & AT91C_CAN_MIDvA)
//#define CAN_RXID_DIP ((0x104 << 18) & AT91C_CAN_MIDvA)
//#define CAN_RXID_DIP ((0x105 << 18) & AT91C_CAN_MIDvA)
//#define CAN_RXID_DOP ((0x184 << 18) & AT91C_CAN_MIDvA)
//#define CAN_RXID_DOP ((0x185 << 18) & AT91C_CAN_MIDvA)
//#define CAN_RXID_HBM ((0x284 << 18) & AT91C_CAN_MIDvA)
//#define CAN_RXID_HBM ((0x285 << 18) & AT91C_CAN_MIDvA)
//#define CAN_RXID_WCM ((0x604 << 18) & AT91C_CAN_MIDvA)
//#define CAN_TXID_WCM ((0x605 << 18) & AT91C_CAN_MIDvA)

//#define CAN_MODULE_MESSAGE_RECEIVED       0x01 // Removed
//#define CAN_MODULE_PRESENT                0x02 // Removed

//#define METER_AUTO 0        // Removed
//#define METER_MANUAL 1      // Removed
//#define METER_CALIBRATION 2 // Removed

//#define MCM_01 0  // Removed 
#define MAX_MCM 1

//#define DIP1 0x01 // Removed
//#define DIP2 0x02 // Removed
//#define DIP3 0x04 // Removed
//#define DIP4 0x08 // Removed

//#define MAX_PRODUCTS 17 // Removed


// Bale Totals (in Bale Stores A to J)
#define MAX_STORES  10

// Programmable Factors (Array) 
#define PF_STEP   0  // Step size
#define PF_MIN    1  // Minimum
#define PF_MAX    2  // Maximum
#define PF_DEF    3  // Default
#define PF_DIM    4  // Array dimension

// Programmable Factors (Array Elements) 
#define PF_cNA        0 // Not Applicable (Char type)
#define PF_fNA      0.0 // Not Applicable (Float type)

// Bale Profile
#define BP_PRESET_A    0
#define BP_PRESET_B    1
#define BP_PRESET_C    2
#define BP_PRESET_D    3
#define BP_PRESET_E    4
#define BP_PRESET_MAX  5

// Bale Shape Indicator
#define BSI_OFF    0
#define BSI_ON     1
#define BSI_BEEP   2

/* Bale Indicator */
#define BI_STEER   0
#define BI_FILL    1

/* Net System */
#define NS_BEAK    0
#define NS_ROLLER  1

/* Twine Option*/
#define TO_OFF    0
#define TO_ON  	  1

/* Twine or Net*/
#define TO_NET 	  0
#define TO_TWINE  1
#define OP_ROLL_ENGAGE_TIMER 30 /*3 seconds*/

/* Chopping Knives Fitted */
#define CKF_DISABLED  0
#define CKF_ENABLED   1


#define DIA_FIXED_MULTIPLIER 0.013 // Bale diameter fixed multiplier

#define NET_METERS_PER_PULSE 0.047 // m/pulse 

  
// Programmable Factors (Array)

// Machine Menu -- Step size, Minimum, Maximum, Default
extern unsigned char const PF_GW302_BaleProfile[PF_DIM];
extern unsigned char const PF_GW302_BaleDiameter[PF_DIM];
extern unsigned char const PF_GW302_CoreDiameter[PF_DIM];
extern unsigned char const PF_GW302_BaleDensity[PF_DIM];
extern unsigned char const PF_GW302_CoreDensity[PF_DIM];
extern float const         PF_GW302_NetLayers[PF_DIM];
extern unsigned long int const PF_GW302_BaleTotal[PF_DIM];
extern unsigned char const PF_GW302_BaleStore[PF_DIM];
// LubeCounter             // Moved to preserved run-time totals 
extern unsigned long int const PF_GW302_GrandBaleTotal[PF_DIM]; 
extern float const         PF_GW302_DiaPotZero[PF_DIM];
extern signed char const   PF_GW302_DiameterCorrection[PF_DIM];
extern unsigned char const PF_GW302_BaleShapeIndicator[PF_DIM];
extern float const         PF_GW302_BaleShapeIndicatorZero[PF_DIM];
extern float const         PF_GW302_TotalNetMetres[PF_DIM];
extern unsigned char const PF_GW302_PreWarningBeep[PF_DIM];
extern float const         PF_GW302_NetDelay[PF_DIM];
extern unsigned char const PF_GW302_NetTwineOption[PF_DIM];

// Technician Menu - Baler Full Page 1/2 -- Step size, Minimum, Maximum, Default
extern unsigned short int const PF_GW302_LubeAlarm[PF_DIM];
extern float const              PF_GW302_PresDeadband[PF_DIM];
extern float const              PF_GW302_NetSpeed[PF_DIM];
extern float const              PF_GW302_PresSwDelay[PF_DIM];
extern float const              PF_GW302_DoorClosed[PF_DIM];
extern unsigned char const      PF_GW302_BaleSizeFactor[PF_DIM];

// Technician Menu - Baler Full Page 2/2 -- Step size, Minimum, Maximum, Default
extern float const         PF_GW302_FillPotRange[PF_DIM];
extern float const         PF_GW302_FillPotIgnore[PF_DIM];
extern unsigned char const PF_GW302_EngageDiameter[PF_DIM];
extern unsigned char const PF_GW302_BaleIndicator[PF_DIM];
extern float const         PF_GW302_RollerDelay[PF_DIM];
  
// Technician Menu - Netting Setup Page 1/2 -- Step size, Minimum, Maximum, Default
extern float const         PF_GW302_NetOn[PF_DIM];
extern unsigned char const PF_GW302_NetCutPulse[PF_DIM];
extern float const         PF_GW302_NetCutTime[PF_DIM];
extern float const         PF_GW302_MinDiaPulsPerLay[PF_DIM];
extern float const         PF_GW302_MaxDiaPulsPerLay[PF_DIM];
extern float const         PF_GW302_NetFactor[PF_DIM];
extern float const         PF_GW302_NoNetPulse[PF_DIM];
extern unsigned char const PF_GW302_PreNetPulse[PF_DIM];

// Technician Menu - Netting Setup Page 2/2 -- Step size, Minimum, Maximum, Default
extern unsigned char const PF_GW302_NetSystem[PF_DIM];
extern unsigned char const PF_GW302_BeakEngage[PF_DIM];
extern unsigned char const PF_GW302_TwineOption[PF_DIM];          // Twine Option
  
// Technician Menu - Diameter Setup -- Step size, Minimum, Maximum, Default
extern unsigned char const PF_GW302_DiameterMin[PF_DIM];
extern unsigned char const PF_GW302_DiameterMax[PF_DIM];
extern float const         PF_GW302_DiaCorrection[PF_DIM];
extern unsigned char const PF_GW302_BaleDiaSmoothingFactor[PF_DIM];
extern unsigned char const PF_GW302_DensityPresSmoothingFactor[PF_DIM];
extern unsigned char const PF_GW302_BaleShapeSmoothingFactor[PF_DIM];
extern unsigned char const PF_GW302_KnifePresSmoothingFactor[PF_DIM];
 
// Technician Menu - Density Setup Page 1/2 -- Step size, Minimum, Maximum, Default
extern float const PF_GW302_Density1[PF_DIM];
extern float const PF_GW302_Density2[PF_DIM];
extern float const PF_GW302_Density3[PF_DIM];
extern float const PF_GW302_Density4[PF_DIM];
extern float const PF_GW302_Density5[PF_DIM];
extern float const PF_GW302_Density6[PF_DIM];

// Technician Menu - Density Setup Page 2/2 -- Step size, Minimum, Maximum, Default
extern float const PF_GW302_Density7[PF_DIM];
extern float const PF_GW302_Density8[PF_DIM];
extern float const PF_GW302_Density9[PF_DIM];
extern float const PF_GW302_Density10[PF_DIM];

// Technician Menu - Knives Setup -- Step size, Minimum, Maximum, Default
extern float const PF_GW302_KnifePressure[PF_DIM];
extern float const PF_GW302_KnifePresWarn[PF_DIM];
extern float const PF_GW302_DiverterDelay[PF_DIM];
  
// Technician Menu - Formation 2 -- Step size, Minimum, Maximum, Default
extern unsigned char const PF_GW302_FormationDensity[PF_DIM];
extern unsigned char const PF_GW302_FormationDiameter[PF_DIM];
extern unsigned char const PF_GW302_Formation2Density[PF_DIM];
extern unsigned char const PF_GW302_Formation2Diameter[PF_DIM];

// Diagnostics - Digital Inputs -- Step size, Minimum, Maximum, Default
extern unsigned char const PF_GW302_ChoppingKnivesFitted[PF_DIM];


// Bale Profile defaults for Presets A - E
extern unsigned char const PresetCoreDiameter[BP_PRESET_MAX];
extern unsigned char const PresetCoreDensity[BP_PRESET_MAX];
extern unsigned char const PresetBaleDiameter[BP_PRESET_MAX]; 
extern unsigned char const PresetBaleDensity[BP_PRESET_MAX];
extern unsigned char const PresetFormationDiameter[BP_PRESET_MAX];
extern unsigned char const PresetFormationDensity[BP_PRESET_MAX];
extern unsigned char const PresetFormation2Diameter[BP_PRESET_MAX];
extern unsigned char const PresetFormation2Density[BP_PRESET_MAX];
extern float const PresetNetLayers[BP_PRESET_MAX];


struct PinWin {
  unsigned short int PinNumber;
  void (*PinWindow)(unsigned int);  
  
  };
extern struct PinWin const PinOption;


// for rarely changed non volatile factors
struct mcmF {
  float encoder_ppr;
  float gearbox_ratio; 
  float shaft1_ppr;
  float shaft2_ppr;
  unsigned short int max_motor_rpm;
  unsigned short int PWM_frequency;
  unsigned short int factor_PI;
  unsigned short int factor_PD;
  unsigned char max_current;
  unsigned char send_to_id;
  };

// for frequently changed non volatile factors
struct mcmR {
  unsigned char product_selected;
  };

struct mcm_struct {
  // not saved
  unsigned char status;
  unsigned char motor_mode;
  unsigned short int timeout;
  unsigned short int software_type;
  unsigned short int software_issue;
  unsigned char reset_fault;
  unsigned char dil_setting;
  unsigned short int factor_R;
  unsigned char factor_D;

  unsigned short int motor_frequency;
  unsigned short int motor_speed;
  unsigned long int motor_pulse_accumulator;
  unsigned long int calibration_motor_pulse_accumulator;

  unsigned short int zero_speed_limit;
  unsigned short int speed_frequency;
  unsigned long int speed_pulse_accumulator;

  unsigned char motor_current;
  unsigned char dip_status;
  unsigned char dip_time_since_last_pulse[6];

  unsigned char controller_temperature;
  unsigned char controller_voltage;
  unsigned char controller_pwm;

  signed char motor_frequency_error;
  signed char speed_frequency_error;

  unsigned char priming;
  float target_motor_rpm;
  float target_motor_freq;

  float actual_rate;
  
  // saved when changed 
  struct mcmF *pF;
  
  // saved on power down 
  struct mcmR *pR;
  };
  
extern struct mcm_struct mcm[MAX_MCM];

#if 0 // Removed
// for rarely changed non volatile factors
struct prodtF {
  char name[20];
  float kg_per_revolution;
  float thousand_grain_weight;
  float scale_multiplier[2];
  };  
#endif

#if 0 // Removed
// for frequently changed non volatile factors
struct prodtR {
  float total_quantity;
  float part_quantity;
  float total_area;
  float part_area;
  
  float target_rate;
  float scale;
  unsigned char type;
  };
#endif

#if 0 // Removed
struct prodt {
  // saved when changed 
  struct prodtF *pF;
  
  // saved on power down 
  struct prodtR *pR;
  };
  
extern struct prodt product[MAX_MCM][MAX_PRODUCTS];
#endif

//extern void can_mcm_initialise( unsigned char msd );  // Removed
//extern void can_mcm_restart_autocal( unsigned char msd );  // Removed
//extern void can_mcm_setPID_info( unsigned char msd );  // Removed
//extern void can_mcm_routines( unsigned char msd );  // Removed
// -----------------------------------------------------------


// -----------------------------------------------------------
// screens.c
// -----------------------------------------------------------
// defines
#ifdef WINDOWS
 //#define M_TWOPI 6.28318530718  // Removed
#endif

//#define GAUGE_MIN (-M_TWOPI * 0.045) // negative!!   // Removed
//#define GAUGE_MAX (M_TWOPI * 0.340)                  // Removed
//#define GAUGE_STEP ((GAUGE_MAX - GAUGE_MIN) / 100.0) // Removed

#define CL0CK_ICON_OFF 0
#define CL0CK_ICON_ON  1

//#define METRIC 0    // Removed
//#define IMPERIAL 1  // Removed

// MWarn
#define MW_MASK         0x3F
#define MW_CLEAR        0x00
#define MW_DENSITYPRESS 0x01    // Density Pressure Too High 
#define MW_CHECKNET     0x02    // Check Net Error (AKA Net Warning)
#define MW_DROPFLOOR    0x04    // Drop Floor Warning (AKA Floor Position) 
#define MW_KNIFEPRES    0x08    // Knife maximum pressure
#define MW_CHOPKNIVPOS1 0x10    // Chopping knives position for bank of 11 knives
#define MW_LOWBAT       0x20    // Low battery warning (AKA Low voltage) 

// OKWarnBox
#define WARNMASK    0x03
#define OK          0x00
#define EXCL        0x01
#define LUBE        0x02

// variables
extern unsigned char reverttime, lastoutput, nextoutput, MWarn, OKWarnBox, KnifePresWasHigh, SecondScreen;
extern unsigned char Screen100Page;
extern unsigned char SelectedBaleStore;

// constant

// function prototypes
extern void screen00( unsigned int lkey );   // Startup Splash Screen
extern void mainscreen( unsigned int lkey ); // Main/Home Screen

extern void screen100( unsigned int lkey );  // Machine Menu

extern void screen110( unsigned int lkey );  // Machine Menu - Bale Setup Menu
extern void screen111( unsigned int lkey );  // Machine Menu - Machine Setup Menu
extern void screen112( unsigned int lkey );  // Machine Menu - Bale Count Menu
extern void screen113( unsigned int lkey );  // Machine Menu - Operator Setup Menu

extern void screen120( unsigned int lkey );  // Lube Count Reset Screen
extern void screen121( unsigned int lkey );  // Bale Count Reset Screen
#if 0 // Removed -- Dia pot zero value fixed
extern void screen123( unsigned int lkey );  // Dia Pot Zero Reset Screen
#endif
extern void screen124( unsigned int lkey );  // Fill Pot Zero Reset Screen (AKA Bale Shape Indicator Zero)
extern void screen125( unsigned int lkey );  // Net Metres Reset Screen

extern void screen131( unsigned int lkey );  // Operator Setup Menu - Brightness & Contrast Adjust Menu
extern void screen132( unsigned int lkey );  // Operator Setup Menu - Time & Date Adjust Menu
#if 0 // Removed -- Languages not needed with iconized menus
extern void screen133( unsigned int lkey ); // Language Menu
#endif

extern void screen140( unsigned int lkey );  // Technician Menu - PIN entry
extern void screen141( unsigned int lkey );  // Technician Menu
extern void screen142( unsigned int lkey );  // Technician Menu - Baler Full
extern void screen143( unsigned int lkey );  // Technician Menu - Netting Setup
extern void screen144( unsigned int lkey );  // Technician Menu - Diameter Setup
extern void screen145( unsigned int lkey );  // Technician Menu - Density Setup
extern void screen146( unsigned int lkey );  // Technician Menu - Knives Setup
extern void screen147( unsigned int lkey );  // Technician Menu - Reset Defaults
extern void screen148( unsigned int lkey );  // Technician Menu - Formation 2

extern void screen150( unsigned int lkey );  // Diagnostics - Digital Inputs
extern void screen152( unsigned int lkey );  // Diagnostics - Analogue Inputs
extern void screen151( unsigned int lkey );  // Diagnostics - Outputs

extern void screen99( unsigned int lkey );
// -----------------------------------------------------------


// -----------------------------------------------------------
// strings.c
// -----------------------------------------------------------
// defines

// variables - constant
extern char const * const * const TextString[MAXLANGUAGES];
// -----------------------------------------------------------


// -----------------------------------------------------------
// functions.c
// -----------------------------------------------------------
// defines

// nextwindow
#define NEXTWINDOW_NOESCAPE 1
#define NEXTWINDOW_ESCAPE   2

// buzz_length -- interface related
#define KEY_BEEP                 48  // 48 mS beep if new key pressed
#define BACK_TO_MAIN_SCR_BEEP   100  // 100 ms beep (x2) on reverting back to the Main Screen (after a period of no is key activity)

// buzz_length -- function related
#define MAN_START_END_CHAMBER_OPEN_DELAY_BEEP  250  // 0.25s beep at start & end of chamber open delay (in MAN mode)
#define AUTO_CHAMBER_OPEN_BEEP                 250  // 0.25s beep while chamber open (in AUTO mode)
#define PROMPT_TO_OPEN_DOOR_BEEP               500  // 0.5 s beep to prompt operator to open the chamber door & unload the bale
#define AUTO_CONFIRM_DOOR_OPENED_BEEP          250  // 0.25s to confirm chamber door has been opened (in AUTO mode)
#define AUTO_CONFIRM_BALE_UNLOADED_BEEP        250  // 0.25s to confirm bale has been unloaded (in AUTO mode)
#define AUTO_CONFIRM_DOOR_CLOSED_BEEP          250  // 0.25s to confirm chamber door has been closed (in AUTO mode)
#define ALL_STAGES_COMPLETE_BEEP              1000  // 1 s beep on completion of all stages
#define NETTING_START_BEEP                    3000  // 3 s beep when netting starts in AUTO mode / can be started in MAN mode
#define PRE_WARNING_BEEP                      1000  // 1 s beep when Pre-target bale diameter reached (in MAN/AUTO mode)

// buzz_length & delay -- for beeps accompanying driving arrows  
#define DRIVING_ARROWS_X4_BEEP        50  // 50ms beep length (for each of 4 beeps)
#define DRIVING_ARROWS_X4_BEEP_DELAY  15  // 1.5 s delay (before first beep of 4 starts)
#define DRIVING_ARROWS_X3_BEEP       100  // 100ms beep length (for each of 3 beeps)
#define DRIVING_ARROWS_X3_BEEP_DELAY  20  // 2.0 s delay (before first beep of 3 starts)
#define DRIVING_ARROWS_X2_BEEP       200  // 200ms beep length (for each of 2 beeps)
#define DRIVING_ARROWS_X2_BEEP_DELAY  25  // 2.5 s delay (before first beep of 2 starts)
#define DRIVING_ARROWS_X1_BEEP       400  // 400ms beep length
#define DRIVING_ARROWS_X1_BEEP_DELAY  30  // 3.0 s delay (before beep starts)

// buzz_length -- general
#define BEEP_OFF                100  // 100 mS
#define BEEP_ON                 100  // 100 mS

#define DEFAULT_BRIGHTNESS   152
#define BRIGHTRANGE           56
#define DEFAULT_CONTRAST     112
#define DCRANGE               64

#define KNIVESCHEV 0x01

#define NETFEEDTIME 30

// Stage (switch case IDs) 
#define STAGE_START                        0x00
#define STAGE_FORMATION                    0x01
#define STAGE_FORMATION2                   0x02
#define STAGE_CORE                         0x03
#define STAGE_MAIN                         0x04
#define STAGE_NETTING_START                0x05
#define STAGE_NETTING_NET_DELAY            0x06
#define STAGE_NETTING_NET_ON               0x07
#define STAGE_NETTING_WAIT_REQ_NET_PULSES  0x08
#define STAGE_NETTING_NET_CUT_ON           0x09
#define STAGE_NETTING_END                  0x0A
#define STAGE_UNLOADING                    0x0B
#define STAGE_UNLOADING_WAIT               0x0C
#define STAGE_END                          0x0D
#define STAGE_PRE_NET_PULSES               0x0E

// stagecomp (flags)
#define SC_CLEAR        0x00
#define NETFEEDON       0x01
#define NETCUTON        0x02
#define CHAMBOP         0x04
#define CHAMBCLO        0x08

// Digital Inputs
#define FLOOR_WORKING_POS       (AT91C_BASE_PIOB->PIO_PDSR & IP07)  // IP1: Floor (Position) Sensor closed
#define CHOPPING_KNIVES_ON      (AT91C_BASE_PIOB->PIO_PDSR & IP08)  // IP2 open: 11 Knife Sensor (AKA Chopping Knives position / Knives Position Sensor 1) in the ‘ON’ position 
#define CHAMBER_CLOSED          (AT91C_BASE_PIOB->PIO_PDSR & IP09)  // IP3: Chamber Locked Sensor closed
#define CHAMBER_OPEN           !(AT91C_BASE_PIOB->PIO_PDSR & IP09)  // IP3: Chamber Locked Sensor open
#define BALE_KICKER_UP          (AT91C_BASE_PIOB->PIO_PDSR & IP11)  // IP5: Bale Kicker Sensor closed
#define BALE_KICKER_DOWN       !(AT91C_BASE_PIOB->PIO_PDSR & IP11)  // IP5: Bale Kicker Sensor open
#define NET_CUT_SENSOR_CLOSED   (AT91C_BASE_PIOB->PIO_PDSR & IP12)  // IP6: Net Cut Sensor closed

// Digital Outputs
#define OP_MASTER_DIV   OUT1 // Master Diverter
#define OP_NET_ON       OUT2 // Net Clutch (AKA Net On)
#define OP_KNIV_DIV     OUT3 // Knife Diverter (AKA Knives Diverter)
#define OP_FLOOR_DIV    OUT4 // Floor Diverter
#define OP_ROLL_ENGAGE  OUT5 // Roller Engage Solenoid (AKA Pre Net Position)
// OUT6 is 12V Sensor Power 
#define OP_NET_CUT      OUT7 // Net Cut Motor (AKA Net Cut Trip )
#define OP_DENSITY_REL  OUT8 // Density Release Solenoid (AKA Density Control)

#define SEQ_ALL     0xff 
#define SEQ_INIT    0x01 
#define SEQ_NET     0x02 
#define SEQ_UNLOAD  0x04  

#define ON  0
#define OFF 1

// DensityPresVolts 
#define DPV_DENSITY_PRESSURE_TOO_HIGH_VOLTS            4.8 // V
#define DPV_DENSITY_PRESSURE_TOO_HIGH_VOLTS_HYSTERESIS 0.2 // V

// ChamberOpenDelayTime (applies to MAN mode only)
#define CODT_CHAMBER_OPEN_TIME          20 // 2s

// ChamberOpenBeepDelay (applies to AUTO mode only)
#define COBD_CHAMBER_OPEN_BEEP_DELAY    10 // 1s

// BaleKickerStatus (flags) 
#define BKS_CLEAR               0x00
#define BKS_BALE_KICKER_DOWN    0x01
#define BKS_BALE_KICKER_UP      0x02

// BaleDiameterStatusInManMode (flags)
#define BDSIMM_CLEAR               0x00
#define BDSIMM_BELOW_ENGAGE_DIA    0x01
#define BDSIMM_ABOVE_ENGAGE_DIA    0x02

// ChamberStatus (flags) 
#define CS_CLEAR                0x00
#define CS_CLOSED               0x01
#define CS_OPEN                 0x02

// OP8OnCount -- Count for Density Release Solenoid (OP8) on after Chamber (IP3) closes then opens (MAN/AUTO) 
#define OP8_ON_COUNT_DURATION   60 // 6s

// IP4PulsesStoppedCount -- Count for Net Pulses (on IP4) to stop after Target Pulses reached for Net Cut (OP7) to be turned off  
#define IP4_PULSES_STOPPED_COUNT_DURATION   15 // 1.5 s

// variables - constant
extern unsigned char const binHex[16];
extern unsigned char const * const DayofWeek[7];
extern unsigned char const * const MonthofYear[13];
extern unsigned char const Store[10];
extern unsigned char const UCkey[];
extern unsigned char const LCkey[];
extern unsigned char const NCkey[];

//extern unsigned char const * const RotateShaft[4];
extern unsigned char const * const ip_status[2];
extern unsigned char const * const op_status[2]; 

extern unsigned char const Bar[10];

// language string pointer
extern char const * const *ptText;

// variables - zeroed
extern unsigned char buffer[GRAPHICS_SIZE+128];
extern unsigned char adm[GRAPHICS_SIZE];
extern unsigned char startup, selection, escape, leaf, windowdepth, nextwindow;
extern unsigned char deftime, reverttime, looptime, keytime, offtime, half_second, AutoMan;
extern unsigned char swrel;
extern unsigned char DisableChoppingKnivesWarnings;
extern unsigned char ContinueOnKeyRelease;
extern unsigned char StartAutoNet;
extern unsigned char Kniv1_IPClosed;
extern unsigned int SeqOpOn, SeqOpOff, KeyOpOn, KeyOpOff;

extern float BaleDiaVolts;
extern float DensityPresVolts;
extern float BaleShapeVolts;
extern float KnifePresVolts;

extern float SupplyVolts;

extern float DiaTargetVolts;    // V

extern float DiaMinVolts;       // V
extern float DiaMaxVolts;       // V

extern float DiaMinExtDisplayPercent;    // 0.0 - 100.0 % of extended display range from PotZero to DiaMax
extern float DiaTargetExtDisplayPercent; // 0.0 - 100.0 % of extended display range from PotZero to DiaMax
extern float BaleDiaExtDisplayPercent;   // 0.0 - 100.0 % of extended display range from PotZero to DiaMax

extern float DiaFormationVolts;
extern float DiaFormation2Volts;
extern float DiaCoreVolts;
extern float DiaEngageVolts;
extern float DiaPreWarnVolts;

extern float DensityBaleVolts;
extern float DensityCoreVolts;
extern float DensityFormation2Volts;
extern float DensityFormationVolts;

extern unsigned char DensityPressureTooHigh;

extern void (*window)( unsigned int );

#define MAX_WINDOWDEPTH 16
struct sdPreviousWindow {
        void (*window)( unsigned int );
        unsigned char selection;
};
extern struct sdPreviousWindow PreviousWindow[MAX_WINDOWDEPTH];

extern unsigned int key;
extern unsigned char savecal, editing, valuechanged, number[16];
extern unsigned char SavecalDelayWithinMenus;
extern unsigned char SavecalDelayOnMainDisplay;

extern unsigned char ClockRam[16];
extern unsigned char RunFactorStatus, PowerSwitchedOff;
extern unsigned char buzz_count;

extern unsigned long int BaleDiaVoltsTotAccumMultSamples;     // V
extern unsigned long int DensityPresVoltsTotAccumMultSamples; // V
extern unsigned long int BaleShapeVoltsTotAccumMultSamples;   // V
extern unsigned long int KnifePresVoltsTotAccumMultSamples;   // V

extern unsigned int TC1_interrupt_count;
extern unsigned int EOC7_interrupt_count;
extern unsigned short int f0count, f6count;
extern unsigned int f0time, f6time;
extern unsigned char newf6;
extern unsigned int TC1_millisec_count;
extern unsigned int buzz_length, off_length;
extern unsigned char AllTime;
extern unsigned int TC1_count0;
extern unsigned char Stage, DelayTime, BeepDelay;
extern unsigned char stagecomp, knivesdelay, cont_beep;
extern unsigned char target_bale_dia_reached_cont_beep;
extern unsigned char pre_warning_cont_beep;
extern unsigned char start_chamber_open_delay_cont_beep;
extern unsigned char end_chamber_open_delay_cont_beep;
extern unsigned int cal_speed_pulses;
extern unsigned long int NetPulseCount; // Pulses
#ifdef DEVELOP // Testing pulse counting
extern unsigned long int Saved_NetPulseCount; // Pulses
extern unsigned long int SavedPreNetPulseCount; // Pulses
extern unsigned long int TargetPreNetPulseCount; // Pulses
extern unsigned long int SavedBeakEngagePulseCount; // Pulses
#endif
extern unsigned long int BeakEngagePulseCount; // Pulses
extern unsigned long int NetPulseCountPrev; // Pulses -- Similar to NetPulseCountAtLastCheck but used in display loop
extern unsigned long int NewNetPulses; // Pulses
extern unsigned long int NetMetresPulseCount; // Pulses
extern float NetMetresAppliedForCount; // m
extern float NetPulseFrequency; // Hz
extern float NetPulsesPerLayer; // Pulses/layer
extern unsigned short int TargetNetPulses; // Pulses

extern unsigned char SequenceInProgress; 

extern unsigned char OP2OnDelayTime, OP2OffDelayTime;
extern unsigned char OP7OffDelayTime;
extern unsigned char OP8OnDelayTime, OP8OffDelayTime; 
extern unsigned char NetWarningDelayTime;

extern unsigned char NetWarningOn; // Check Net Error (AKA Net Warning) On/Off switch

extern unsigned char RollEngageTurnedOn;      // OP5 Roller Engage Solenoid (AKA Pre Net Position) turned on
extern unsigned char SavedRollEngageTurnedOn; // Saved when IP3 opens in AUTO mode
extern unsigned char NetWarningDisabledWhileIP3OpenInAutoMode;
extern unsigned char NettingStartBeepsDisabledWhileIP3OpenInAutoMode; // Including Pre Warning & Net Delay beep
extern unsigned char RollerTimer;      // OP5 Roller Engage Solenoid (AKA Pre Net Position) turned on for 3 seconds


extern unsigned char RollEngageTurnedOnInManMode; // OP5 Roller Engage Solenoid (AKA Pre Net Position) turned on in MAN mode

extern unsigned char ChoppingKnives1WarningOn; // Chopping knives position 1 Warning On/Off switch

extern unsigned char ChamberClosedDelayTime; // Door Closed

extern unsigned char BalerFullDelayTime;     // applies to MAN mode only -- Set to Net Delay

extern unsigned char ChamberOpenDelayTime;   // applies to MAN mode only

extern unsigned char ChamberOpenBeepDelay;   // applies to AUTO mode only

extern unsigned char BaleKickerStatus;                // Bale Kicker Down/Up flags
extern unsigned char BaleDiameterStatusInManMode;     // Bale Diameter Below/Above Engage Diameter (in MAN mode) flags
extern unsigned char ChamberStatus;                   // Chamber Closed/Open flags

extern unsigned char OP8OnCount;             // Count for Density Release Solenoid (OP8) on after Chamber (IP3) closes then opens (MAN/AUTO)

extern unsigned char IP4PulsesStoppedCount;        // Count for Net Pulses (on IP4) to stop after Target Pulses reached for Net Cut (OP7) to be turned off 
extern unsigned long int NetPulseCountAtLastCheck; // Pulses -- Similar NetPulseCountPrev to but used in auto sequence loop

extern unsigned char DrivingArrowBeepsDisabled;    // Driving Arrow Beeps turned off/on

extern unsigned char DensityControlFunctionActive;                 // Density Control Function (based on Target Bale Density) active
extern unsigned char NetPulseCheckingEnabled;                      // Checking for no pulses on IP4 (for NetWarningDelayTime) enabled
extern unsigned char OP5TurnedOnAfterBeakEngagePulsesEnabled;      // Turning on Roller Engage Solenoid (OP5) after 'Beak Engage' pulses counted on IP4
extern unsigned char SavedOP5TurnedOnAfterBeakEngagePulsesEnabled; // Saved when IP3 opens in AUTO mode

#ifdef DEVELOP // Testing memory
extern unsigned char TestOutputReadMemAllocFail;  // flag set in SPI1FlashRead() -- brought out for testing
extern unsigned char TestOutputWriteMemAllocFail; // flag set in SPI1FlashWrite() -- brought out for testing
#endif

//extern unsigned long int forward_speed_buffer[6]; // removed on removing contents of functions in CANtest.c & CAN_mcm.c
//extern float mcm_forward_speed, local_forward_speed; // removed on removing contents of functions in CANtest.c & CAN_mcm.c

//extern unsigned char use_simulated_speed;  // removed on removing contents of functions in CANtest.c
extern unsigned char tl_target_count[2], tl_confimation_delay[2]; // TC4_irq_handler
//extern unsigned char prestart_status;  // removed on removing contents of functions in CANtest.c


#ifdef WINDOWS
 extern unsigned char Sam7A3sfr[0x4000];
#endif

// runtime variables for serial flash save held in this structure
struct PreservedRuntime {

  // General runtime

  unsigned char Brightness[2];
  unsigned char Contrast[2];
  unsigned char DayNight;
  
  signed short int LubeCounter;

  unsigned short int PowerCount;
  
  unsigned long int CheckSum;
  };

// define union for serial flash save
union PreservedRuntimeToChar {
  struct PreservedRuntime pr;
  unsigned char ch[sizeof(struct PreservedRuntime)]; // sizeof PreservedRuntime
  };
extern union PreservedRuntimeToChar pRun;

// factors for serial flash save held in this structure
struct PreservedFactors {

  // General factors
  unsigned short int software_type;
  unsigned short int software_issue;
  unsigned short int software_revision;
  
  unsigned long int GrandBaleTotal;        // Default=0 Bales; Increment=1 Bales; Range=0-99999 Bales

  unsigned char Lang;                      // Language
  
  unsigned long int BaleTotal[MAX_STORES]; // Bale Totals (in Bale Stores A to J)
  unsigned char BaleStore;                 // Bale Store ID: A - J
  
  unsigned char BaudRate;                  // Not set/used                 
  
  // Machine Menu
  unsigned char GW302_BaleProfile;        // Preset A to E
  unsigned char GW302_BaleDiameter[BP_PRESET_MAX]; // cm
  unsigned char GW302_CoreDiameter[BP_PRESET_MAX]; // cm
  unsigned char GW302_BaleDensity[BP_PRESET_MAX];  // ID
  unsigned char GW302_CoreDensity[BP_PRESET_MAX];  // ID
  float GW302_NetLayers[BP_PRESET_MAX];            // layers
  // LubeCounter                          // Moved to preserved run-time totals 
  // GrandBaleTotal                       // Moved to early position in memory since value is to be preserved between successive software versions 
  float GW302_DiaPotZero;                 // V
  signed char GW302_DiameterCorrection;   // cm
  unsigned char GW302_BaleShapeIndicator; // On/Off/Beep
  float GW302_BaleShapeIndicatorZero;     // V
  float GW302_TotalNetMetres;             // m
  unsigned char GW302_PreWarningBeep;     // %
  float GW302_NetDelay;                   // s
  unsigned char GW302_NetTwineOption; 		  // Net/Twine
  
  // Technician Menu - Baler Full Page 1/2
  unsigned short int GW302_LubeAlarm;     // bales
  float GW302_PresDeadband;               // V
  float GW302_NetSpeed;                   // Hz
  float GW302_PresSwDelay;                // s
  float GW302_DoorClosed;                 // s
  unsigned char GW302_BaleSizeFactor;         

  // Technician Menu - Baler Full Page 2/2
  float GW302_FillPotRange;               // V
  float GW302_FillPotIgnore;              // V
  unsigned char GW302_EngageDiameter;     // %
  unsigned char GW302_BaleIndicator;      // Steer/Fill
  float GW302_RollerDelay;                // s
  
  // Technician Menu - Netting Setup Page 1/2
  float GW302_NetOn;                      // s 
  unsigned char GW302_NetCutPulse;        // pulses
  float GW302_NetCutTime;                 // s
  float GW302_MinDiaPulsPerLay;           // pulses/layer
  float GW302_MaxDiaPulsPerLay;           // pulses/layer
  float GW302_NetFactor;                  
  float GW302_NoNetPulse;                 // s
  unsigned char GW302_PreNetPulse;        // pulses 

  // Technician Menu - Netting Setup Page 2/2
  unsigned char GW302_NetSystem;          // Beak/Roller
  unsigned char GW302_BeakEngage;         // pulses
  unsigned char GW302_TwineOption;          // Twine Option

  
  // Technician Menu - Diameter Setup
  unsigned char GW302_DiameterMin;        // %
  unsigned char GW302_DiameterMax;        // %
  float GW302_DiaCorrection;              // V
  unsigned char GW302_BaleDiaSmoothingFactor;     // %
  unsigned char GW302_DensityPresSmoothingFactor; // %
  unsigned char GW302_BaleShapeSmoothingFactor;   // %
  unsigned char GW302_KnifePresSmoothingFactor;   // %
 
  // Technician Menu - Density Setup Page 1/2
  float GW302_Density1;                   // V
  float GW302_Density2;                   // V
  float GW302_Density3;                   // V
  float GW302_Density4;                   // V
  float GW302_Density5;                   // V
  float GW302_Density6;                   // V

  // Technician Menu - Density Setup Page 2/2
  float GW302_Density7;                   // V
  float GW302_Density8;                   // V
  float GW302_Density9;                   // V
  float GW302_Density10;                  // V

  // Technician Menu - Knives Setup
  float GW302_KnifePressure;              // V
  float GW302_KnifePresWarn;              // V
  float GW302_DiverterDelay;              // s
  
  // Technician Menu - Formation 2
  unsigned char GW302_FormationDensity[BP_PRESET_MAX];   // ID
  unsigned char GW302_FormationDiameter[BP_PRESET_MAX];  // %
  unsigned char GW302_Formation2Density[BP_PRESET_MAX];  // ID
  unsigned char GW302_Formation2Diameter[BP_PRESET_MAX]; // %

  // Diagnostics - Digital Inputs
  unsigned char GW302_ChoppingKnivesFitted; // Disabled/Enabled
  
  unsigned long int CheckSum;
  };

// define union for serial flash save
union PreservedFactorsToChar {
  struct PreservedFactors pf;
  unsigned char ch[sizeof(struct PreservedFactors)]; // sizeof PreservedFactors
  };
extern union PreservedFactorsToChar pFac; 

// function prototypes
#ifdef WINDOWS
 // function not available in borland builder
 extern float cosf( float x );
 extern float sinf( float x );
 extern float atoff( const char *s );
#endif

#ifdef SHOW_TIME
extern void ClockDisplay( unsigned char icon  );
#endif

extern unsigned long int CheckSumIt( unsigned char *data, unsigned char *end );

//extern void SetFactorPointers( void );
extern void SetDefaultFactors( void ); 
extern void SetDefaultTotals( void );
extern void SetStartupDefaults( void );
extern unsigned char btobcd( unsigned char bin );
extern unsigned char bcdtob( unsigned char bcd );
extern void SetOutputPWM( unsigned char output, unsigned short int freq, unsigned char duty );
extern void Inputs( void );
extern void CheckAlarms( void );
extern void ResetLubeCount( void );
extern void Sequence( void );
extern void ResetSequenceFactors( unsigned char seq );
extern void AllSeqOpOff( void );
extern void UpdatingAndSavingTotalsToMemory( void );
extern void CalculateNetPulseFrequency( void );
extern void CalculateNetPulsesPerLayer( void );
extern void CalculateTargetNetPulses( void );
#ifndef WINDOWS
extern void CalculateAnalogInputRollingAverages( void );
#endif
#ifdef DEVELOP // Testing Stage Completed (stagecomp) Flags
extern void DisplayStageCompletedIndicator( void );
#endif
// -----------------------------------------------------------

#endif /* Board_h */

