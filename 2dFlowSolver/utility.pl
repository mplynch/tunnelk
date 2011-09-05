#!/usr/bin/env perl
# utility43.pl
# USAGE: ./utility43.pl

# --- Comparing resluts for when pt. is perturbed ---
$p43_o  = 5.9517206980e-02; # y position of point 43 for the original airfoil
$p43_p  = 5.9517306980e-02; # y position of point 43 for the perturbed airfoil
$deltaB = $p43_p - $p43_o; 

printf("p43 Original  = %13.10e\n",$p43_o); 
printf("p43 Perturbed = %13.10e\n",$p43_p); 
printf("Delta Beta    = %13.10e\n",$deltaB);

# --- Residual ---------------------------------------------------------------80
$R0_o = 0.0;
$R1_o = 0.0;
$R2_o = 0.0;
$R3_o = 0.0;

$R42_0_p  = -3.8618990120e-08;
$R42_1_p  = -3.8317682933e-08;
$R42_2_p  = -3.2122112158e-10;
$R42_3_p  = -1.0905297163e-07;

$R43_0_p  = -8.1643296290e-11;
$R43_1_p  = -1.9940507643e-10;
$R43_2_p  = 3.9994900892e-11;
$R43_3_p  = -2.9487492613e-10;

$R44_0_p  = 3.8364730709e-08;
$R44_1_p  = 3.9008325452e-08;
$R44_2_p  = -1.0983301353e-09;
$R44_3_p  = 1.0817823660e-07;

$R214_0_p = -3.5344590479e-10;
$R214_1_p = -6.3553186287e-10;
$R214_2_p = -1.2022321277e-10;
$R214_3_p = -1.1252957481e-09;

$dRdB42_0 = ($R42_0_p - $R0_o)/$deltaB;
$dRdB42_1 = ($R42_1_p - $R1_o)/$deltaB;
$dRdB42_2 = ($R42_2_p - $R2_o)/$deltaB;
$dRdB42_3 = ($R42_3_p - $R3_o)/$deltaB;

$dRdB43_0 = ($R43_0_p - $R0_o)/$deltaB;
$dRdB43_1 = ($R43_1_p - $R1_o)/$deltaB;
$dRdB43_2 = ($R43_2_p - $R2_o)/$deltaB;
$dRdB43_3 = ($R43_3_p - $R3_o)/$deltaB;

$dRdB44_0 = ($R44_0_p - $R0_o)/$deltaB;
$dRdB44_1 = ($R44_1_p - $R1_o)/$deltaB;
$dRdB44_2 = ($R44_2_p - $R2_o)/$deltaB;
$dRdB44_3 = ($R44_3_p - $R3_o)/$deltaB;

$dRdB214_0 = ($R214_0_p - $R0_o)/$deltaB;
$dRdB214_1 = ($R214_1_p - $R1_o)/$deltaB;
$dRdB214_2 = ($R214_2_p - $R2_o)/$deltaB;
$dRdB214_3 = ($R214_3_p - $R3_o)/$deltaB;

$R_B42_0  = -3.8619011696e-01;
$R_B42_1  = -3.8317678373e-01;
$R_B42_2  = -3.2121783304e-03;
$R_B42_3  = -1.0905301211e+00;

$R_B43_0  = -8.1593982480e-04;
$R_B43_1  = -1.9935122093e-03;
$R_B43_2  = 4.0007074771e-04;
$R_B43_3  = -2.9472866438e-03;

$R_B44_0  = 3.8364708715e-01;
$R_B44_1  = 3.9008323504e-01;
$R_B44_2  = -1.0983269168e-02;
$R_B44_3  = 1.0817819375e+00;

$R_B214_0 = -3.5339851978e-03;
$R_B214_1 = -6.3546899550e-03;
$R_B214_2 = -1.2025514837e-03;
$R_B214_3 = -1.1251562663e-02;

