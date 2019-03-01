#include "TROOT.h"


int CommonStyle() {

  // -- margin --
  gStyle->SetPadTopMargin(0.10);
  gStyle->SetPadBottomMargin(0.19);
  gStyle->SetPadRightMargin(0.075);//more space if no colz option 
  gStyle->SetPadLeftMargin(0.165); 

  // -- title/lable offset --
  gStyle->SetTitleOffset(1.1, "x");
  gStyle->SetTitleOffset(1.1, "y");
  gStyle->SetLabelOffset(0.01, "x");
  gStyle->SetLabelOffset(0.02, "y");

  // -- title/label size --
  gStyle->SetTitleFontSize(0.09); 
  gStyle->SetTitleSize(0.07, "x"); 
  gStyle->SetTitleSize(0.07, "y"); 
  gStyle->SetTitleSize(0.07, "z"); 
  gStyle->SetLabelSize(0.06,"x"); 
  gStyle->SetLabelSize(0.06,"y"); 
  gStyle->SetLabelSize(0.06,"z"); 

  // -- statistic and title info --
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(1);
  //  gStyle->SetOptStat(1111);

  // -- lines --
   gStyle->SetLineWidth(1);

  // -- fills/color --
  //  gStyle->SetFrameFillColor(0); // white color for backgroud
  //  gStyle->SetFillColor(1);

  // -- color scheme --
  // - "warm"/red-ish -
  //  const Int_t NRGBs = 3;
  //  const Int_t NCont = 500;
  //  Double_t red[]   = {1.00, 1.00, 0.25 };
  //  Double_t green[] = {1.00, 0.00, 0.00 };
  //  Double_t blue[]  = {0.00, 0.00, 0.00 };
  //  Double_t stops[] = { 0.25, 0.75, 1.00 };
  //  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  //  gStyle->SetNumberContours(NCont);   
  //  - rainbow -
  //  gStyle->SetPalette(1);  // use the rainbow color set

  // -- horizontal error bars back --
   gStyle->SetErrorX(0.5);

  // -- transparent stuff --
  //  gStyle->SetFillStyle(0);

  // -- axis --
  //  t2kStyle->SetStripDecimals(kFALSE); // don't do 1.0 -> 1
  //  TGaxis::SetMaxDigits(3); // doesn't seem to work :<
  // supressed zeroes!
  //  gStyle->SetHistMinimumZero(kFALSE);


  return(0);
}
