void cuts(){

  //look at rec-proj pi0 angle effect on pi0:mm2,im,mp
  TCanvas *C1 = new TCanvas("C1", "Angle cuts effects on pi0:mm2,im,mp", 2000, 1500);
  C1->Divide(4,3);
  C1->cd(1);
  data->Draw("pi0im:pi0mm2>>h1(200, -.5, .5, 200, 0, 0.2)","","colz");
  C1->cd(2);
  data->Draw("pi0im:pi0mm2>>h2(200, -.5, .5, 200, 0, 0.2)","pi0anglediff<30","colz");
  C1->cd(3);
  data->Draw("pi0im:pi0mm2>>h3(200, -.5, .5, 200, 0, 0.2)","pi0anglediff<20","colz");
  C1->cd(4);
  data->Draw("pi0im:pi0mm2>>h4(200, -.5, .5, 200, 0, 0.2)","pi0anglediff<10","colz");
  C1->cd(5);
  data->Draw("pi0im:pi0mp>>h5(200, 0, 6, 200, 0, 0.2)","","colz");
  C1->cd(6);
  data->Draw("pi0im:pi0mp>>h6(200, 0, 6, 200, 0, 0.2)","pi0anglediff<30","colz");
  C1->cd(7);
  data->Draw("pi0im:pi0mp>>h7(200, 0, 6, 200, 0, 0.2)","pi0anglediff<20","colz");
  C1->cd(8);
  data->Draw("pi0im:pi0mp>>h8(200, 0, 6, 200, 0, 0.2)","pi0anglediff<10","colz");
  C1->cd(9);
  data->Draw("pi0mm2:pi0mp>>h9(200, 0, 6, 200, -.5, .5)","","colz");
  C1->cd(10);
  data->Draw("pi0mm2:pi0mp>>h10(200, 0, 6, 200, -.5, .5)","pi0anglediff<30","colz");
  C1->cd(11);
  data->Draw("pi0mm2:pi0mp>>h11(200, 0, 6, 200, -.5, .5)","pi0anglediff<20","colz");
  C1->cd(12);
  data->Draw("pi0mm2:pi0mp>>h12(200, 0, 6, 200, -.5, .5)","pi0anglediff<10","colz");

  //look at mm2 cut effect on on pi0:mm2,im,mp -- still crude fit (flat bg) [+/- 5,4,3 sig]
  //mu  = -0.006743, sig = 0.02618

  TCanvas *C2 = new TCanvas("C2", "pi0MM2 cuts on pi0:mm2,im,mp", 2000, 1500);
  C2->Divide(4,3);
  C2->cd(1);
  data->Draw("pi0im:pi0mm2>>i1(200, -.5, .5, 200, 0, 0.2)","","colz");
  C2->cd(2);
  data->Draw("pi0im:pi0mm2>>i2(200, -.5, .5, 200, 0, 0.2)","pi0mm2>-0.13764300&&pi0mm2<0.12415700","colz");
  C2->cd(3);
  data->Draw("pi0im:pi0mm2>>i3(200, -.5, .5, 200, 0, 0.2)","pi0mm2>-0.11146300&&pi0mm2<0.097977000","colz");
  C2->cd(4);
  data->Draw("pi0im:pi0mm2>>i4(200, -.5, .5, 200, 0, 0.2)","pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");
  C2->cd(5);
  data->Draw("pi0im:pi0mp>>i5(200, 0, 6, 200, 0, 0.2)","","colz");
  C2->cd(6);
  data->Draw("pi0im:pi0mp>>i6(200, 0, 6, 200, 0, 0.2)","pi0mm2>-0.13764300&&pi0mm2<0.12415700","colz");
  C2->cd(7);
  data->Draw("pi0im:pi0mp>>i7(200, 0, 6, 200, 0, 0.2)","pi0mm2>-0.11146300&&pi0mm2<0.097977000","colz");
  C2->cd(8);
  data->Draw("pi0im:pi0mp>>i8(200, 0, 6, 200, 0, 0.2)","pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");
  C2->cd(9);
  data->Draw("pi0mm2:pi0mp>>i9(200, 0, 6, 200, -.5, .5)","","colz");
  C2->cd(10);
  data->Draw("pi0mm2:pi0mp>>i10(200, 0, 6, 200, -.5, .5)","pi0mm2>-0.13764300&&pi0mm2<0.12415700","colz");
  C2->cd(11);
  data->Draw("pi0mm2:pi0mp>>i11(200, 0, 6, 200, -.5, .5)","pi0mm2>-0.11146300&&pi0mm2<0.097977000","colz");
  C2->cd(12);
  data->Draw("pi0mm2:pi0mp>>i12(200, 0, 6, 200, -.5, .5)","pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");

  //look at pi0mm2 cut effect on reconstructed particles -- notes above
  TCanvas *C3 = new TCanvas("C3", "pi0MM2 cuts on rec particles", 2000, 1000);
  C3->Divide(4,2);
  C3->cd(1);
  data->Draw("recprotmm:recprotmp>>j1(200, 0, 10, 200, -5, 5)", "", "colz");
  C3->cd(2);
  data->Draw("recprotmm:recprotmp>>j2(200, 0, 10, 200, -5, 5)", "pi0mm2>-0.13764300&&pi0mm2<0.12415700", "colz");
  C3->cd(3);
  data->Draw("recprotmm:recprotmp>>j3(200, 0, 10, 200, -5, 5)", "pi0mm2>-0.11146300&&pi0mm2<0.097977000", "colz");
  C3->cd(4);
  data->Draw("recprotmm:recprotmp>>j4(200, 0, 10, 200, -5, 5)", "pi0mm2>-0.085283000&&pi0mm2<0.071797000", "colz");
  C3->cd(5);
  data->Draw("specneutmm:specneutmp>>j5(200, 0, 10, 200, -5, 5)", "", "colz");
  C3->cd(6);
  data->Draw("specneutmm:specneutmp>>j6(200, 0, 10, 200, -5, 5)", "pi0mm2>-0.13764300&&pi0mm2<0.12415700", "colz");
  C3->cd(7);
  data->Draw("specneutmm:specneutmp>>j7(200, 0, 10, 200, -5, 5)", "pi0mm2>-0.11146300&&pi0mm2<0.097977000", "colz");
  C3->cd(8);
  data->Draw("specneutmm:specneutmp>>j8(200, 0, 10, 200, -5, 5)", "pi0mm2>-0.085283000&&pi0mm2<0.071797000", "colz");

  //look at spectator neutron MP cut effect on pi0:mm2,im,mp
  TCanvas *C4 = new TCanvas("C4", "spectator neutron MP cuts on rec particles", 2000, 1000);
  C4->Divide(2,3);
  C4->cd(1);
  data->Draw("pi0im:pi0mm2>>k1(200, -.5, .5, 200, 0, 0.2)","","colz");
  C4->cd(2);
  data->Draw("pi0im:pi0mm2>>k2(200, -.5, .5, 200, 0, 0.2)","specneutmp<0.3","colz");
  C4->cd(3);
  data->Draw("pi0im:pi0mp>>k3(200, 0, 6, 200, 0, 0.2)","","colz");
  C4->cd(4);
  data->Draw("pi0im:pi0mp>>k4(200, 0, 6, 200, 0, 0.2)","specneutmp<0.3","colz");
  C4->cd(5);
  data->Draw("pi0mm2:pi0mp>>k5(200, 0, 6, 200, -.5, .5)","","colz");
  C4->cd(6);
  data->Draw("pi0mm2:pi0mp>>k6(200, 0, 6, 200, -.5, .5)","specneutmp<0.3","colz");

  //combined angle and pi0mm2 cuts
  TCanvas *C5 = new TCanvas("C5", "combined pi0mm2 and angle cuts", 2000, 1000);
  C5->Divide(3,4);
  C5->cd(1);
  data->Draw("pi0im:pi0mm2>>l1(200, -.5, .5, 200, 0, 0.2)","","colz");
  C5->cd(2);
  data->Draw("pi0im:pi0mp>>l2(200, 0, 6, 200, 0, 0.2)","","colz");
  C5->cd(3);
  data->Draw("pi0mm2:pi0mp>>l3(200, 0, 6, 200, -.5, .5)","","colz");
  C5->cd(4);
  data->Draw("pi0im:pi0mm2>>l7(200, -.5, .5, 200, 0, 0.2)","pi0anglediff<10","colz");
  C5->cd(5);
  data->Draw("pi0im:pi0mp>>l8(200, 0, 6, 200, 0, 0.2)","pi0anglediff<10","colz");
  C5->cd(6);
  data->Draw("pi0mm2:pi0mp>>l9(200, 0, 6, 200, -.5, .5)","pi0anglediff<10","colz");
  C5->cd(7);
  data->Draw("pi0im:pi0mm2>>l4(200, -.5, .5, 200, 0, 0.2)","pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");
  C5->cd(8);
  data->Draw("pi0im:pi0mp>>l5(200, 0, 6, 200, 0, 0.2)","pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");
  C5->cd(9);
  data->Draw("pi0mm2:pi0mp>>l6(200, 0, 6, 200, -.5, .5)","pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");
  C5->cd(10);
  data->Draw("pi0im:pi0mm2>>l10(200, -.5, .5, 200, 0, 0.2)","pi0anglediff<10&&pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");
  C5->cd(11);
  data->Draw("pi0im:pi0mp>>l11(200, 0, 6, 200, 0, 0.2)","pi0anglediff<10&&pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");
  C5->cd(12);
  data->Draw("pi0mm2:pi0mp>>l12(200, 0, 6, 200, -.5, .5)","pi0anglediff<10&&pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");

  TCanvas *C6 = new TCanvas("C6", "combined pi0mm2, angle and spect neutron MP cuts", 2000, 1000);
  C6->Divide(3,4);
  C6->cd(1);
  data->Draw("pi0im:pi0mm2>>m1(200, -.5, .5, 200, 0, 0.2)","pi0anglediff<10","colz");
  C6->cd(2);
  data->Draw("pi0im:pi0mp>>m2(200, 0, 6, 200, 0, 0.2)","pi0anglediff<10","colz");
  C6->cd(3);
  data->Draw("pi0mm2:pi0mp>>m3(200, 0, 6, 200, -.5, .5)","pi0anglediff<10","colz");
  C6->cd(4);
  data->Draw("pi0im:pi0mm2>>m4(200, -.5, .5, 200, 0, 0.2)","pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");
  C6->cd(5);
  data->Draw("pi0im:pi0mp>>m5(200, 0, 6, 200, 0, 0.2)","pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");
  C6->cd(6);
  data->Draw("pi0mm2:pi0mp>>m6(200, 0, 6, 200, -.5, .5)","pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");
  C6->cd(7);
  data->Draw("pi0im:pi0mm2>>m7(200, -.5, .5, 200, 0, 0.2)","pi0anglediff<10&&pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");
  C6->cd(8);
  data->Draw("pi0im:pi0mp>>m8(200, 0, 6, 200, 0, 0.2)","pi0anglediff<10&&pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");
  C6->cd(9);
  data->Draw("pi0mm2:pi0mp>>m9(200, 0, 6, 200, -.5, .5)","pi0anglediff<10&&pi0mm2>-0.085283000&&pi0mm2<0.071797000","colz");
  C6->cd(10);
  data->Draw("pi0im:pi0mm2>>m10(200, -.5, .5, 200, 0, 0.2)","pi0anglediff<10&&pi0mm2>-0.085283000&&pi0mm2<0.071797000&&specneutmp<0.3","colz");
  C6->cd(11);
  data->Draw("pi0im:pi0mp>>m11(200, 0, 6, 200, 0, 0.2)","pi0anglediff<10&&pi0mm2>-0.085283000&&pi0mm2<0.071797000&&specneutmp<0.3","colz");
  C6->cd(12);
  data->Draw("pi0mm2:pi0mp>>m12(200, 0, 6, 200, -.5, .5)","pi0anglediff<10&&pi0mm2>-0.085283000&&pi0mm2<0.071797000&&specneutmp<0.3","colz");


  //n_spect cut vs all everything cut
  TCanvas *A1 = new TCanvas("A1", "n_spect cut vs all everything cut", 2000, 2000);
  A1->Divide(2,2);
  A1->cd(1);
  k2->Draw("colz");
  A1->cd(2);
  m10->Draw("colz");
  A1->cd(3);
  data->Draw("pi0im>>a1(200,0,0.2)","specneutmp<0.3","");
  A1->cd(4);
  data->Draw("pi0im>>a2(200,0,0.2)","pi0anglediff<10&&pi0mm2>-0.085283000&&pi0mm2<0.071797000&&specneutmp<0.3","");


}//macro