#Percent Error
$eR_0 = ($R_B43_0 - $dRdB43_0)/$dRdB43_0 * 100;
$eR_1 = ($R_B43_1 - $dRdB43_1)/$dRdB43_1 * 100;
$eR_2 = ($R_B43_2 - $dRdB43_2)/$dRdB43_2 * 100;
$eR_3 = ($R_B43_3 - $dRdB43_3)/$dRdB43_3 * 100;

printf("\n---------- d/dB Residuals ----------");
printf("\n--- Pt. 214 ---");
printf("\nFinite diff :");
printf(" 0=%17.10e",$dRdB214_0); 
printf(" 1=%17.10e",$dRdB214_1); 
printf(" 2=%17.10e",$dRdB214_2); 
printf(" 3=%17.10e\n",$dRdB214_3); 
printf("Forward Mode:");
printf(" 0=%17.10e",$R_B214_0); 
printf(" 1=%17.10e",$R_B214_1); 
printf(" 2=%17.10e",$R_B214_2); 
printf(" 3=%17.10e\n",$R_B214_3); 

printf("\n--- Pt. 42 ---");
printf("\nFinite diff :");
printf(" 0=%17.10e",$dRdB42_0); 
printf(" 1=%17.10e",$dRdB42_1); 
printf(" 2=%17.10e",$dRdB42_2); 
printf(" 3=%17.10e\n",$dRdB42_3); 
printf("Forward Mode:");
printf(" 0=%17.10e",$R_B42_0); 
printf(" 1=%17.10e",$R_B42_1); 
printf(" 2=%17.10e",$R_B42_2); 
printf(" 3=%17.10e\n",$R_B42_3); 

printf("\n--- Pt. 43 ---");
printf("\nFinite diff :");
printf(" 0=%17.10e",$dRdB43_0); 
printf(" 1=%17.10e",$dRdB43_1); 
printf(" 2=%17.10e",$dRdB43_2); 
printf(" 3=%17.10e\n",$dRdB43_3); 
printf("Forward Mode:");
printf(" 0=%17.10e",$R_B43_0); 
printf(" 1=%17.10e",$R_B43_1); 
printf(" 2=%17.10e",$R_B43_2); 
printf(" 3=%17.10e\n",$R_B43_3); 

printf("\n--- Pt. 44 ---");
printf("\nFinite diff :");
printf(" 0=%17.10e",$dRdB44_0); 
printf(" 1=%17.10e",$dRdB44_1); 
printf(" 2=%17.10e",$dRdB44_2); 
printf(" 3=%17.10e\n",$dRdB44_3); 
printf("Forward Mode:");
printf(" 0=%17.10e",$R_B44_0); 
printf(" 1=%17.10e",$R_B44_1); 
printf(" 2=%17.10e",$R_B44_2); 
printf(" 3=%17.10e\n",$R_B44_3); 

printf("\nPoint 43 Percent Error:");
printf("eR_0 = %5.3f%",$eR_0); 
printf("       eR_1 = %5.3f%",$eR_1); 
printf("       eR_2 = %5.3f%",$eR_2); 
printf("       eR_3 = %5.3f%\n",$eR_3); 
print "\n";

# --- Q ----------------------------------------------------------------------80
# --- pt 214 ---
$Q214o_0 = 7.8937570426485e-01;
$Q214o_1 = 7.7939985831417e-01;
$Q214o_2 = 3.2199828803427e-03;
$Q214o_3 = 1.7040071855033e+00;

$Q214p_0 = 7.8937628538951e-01;
$Q214p_1 = 7.7940006749953e-01;
$Q214p_2 = 3.2204805100905e-03;
$Q214p_3 = 1.7040084138918e+00;

# --- pt 42 ---
$Q42o_0 =  7.760668046416e-01;
$Q42o_1 =  7.679507719657e-01;
$Q42o_2 =  9.314120425214e-03;
$Q42o_3 =  1.673006141724e+00;

$Q42p_0 =  7.7606858050925e-01;
$Q42p_1 =  7.6795130630205e-01;
$Q42p_2 =  9.3149152459156e-03;
$Q42p_3 =  1.6730098194909e+00;

