/*
 * dinupa3@gmail.com
 *
 */

#include <TFile.h>
#include <TEfficiency.h>
#include <TGraphAsymmErrors.h>
#include <TString.h>
#include <TCanvas.h>
#include <vector>
#include <iostream>

using namespace std;

vector<TString> effi_files = {"plane_effi.root"};
vector<double> voltage = {0.};

void getEffi(TString det_name, int bins)
{

    auto c1 = new TCanvas();

    for(int ii = 0; ii < bins; ii++)
    {
        TString graph_name = Form("%s_pad_effi_%d", det_name.Data(), ii);

        auto pad_effi = new TGraphAsymmErrors();
        pad_effi->SetNameTitle(graph_name.Data(), "; voltage [V]; efficiency [a.u.]");

        int num_files = effi_files.size();

        for(int jj = 0; jj < num_files; jj++)
        {
            unique_ptr<TFile> inputs(TFile::Open(effi_files.at(jj).Data()));

            TString effi_name = Form("%s_effi", det_name.Data());

            auto effi = (TEfficiency*)inputs->Get(effi_name.Data());

            pad_effi->SetPoint(jj, voltage.at(jj), effi->GetEfficiency(ii));
            pad_effi->SetPointError(jj, 0.0 , 0.0 , effi->GetEfficiencyErrorLow(ii), effi->GetEfficiencyErrorUp(ii));
        }

        pad_effi->SetMarkerColor(kMagenta);
        pad_effi->SetMarkerStyle(20);

        pad_effi->Draw("APE1");

        TString save = Form("imgs/paddleEffi/%s.png", graph_name.Data());
        c1->SaveAs(save.Data());
    }
}


void paddleEffi()
{
    gSystem->mkdir("imgs/paddleEffi", 1);
    getEffi("H2L", 19);
}
