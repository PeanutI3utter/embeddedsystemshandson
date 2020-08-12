<Qucs Schematic 0.0.19>
<Properties>
  <View=-60,29,1003,1053,1,120,147>
  <Grid=10,10,1>
  <DataSet=LED.dat>
  <DataDisplay=LED.dpl>
  <OpenDisplay=1>
  <Script=LED.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Erstellt von:>
  <FrameText2=Datum:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <.DC DC1 1 690 180 0 39 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "CroutLU" 0>
  <GND * 1 510 730 0 0 0 0>
  <.SW SW1 1 240 180 0 51 0 0 "DC1" 1 "lin" 1 "UV1" 1 "0V" 1 "12V" 1 "200" 1>
  <Vdc V1 1 780 520 18 -26 0 1 "UV1" 1>
  <R R1 1 510 480 15 -26 0 1 "344 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <Diode D2 1 510 590 15 -26 0 1 "3.67e-31 A" 1 "4.45" 1 "3 pF" 1 "0.5" 0 "0.7 V" 0 "0.5" 0 "0.0 fF" 0 "0.0" 0 "2.0" 0 "0.1 Ohm" 0 "4 us" 0 "0" 0 "0.0" 0 "1.0" 0 "1.0" 0 "5" 0 "10 uA" 0 "26.85" 0 "3.0" 0 "1.11" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "26.85" 0 "1.0" 0 "normal" 0>
  <IProbe Current 1 650 400 -26 -35 0 2>
</Components>
<Wires>
  <780 550 780 730 "" 0 0 0 "">
  <510 730 780 730 "" 0 0 0 "">
  <680 400 780 400 "" 0 0 0 "">
  <780 400 780 490 "" 0 0 0 "">
  <510 620 510 730 "" 0 0 0 "">
  <510 400 620 400 "" 0 0 0 "">
  <510 400 510 450 "" 0 0 0 "">
  <510 510 510 560 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