# --- pt 43 ---
$Q43o_0 =  7.6405334183385e-01;
$Q43o_1 =  7.6451457330957e-01;
$Q43o_2 = -6.7519245079753e-03;
$Q43o_3 =  1.6472434265436e+00;

$Q43p_0 =  7.6405292632861e-01;
$Q43p_1 =  7.6451460613167e-01;
$Q43p_2 = -6.7517071034050e-03;
$Q43p_3 =  1.6472426775642e+00;

# --- pt 44 ---
$Q44o_0 = 7.544147087175e-01;
$Q44o_1 = 7.607545246050e-01;
$Q44o_2 = -2.140895349080e-02;
$Q44o_3 = 1.626357704498e+00;

$Q44p_0 =  7.5441247359781e-01;
$Q44p_1 =  7.6075378972474e-01;
$Q44p_2 = -2.1409679551165e-02;
$Q44p_3 =  1.6263531075737e+00;

# --- Finite Difference ---
$dQdB42_0 = ($Q42p_0 - $Q42o_0)/$deltaB;
$dQdB42_1 = ($Q42p_1 - $Q42o_1)/$deltaB;
$dQdB42_2 = ($Q42p_2 - $Q42o_2)/$deltaB;
$dQdB42_3 = ($Q42p_3 - $Q42o_3)/$deltaB;

$dQdB43_0 = ($Q43p_0 - $Q43o_0)/$deltaB;
$dQdB43_1 = ($Q43p_1 - $Q43o_1)/$deltaB;
$dQdB43_2 = ($Q43p_2 - $Q43o_2)/$deltaB;
$dQdB43_3 = ($Q43p_3 - $Q43o_3)/$deltaB;

$dQdB44_0 = ($Q44p_0 - $Q44o_0)/$deltaB;
$dQdB44_1 = ($Q44p_1 - $Q44o_1)/$deltaB;
$dQdB44_2 = ($Q44p_2 - $Q44o_2)/$deltaB;
$dQdB44_3 = ($Q44p_3 - $Q44o_3)/$deltaB;

$dQdB214_0 = ($Q214p_0 - $Q214o_0)/$deltaB;
$dQdB214_1 = ($Q214p_1 - $Q214o_1)/$deltaB;
$dQdB214_2 = ($Q214p_2 - $Q214o_2)/$deltaB;
$dQdB214_3 = ($Q214p_3 - $Q214o_3)/$deltaB;

#--- Forward Mode Difference ---
$Q_B214_0 =  5.8112238406026e+00;
$Q_B214_1 =  2.0918634990160e+00;
$Q_B214_2 =  4.9762884470415e+00;
$Q_B214_3 =  1.2283843215845e+01;

$Q_B42_0 =  1.7758661174688e+01;
$Q_B42_1 =  5.3433937775264e+00;
$Q_B42_2 =  7.9481965726156e+00;
$Q_B42_3 =  3.6777637060972e+01;

$Q_B43_0 = -4.1550798299844e+00;
$Q_B43_1 =  3.2823150093236e-01;
$Q_B43_2 =  2.1740361775729e+00;
$Q_B43_3 = -7.4898439101663e+00;

$Q_B44_0 = -2.2351203777455e+01;
$Q_B44_1 = -7.3487413636826e+00;
$Q_B44_2 = -7.2606283359232e+00;
$Q_B44_3 = -4.5969248625386e+01;

#Percent Error
$eQ_0 = ($Q_B43_0 - $dQdB43_0)/$dQdB43_0 * 100;
$eQ_1 = ($Q_B43_1 - $dQdB43_1)/$dQdB43_1 * 100;
$eQ_2 = ($Q_B43_2 - $dQdB43_2)/$dQdB43_2 * 100;
$eR_3 = ($Q_B43_3 - $dQdB43_3)/$dQdB43_3 * 100;

