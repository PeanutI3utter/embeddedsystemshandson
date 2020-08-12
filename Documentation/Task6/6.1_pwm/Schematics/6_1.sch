<Qucs Schematic 0.0.19>
<Properties>
  <View=-290,140,937,1189,1.01726,0,180>
  <Grid=10,10,1>
  <DataSet=6_1.dat>
  <DataDisplay=6_1.dpl>
  <OpenDisplay=1>
  <Script=6_1.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Erstellt von:>
  <FrameText2=Datum:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
  <.PortSym 40 20 1 0>
</Symbol>
<Components>
  <Vrect V3 1 90 570 18 -26 0 1 "3 V" 1 "TH" 1 "TL" 1 "0.001 ns" 0 "0.001 ns" 0 "0" 0>
  <GND * 1 90 950 0 0 0 0>
  <Port P1 5 360 720 4 -14 0 2 "1" 0 "analog" 0>
  <C C1 1 280 800 17 -26 0 1 "FILTER_C" 1 "" 0 "neutral" 0>
  <R R1 1 280 580 15 -26 0 1 "FILTER_R" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <Eqn Eqn1 1 540 740 -34 16 0 0 "CUTOFF=3" 1 "PWM_FREQ=1k" 1 "TH=duty_cycle / PWM_FREQ" 1 "TL=(1-duty_cycle) / PWM_FREQ" 1 "FILTER_R=1 / (2 * pi * FILTER_C * CUTOFF)" 1 "FILTER_C=10n" 1 "DIFF=PWM_UNFILTERED.Vt - PWM_FILTERED.Vt" 1 "duty_cycle=0.5" 1 "yes" 0>
  <.TR TR1 1 490 490 0 51 0 0 "lin" 1 "0" 1 "500ms" 1 "50001" 0 "Trapezoidal" 0 "2" 0 "1 ns" 0 "1e-16" 0 "150" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "26.85" 0 "1e-3" 0 "1e-6" 0 "1" 0 "CroutLU" 0 "no" 0 "yes" 0 "0" 0>
</Components>
<Wires>
  <90 490 90 540 "" 0 0 0 "">
  <90 490 280 490 "PWM_UNFILTERED" 50 420 0 "">
  <280 490 280 550 "" 0 0 0 "">
  <90 600 90 900 "" 0 0 0 "">
  <280 610 280 720 "" 0 0 0 "">
  <90 900 280 900 "" 0 0 0 "">
  <90 900 90 950 "" 0 0 0 "">
  <280 740 280 770 "" 0 0 0 "">
  <280 720 280 740 "" 0 0 0 "">
  <280 720 360 720 "PWM_FILTERED" 360 690 41 "">
  <280 830 280 900 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
