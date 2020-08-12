<Qucs Schematic 0.0.19>
<Properties>
  <View=0,11,874,1128,1,0,240>
  <Grid=10,10,1>
  <DataSet=Transistor_Sim.dat>
  <DataDisplay=Transistor_Sim.dpl>
  <OpenDisplay=1>
  <Script=Transistor_Sim.m>
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
  <Vdc V1 1 660 440 18 -26 0 1 "Vce" 1>
  <_BJT BC547BP_1 1 460 440 8 -26 0 0 "npn" 0 "1.8e-14" 0 "0.9955" 0 "1.005" 0 "0.14" 0 "0.03" 0 "80" 0 "12.5" 0 "5e-14" 0 "1.46" 0 "1.72e-13" 0 "1.27" 0 "400" 0 "35.5" 0 "0" 0 "0" 0 "0.25" 0 "0.6" 0 "0.56" 0 "1.3e-11" 0 "0.75" 0 "0.33" 0 "4e-12" 0 "0.54" 0 "0.33" 0 "1" 0 "0" 0 "0.75" 0 "0" 0 "0.5" 0 "6.4e-10" 0 "0" 0 "0" 0 "0" 0 "5.072e-08" 0 "26.85" 0 "0" 0 "1" 0 "1" 0 "0" 0 "1" 0 "1" 0 "0" 0 "0" 0 "3" 0 "1.11" 0 "26.85" 0 "1" 0>
  <GND * 1 460 630 0 0 0 0>
  <Idc I1 1 310 440 -26 18 0 0 "Ib" 1>
  <IProbe Ice 1 560 180 -26 -35 0 2>
  <.SW SW1 1 130 40 0 51 0 0 "DC1" 1 "lin" 1 "Vce" 1 "0" 1 "20" 1 "10001" 1>
  <.SW SW2 1 110 730 0 51 0 0 "SW1" 1 "lin" 1 "Ib" 1 "50u" 1 "400u" 1 "8" 1>
  <.DC DC1 1 130 260 0 39 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "HouseholderQR" 0>
</Components>
<Wires>
  <660 180 660 410 "" 0 0 0 "">
  <460 180 530 180 "" 0 0 0 "">
  <460 180 460 410 "Collector" 480 250 42 "">
  <460 470 460 630 "" 0 0 0 "">
  <460 630 660 630 "" 0 0 0 "">
  <660 470 660 630 "" 0 0 0 "">
  <340 440 430 440 "Base" 390 390 43 "">
  <190 630 460 630 "" 0 0 0 "">
  <190 440 190 630 "" 0 0 0 "">
  <190 440 280 440 "" 0 0 0 "">
  <590 180 660 180 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