printf("\n---------- d/dB Q ----------");
printf("\n--- Pt. 214 ---");
printf("\nFinite diff :");
printf(" 0=%17.10e",$dQdB214_0); 
printf(" 1=%17.10e",$dQdB214_1); 
printf(" 2=%17.10e",$dQdB214_2); 
printf(" 3=%17.10e\n",$dQdB214_3); 
printf("Forward Mode:");
printf(" 0=%17.10e",$Q_B214_0); 
printf(" 1=%17.10e",$Q_B214_1); 
printf(" 2=%17.10e",$Q_B214_2); 
printf(" 3=%17.10e\n",$Q_B214_3); 

printf("\n--- Pt. 42 ---");
printf("\nFinite diff :");
printf(" 0=%17.10e",$dQdB42_0); 
printf(" 1=%17.10e",$dQdB42_1); 
printf(" 2=%17.10e",$dQdB42_2); 
printf(" 3=%17.10e\n",$dQdB42_3); 
printf("Forward Mode:");
printf(" 0=%17.10e",$Q_B42_0); 
printf(" 1=%17.10e",$Q_B42_1); 
printf(" 2=%17.10e",$Q_B42_2); 
printf(" 3=%17.10e\n",$Q_B42_3); 

printf("\n--- Pt. 43 ---");
printf("\nFinite diff :");
printf(" 0=%17.10e",$dQdB43_0); 
printf(" 1=%17.10e",$dQdB43_1); 
printf(" 2=%17.10e",$dQdB43_2); 
printf(" 3=%17.10e\n",$dQdB43_3); 
printf("Forward Mode:");
printf(" 0=%17.10e",$Q_B43_0); 
printf(" 1=%17.10e",$Q_B43_1); 
printf(" 2=%17.10e",$Q_B43_2); 
printf(" 3=%17.10e\n",$Q_B43_3); 

printf("\n--- Pt. 44 ---");
printf("\nFinite diff :");
printf(" 0=%17.10e",$dQdB44_0); 
printf(" 1=%17.10e",$dQdB44_1); 
printf(" 2=%17.10e",$dQdB44_2); 
printf(" 3=%17.10e\n",$dQdB44_3); 
printf("Forward Mode:");
printf(" 0=%17.10e",$Q_B44_0); 
printf(" 1=%17.10e",$Q_B44_1); 
printf(" 2=%17.10e",$Q_B44_2); 
printf(" 3=%17.10e\n",$Q_B44_3); 

print "\n";
printf("Percent Error on point 43:");
printf("eQ_0 = %5.3f%",$eQ_0); 
printf("       eQ_1 = %5.3f%",$eQ_1); 
printf("       eQ_2 = %5.3f%",$eQ_2); 
printf("       eQ_3 = %5.3f%\n",$eQ_3); 
print "\n";

# --- Lift on Pts 42, 43, 44 -------------------------------------------------80
#--- original mesh ---
$lift42o = 2.1746711993e-02;
$lift43o = 2.3070365810e-02;
$lift44o = 2.4124751401e-02;
#--- perturbed mesh ---
$lift42p = 2.1746535722e-02;
$lift43p = 2.3070409557e-02;
$lift44p = 2.4124973320e-02;

#--- Finite Difference ---
$dlift42dB = ($lift42p - $lift42o)/$deltaB;
$dlift43dB = ($lift43p - $lift43o)/$deltaB;
$dlift44dB = ($lift44p - $lift44o)/$deltaB;

#--- Forward Mode Difference ---
$lift42_B = -1.7627085185e+00;
$lift43_B = 4.3747287232e-01;
$lift44_B = 2.2191970798e+00;

printf("\n---------- d/dB Lift on Points ----------");
printf("\n--- 42 ---");
printf("\nFinite diff :");
printf(" dliftdB  = %17.10e\n",$dlift42dB); 
printf("Forward Mode:");
printf(" lift42_B = %17.10e",$lift42_B); 
printf("\n--- 43 ---");
printf("\nFinite diff :");
printf(" dliftdB  = %17.10e\n",$dlift43dB); 
printf("Forward Mode:");
printf(" lift43_B = %17.10e",$lift43_B); 
printf("\n--- 44 ---");
printf("\nFinite diff :");
printf(" dliftdB  = %17.10e\n",$dlift44dB); 
printf("Forward Mode:");
printf(" lift44_B = %17.10e",$lift44_B); 
printf("\n");

