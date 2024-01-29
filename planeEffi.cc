/*
 * dinupa3@gmail.com
 *
 */
R__LOAD_LIBRARY(libinterface_main)
R__LOAD_LIBRARY(libfun4all)
R__LOAD_LIBRARY(libg4detectors)
R__LOAD_LIBRARY(libg4eval)
R__LOAD_LIBRARY(libktracker)
R__LOAD_LIBRARY(libanamodule)

#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include <TFile.h>
#include <TEfficiency.h>
#include <TGraphAsymmErrors.h>
#include <TString.h>
#include <TCanvas.h>
#include <iostream>

using namespace ROOT::VecOps;
using namespace std;
using ROOT::RDataFrame;

TEfficiency* getEffi(TString det_name, TString trigger, TString acceptance, TString hodo_name, int bins)
{
    RDataFrame df("save", "ana.root");

//     auto d1 = df.Display();
//     d1->Print();

    TString exp_name = Form("hist_%s_exp", hodo_name.Data());
    TString closest_name = Form("hist_%s_closest", hodo_name.Data());

    TString cut = Form("(%s) && (%s) && (%s)", det_name.Data(), trigger.Data(), acceptance.Data());
    TString cut_closest = Form("%s && (eleID_closest > 0)", cut.Data());

    auto hExp = df.Filter(cut.Data()).Histo1D({exp_name.Data(), "; counts [a.u.]; paddles [a.u.]", bins+1, 0.5, bins+0.5}, "eleID_exp");
    auto hClosest = df.Filter(cut_closest.Data()).Histo1D({closest_name.Data(), "; counts [a.u.]; paddles [a.u.]", bins+1, 0.5, bins+0.5}, "eleID_exp");

    auto c1 = new TCanvas();

    hExp->SetFillColorAlpha(kAzure+6, 0.35);
    TString save1 = Form("imgs/planeEffi/%s.png", exp_name.Data());
    hExp->Draw("HIST");
    c1->SaveAs(save1.Data());

    hClosest->SetFillColorAlpha(kAzure+6, 0.35);
    TString save2 = Form("imgs/planeEffi/%s.png", closest_name.Data());
    hClosest->Draw("HIST");
    c1->SaveAs(save2.Data());

    auto effi = new TEfficiency(*hClosest.GetPtr(), *hExp.GetPtr());

    TString effi_name = Form("%s_effi", hodo_name.Data());

    effi->SetName(effi_name.Data());
    effi->SetTitle("; paddle [a.u.]; efficiency [a.u.]");

    effi->SetMarkerColor(kMagenta);
    effi->SetMarkerStyle(20);

    TString save3 = Form("imgs/planeEffi/%s.png", effi_name.Data());
    effi->Draw("APE1");
    c1->SaveAs(save3.Data());

    auto df2 = df.Define("pad_diff", "eleID_exp - eleID_closest");

    TString pad_name = Form("%s_pad_diff", hodo_name.Data());

    auto pad_diff = df2.Filter(cut_closest.Data()).Histo1D({pad_name.Data(), "; padd diff [a.u.]; counts [a.u.]", 20, -15., 15.}, "pad_diff");

    pad_diff->SetFillColorAlpha(kAzure+6, 0.35);

    TString save4 = Form("imgs/planeEffi/%s_pad_diff.png", hodo_name.Data());

    pad_diff->Draw("HIST");
    c1->SaveAs(save4.Data());

    return effi;
}


void planeEffi()
{
    gSystem->mkdir("imgs/planeEffi", 1);

    auto outFile = new TFile("plane_effi.root", "RECREATE");

    auto H2L_effi = (TEfficiency*)getEffi("detID==35", "trigger==1 || trigger==2 || trigger==4 || trigger==5", "D2>3 && diff2 < 10 && D3>3 && diff3 < 10", "H2L", 19);

    H2L_effi->Write();

    outFile->Close();
}
