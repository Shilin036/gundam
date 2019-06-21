int GetIngridReaction(int code)
{
    int reaction = -1;

    if(code == 1)
        reaction = 0;
    else if(code == 2)
        reaction = 9;
    else if(code == 11 || code == 12 || code == 13)
        reaction = 1;
    else if(code == 21 || code == 26)
        reaction = 2;
    else if(code == 16)
        reaction = 3;
    else if(code > 30 || code < 53)
        reaction = 4;
    else if(code < 0)
        reaction = 5;

    return reaction;
}

void plot_ingrid_topology(const std::string fname)
{
    TFile* input_file = TFile::Open(fname.c_str(), "READ");
    TTree* ing_tree = (TTree*)input_file -> Get("wtree");

    gStyle->SetPadTopMargin(0.10);
    gStyle->SetPadRightMargin(0.12);
    gStyle->SetPadLeftMargin(0.14);
    gStyle->SetPadBottomMargin(0.14);
    gStyle->SetLabelSize(0.050, "xyzt");
    gStyle->SetLabelOffset(0.010, "xyzt");
    gStyle->SetTitleSize(0.060, "xyzt");
    gStyle->SetTitleOffset(1.10, "xyzt");
    gStyle->SetStripDecimals(false);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);

    int interaction_type, fsi_int;
    int muon_track, selected_sample;
    int track_sample;
    float enu, event_weight;
    float pmu_true, angle_true;
    float pmu_reco, angle_reco;
    bool is_anti, is_nue, is_fv;
    bool new_event;

    ing_tree -> SetBranchAddress("InteractionType", &interaction_type);
    ing_tree -> SetBranchAddress("FSIInt", &fsi_int);
    ing_tree -> SetBranchAddress("SelectedSample", &selected_sample);
    ing_tree -> SetBranchAddress("MuonCandidateTrack", &muon_track);
    ing_tree -> SetBranchAddress("Enu", &enu);
    ing_tree -> SetBranchAddress("IsAnti", &is_anti);
    ing_tree -> SetBranchAddress("IsNuE", &is_nue);
    ing_tree -> SetBranchAddress("IsFV", &is_fv);
    ing_tree -> SetBranchAddress("NewEvent", &new_event);
    ing_tree -> SetBranchAddress("weight", &event_weight);
    ing_tree -> SetBranchAddress("TrueMomentumMuon", &pmu_true);
    ing_tree -> SetBranchAddress("TrueAngleMuon", &angle_true);

    const float deg_to_rad = TMath::Pi() / 180;
    const float iron_carbon_ratio = 7.640777;
    int sample_track0, sample_track1, sample_track2;
    float iron_track0, plastic_track0, angle_track0;
    float iron_track1, plastic_track1, angle_track1;
    float iron_track2, plastic_track2, angle_track2;

    ing_tree -> SetBranchAddress("Sample_track0", &sample_track0);
    ing_tree -> SetBranchAddress("TrackAngle_track0", &angle_track0);
    ing_tree -> SetBranchAddress("IronDistance_track0", &iron_track0);
    ing_tree -> SetBranchAddress("PlasticDistance_track0", &plastic_track0);
    ing_tree -> SetBranchAddress("Sample_track1", &sample_track1);
    ing_tree -> SetBranchAddress("TrackAngle_track1", &angle_track1);
    ing_tree -> SetBranchAddress("IronDistance_track1", &iron_track1);
    ing_tree -> SetBranchAddress("PlasticDistance_track1", &plastic_track1);
    ing_tree -> SetBranchAddress("Sample_track2", &sample_track2);
    ing_tree -> SetBranchAddress("TrackAngle_track2", &angle_track2);
    ing_tree -> SetBranchAddress("IronDistance_track2", &iron_track2);
    ing_tree -> SetBranchAddress("PlasticDistance_track2", &plastic_track2);

    TH1D* h1d_angle = new TH1D("h1d_angle", "", 30, 0, 90);
    TH1D* h1d_dist = new TH1D("h1d_dist", "", 50, 0, 100);
    TH2D* h2d_angle = new TH2D("h2d_angle", "", 30, 0, 90, 30, 0, 90);
    TH2D* h2d_dist = new TH2D("h2d_dist", "", 50, 0, 100, 30, 0, 1.5);
    //TH2D* h2d_dist = new TH2D("h2d_dist", "", 15, 0, 1.5, 15, 0, 1.5);

    TH1D* h1d_angle_fsi0 = new TH1D("angle_fsi0", "", 30, 0, 90);
    h1d_angle_fsi0 -> SetFillColor(kRed+3); //kOrange+10
    h1d_angle_fsi0 -> SetFillStyle(3001);
    h1d_angle_fsi0 -> SetLineColor(kRed+3);
    TH1D* h1d_angle_fsi1 = new TH1D("angle_fsi1", "", 30, 0, 90);
    h1d_angle_fsi1 -> SetFillColor(kRed);
    h1d_angle_fsi1 -> SetFillStyle(3001);
    h1d_angle_fsi1 -> SetLineColor(kRed);
    TH1D* h1d_angle_fsi2 = new TH1D("angle_fsi2", "", 30, 0, 90);
    h1d_angle_fsi2 -> SetFillColor(kOrange+8);
    h1d_angle_fsi2 -> SetFillStyle(3001);
    h1d_angle_fsi2 -> SetLineColor(kOrange+8);
    TH1D* h1d_angle_fsi3 = new TH1D("angle_fsi3", "", 30, 0, 90);
    h1d_angle_fsi3 -> SetFillColor(kAzure+10);
    h1d_angle_fsi3 -> SetFillStyle(3001);
    h1d_angle_fsi3 -> SetLineColor(kAzure+10);
    TH1D* h1d_angle_fsi4 = new TH1D("angle_fsi4", "", 30, 0, 90);
    h1d_angle_fsi4 -> SetFillColor(kAzure+7);
    h1d_angle_fsi4 -> SetFillStyle(3001);
    h1d_angle_fsi4 -> SetLineColor(kAzure+7);
    TH1D* h1d_angle_fsi5 = new TH1D("angle_fsi5", "", 30, 0, 90);
    h1d_angle_fsi5 -> SetFillColor(kBlue+2);
    h1d_angle_fsi5 -> SetFillStyle(3001);
    h1d_angle_fsi5 -> SetLineColor(kBlue+2);
    TH1D* h1d_angle_fsi6 = new TH1D("angle_fsi6", "", 30, 0, 90);
    h1d_angle_fsi6 -> SetFillColor(kGray+1);
    h1d_angle_fsi6 -> SetFillStyle(3001);
    h1d_angle_fsi6 -> SetLineColor(kGray+1);
    TH1D* h1d_angle_fsi7 = new TH1D("angle_fsi7", "", 30, 0, 90);
    h1d_angle_fsi7 -> SetFillColor(kYellow);
    h1d_angle_fsi7 -> SetFillStyle(3001);
    h1d_angle_fsi7 -> SetLineColor(kYellow);

    TH1D* h1d_dist_fsi0 = new TH1D("dist_fsi0", "", 20, 0, 100);
    h1d_dist_fsi0 -> SetFillColor(kRed+3);
    h1d_dist_fsi0 -> SetFillStyle(3001);
    h1d_dist_fsi0 -> SetLineColor(kRed+3);
    TH1D* h1d_dist_fsi1 = new TH1D("dist_fsi1", "", 20, 0, 100);
    h1d_dist_fsi1 -> SetFillColor(kRed);
    h1d_dist_fsi1 -> SetFillStyle(3001);
    h1d_dist_fsi1 -> SetLineColor(kRed);
    TH1D* h1d_dist_fsi2 = new TH1D("dist_fsi2", "", 20, 0, 100);
    h1d_dist_fsi2 -> SetFillColor(kOrange+8);
    h1d_dist_fsi2 -> SetFillStyle(3001);
    h1d_dist_fsi2 -> SetLineColor(kOrange+8);
    TH1D* h1d_dist_fsi3 = new TH1D("dist_fsi3", "", 20, 0, 100);
    h1d_dist_fsi3 -> SetFillColor(kAzure+10);
    h1d_dist_fsi3 -> SetFillStyle(3001);
    h1d_dist_fsi3 -> SetLineColor(kAzure+10);
    TH1D* h1d_dist_fsi4 = new TH1D("dist_fsi4", "", 20, 0, 100);
    h1d_dist_fsi4 -> SetFillColor(kAzure+7);
    h1d_dist_fsi4 -> SetFillStyle(3001);
    h1d_dist_fsi4 -> SetLineColor(kAzure+7);
    TH1D* h1d_dist_fsi5 = new TH1D("dist_fsi5", "", 20, 0, 100);
    h1d_dist_fsi5 -> SetFillColor(kBlue+2);
    h1d_dist_fsi5 -> SetFillStyle(3001);
    h1d_dist_fsi5 -> SetLineColor(kBlue+2);
    TH1D* h1d_dist_fsi6 = new TH1D("dist_fsi6", "", 20, 0, 100);
    h1d_dist_fsi6 -> SetFillColor(kGray+1);
    h1d_dist_fsi6 -> SetFillStyle(3001);
    h1d_dist_fsi6 -> SetLineColor(kGray+1);
    TH1D* h1d_dist_fsi7 = new TH1D("dist_fsi7", "", 20, 0, 100);
    h1d_dist_fsi7 -> SetFillColor(kYellow);
    h1d_dist_fsi7 -> SetFillStyle(3001);
    h1d_dist_fsi7 -> SetLineColor(kYellow);

    double sel = 0;
    double gen = 0;
    double sig = 0;

    const unsigned int nevents = ing_tree -> GetEntries();
    for(unsigned int i = 0; i < nevents; ++i)
    {
        ing_tree -> GetEntry(i);
        //if(pmu_true == 0 || angle_true == 0)
        //    continue;

        switch(muon_track)
        {
            case 0:
                pmu_reco = iron_track0 + (plastic_track0 / iron_carbon_ratio);
                angle_reco = angle_track0;
                track_sample = sample_track0;
                break;
            case 1:
                pmu_reco = iron_track1 + (plastic_track1 / iron_carbon_ratio);
                angle_reco = angle_track1;
                track_sample = sample_track1;
                break;
            case 2:
                pmu_reco = iron_track2 + (plastic_track2 / iron_carbon_ratio);
                angle_reco = angle_track2;
                track_sample = sample_track2;
                break;
            default:
                pmu_reco = -999;
                angle_reco = -999;
                track_sample = 999;
                break;
        }

        //event_weight *= 1.16;

        if(selected_sample == 0 && track_sample < 4)
        //if(selected_sample == 1 && track_sample < 4)
            sel += event_weight;

        if(is_fv && !is_anti && !is_nue && fsi_int < 3 && new_event == 1)
        //if(is_fv && !is_anti && !is_nue && fsi_int == 3 && new_event == 1)
            gen += event_weight;

        if(selected_sample == 0 && fsi_int < 3 && !is_anti && !is_nue && track_sample < 4)
        //if(selected_sample == 1 && fsi_int == 3 && !is_anti && !is_nue && track_sample < 4)
            sig += event_weight;

        if(selected_sample == 0 && track_sample < 4)
        //if(selected_sample == 1 && track_sample < 4)
        {
            h1d_angle->Fill(angle_reco, event_weight);
            h1d_dist->Fill(pmu_reco, event_weight);

            //pmu_reco = (pmu_reco * 0.0114127 + 0.230608);
            h2d_angle->Fill(angle_reco, angle_true, event_weight);
            h2d_dist->Fill(pmu_reco, pmu_true, event_weight);

            if(fsi_int == 0)
            {
                h1d_angle_fsi0->Fill(angle_reco, event_weight);
                h1d_dist_fsi0->Fill(pmu_reco, event_weight);
            }
            else if(fsi_int == 1)
            {
                h1d_angle_fsi1->Fill(angle_reco, event_weight);
                h1d_dist_fsi1->Fill(pmu_reco, event_weight);
            }
            else if(fsi_int == 2)
            {
                h1d_angle_fsi2->Fill(angle_reco, event_weight);
                h1d_dist_fsi2->Fill(pmu_reco, event_weight);
            }
            else if(fsi_int == 3)
            {
                h1d_angle_fsi3->Fill(angle_reco, event_weight);
                h1d_dist_fsi3->Fill(pmu_reco, event_weight);
            }
            else if(fsi_int == 4)
            {
                h1d_angle_fsi4->Fill(angle_reco, event_weight);
                h1d_dist_fsi4->Fill(pmu_reco, event_weight);
            }
            else if(fsi_int == 5)
            {
                h1d_angle_fsi5->Fill(angle_reco, event_weight);
                h1d_dist_fsi5->Fill(pmu_reco, event_weight);
            }
            else if(fsi_int == 6)
            {
                h1d_angle_fsi6->Fill(angle_reco, event_weight);
                h1d_dist_fsi6->Fill(pmu_reco, event_weight);
            }
            else
            {
                h1d_angle_fsi7->Fill(angle_reco, event_weight);
                h1d_dist_fsi7->Fill(pmu_reco, event_weight);
            }
        }
    }

    std::cout << "True Signal Events: " << sig << std::endl;
    std::cout << "Sel  Signal Events: " << sel << std::endl;
    std::cout << "Gen  Signal Events: " << gen << std::endl;
    std::cout << "Eff: " << sel / gen << std::endl;
    std::cout << "Pur: " << sig / sel << std::endl;

    /*
    TCanvas* c = new TCanvas("c","c",1200,900);
    c->Divide(2,2);
    c->cd(1);
    h1d_angle->Draw("hist");
    c->cd(2);
    h1d_dist->Draw("hist");
    c->cd(3);
    h2d_angle->Draw("colz");
    c->cd(4);
    h2d_dist->Draw("colz");
    */

    TCanvas* d = new TCanvas("d","d",1200,900);
    THStack* hs_angle_fsi = new THStack("hs_angle_fsi", "");
    hs_angle_fsi->Add(h1d_angle_fsi7);
    hs_angle_fsi->Add(h1d_angle_fsi6);
    hs_angle_fsi->Add(h1d_angle_fsi5);
    hs_angle_fsi->Add(h1d_angle_fsi4);
    hs_angle_fsi->Add(h1d_angle_fsi3);
    hs_angle_fsi->Add(h1d_angle_fsi2);
    hs_angle_fsi->Add(h1d_angle_fsi1);
    hs_angle_fsi->Add(h1d_angle_fsi0);
    hs_angle_fsi->Draw("hist");
    hs_angle_fsi->GetXaxis()->SetTitle("#theta^{reco}_{#mu} (degrees)");
    hs_angle_fsi->GetYaxis()->SetTitle("Num. Events");

    TLegend* ld = new TLegend(0.70, 0.60, 0.90, 0.90);
    ld->AddEntry(h1d_angle_fsi0, "CC0#pi (0p)", "f");
    ld->AddEntry(h1d_angle_fsi1, "CC0#pi (1p)", "f");
    ld->AddEntry(h1d_angle_fsi2, "CC0#pi (2p)", "f");
    ld->AddEntry(h1d_angle_fsi3, "CC1#pi^{#pm}", "f");
    ld->AddEntry(h1d_angle_fsi4, "CC1#pi^{0}", "f");
    ld->AddEntry(h1d_angle_fsi5, "CCOther", "f");
    ld->AddEntry(h1d_angle_fsi6, "NC", "f");
    ld->AddEntry(h1d_angle_fsi7, "Other BKG", "f");
    ld->SetTextSize(0.035);
    ld->Draw("same");
    d->Print("ingrid_angle_topology.pdf");

    TCanvas* e = new TCanvas("e","e",1200,900);
    THStack* hs_dist_fsi = new THStack("hs_dist_fsi", "");
    hs_dist_fsi->Add(h1d_dist_fsi7);
    hs_dist_fsi->Add(h1d_dist_fsi6);
    hs_dist_fsi->Add(h1d_dist_fsi5);
    hs_dist_fsi->Add(h1d_dist_fsi4);
    hs_dist_fsi->Add(h1d_dist_fsi3);
    hs_dist_fsi->Add(h1d_dist_fsi2);
    hs_dist_fsi->Add(h1d_dist_fsi1);
    hs_dist_fsi->Add(h1d_dist_fsi0);
    hs_dist_fsi->Draw("hist");
    hs_dist_fsi->GetXaxis()->SetTitle("d^{reco}_{#mu} (cm)");
    hs_dist_fsi->GetYaxis()->SetTitle("Num. Events");

    TLegend* le = new TLegend(0.70, 0.60, 0.90, 0.90);
    le->AddEntry(h1d_dist_fsi0, "CC0#pi (0p)", "f");
    le->AddEntry(h1d_dist_fsi1, "CC0#pi (1p)", "f");
    le->AddEntry(h1d_dist_fsi2, "CC0#pi (2p)", "f");
    le->AddEntry(h1d_dist_fsi3, "CC1#pi^{#pm}", "f");
    le->AddEntry(h1d_dist_fsi4, "CC1#pi^{0}", "f");
    le->AddEntry(h1d_dist_fsi5, "CCOther", "f");
    le->AddEntry(h1d_dist_fsi6, "NC", "f");
    le->AddEntry(h1d_dist_fsi7, "Other BKG", "f");
    le->SetTextSize(0.035);
    le->Draw("same");
    e->Print("ingrid_distance_topology.pdf");

    TCanvas* f = new TCanvas("f","f",1200,900);
    h2d_angle->GetXaxis()->SetTitle("#theta^{reco}_{#mu} (degrees)");
    h2d_angle->GetYaxis()->SetTitle("#theta^{true}_{#mu} (degrees)");
    h2d_angle->Draw("colz");
    f->Print("ingrid_angle_resolution.pdf");

    TCanvas* g = new TCanvas("g","g",1200,900);
    h2d_dist->GetXaxis()->SetTitle("d^{reco}_{#mu} (cm)");
    h2d_dist->GetYaxis()->SetTitle("p^{true}_{#mu} (GeV/c)");
    h2d_dist->Draw("colz");
    g->Print("ingrid_dist_resolution.pdf");
}