# --- Miscellaneous Derivatives for Point 42 ---------------------------------80
=head

#--- original mesh ---
$x42o    =  2.9677458230e-01;
$y42o    =  5.9681095390e-02;
$Q42_0o  =  7.7606680464e-01;
$Q42_1o  =  7.6795077197e-01;
$Q42_2o  =  9.3141204252e-03;
$Q42_3o  =  1.6730061417e+00;
$p42o    =  5.1719617644e-01;
$cp42o   = -6.1590480577e-01;
$xPluso  =  3.1446502115e-01;
$yPluso  =  5.9599151185e-02;
$xMinuso =  2.7915362105e-01;
$yMinuso =  5.9344124010e-02;
$xHato   =  7.2220451000e-03;
$yHato   = -9.9997392069e-01;
$leno    =  3.5312321021e-02;
$forceo  = -2.1749028220e-02;

#--- perturbed mesh ---
$x42p    =  2.9677458230e-01;
$y42p    =  5.9681095390e-02;
$Q42_0p  =  7.7606858051e-01;
$Q42_1p  =  7.6795130630e-01;
$Q42_2p  =  9.3149152459e-03;
$Q42_3p  =  1.6730098195e+00;
$p42p    =  5.1719778006e-01;
$cp42p   = -6.1589979444e-01;
$xPlusp  =  3.1446502115e-01;
$yPlusp  =  5.9599201185e-02;
$xMinusp =  2.7915362105e-01;
$yMinusp =  5.9344124010e-02;
$xHatp   =  7.2234609625e-03;
$yHatp   = -9.9997391047e-01;
$lenp    =  3.5312321382e-02;
$forcep  = -2.1748851481e-02;

#--- finite diff ---
$Dx42    = ($x42p    - $x42o    ) / $deltaB;
$Dy42    = ($y42p    - $y42o    ) / $deltaB;
$DQ42_0  = ($Q42_0p  - $Q42_0o  ) / $deltaB;
$DQ42_1  = ($Q42_1p  - $Q42_1o  ) / $deltaB;
$DQ42_2  = ($Q42_2p  - $Q42_2o  ) / $deltaB;
$DQ42_3  = ($Q42_3p  - $Q42_3o  ) / $deltaB;
$Dp42    = ($p42p    - $p42o    ) / $deltaB;
$Dcp42   = ($cp42p   - $cp42o   ) / $deltaB;
$DxPlus  = ($xPlusp  - $xPluso  ) / $deltaB;
$DyPlus  = ($yPlusp  - $yPluso  ) / $deltaB;
$DxMinus = ($xMinusp - $xMinuso ) / $deltaB;
$DyMinus = ($yMinusp - $yMinuso ) / $deltaB;
$DxHat   = ($xHatp   - $xHato   ) / $deltaB;
$DyHat   = ($yHatp   - $yHato   ) / $deltaB;
$Dlen    = ($lenp    - $leno    ) / $deltaB;
$Dforce  = ($forcep  - $forceo  ) / $deltaB;

printf("\nDx42    = %17.10e",$Dx42);
printf("\nDy42    = %17.10e",$Dy42);
printf("\nDQ42_0  = %17.10e",$DQ42_0);
printf("\nDQ42_1  = %17.10e",$DQ42_1);
printf("\nDQ42_2  = %17.10e",$DQ42_2);
printf("\nDQ42_3  = %17.10e",$DQ42_3);
printf("\nDp42    = %17.10e",$Dp42);
printf("\nDcp42   = %17.10e",$Dcp42);
printf("\nDxPlus  = %17.10e",$DxPlus);
printf("\nDyPlus  = %17.10e",$DyPlus);
printf("\nDxMinus = %17.10e",$DxMinus);
printf("\nDyMinus = %17.10e",$DyMinus);
printf("\nDxHat   = %17.10e",$DxHat);
printf("\nDyHat   = %17.10e",$DyHat);
printf("\nDlen    = %17.10e",$Dlen);
printf("\nDforce  = %17.10e",$Dforce);
printf("\n");

