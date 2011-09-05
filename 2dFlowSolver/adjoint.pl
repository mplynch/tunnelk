#!/usr/bin/env perl
# adjoint.pl
# USAGE: ./adjoint.pl

# --- Comparing resluts for when Q is perturbed ---
# --- Q ----------------------------------------------------------------------80
# --- pt 43 ---
$Q43o_0 =  7.6405334183385e-01;
$Q43o_1 =  7.6451457330957e-01;
$Q43o_2 = -6.7519245079753e-03;
$Q43o_3 =  1.6472434265436e+00;

# --- Finite Difference ---
$dQ     = 1.0e-7;
$Q43p_0 = $Q43o_0 + $dQ;
$Q43p_1 = $Q43o_1 + $dQ;
$Q43p_2 = $Q43o_2 + $dQ;
$Q43p_3 = $Q43o_3 + $dQ;

# --- Various Variables-------------------------------------------------------80
=head
$rhoo  =  7.6405334183e-01;
$uo    =  1.0006036640e+00;
$vo    = -8.8369805330e-03;
$eo    =  1.6472434265e+00;
$spd2o =  1.0012857846e+00;
$po    =  5.0589022065e-01;
$cpo   = -6.5123591761e-01;
$lifto = 2.3070365810e-02;

$rhop  =  7.6505334183e-01;
$up    =  9.9929577652e-01;
$vp    = -8.8254297299e-03;
$ep    =  1.6472434265e+00;
$spd2p =  9.9866993719e-01;
$pp    =  5.0609021605e-01;
$cpp   = -6.5061093198e-01;
$liftp =  2.3048225375e-02;

$drho  = ($rhop - $rhoo)/$dQ;
$du    = ($up - $uo)/$dQ;
$dv    = ($vp - $vo)/$dQ;
$de    = ($ep - $eo)/$dQ;
$dspd2 = ($spd2p - $spd2o)/$dQ;
$dp    = ($pp - $po)/$dQ;
$dcp   = ($cpp - $cpo)/$dQ;
$dlift = ($liftp - $lifto)/$dQ;

printf("\n");
printf(" drho  = %17.10e\n",$drho);
printf(" de    = %17.10e\n",$de);
printf(" dspd2 = %17.10e\n",$dspd2);
printf(" dp    = %17.10e\n",$dp);
printf(" dcp   = %17.10e\n",$dcp);
printf("\n");
=cut
# --- lift on point 43 -------------------------------------------------------80
printf("\n");
$lifto = 2.3070365810e-02;
$liftp =  2.3048225375e-02;
$dlift = ($liftp - $lifto)/$dQ;
printf(" dlift = %17.10e\n",$dlift);

# --- Total Lift -------------------------------------------------------------80
printf("\nTotal Lift:\n");
$lifto = 1.5355456337e-01;
$liftp = 1.5353242293e-01;
$dlift = ($liftp - $lifto)/$dQ;
printf(" dTotalLift = %17.10e\n",$dlift);

# --- Cost -------------------------------------------------------------------80
#--- original mesh ---
$Costo = 2.230064936968e-03;
#--- perturbed Cost ---
$Costp_0 = 2.2300647276e-03;
$Costp_1 = 2.2300653555e-03;
$Costp_2 = 2.2300649333e-03;
$Costp_3 = 2.2300645187e-03;

printf("---------- d/dQ Cost ----------\n");
printf("\n");
printf(" dI/dQ_0 = %17.10e\n",($Costp_0-$Costo)/$dQ); 
printf(" dI/dQ_1 = %17.10e\n",($Costp_1-$Costo)/$dQ); 
printf(" dI/dQ_2 = %17.10e\n",($Costp_2-$Costo)/$dQ); 
printf(" dI/dQ_3 = %17.10e\n",($Costp_3-$Costo)/$dQ); 

# --- Checking [dX/dB][dI/dX] ------------------------------------------------80
printf("\n--- Checking [dX/dB][dI/dX] ---\n");
$lift42o = 2.1746711993e-02;
$lift42p = 2.1746712665e-02;
$lift43o = 2.3070365810e-02;
$lift43p = 2.3070365810e-02;
$lift44o = 2.4124751401e-02;
$lift44p = 2.4124750660e-02;
printf(" dlift42/dB = %17.10e\n",($lift42p-$lift42o)/1.0e-7); 
printf(" dlift43/dB = %17.10e\n",($lift43p-$lift43o)/1.0e-7); 
printf(" dlift44/dB = %17.10e\n",($lift44p-$lift44o)/1.0e-7); 
$Costo = 2.2300649370e-03;
$Costp = 2.2300649305e-03;
printf(" dI/dB = %17.10e\n",($Costp-$Costo)/1.0e-7); 




printf("\n");