void plot_ingrid_reaction(const std::string fname)
{
    TFile* input_file = TFile::Open(fname.c_str(), "READ");
    TTree* ing_tree = (TTree*)input_file -> Get("wtree");

    gStyle->SetPadTopMargin(0.10);
    gStyle->SetPadRightMargin(0.12);
    gStyle->SetPadLeftMargin(0.14);
    gStyle->SetPadBottomMargin(0.14);
    gStyle->SetLabelSize(0.050, "xyzt");
    gStyle->SetLabelOffset(0.010, "xyzt");
    gStyle->SetTitleSize(0.060, "xyzt");
    gStyle->SetTitleOffset(1.10, "xyzt");
    gStyle->SetStripDecimals(false);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);

    int interaction_type, fsi_int;
    int muon_track, selected_sample;
    int track_sample;
    float enu, event_weight;
    float pmu_true, angle_true;
    float pmu_reco, angle_reco;
    bool is_anti, is_nue, is_fv;
    bool new_event;

    ing_tree -> SetBranchAddress("InteractionType", &interaction_type);
    ing_tree -> SetBranchAddress("FSIInt", &fsi_int);
    ing_tree -> SetBranchAddress("SelectedSample", &selected_sample);
    ing_tree -> SetBranchAddress("MuonCandidateTrack", &muon_track);
    ing_tree -> SetBranchAddress("Enu", &enu);
    ing_tree -> SetBranchAddress("IsAnti", &is_anti);
    ing_tree -> SetBranchAddress("IsNuE", &is_nue);
    ing_tree -> SetBranchAddress("IsFV", &is_fv);
    ing_tree -> SetBranchAddress("NewEvent", &new_event);
    ing_tree -> SetBranchAddress("weight", &event_weight);
    ing_tree -> SetBranchAddress("TrueMomentumMuon", &pmu_true);
    ing_tree -> SetBranchAddress("TrueAngleMuon", &angle_true);

    const float deg_to_rad = TMath::Pi() / 180;
    const float iron_carbon_ratio = 7.640777;
    int sample_track0, sample_track1, sample_track2;
    float iron_track0, plastic_track0, angle_track0;
    float iron_track1, plastic_track1, angle_track1;
    float iron_track2, plastic_track2, angle_track2;

    ing_tree -> SetBranchAddress("Sample_track0", &sample_track0);
    ing_tree -> SetBranchAddress("TrackAngle_track0", &angle_track0);
    ing_tree -> SetBranchAddress("IronDistance_track0", &iron_track0);
    ing_tree -> SetBranchAddress("PlasticDistance_track0", &plastic_track0);
    ing_tree -> SetBranchAddress("Sample_track1", &sample_track1);
    ing_tree -> SetBranchAddress("TrackAngle_track1", &angle_track1);
    ing_tree -> SetBranchAddress("IronDistance_track1", &iron_track1);
    ing_tree -> SetBranchAddress("PlasticDistance_track1", &plastic_track1);
    ing_tree -> SetBranchAddress("Sample_track2", &sample_track2);
    ing_tree -> SetBranchAddress("TrackAngle_track2", &angle_track2);
    ing_tree -> SetBranchAddress("IronDistance_track2", &iron_track2);
    ing_tree -> SetBranchAddress("PlasticDistance_track2", &plastic_track2);

    TH1D* h1d_angle = new TH1D("h1d_angle", "", 30, 0, 90);
    TH1D* h1d_dist = new TH1D("h1d_dist", "", 50, 0, 100);
    TH2D* h2d_angle = new TH2D("h2d_angle", "", 30, 0, 90, 30, 0, 90);
    TH2D* h2d_dist = new TH2D("h2d_dist", "", 50, 0, 100, 30, 0, 1.5);
    //TH2D* h2d_dist = new TH2D("h2d_dist", "", 15, 0, 1.5, 15, 0, 1.5);

    TH1D* h1d_sel_mom = new TH1D("h1d_sel_mom", "", 40, 0, 5);
    TH1D* h1d_sel_cos = new TH1D("h1d_sel_cos", "", 30, 0, 90);
    TH1D* h1d_gen_mom = new TH1D("h1d_gen_mom", "", 40, 0, 5);
    TH1D* h1d_gen_cos = new TH1D("h1d_gen_cos", "", 30, 0, 90);
    TH1D* h1d_eff_mom = new TH1D("h1d_eff_mom", "", 40, 0, 5);
    TH1D* h1d_eff_cos = new TH1D("h1d_eff_cos", "", 30, 0, 90);

    TH1D* h1d_angle_reac0 = new TH1D("angle_reac0", "", 30, 0, 90);
    h1d_angle_reac0 -> SetFillColor(2); //kOrange+10
    h1d_angle_reac0 -> SetFillStyle(3001);
    h1d_angle_reac0 -> SetLineColor(2);
    TH1D* h1d_angle_reac1 = new TH1D("angle_reac1", "", 30, 0, 90);
    h1d_angle_reac1 -> SetFillColor(3);
    h1d_angle_reac1 -> SetFillStyle(3001);
    h1d_angle_reac1 -> SetLineColor(3);
    TH1D* h1d_angle_reac2 = new TH1D("angle_reac2", "", 30, 0, 90);
    h1d_angle_reac2 -> SetFillColor(4);
    h1d_angle_reac2 -> SetFillStyle(3001);
    h1d_angle_reac2 -> SetLineColor(4);
    TH1D* h1d_angle_reac3 = new TH1D("angle_reac3", "", 30, 0, 90);
    h1d_angle_reac3 -> SetFillColor(7);
    h1d_angle_reac3 -> SetFillStyle(3001);
    h1d_angle_reac3 -> SetLineColor(7);
    TH1D* h1d_angle_reac4 = new TH1D("angle_reac4", "", 30, 0, 90);
    h1d_angle_reac4 -> SetFillColor(6);
    h1d_angle_reac4 -> SetFillStyle(3001);
    h1d_angle_reac4 -> SetLineColor(6);
    TH1D* h1d_angle_reac5 = new TH1D("angle_reac5", "", 30, 0, 90);
    h1d_angle_reac5 -> SetFillColor(31);
    h1d_angle_reac5 -> SetFillStyle(3001);
    h1d_angle_reac5 -> SetLineColor(31);
    TH1D* h1d_angle_reac6 = new TH1D("angle_reac6", "", 30, 0, 90);
    h1d_angle_reac6 -> SetFillColor(874);
    h1d_angle_reac6 -> SetFillStyle(3001);
    h1d_angle_reac6 -> SetLineColor(874);
    TH1D* h1d_angle_reac7 = new TH1D("angle_reac7", "", 30, 0, 90);
    h1d_angle_reac7 -> SetFillColor(48);
    h1d_angle_reac7 -> SetFillStyle(3001);
    h1d_angle_reac7 -> SetLineColor(48);

    TH1D* h1d_dist_reac0 = new TH1D("dist_reac0", "", 20, 0, 100);
    h1d_dist_reac0 -> SetFillColor(2);
    h1d_dist_reac0 -> SetFillStyle(3001);
    h1d_dist_reac0 -> SetLineColor(2);
    TH1D* h1d_dist_reac1 = new TH1D("dist_reac1", "", 20, 0, 100);
    h1d_dist_reac1 -> SetFillColor(3);
    h1d_dist_reac1 -> SetFillStyle(3001);
    h1d_dist_reac1 -> SetLineColor(3);
    TH1D* h1d_dist_reac2 = new TH1D("dist_reac2", "", 20, 0, 100);
    h1d_dist_reac2 -> SetFillColor(4);
    h1d_dist_reac2 -> SetFillStyle(3001);
    h1d_dist_reac2 -> SetLineColor(4);
    TH1D* h1d_dist_reac3 = new TH1D("dist_reac3", "", 20, 0, 100);
    h1d_dist_reac3 -> SetFillColor(7);
    h1d_dist_reac3 -> SetFillStyle(3001);
    h1d_dist_reac3 -> SetLineColor(7);
    TH1D* h1d_dist_reac4 = new TH1D("dist_reac4", "", 20, 0, 100);
    h1d_dist_reac4 -> SetFillColor(6);
    h1d_dist_reac4 -> SetFillStyle(3001);
    h1d_dist_reac4 -> SetLineColor(6);
    TH1D* h1d_dist_reac5 = new TH1D("dist_reac5", "", 20, 0, 100);
    h1d_dist_reac5 -> SetFillColor(31);
    h1d_dist_reac5 -> SetFillStyle(3001);
    h1d_dist_reac5 -> SetLineColor(31);
    TH1D* h1d_dist_reac6 = new TH1D("dist_reac6", "", 20, 0, 100);
    h1d_dist_reac6 -> SetFillColor(874);
    h1d_dist_reac6 -> SetFillStyle(3001);
    h1d_dist_reac6 -> SetLineColor(874);
    TH1D* h1d_dist_reac7 = new TH1D("dist_reac7", "", 20, 0, 100);
    h1d_dist_reac7 -> SetFillColor(48);
    h1d_dist_reac7 -> SetFillStyle(3001);
    h1d_dist_reac7 -> SetLineColor(48);

    double sel = 0;
    double gen = 0;
    double sig = 0;

    const unsigned int nevents = ing_tree -> GetEntries();
    for(unsigned int i = 0; i < nevents; ++i)
    {
        ing_tree -> GetEntry(i);
        //if(pmu_true == 0 || angle_true == 0)
        //    continue;

        int reaction = GetIngridReaction(interaction_type);

        switch(muon_track)
        {
            case 0:
                pmu_reco = iron_track0 + (plastic_track0 / iron_carbon_ratio);
                angle_reco = angle_track0;
                track_sample = sample_track0;
                break;
            case 1:
                pmu_reco = iron_track1 + (plastic_track1 / iron_carbon_ratio);
                angle_reco = angle_track1;
                track_sample = sample_track1;
                break;
            case 2:
                pmu_reco = iron_track2 + (plastic_track2 / iron_carbon_ratio);
                angle_reco = angle_track2;
                track_sample = sample_track2;
                break;
            default:
                pmu_reco = -999;
                angle_reco = -999;
                track_sample = 999;
                break;
        }

        //event_weight *= 1.16;

        if(selected_sample == 0 && track_sample < 6)
        //if(selected_sample == 1 && track_sample < 4)
        {
            sel += event_weight;
            h1d_sel_mom->Fill(pmu_true, event_weight);
            h1d_sel_cos->Fill(angle_true, event_weight);
        }

        if(is_fv && !is_anti && !is_nue && fsi_int < 3 && new_event == 1)
        //if(is_fv && !is_anti && !is_nue && fsi_int == 3 && new_event == 1)
        {
            gen += event_weight;
            h1d_gen_mom->Fill(pmu_true, event_weight);
            h1d_gen_cos->Fill(angle_true, event_weight);
        }

        if(selected_sample == 0 && fsi_int < 3 && !is_anti && !is_nue && track_sample < 6)
        //if(selected_sample == 1 && fsi_int == 3 && !is_anti && !is_nue && track_sample < 4)
            sig += event_weight;

        if(selected_sample == 0 && track_sample < 6)
        //if(selected_sample == 1 && track_sample < 4)
        {
            h1d_angle->Fill(angle_reco, event_weight);
            h1d_dist->Fill(pmu_reco, event_weight);

            //pmu_reco = (pmu_reco * 0.0114127 + 0.230608);
            h2d_angle->Fill(angle_reco, angle_true, event_weight);
            h2d_dist->Fill(pmu_reco, pmu_true, event_weight);

            if(reaction == 0)
            {
                h1d_angle_reac0->Fill(angle_reco, event_weight);
                h1d_dist_reac0->Fill(pmu_reco, event_weight);
            }
            else if(reaction == 1)
            {
                h1d_angle_reac1->Fill(angle_reco, event_weight);
                h1d_dist_reac1->Fill(pmu_reco, event_weight);
            }
            else if(reaction == 2)
            {
                h1d_angle_reac2->Fill(angle_reco, event_weight);
                h1d_dist_reac2->Fill(pmu_reco, event_weight);
            }
            else if(reaction == 3)
            {
                h1d_angle_reac3->Fill(angle_reco, event_weight);
                h1d_dist_reac3->Fill(pmu_reco, event_weight);
            }
            else if(reaction == 4)
            {
                h1d_angle_reac4->Fill(angle_reco, event_weight);
                h1d_dist_reac4->Fill(pmu_reco, event_weight);
            }
            else if(reaction == 5)
            {
                h1d_angle_reac5->Fill(angle_reco, event_weight);
                h1d_dist_reac5->Fill(pmu_reco, event_weight);
            }
            else if(reaction == 9)
            {
                h1d_angle_reac6->Fill(angle_reco, event_weight);
                h1d_dist_reac6->Fill(pmu_reco, event_weight);
            }
            else
            {
                h1d_angle_reac7->Fill(angle_reco, event_weight);
                h1d_dist_reac7->Fill(pmu_reco, event_weight);
            }
        }
    }

    std::cout << "True Signal Events: " << sig << std::endl;
    std::cout << "Sel  Signal Events: " << sel << std::endl;
    std::cout << "Gen  Signal Events: " << gen << std::endl;
    std::cout << "Eff: " << sel / gen << std::endl;
    std::cout << "Pur: " << sig / sel << std::endl;

    /*
    TCanvas* c = new TCanvas("c","c",1200,900);
    c->Divide(2,2);
    c->cd(1);
    h1d_angle->Draw("hist");
    c->cd(2);
    h1d_dist->Draw("hist");
    c->cd(3);
    h2d_angle->Draw("colz");
    c->cd(4);
    h2d_dist->Draw("colz");
    */

    TCanvas* d = new TCanvas("d","d",1200,900);
    THStack* hs_angle_reac = new THStack("hs_angle_reac", "");
    hs_angle_reac->Add(h1d_angle_reac7);
    hs_angle_reac->Add(h1d_angle_reac6);
    hs_angle_reac->Add(h1d_angle_reac5);
    hs_angle_reac->Add(h1d_angle_reac4);
    hs_angle_reac->Add(h1d_angle_reac3);
    hs_angle_reac->Add(h1d_angle_reac2);
    hs_angle_reac->Add(h1d_angle_reac1);
    hs_angle_reac->Add(h1d_angle_reac0);
    hs_angle_reac->Draw("hist");
    hs_angle_reac->GetXaxis()->SetTitle("#theta^{reco}_{#mu} (degrees)");
    hs_angle_reac->GetYaxis()->SetTitle("Num. Events");

    TLegend* ld = new TLegend(0.70, 0.60, 0.90, 0.90);
    ld->AddEntry(h1d_angle_reac0, "CCQE", "f");
    ld->AddEntry(h1d_angle_reac1, "RES", "f");
    ld->AddEntry(h1d_angle_reac2, "DIS", "f");
    ld->AddEntry(h1d_angle_reac3, "COH", "f");
    ld->AddEntry(h1d_angle_reac4, "NC", "f");
    ld->AddEntry(h1d_angle_reac5, "CC #bar{#nu_{#mu}}", "f");
    ld->AddEntry(h1d_angle_reac6, "2p2h", "f");
    ld->AddEntry(h1d_angle_reac7, "Other", "f");
    ld->SetTextSize(0.035);
    ld->Draw("same");
    d->Print("ingrid_angle_reaction.pdf");

    TCanvas* e = new TCanvas("e","e",1200,900);
    THStack* hs_dist_reac = new THStack("hs_dist_reac", "");
    hs_dist_reac->Add(h1d_dist_reac7);
    hs_dist_reac->Add(h1d_dist_reac6);
    hs_dist_reac->Add(h1d_dist_reac5);
    hs_dist_reac->Add(h1d_dist_reac4);
    hs_dist_reac->Add(h1d_dist_reac3);
    hs_dist_reac->Add(h1d_dist_reac2);
    hs_dist_reac->Add(h1d_dist_reac1);
    hs_dist_reac->Add(h1d_dist_reac0);
    hs_dist_reac->Draw("hist");
    hs_dist_reac->GetXaxis()->SetTitle("d^{reco}_{#mu} (cm)");
    hs_dist_reac->GetYaxis()->SetTitle("Num. Events");

    TLegend* le = new TLegend(0.70, 0.60, 0.90, 0.90);
    le->AddEntry(h1d_dist_reac0, "CCQE", "f");
    le->AddEntry(h1d_dist_reac1, "RES", "f");
    le->AddEntry(h1d_dist_reac2, "DIS", "f");
    le->AddEntry(h1d_dist_reac3, "COH", "f");
    le->AddEntry(h1d_dist_reac4, "NC", "f");
    le->AddEntry(h1d_dist_reac5, "CC #bar{#nu_{#mu}}", "f");
    le->AddEntry(h1d_dist_reac6, "2p2h", "f");
    le->AddEntry(h1d_dist_reac7, "Other", "f");
    le->SetTextSize(0.035);
    le->Draw("same");
    e->Print("ingrid_distance_reaction.pdf");

    TCanvas* f = new TCanvas("f","f",1200,900);
    h2d_angle->GetXaxis()->SetTitle("#theta^{reco}_{#mu} (degrees)");
    h2d_angle->GetYaxis()->SetTitle("#theta^{true}_{#mu} (degrees)");
    h2d_angle->Draw("colz");
    f->Print("ingrid_angle_resolution.pdf");

    TCanvas* g = new TCanvas("g","g",1200,900);
    h2d_dist->GetXaxis()->SetTitle("d^{reco}_{#mu} (cm)");
    h2d_dist->GetYaxis()->SetTitle("p^{true}_{#mu} (GeV/c)");
    h2d_dist->Draw("colz");
    g->Print("ingrid_dist_resolution.pdf");

    TCanvas* eff_mom = new TCanvas("eff_mom","eff_mom",1200,900);
    h1d_eff_mom->Divide(h1d_sel_mom, h1d_gen_mom);
    h1d_eff_mom->SetLineColor(kBlack);
    h1d_eff_mom->SetLineWidth(2);
    h1d_eff_mom->SetMarkerColor(kBlack);
    h1d_eff_mom->SetMarkerSize(1.5);
    h1d_eff_mom->SetMarkerStyle(kFullCircle);
    h1d_eff_mom->Draw();
    h1d_eff_mom->GetXaxis()->SetTitle("p^{true}_{#mu} (GeV/c)");
    h1d_eff_mom->GetYaxis()->SetTitle("Efficiency");
    h1d_eff_mom->GetYaxis()->SetRangeUser(0.0, 1.0);
    eff_mom->Print("ingrid_eff_mom.pdf");

    TCanvas* eff_cos = new TCanvas("eff_cos","eff_cos",1200,900);
    h1d_eff_cos->Divide(h1d_sel_cos, h1d_gen_cos);
    h1d_eff_cos->SetLineColor(kBlack);
    h1d_eff_cos->SetLineWidth(2);
    h1d_eff_cos->SetMarkerColor(kBlack);
    h1d_eff_cos->SetMarkerSize(1.5);
    h1d_eff_cos->SetMarkerStyle(kFullCircle);
    h1d_eff_cos->Draw();
    h1d_eff_cos->GetXaxis()->SetTitle("#theta^{true}_{#mu} (degrees)");
    h1d_eff_cos->GetYaxis()->SetTitle("Efficiency");
    h1d_eff_cos->GetYaxis()->SetRangeUser(0.0, 1.0);
    eff_cos->Print("ingrid_eff_cos.pdf");
}