exit(0);
=cut

# --- Lift on Additional Poionts ---------------------------------------------80
=head
$input = "surfLiftO.txt";
open(INPUT, $input) or die "Can't open $input";
while (<INPUT>) {
      @line = split;
      push @Lifts_orig, [ @line ];
      }

$input = "surfLiftP.txt";
open(INPUT, $input) or die "Can't open $input";
while (<INPUT>) {
      @line = split;
      push @Lifts_pert, [ @line ];
      }

printf("\n---------- d/dB Additional Lifts ----------\n");
for $i (0 .. 61){
   $dLdB = ($Lifts_pert[$i][1] - $Lifts_orig[$i][1]) / $deltaB; 
   printf(" %2d   %17.10e\n",$i,$dLdB); 
}
=cut

# --- Total Lift -------------------------------------------------------------80
#--- original mesh ---
$Lifto = 1.5355456337e-01;
#--- perturbed mesh ---
$Liftp = 1.5355456977e-01;

#--- Finite Difference ---
$dLiftdB = ($Liftp - $Lifto)/$deltaB;

#--- Forward Mode Difference ---
$Lift_B =  6.4051356159e-02;

printf("\n---------- d/dB Total Lift ----------");
printf("\nFinite diff :");
printf(" dLiftdB = %17.10e\n",$dLiftdB); 
printf("Forward Mode:");
printf(" Lift_B  = %17.10e",$Lift_B); 
printf("\n");


# --- Cost -------------------------------------------------------------------80
#--- original mesh ---
$Costo = 2.230064936968e-03;
#--- perturbed mesh ---
$Costp = 2.230065541660e-03;

#--- Finite Difference ---
$dCostdB = ($Costp - $Costo)/$deltaB;

#--- Forward Mode Difference ---
$Cost_B = 6.049466552497e-03;

#--- Percent Error
$eCost = ($Cost_B - $dCostdB)/$dCostdB * 100;

printf("\n---------- d/dB Cost ----------");
printf("\nFinite diff  :");
printf(" dCostdB = %17.10e\n",$dCostdB); 
printf("Forward Mode :");
printf(" Cost_B  = %17.10e\n",$Cost_B); 
printf("Percent Error:");
printf("  Error  = %8.5f %",$eCost); 
printf("\n");


# --- Costs for Design Cycle -------------------------------------------------80
printf("\n---------- Cost on original Airfoil -----------------");
printf("\n Original cost = %17.10e\n",$Costo); 

printf("\n---------- d/dB Cost for points 42, 43, 44 ----------");
$deltaB = 1.0e-7;
$Costo = 2.230064936968e-03;

# --- Pt. 42 ---
$Costp = 2.230065610577e-03;
$dCostdB = ($Costp - $Costo)/$deltaB;
$Cost_B = 6.739924059478e-03;
printf("\nFinite diff  pt. 42:");
printf(" dCostdB = %17.10e",$dCostdB); 
printf("\nForward Mode  pt. 42:");
printf(" Cost_B = %17.10e\n",$Cost_B); 

# --- Pt. 43 ---
$Costp = 2.230065541681e-03;
$dCostdB = ($Costp - $Costo)/$deltaB;
$Cost_B = 6.049621590496e-03;
printf("\nFinite diff  pt. 43:");
printf(" dCostdB = %17.10e",$dCostdB); 
printf("\nForward Mode  pt. 43:");
printf(" Cost_B = %17.10e\n",$Cost_B); 


# --- Pt. 44 ---
$Costp = 2.230066200680e-03;
$dCostdB = ($Costp - $Costo)/$deltaB;
$Cost_B = 1.264225099747e-02;
printf("\nFinite diff  pt. 44:");
printf(" dCostdB = %17.10e",$dCostdB); 
printf("\nForward Mode  pt. 44:");
printf(" Cost_B = %17.10e\n",$Cost_B); 



printf("\n");
