void photonreg(){
  // TCanvas *A1 = new TCanvas("A1", "Comparison of photons by region");
  // A1->Divide(4,2);
  // A1->cd(1);
  // data->Draw("pi0im>>a1(200,0,0.2)","specneutmp<0.3&&flag_photon1_wCAL==1","");
  // A1->cd(2);
  // data->Draw("pi0im>>a2(200,0,0.2)","specneutmp<0.3&&flag_photon1_wCAL==2","");
  // A1->cd(3);
  // data->Draw("pi0im>>a3(200,0,0.2)","specneutmp<0.3&&flag_photon1_wCAL==3","");
  // A1->cd(4);
  // data->Draw("pi0im>>a4(200,0,0.2)","specneutmp<0.3&&flag_photon1_FT==1","");
  // A1->cd(5);
  // data->Draw("pi0im>>a5(200,0,0.2)","specneutmp<0.3&&flag_photon2_wCAL==1","");
  // A1->cd(6);
  // data->Draw("pi0im>>a6(200,0,0.2)","specneutmp<0.3&&flag_photon2_wCAL==2","");
  // A1->cd(7);
  // data->Draw("pi0im>>a7(200,0,0.2)","specneutmp<0.3&&flag_photon2_wCAL==3","");
  // A1->cd(8);
  // data->Draw("pi0im>>a8(200,0,0.2)","specneutmp<0.3&&flag_photon2_FT==1","");

  // TCanvas *B1 = new TCanvas("B1", "Comparison of photon pairs by region");
  // B1->Divide(4,2);
  // B1->cd(1);
  // data->Draw("pi0im>>b0(200,0,0.2)","specneutmp<0.3","");
  // B1->cd(2);
  // data->Draw("pi0im>>b1(200,0,0.2)","specneutmp<0.3&&flag_photon1_CAL==1&&flag_photon2_CAL==1","");
  // B1->cd(3);
  // data->Draw("pi0im>>b2(200,0,0.2)","specneutmp<0.3&&flag_photon1_FT==1&&flag_photon2_FT==1","");
  // B1->cd(4);
  // data->Draw("pi0im>>b3(200,0,0.2)","specneutmp<0.3&&((flag_photon1_CAL==1&&flag_photon2_FT==1)||(flag_photon1_FT==1&&flag_photon2_CAL==1))","");
  // B1->cd(5);
  // //data->Draw("pi0im>>b4(200,0,0.2)","specneutmp<0.3","");
  // B1->cd(6);
  // //data->Draw("pi0im>>b5(200,0,0.2)","specneutmp<0.3&&flag_photon1_CAL==1&&flag_photon2_CAL==1","");
  // B1->cd(7);
  // data->Draw("pi0im>>b6(200,0,0.2)","specneutmp<0.3&&flag_photon1_ft==1&&flag_photon2_ft==1","");
  // B1->cd(8);
  // data->Draw("pi0im>>b7(200,0,0.2)","specneutmp<0.3&&((flag_photon1_CAL==1&&flag_photon2_ft==1)||(flag_photon1_ft==1&&flag_photon2_CAL==1))","");

  TCanvas *C1 = new TCanvas("C1", "C: Both photons same reg (EIN/OUT SPLIT)");
  C1->Divide(4,1);
  C1->cd(1);
  data->Draw("pi0im>>c1(150,0,0.2)","specneutmp<0.3&&flag_photon1_wCAL==1&&flag_photon2_wCAL==1","");
  C1->cd(2);
  data->Draw("pi0im>>c2(200,0,0.2)","specneutmp<0.3&&flag_photon1_wCAL==2&&flag_photon2_wCAL==2","");
  C1->cd(3);
  data->Draw("pi0im>>c3(150,0,0.2)","specneutmp<0.3&&flag_photon1_wCAL==3&&flag_photon2_wCAL==3","");
  C1->cd(4);
  data->Draw("pi0im>>c4(50,0,0.2)","specneutmp<0.3&&flag_photon1_ft==1&&flag_photon2_ft==1","");


  TCanvas *D1 = new TCanvas("D1", "D: Paired photons by region");
  D1->Divide(3,3);
  D1->cd(1);
  data->Draw("pi0im>>d1(100,0,0.2)","specneutmp<0.3&&flag_photon1_wCAL==1&&flag_photon2_wCAL==1","");
  D1->cd(2);
  data->Draw("pi0im>>d2(150,0,0.2)","specneutmp<0.3&&(flag_photon1_wCAL==2||flag_photon1_wCAL==3)&&(flag_photon2_wCAL==2||flag_photon2_wCAL==3)","");
  D1->cd(3);
  data->Draw("pi0im>>d3(50,0,0.2)","specneutmp<0.3&&flag_photon1_ft==1&&flag_photon2_ft==1","");
  D1->cd(4);
  data->Draw("pi0im>>d4(100,0,0.2)","specneutmp<0.3&&flag_photon1_wCAL==1&&(flag_photon2_wCAL==2||flag_photon2_wCAL==3)","");
  D1->cd(5);
  data->Draw("pi0im>>d5(50,0,0.2)","specneutmp<0.3&&flag_photon1_wCAL==1&&flag_photon2_ft==1","");
  D1->cd(6);
  data->Draw("pi0im>>d6(50,0,0.2)","specneutmp<0.3&&(flag_photon1_wCAL==2||flag_photon1_wCAL==3)&&flag_photon2_ft==1","");
  D1->cd(7);
  data->Draw("pi0im>>d7(150,0,0.2)","specneutmp<0.3&&flag_photon2_wCAL==1&&(flag_photon1_wCAL==2||flag_photon1_wCAL==3)","");
  D1->cd(8);
  data->Draw("pi0im>>d8(50,0,0.2)","specneutmp<0.3&&flag_photon2_wCAL==1&&flag_photon1_ft==1","");
  D1->cd(9);
  data->Draw("pi0im>>d9(50,0,0.2)","specneutmp<0.3&&(flag_photon2_wCAL==2||flag_photon2_wCAL==3)&&flag_photon1_ft==1","");


  TCanvas *E1 = new TCanvas("E1", "E: Paired photons by region (COMBINED)");
  E1->Divide(3,2);
  E1->cd(1);
  data->Draw("pi0im>>e1(150,0,0.2)","specneutmp<0.3&&flag_photon1_wCAL==1&&flag_photon2_wCAL==1","");
  E1->cd(2);
  data->Draw("pi0im>>e2(150,0,0.2)","specneutmp<0.3&&(flag_photon1_wCAL==2||flag_photon1_wCAL==3)&&(flag_photon2_wCAL==2||flag_photon2_wCAL==3)","");
  E1->cd(3);
  data->Draw("pi0im>>e3(50,0,0.2)","specneutmp<0.3&&flag_photon1_ft==1&&flag_photon2_ft==1","");
  E1->cd(4);
  data->Draw("pi0im>>e4(500,0,0.2)","specneutmp<0.3&&(flag_photon1_wCAL==1&&(flag_photon2_wCAL==2||flag_photon2_wCAL==3))||(flag_photon2_wCAL==1&&(flag_photon1_wCAL==2||flag_photon1_wCAL==3))","");
  E1->cd(5);
  data->Draw("pi0im>>e5(250,0,0.2)","specneutmp<0.3&&(flag_photon1_wCAL==1&&flag_photon2_ft==1)||(flag_photon2_wCAL==1&&flag_photon1_ft==1)","");
  E1->cd(6);
  data->Draw("pi0im>>e6(200,0,0.2)","specneutmp<0.3&&((flag_photon1_wCAL==2||flag_photon1_wCAL==3)&&flag_photon2_ft==1)||((flag_photon2_wCAL==2||flag_photon2_wCAL==3)&&flag_photon1_ft==1)","");


  TCanvas *F1 = new TCanvas("F1", "F: Paired photons by region with simpler flag (COMBINED)");
  F1->Divide(3,2);
  F1->cd(1);
  data->Draw("pi0im>>f1(150,0,0.2)","specneutmp<0.3&&flag_photon1_wCAL==1&&flag_photon2_wCAL==1","");
  F1->cd(2);
  data->Draw("pi0im>>f2(150,0,0.2)","specneutmp<0.3&&flag_photon1_ECAL==1&&flag_photon2_ECAL==1","");
  F1->cd(3);
  data->Draw("pi0im>>f3(50,0,0.2)","specneutmp<0.3&&flag_photon1_ft==1&&flag_photon2_ft==1","");
  F1->cd(4);
  data->Draw("pi0im>>f4(500,0,0.2)","specneutmp<0.3&&(flag_photon1_wCAL==1&&flag_photon2_ECAL==1)||(flag_photon2_wCAL==1&&flag_photon1_ECAL==1)","");
  F1->cd(5);
  data->Draw("pi0im>>f5(250,0,0.2)","specneutmp<0.3&&(flag_photon1_wCAL==1&&flag_photon2_ft==1)||(flag_photon2_wCAL==1&&flag_photon1_ft==1)","");
  F1->cd(6);
  data->Draw("pi0im>>f6(200,0,0.2)","specneutmp<0.3&&(flag_photon1_ECAL==1&&flag_photon2_ft==1)||(flag_photon2_ECAL==1&&flag_photon1_ft==1)","");
  
  //WHATS GOING ON BETWEEN E1 and D1 here? Suddenly ALL the stats.  Maybe set up new flag so make things clearer...
}


