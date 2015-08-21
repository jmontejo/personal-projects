#include "Sherpa2_xsec.h"
#include "Dataset.h"
#include "TFile.h"
#include <assert.h>
#include <iostream>
#include <utility>
#include <cstdlib>


Dataset::Dataset(TString sample, const TopData *td, double *xsec_external, double *ktfac2_external, int maxEntries){

  tree_num = -1;
  m_td = (TopData *) td;
  xsec_pointer = xsec_external;
  ktfac2_pointer = ktfac2_external;
  hasHFtype = false;
  loadFiles(sample, maxEntries);
  SetBranchAddresses();
  d_particle_jet_id=0;
  d_particle_jet_trueflav=0;
  d_particle_jet_count=0;

}

int Dataset::GetEntry(int i){

  int status = chain->GetEntry(i);
  if(tree_num != chain->GetTreeNumber()){
    tree_num = chain->GetTreeNumber();
    std::cout << "tree: " << tree_num << std::endl;
    *xsec_pointer = xsec.at(tree_num);
    *ktfac2_pointer = ktfac2.at(tree_num);
  }
  if(TString(chain->GetFile()->GetName()).Contains("ttbb_mbb2")) m_td->mc_channel_number = 116108;
  if(TString(chain->GetFile()->GetName()).Contains("ttcc_mbb2")) m_td->mc_channel_number = 116109;
  if(!isNtupTruth){
    m_td->mcevt_pdf_id1->at(0) = d_mcevt_pdf_id1;
    m_td->mcevt_pdf_id2->at(0) = d_mcevt_pdf_id2;
    m_td->EventNumber = d_eventNumber;
    m_td->mc_channel_number = d_mc_channel_number;
    m_td->jet_AntiKt4Truth_n = d_jet_AntiKt4Truth_n;
  }
  if(is7TeV)
    m_td->jet_AntiKt4Truth_n = m_td->jet_AntiKt4Truth_pt->size();
  return status;
}


void Dataset::loadFiles(TString sample, int maxEntries){

  const float BR_nohad = (1-(1-0.108*3)*(1-0.108*3));
  const float BR_dl = (0.108*3)*(0.108*3);
  const float BR_sl = 2*(0.108*3)*(1-0.108*3);
  const float ttbarXS = 252.89;
  xsec_map["Test"] = 1;
  xsec_map["ttcc_mbb2"] = 26.04;
  xsec_map["ttbb_mbb2"] =  9.77;
  xsec_map["116108"] = 1.435;
  xsec_map["116109"] = 2.715;
  xsec_map["164440"] = 4.793;
  xsec_map["164441"] = 5.068;
  xsec_map["164442"] = 3.257;
  xsec_map["164443"] = 2.176;
  xsec_map["164450"] = 19.19;
  xsec_map["164451"] = 20.29;
  xsec_map["164452"] = 13.09;
  xsec_map["164453"] = 8.692;
  xsec_map["Powheg"] = ttbarXS * BR_nohad;
  xsec_map["MadgraphQ2up"] = ttbarXS;
  xsec_map["MadgraphQ2down"] = ttbarXS;
  xsec_map["MadgraphOff_SL"] = ttbarXS * BR_sl;
  xsec_map["MadgraphOff_DL"] = ttbarXS * BR_dl;
  xsec_map["singlelepton"] = ttbarXS * BR_sl;
  xsec_map["dilepton"] = ttbarXS * BR_dl;
  xsec_map["Madgraph4b"] = ttbarXS * BR_nohad;
  xsec_map["Madgraph5F"] = ttbarXS;
  xsec_map["MadgraphKtfac2"] = ttbarXS * BR_nohad;
  xsec_map["Madgraph_CT10"] = ttbarXS * BR_nohad;
  xsec_map["Madgraph_CTEQ6"] = ttbarXS * BR_nohad;
  xsec_map["LeptLept"] = ttbarXS * BR_dl;
  xsec_map["LeptHad"] =  ttbarXS * (BR_nohad - BR_dl);
  kfac_map["singlelepton"] = 1.;
  kfac_map["dilepton"] = 1.;
  kfac_map["Test"] = 1.;
  kfac_map["ttcc_mbb2"] = 1.36*0.718;
  kfac_map["ttbb_mbb2"] = 1.36*0.718;
  kfac_map["116108"] = 1.7937;
  kfac_map["116109"] = 1.7937;
  kfac_map["164440"] = 1.736;
  kfac_map["164441"] = 1.736;
  kfac_map["164442"] = 1.736;
  kfac_map["164443"] = 1.736;
  kfac_map["164450"] = 1.808;
  kfac_map["164451"] = 1.808;
  kfac_map["164452"] = 1.808;
  kfac_map["164453"] = 1.808;
  kfac_map["Powheg"] = 1.;
  kfac_map["Madgraph5F"] = 1.;
  kfac_map["MadgraphKtfac2"] = 1.;
  kfac_map["Madgraph_CT10"] = 1.;
  kfac_map["Madgraph_CTEQ6"] = 1.;
  kfac_map["LeptLept"] = 1.;
  kfac_map["LeptHad"] = 1.;
  ktfac2_map["164440"] = 1.1443;
  ktfac2_map["164441"] = 1.0228;
  ktfac2_map["164442"] = 0.9134;
  ktfac2_map["164443"] = 0.7725;
  ktfac2_map["164450"] = 1.1465;
  ktfac2_map["164451"] = 1.0172;
  ktfac2_map["164452"] = 0.9204;
  ktfac2_map["164453"] = 0.7712;

  isNtupTruth = (sample.Contains("Sherpa") || sample.Contains("AlpgenMbb2") || (sample.Contains("Madgraph")  && !sample.Contains("MadgraphMPI")&& !sample.Contains("MadgraphOff")) || sample.Contains("PowhegPythiaSlim") || sample.Contains("aMcAtNlo") || sample.Contains("NtupTruth") || sample.Contains("ttA")  || sample.Contains("ttH"));
  isNtupTop = (sample.Contains("NtupTop"));
  
  isNotDecayed = sample.Contains("stable");
  is7TeV = sample.Contains("7TeV");
  TString treename = "mini";
  if(isNtupTruth) treename = "truth";
  if(is7TeV || isNtupTop) treename = "physics";
  chain = new TChain(treename);

  TString base = "/nfs/atlas-data06/scratch/jmontejo/ntup_truth_prod/";
  vector<vector<TString> > tmp_pack;
  vector<TString>  files;
  if (sample.Contains("AlpgenMbb2")){
    files.push_back(base+"alpgen_mbb2/ttbb_mbb2.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(base+"alpgen_mbb2/ttcc_mbb2.root");
    tmp_pack.push_back(files); files.clear();
  }
  if (sample.Contains("PowhegPythiaTest")){
    files.push_back("/nfs/atlas-data06/scratch/jmontejo/FolderProduction_V23_stolenAndrea/__panda_rootCoreWorkDir/run/truth.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Alpgen")){
    files.push_back(base+"user.jmontejo.mc12_8TeV.164450.AlpgenJimmy_baseline.merge.NTUP_TOP.e1527_s1499_s1504_r3658_r3549_p1400_miniTruth2/merge.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(base+"user.jmontejo.mc12_8TeV.164451.AlpgenJimmy_baseline.merge.NTUP_TOP.e1527_s1499_s1504_r3658_r3549_p1400_miniTruth2/merge.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(base+"user.jmontejo.mc12_8TeV.164452.AlpgenJimmy_baseline.merge.NTUP_TOP.e1527_s1499_s1504_r3658_r3549_p1400_miniTruth2/merge.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(base+"user.jmontejo.mc12_8TeV.164453.AlpgenJimmy_baseline.merge.NTUP_TOP.e1527_s1499_s1504_r3658_r3549_p1400_miniTruth2/merge.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(base+"user.jmontejo.mc12_8TeV.164440.AlpgenJimmy_baseline.merge.NTUP_TOP.e1527_s1499_s1504_r3658_r3549_p1400_miniTruth2/merge.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(base+"user.jmontejo.mc12_8TeV.164441.AlpgenJimmy_baseline.merge.NTUP_TOP.e1527_s1499_s1504_r3658_r3549_p1400_miniTruth2/merge.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(base+"user.jmontejo.mc12_8TeV.164442.AlpgenJimmy_baseline.merge.NTUP_TOP.e1527_s1499_s1504_r3658_r3549_p1400_miniTruth2/merge.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(base+"user.jmontejo.mc12_8TeV.164443.AlpgenJimmy_baseline.merge.NTUP_TOP.e1527_s1499_s1504_r3658_r3549_p1400_miniTruth2/merge.root");
    tmp_pack.push_back(files); files.clear();
    //files.push_back(base+"user.jmontejo.mc12_8TeV.116108.AlpgenJimmy_AUET2CTEQ6L1_ttbbincl.merge.NTUP_TOP.e1533_a159_a171_r3549_p1400_miniTruth2/merge.root");
    //tmp_pack.push_back(files); files.clear();
    //files.push_back(base+"user.jmontejo.mc12_8TeV.116109.AlpgenJimmy_AUET2CTEQ6L1_ttccincl.merge.NTUP_TOP.e1533_a159_a171_r3549_p1400_miniTruth2/merge.root");
    //tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_NoHad")){
    //files.push_back(Form("/nfs/atlas-data06/scratch/jmontejo/ntup_truth_prod/liron_Htb/NoHad/workSLNoHad.1.truth.root"));
    files.push_back(Form("/nfs/atlas-data06/scratch/jmontejo/ntup_truth_prod/workSLNoHad.2.truth.root"));
    files.push_back(Form("/nfs/atlas-data06/scratch/jmontejo/ntup_truth_prod/workSLNoHad.22.truth.root"));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_Had")){
    //files.push_back(Form("/nfs/atlas-data06/scratch/jmontejo/ntup_truth_prod/liron_Htb/Had/workSLHad.1.truth.root"));
    files.push_back(Form("/nfs/atlas-data06/scratch/jmontejo/ntup_truth_prod/workSLHad.2.truth.root"));
    files.push_back(Form("/nfs/atlas-data06/scratch/jmontejo/ntup_truth_prod/workSLHad.22.truth.root"));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_CTEQ")){
    for(int i=0;i<=250;i++)
      files.push_back(Form("/pnfs-disk/pic.es/at3/projects/scratch/TOYinput/www.physik.uzh.ch/data/ttHsim/mcatnlo_ttbb/CTEQ/ntup_truth%d.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_LO_stable")){
    for(int i=0;i<=40;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/SherpaLO_ttbb_stable/S_stab_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_LO_decayed")){
    for(int i=0;i<=40;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/SherpaLO_ttbb_decayed/S_dec_had_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_LO_SemiLep")){
    for(int i=0;i<=40;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/Samples_LO_SemiLep/S_dec_had_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_NLO_stable")){
    for(int i=0;i<=2047;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/SherpaNLO_ttbb_stable/S_st_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_NLO_decayed")){
    for(int i=0;i<=2047;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/SherpaNLO_ttbb_decayed/S_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_LOPS_stable")){
    for(int i=0;i<=40;i++)
      files.push_back(Form("/pnfs-disk/pic.es/at3/projects/scratch/TOYinput/www.physik.uzh.ch/data/ttHsim/mcatnlo_ttbb/LOPS_stable/ntup_truth%d.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_LOPS_dec_hadr")){
    for(int i=0;i<=40;i++)
      files.push_back(Form("/pnfs-disk/pic.es/at3/projects/scratch/TOYinput/www.physik.uzh.ch/data/ttHsim/mcatnlo_ttbb/LOPS_dec_hadr/ntup_truth%d.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_Q_CMMPS")){
    for(int i=0;i<=256;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/Samples_Q_CMMPS/S_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_CSS_KIN")){
    for(int i=0;i<=256;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/Samples_CSS_KIN/S_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_NNPDF")){
    for(int i=0;i<=256;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/Samples_NNPDF/S_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_MSTW")){
    for(int i=0;i<=256;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/Samples_MSTW/S_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_defaultX05")){
    for(int i=0;i<=256;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/Samples_defaultX05/S_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_defaultX2")){
    for(int i=0;i<=256;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/Samples_defaultX2/S_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_default")){
    for(int i=0;i<=256;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/Samples_default/S_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_R_Mbb")){
    for(int i=0;i<=256;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/Samples_R_Mbb/S_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_glo_soft")){
    for(int i=0;i<=256;i++)
      files.push_back(Form("/pnfs/pic.es/data/at3/disk/TOPD3PD/mcatnlo_ttbb/Samples_glo_soft/S_%d.hepmc2g.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2_toy_ttbb")){
    files.push_back("/pnfs-disk/pic.es/at3/projects/jmontejo/projects/work/Sherpa/hepmc_convert/job_hep_full_1057348891.hepmc2g/hep_full_1057348891.hepmc2g.NTUP_TRUTH.root");
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa2")){
    files.push_back(base+"sherpa2_tt3j/events.NTUP_TRUTH.root");
    tmp_pack.push_back(files); files.clear();
  } 
  else if(sample.Contains("Sherpa")){
    //files.push_back(base+"user.jmontejo.mc12_8TeV.181380.Sherpa_CT10_TTbarLeptLeptMassiveCB.NTUP_TRUTH.v3/merge.root");
    //tmp_pack.push_back(files);
    //files.clear();
    files.push_back(base+"user.jmontejo.mc12_8TeV.181383.Sherpa_CT10_TTbarLeptHadMassiveCB.NTUP_TRUTH.v3/merge0.root");
    files.push_back(base+"user.jmontejo.mc12_8TeV.181383.Sherpa_CT10_TTbarLeptHadMassiveCB.NTUP_TRUTH.v3/merge1.root");
    files.push_back(base+"user.jmontejo.mc12_8TeV.181383.Sherpa_CT10_TTbarLeptHadMassiveCB.NTUP_TRUTH.v3/merge2.root");
    files.push_back(base+"user.jmontejo.mc12_8TeV.181383.Sherpa_CT10_TTbarLeptHadMassiveCB.NTUP_TRUTH.v3/merge3.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("ttH_40")){
    files.push_back("/nfs/at3/scratch2/mcasolino/VLQ_BSM/ttH/tth_40.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("ttH_60")){
    files.push_back("/nfs/at3/scratch2/mcasolino/VLQ_BSM/ttH/tth_60.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("ttH_125")){
    files.push_back("/nfs/at3/scratch2/mcasolino/VLQ_BSM/ttH/tth_125.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("ttA_20")){
    files.push_back("/nfs/at3/scratch2/mcasolino/VLQ_BSM/WHadrons/tta_20.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("ttA_40")){
    files.push_back("/nfs/at3/scratch2/mcasolino/VLQ_BSM/WHadrons/tta_40.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("ttA_60")){
    files.push_back("/nfs/at3/scratch2/mcasolino/VLQ_BSM/WHadrons/tta_60.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("ttA_80")){
    files.push_back("/nfs/at3/scratch2/mcasolino/VLQ_BSM/WHadrons/tta_80.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("ttA_125")){
    files.push_back("/nfs/at3/scratch2/mcasolino/VLQ_BSM/WHadrons/tta_125.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("ZbbNtupTop")){
    files.push_back("/pnfs-disk/pic.es/at3/projects/scratch/mc12_8TeV.110817.AlpgenPythia_P2011C_ZeebbNp0.merge.NTUP_TOP.e1477_s1499_s1504_r3658_r3549_p1400_tid01213608_00/NTUP_TOP.01213608._000127.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegHerwigDileptonMPIp15")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI_p15/user.jmontejo.mc12_8TeV.181088.PowhegJimmy_AUET2CT10_ttbar_DilepFilter.NTUP_TRUTH.v7_EXT0/";
    for(int i=0;i<=8;i++)
      files.push_back(folder+Form("Run_jmontejo_truth.%d.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegHerwigDileptonMPI")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI/mc12_8TeV.181088.PowhegJimmy_AUET2CT10_ttbar_DilepFilter.merge.NTUP_TOP.e2091_a188_a205_r4540_p1400/";
    for(int i=0;i<=1;i++)
      files.push_back(folder+Form("Run_jmontejo_truth.%d.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegHerwigMPIp15")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI_p15/user.jmontejo.mc12_8TeV.105860.PowhegJimmy_AUET2CT10_ttbar_LeptonFilter.NTUP_TRUTH.v7_EXT0/";
      for(int i=0;i<=9;i++)
        files.push_back(folder+Form("Run_jmontejo_truth.%d.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegHerwigMPI")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI/user.jmontejo.mc12_8TeV.105860.PowhegJimmy_AUET2CT10_ttbar_LeptonFilter.NTUP_TRUTH.v5/";
    for(int i=0;i<=7;i++)
      files.push_back(folder+Form("Run_jmontejo_truth.%d.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegHerwig")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth/mc12_8TeV.105860.PowhegJimmy_AUET2CT10_ttbar_LeptonFilter.merge.NTUP_TOP.e1576_a159_a171_r3549_p1400/";
    for(int i=0;i<=3;i++)
      files.push_back(folder+Form("Run_105860_truth.%d.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythiaNtupTop")){
    files.push_back("/pnfs-disk/pic.es/at3/projects/scratch/mc12_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.NTUP_TOP.e1728_s1581_s1586_r3658_r3549_p1635_tid01427836_00/NTUP_TOP.01427836._001029.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythiaNtupTruth")){
    files.push_back("/pnfs-disk/pic.es/at3/projects/scratch/mc12_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.NTUP_TRUTH.e1728_p1032_tid01195180_00/NTUP_TRUTH.01195180._000798.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythia_CT10_nom")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_miniTruth/mc12_8TeV.105861.PowhegPythia_AUET2BCT10_ttbar_LeptonFilter.merge.NTUP_TRUTH.e1317_p1032/";
    for(int i=0;i<=59;i++)
      files.push_back(folder+Form("user.jmontejo.007667._000%02d.merge.truth.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythia_CT10_rensc2")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_miniTruth/mc12_8TeV.174763.PowhegPythia_AUET2BCT10_ttbar_LeptonFilter_rensc2.merge.NTUP_TRUTH.e1721_p1032/";
    for(int i=0;i<=59;i++)
      files.push_back(folder+Form("user.jmontejo.007634._000%02d.merge.truth.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythia_CT10_rensc05")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_miniTruth/mc12_8TeV.174764.PowhegPythia_AUET2BCT10_ttbar_LeptonFilter_rensc05.merge.NTUP_TRUTH.e1721_p1032/";
    for(int i=0;i<=59;i++)
      files.push_back(folder+Form("user.jmontejo.007644._000%02d.merge.truth.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythia_CT10_facsc2")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_miniTruth/mc12_8TeV.174765.PowhegPythia_AUET2BCT10_ttbar_LeptonFilter_facsc2.merge.NTUP_TRUTH.e1721_p1032/";
    for(int i=0;i<=59;i++)
      files.push_back(folder+Form("user.jmontejo.007649._000%02d.merge.truth.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythia_CT10_facsc05")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_miniTruth/mc12_8TeV.174766.PowhegPythia_AUET2BCT10_ttbar_LeptonFilter_facsc05.merge.NTUP_TRUTH.e1721_p1032/";
    for(int i=0;i<=59;i++)
      files.push_back(folder+Form("user.jmontejo.007657._000%02d.merge.truth.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegJimmy_CT10_nom")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_miniTruth/mc12_8TeV.105860.PowhegJimmy_AUET2CT10_ttbar_LeptonFilter.merge.NTUP_TRUTH.e1576_p1032/";
    for(int i=0;i<=59;i++)
      files.push_back(folder+Form("user.jmontejo.007627._000%02d.merge.truth.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegJimmy_CT10_rensc05")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_miniTruth/mc12_8TeV.117856.PowhegJimmy_AUET2CT10_ttbar_LeptonFilter_rensc05.merge.NTUP_TRUTH.e1916_p1032/";
    for(int i=0;i<=59;i++)
      files.push_back(folder+Form("user.jmontejo.007618._000%02d.merge.truth.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegJimmy_CT10_rensc2")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_miniTruth/mc12_8TeV.117857.PowhegJimmy_AUET2CT10_ttbar_LeptonFilter_rensc2.merge.NTUP_TRUTH.e1916_p1032/";
    for(int i=0;i<=59;i++)
      files.push_back(folder+Form("user.jmontejo.007620._000%02d.merge.truth.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegJimmy_CT10_facsc05")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_miniTruth/mc12_8TeV.117858.PowhegJimmy_AUET2CT10_ttbar_LeptonFilter_facsc05.merge.NTUP_TRUTH.e1916_p1032/";
    for(int i=0;i<=59;i++)
      files.push_back(folder+Form("user.jmontejo.007623._000%02d.merge.truth.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegJimmy_CT10_facsc2")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_miniTruth/mc12_8TeV.117859.PowhegJimmy_AUET2CT10_ttbar_LeptonFilter_facsc2.merge.NTUP_TRUTH.e1916_p1032/";
    for(int i=0;i<=59;i++)
      files.push_back(folder+Form("user.jmontejo.007625._000%02d.merge.truth.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythia_7TeV")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc11/sl8/mc11_7TeV.105861.TTbar_PowHeg_Pythia.merge.NTUP_TOP.e873_s1372_s1370_r3043_r2993_p834_sl8/";
    files.push_back(folder+"merge2190194867_0.root");
    files.push_back(folder+"merge2190194867_1.root");
    files.push_back(folder+"merge2190194867_2.root");
    files.push_back(folder+"merge2190194867_3.root");
    files.push_back(folder+"merge2190194867_4.root");
    files.push_back(folder+"merge2190194867_5.root");
    files.push_back(folder+"merge2190194867_6.root");
    files.push_back(folder+"merge2190194867_7.root");
    files.push_back(folder+"merge2955167396_0.root");
    files.push_back(folder+"merge862637302_0.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythiaMiniTruth")){
    for(int i=39;i<=375;i++)
      files.push_back(base+Form("user.jmontejo.mc12_8TeV.117050.miniTruth/user.jmontejo.008260._00%03d.merge.truth.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythiaSlim")){
    for(int i=11;i<=22;i++)
      files.push_back(base+Form("user.jmontejo.mc12_8TeV.117050.NTUP_TRUTH.slim/user.jmontejo.008242._000%d.merge.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythiaMPIhigh")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI/user.jmontejo.mc12_8TeV.117429.PowhegPythia_P2012mpiHi_TTbar.NTUP_TRUTH.v4/";
      files.push_back(folder+"Run_jmontejo_truth.0.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythiaMPIref")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI/user.jmontejo.mc12_8TeV.117428.PowhegPythia_P2012_TTbar.NTUP_TRUTH.v4/";
      files.push_back(folder+"Run_jmontejo_truth.0.root");
      files.push_back(folder+"Run_jmontejo_truth.1.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythiaDileptonMPIp15")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI_p15/user.jmontejo.mc12_8TeV.181087.PowhegPythia_P2011C_ttbar_dilepton.NTUP_TRUTH.v7_EXT0/";
      for(int i=0;i<=6;i++)
        files.push_back(folder+Form("Run_jmontejo_truth.%d.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythiaDileptonMPI")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI/user.jmontejo.mc12_8TeV.181087.PowhegPythia_P2011C_ttbar_dilepton.NTUP_TRUTH.v5/";
      for(int i=0;i<=5;i++)
        files.push_back(folder+Form("Run_jmontejo_truth.%d.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythiaMPIp15")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI_p15//user.jmontejo.mc12_8TeV.117050.PowhegPythia_P2011C_ttbar.NTUP_TRUTH.v7_EXT0/";
      for(int i=0;i<=14;i++)
        files.push_back(folder+Form("Run_jmontejo_truth.%d.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythiaMPI")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI/user.jmontejo.mc12_8TeV.117050.PowhegPythia_P2011C_ttbar.NTUP_TRUTH.v4/";
      files.push_back(folder+"Run_jmontejo_truth.0.root");
      files.push_back(folder+"Run_jmontejo_truth.1.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythiaFastMPIp15")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI_p15/user.jmontejo.mc12_8TeV.117050.PowhegPythia_P2011C_ttbar.NTUP_TRUTH.v8fast_EXT0/";
      for(int i=0;i<=10;i++)
        files.push_back(folder+Form("Run_jmontejo_truth.%d.root",i));
    folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI_p15/user.jmontejo.mc12_8TeV.117050.PowhegPythia_P2011C_ttbar.NTUP_TRUTH.v8fast_EXT0_v2/";
      for(int i=0;i<=12;i++)
        files.push_back(folder+Form("Run_jmontejo_truth.%d.root",i));
    folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI_p15/user.jmontejo.mc12_8TeV.117050.PowhegPythia_P2011C_ttbar.NTUP_TRUTH.v8fast_EXT0_v3/";
      for(int i=0;i<=0;i++)
        files.push_back(folder+Form("Run_jmontejo_truth.%d.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythiaFastMPI")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI/user.jmontejo.mc12_8TeV.117050.PowhegPythia_P2011C_ttbar.NTUP_TRUTH.v5fast/";
      for(int i=0;i<=17;i++)
        files.push_back(folder+Form("Run_jmontejo_truth.%d.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythiaFix")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth/mc12_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.NTUP_TOP.e1727_a188_a171_r3549_p1400/";
    for(int i=0;i<=11;i++)
      files.push_back(folder+Form("Run_117050_truth.%d.root",i));
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("PowhegPythia")){
    files.push_back(base+"user.jmontejo.mc12_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.NTUP_TOP.e1728_s1581_s1586_r3658_r3549_p1400/merge.root");
    tmp_pack.push_back(files);
  } 
  else if(sample.Contains("aMcAtNlo")){
    for(int i=0;i<=1000;i++)
      files.push_back(base+Form("user.mamolla.MC12.110085.aMcAtNloHerwigpp_UEEE4_MRSTMCal_CT10ME_ttbar_8TeV_ljets.NTUP_TRUTH.mc12_v2.131123152304/user.mamolla.003569.EXT0._0%04d.NTUP_TRUTH.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphFinal")){
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_10017_307.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_10259_306.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_10304_303.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_10415_81.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_10597_174.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_10792_214.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_10910_318.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_10968_402.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_11066_89.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_11147_3.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_11167_98.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_11168_11.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_11381_248.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_11475_398.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_11867_312.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_1_194.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_11968_270.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_12107_178.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_12189_403.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_12245_224.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_12351_268.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_12497_399.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_13023_15.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_13468_198.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_13514_218.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_1352_358.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_1360_32.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_13788_374.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_13923_233.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_14095_316.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_15317_343.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_15799_204.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_15872_90.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_16547_353.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_16894_354.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_16998_7.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_17016_296.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_17196_339.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_17199_346.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_17408_360.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_1747_314.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_17886_1.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_18440_238.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_18454_240.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_18514_381.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_18523_2.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_1882_315.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_18845_113.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_1890_27.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_19090_38.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_19103_400.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_19430_70.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_19666_409.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_1976_230.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_20259_255.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_20611_5.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_20857_365.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_20944_322.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_21429_147.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_21685_51.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_22118_75.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_22212_31.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_22360_311.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_22656_162.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_22700_19.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_22795_319.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_22911_378.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_2294_384.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_22976_408.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_23110_392.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_23341_310.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_23425_370.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_23631_363.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_24184_362.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_24225_30.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_2474_246.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_24756_405.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_24758_215.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_24788_148.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_25051_25.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_2517_414.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_25428_12.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_25511_9.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_2585_305.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_25853_372.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_25902_280.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_26036_13.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_26330_380.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_26564_97.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_27473_388.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_27788_368.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_27791_18.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_27824_420.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_27828_294.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_28110_349.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_28361_386.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_28448_271.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_28620_84.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_28681_413.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_28991_410.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_29287_309.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_29387_393.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_29419_216.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_2947_260.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_29523_406.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_30184_235.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_30220_120.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_30513_39.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_30628_356.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_30786_275.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_30878_417.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_31701_265.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_31754_62.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_31957_290.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_32345_404.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_32458_382.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_32_8.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_3828_33.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_4077_293.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_4142_54.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_4608_313.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_4650_67.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_5203_390.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_5246_395.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_5413_412.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_5558_407.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_5576_347.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_6229_367.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_6348_221.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_6519_329.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_6788_342.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_6952_104.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_7188_328.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_7202_419.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_7248_225.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_770_6.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_7707_415.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_7861_418.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_7868_323.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_796_36.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_8227_41.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_8535_345.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_887_126.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_8916_317.pool.root");
    files.push_back(base+"OutputLHE_MadgraphControl_FinalMadgraphSetup_Shower_NtupTruth/Madgraph_Validation_FirstTrial_110871_9913_366.pool.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Madgraph_nominal_NtupTruth")){
    for(int i=0;i<=2981;i++)
      files.push_back(Form("/pnfs-disk/pic.es/at3/projects/scratch/user.jmontejo.mc12_8TeV.110871.MadGraphPythia_CT10_P2011C_ttbar_singlelepton.NTUP_TRUTH.v1/ntup_%i.root",i));
    tmp_pack.push_back(files); files.clear();
    for(int i=0;i<=15;i++)
      files.push_back(Form("/pnfs-disk/pic.es/at3/projects/scratch/user.jmontejo.mc12_8TeV.110872.MadGraphCT10Pythia_CT10_P2011C_ttbar_dilepton.NTUP_TRUTH.v1/ntup_%i.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphMPI_nominalp15")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI_p15/user.jmontejo.mc12_8TeV.110871.MadGraphPythia_CT10_P2011C_ttbar_singlelepton.NTUP_TRUTH.v7_EXT0/";
      for(int i=0;i<=3;i++)
        files.push_back(folder+Form("Run_jmontejo_truth.%d.root",i));
    tmp_pack.push_back(files); files.clear();
    files.push_back("/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI_p15/user.jmontejo.mc12_8TeV.110872.MadGraphPythia_CT10_P2011C_ttbar_dilepton.NTUP_TRUTH.v8_EXT0/truth.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphMPI_nominal")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth_MPI/";
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110871.MadGraphPythia_CT10_P2011C_ttbar_singlelepton.NTUP_TRUTH.v5/Run_jmontejo_truth.0.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110871.MadGraphPythia_CT10_P2011C_ttbar_singlelepton.NTUP_TRUTH.v5/Run_jmontejo_truth.1.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110872.MadGraphCT10Pythia_CT10_P2011C_ttbar_dilepton.NTUP_TRUTH.v4/Run_jmontejo_truth.0.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphOff_nominal")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth/";
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110871.MadGraphPythia_CT10_P2011C_ttbar_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110871.MadGraphPythia_CT10_P2011C_ttbar_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.1.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110871.MadGraphPythia_CT10_P2011C_ttbar_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.2.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110872.MadGraphCT10Pythia_CT10_P2011C_ttbar_dilepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphOff_Q2down")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth/";
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110877.MadgraphCT10_PythiaPerugia2011c_ttbarQ2down_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110877.MadgraphCT10_PythiaPerugia2011c_ttbarQ2down_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.1.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110877.MadgraphCT10_PythiaPerugia2011c_ttbarQ2down_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.2.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110878.MadgraphCT10_PythiaPerugia2011c_ttbarQ2down_dilepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphOff_Matchup")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth/";
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110880.MadGraphPythia_CT10_P2011C_ttbarMatchup_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110880.MadGraphPythia_CT10_P2011C_ttbarMatchup_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.1.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110881.MadGraphPythia_CT10_P2011C_ttbarMatchup_dilepton.NTUP_TRUTH.v2/Run_jmontejo_truth.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphOff_Mbup")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth/";
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110886.MadGraphPythia_CT10_P2011C_ttbarMb505_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110886.MadGraphPythia_CT10_P2011C_ttbarMb505_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.1.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110886.MadGraphPythia_CT10_P2011C_ttbarMb505_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.2.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110886.MadGraphPythia_CT10_P2011C_ttbarMb505_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.3.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110887.MadGraphPythia_CT10_P2011C_ttbarMb505_dilepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphOff_Mbdown")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth/";
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110889.MadGraphPythia_CT10_P2011C_ttbarMb455_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110889.MadGraphPythia_CT10_P2011C_ttbarMb455_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.1.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110889.MadGraphPythia_CT10_P2011C_ttbarMb455_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.2.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110890.MadGraphPythia_CT10_P2011C_ttbarMb455_dilepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphOff_Q2up")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth/";
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110874.MadgraphCT10_PythiaPerugia2011c_ttbarQ2up_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110874.MadgraphCT10_PythiaPerugia2011c_ttbarQ2up_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.1.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110875.MadgraphCT10_PythiaPerugia2011c_ttbarQ2up_dilepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphOff_Matchup")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth/";
    //files.push_back(folder+"mc12_8TeV.110879.MadGraphPythia_CT10_P2011C_ttbarMatchup_allhad.merge.NTUP_TRUTH.e2669_p1581/Run_110879_truth.0.root");
    files.push_back(folder+"mc12_8TeV.110880.MadGraphPythia_CT10_P2011C_ttbarMatchup_singlelepton.merge.NTUP_TRUTH.e2669_p1581/Run_110880_truth.0.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(folder+"mc12_8TeV.110881.MadGraphPythia_CT10_P2011C_ttbarMatchup_dilepton.merge.NTUP_TRUTH.e2669_p1581/Run_110881_truth.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphOff_Mbup")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth/";
    //files.push_back(folder+"mc12_8TeV.110885.MadGraphPythia_CT10_P2011C_ttbarMb505_allhad.merge.NTUP_TRUTH.e2665_p1581/Run_110885_truth.0.root");
    files.push_back(folder+"mc12_8TeV.110886.MadGraphPythia_CT10_P2011C_ttbarMb505_singlelepton.merge.NTUP_TRUTH.e2665_p1581/Run_110886_truth.0.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(folder+"mc12_8TeV.110887.MadGraphPythia_CT10_P2011C_ttbarMb505_dilepton.merge.NTUP_TRUTH.e2665_p1581/Run_110887_truth.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphOff_Mbdown")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth/";
    //files.push_back(folder+"mc12_8TeV.110888.MadGraphPythia_CT10_P2011C_ttbarMb455_allhad.merge.NTUP_TRUTH.e2665_p1581/Run_110888_truth.0.root");
    files.push_back(folder+"mc12_8TeV.110889.MadGraphPythia_CT10_P2011C_ttbarMb455_singlelepton.merge.NTUP_TRUTH.e2665_p1581/Run_110889_truth.0.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(folder+"mc12_8TeV.110890.MadGraphPythia_CT10_P2011C_ttbarMb455_dilepton.merge.NTUP_TRUTH.e2665_p1581/Run_110890_truth.0.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphOff_Mcup")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth/";
    //files.push_back(folder+"mc12_8TeV.110891.MadGraphPythia_CT10_P2011C_ttbarMc158_allhad.merge.NTUP_TRUTH.e2639_p1581/Run_110891_truth.0.root");
    //files.push_back(folder+"mc12_8TeV.110892.MadGraphPythia_CT10_P2011C_ttbarMc158_singlelepton.merge.NTUP_TRUTH.e2639_p1581/Run_110892_truth.0.root");
    //files.push_back(folder+"mc12_8TeV.110893.MadGraphPythia_CT10_P2011C_ttbarMc158_dilepton.merge.NTUP_TRUTH.e2639_p1581/Run_110893_truth.0.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110892.MadGraphPythia_CT10_P2011C_ttbarMc158_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110893.MadGraphPythia_CT10_P2011C_ttbarMc158_dilepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphOff_Mcdown")){
    TString folder = "/pnfs/pic.es/data/at3/disk/TOPD3PD/mc12/prod_truth/";
    //files.push_back(folder+"mc12_8TeV.110894.MadGraphPythia_CT10_P2011C_ttbarMc142_allhad.merge.NTUP_TRUTH.e2639_p1581/Run_110894_truth.0.root");
    //files.push_back(folder+"mc12_8TeV.110895.MadGraphPythia_CT10_P2011C_ttbarMc142_singlelepton.merge.NTUP_TRUTH.e2639_p1581/Run_110895_truth.0.root");
    //files.push_back(folder+"mc12_8TeV.110896.MadGraphPythia_CT10_P2011C_ttbarMc142_dilepton.merge.NTUP_TRUTH.e2639_p1581/Run_110896_truth.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110895.MadGraphPythia_CT10_P2011C_ttbarMc142_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110895.MadGraphPythia_CT10_P2011C_ttbarMc142_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.1.root");
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110895.MadGraphPythia_CT10_P2011C_ttbarMc142_singlelepton.NTUP_TRUTH.v2/Run_jmontejo_truth.2.root");
    tmp_pack.push_back(files); files.clear();
    files.push_back(folder+"user.jmontejo.mc12_8TeV.110896.MadGraphPythia_CT10_P2011C_ttbarMc142_dilepton.NTUP_TRUTH.v2/Run_jmontejo_truth.0.root");
    tmp_pack.push_back(files); files.clear();
  }

  else if(sample.Contains("MadgraphCT10up")){
    for(int i=0;i<=250;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_CT10_Q2down_RadHi_Shower_NtupTruth/user.grohsjea.000968._00%03d.XYZlheXYZ.NTUP_Truth.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphCTEQ6down")){
    for(int i=0;i<=250;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_CTEQ6L1_Q2up_RadLo_FixedKin_Shower_NtupTruth/user.grohsjea.001468._00%03d.XYZlheXYZ.NTUP_Truth.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Madgraph0a")){
    for(int i=161;i<=559;i++)
      files.push_back(base+Form("OutputLHE_CMSGridpackBatch_ProducedByJordiOnGrid_ShowerBatch_NtupTruth/evgen.MG_nomin_user.jnadal.004%d._00001.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Madgraph0b")){
    for(int i=161;i<=559;i++)
      files.push_back(base+Form("OutputLHE_CMSGridpackBatch_ProducedByJordiOnGrid_woMadspin_ShowerBatch_NtupTruth/evgen.MG_nomin_user.jnadal.004%d._00001.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Madgraph3b")){
    for(int i=0;i<=207;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_MasslessB_woMadspin_ShowerBatch_NtupTruth/evgen.MG_MasslessB_user.aknue.112425._00%03d.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    for(int i=0;i<=49;i++) 
      files.push_back(base+Form("OutputLHE_ValerioGridpack_MasslessB_woMadspin_ShowerBatch_NtupTruth/evgen.MG_MasslessB_user.wbreaden.000022._00%03d.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphCT10pdftrue")){
    for(int i=0;i<=301;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_CT10_PDFwgtTrue_woMadspin_ShowerBatch_NtupTruth/evgen.MG_CMSlike_fixed_user.aknue.115364._00%03d.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Madgraph_masslessB") || sample.Contains("Madgraph3a")){
    for(int i=0;i<=207;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_MasslessB_ShowerBatch_NtupTruth/evgen.MG_MasslessB_user.aknue.112425._00%03d.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    for(int i=0;i<=49;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_MasslessB_ShowerBatch_NtupTruth/evgen.MG_MasslessB_user.wbreaden.000022._00%03d.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Madgraph_Q2up_radLo")){
    for(int i=0;i<=200;i++)
      files.push_back(base+Form("MADGRAPHQ2UPMYMS_PYTHIA2012radLo_NtupTruth/user.grohsjea.000068._00%03d.XYZlheXYZ.NTUP_Truth.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Madgraph_Q2up_modPerugia")){
    for(int i=0;i<=200;i++)
      files.push_back(base+Form("MADGRAPHQ2UPMYMS_PYTHIA2012mod_NtupTruth/user.grohsjea.000068._00%03d.XYZlheXYZ.NTUP_Truth.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Madgraph_CMSpack_Perugia")){
    for(int i=0;i<=16;i++)
      files.push_back(base+Form("CMS_gridPack_4M_u/Madgraph_NTUP_%d.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Madgraph_CMSpack_Z2")){
    for(int i=0;i<=16;i++)
      files.push_back(base+Form("CMS_gridPack_4M_u_ForZ2test/Madgraph_NTUP_%d.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Madgraph_CT10") || sample.Contains("Madgraph4a")){
    for(int i=0;i<=202;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_CT10_ShowerBatch_NTupTruth/evgen.MG_CT10_user.aknue.112640._00%03d.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    for(int i=0;i<=100;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_CT10_ShowerBatch_NTupTruth/evgen.MG_CT10_user.kirby.000102._00%03d.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    for(int i=0;i<=100;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_CT10_ShowerBatch_NTupTruth/evgen.MG_CT10_user.scrawley.000022._00%03d.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    for(int i=0;i<=200;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_CT10_ShowerBatch_NTupTruth/evgen.MG_CT10_user.wbreaden.000120._00%03d.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Madgraph4b")){
    for(int i=0;i<=202;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_CT10_woMadspin_ShowerBatch_NTupTruth/evgen.MG_CT10_user.aknue.112640._00%03d.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    for(int i=0;i<=100;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_CT10_woMadspin_ShowerBatch_NTupTruth/evgen.MG_CT10_user.kirby.000102._00%03d.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    for(int i=0;i<=100;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_CT10_woMadspin_ShowerBatch_NTupTruth/evgen.MG_CT10_user.scrawley.000022._00%03d.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    for(int i=0;i<=200;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_CT10_woMadspin_ShowerBatch_NTupTruth/evgen.MG_CT10_user.wbreaden.000120._00%03d.XYZlheXYZ.tgz_NTUP_Truth.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Madgraph_CTEQ6") || sample.Contains("Madgraph1b")){
    for(int i=0;i<=300;i++)
      files.push_back(base+Form("OutputLHE_ValerioGridpack_ForComparisonCMS_NTupTruth/evgen.CMSpack_%d_NTUP_Truth.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("MadgraphKtfac2")){
    for(int i=0;i<=33;i++){
      files.push_back(base+Form("Production_q2UP_500-4000/Madgraph_NTUP_%d.root",i));
      files.push_back(base+Form("Production_q2UP__1000-4000/Madgraph_NTUP_%d.root",i));
      files.push_back(base+Form("Production_q2UP__1000-4000_1/Madgraph_NTUP_%d.root",i));
    }
    tmp_pack.push_back(files); files.clear();
  }
  else if(sample.Contains("Madgraph5F")){
    for(int i=0;i<=11;i++)
      files.push_back(base+Form("madgraph_vdao/Madgraph_NTUP_%d.root",i));
    tmp_pack.push_back(files); files.clear();
  }
  else{
    std::cout << "Dataset not recognized: " << sample << std::endl;
    exit(2);
  }

  for(unsigned int p=0;p<tmp_pack.size();p++){
    vector<TString> files = tmp_pack.at(p);
    Long64_t nentries = 0;
    for(int j=files.size()-1;j>=0;j--){
      if(maxEntries >0 && nentries>maxEntries){
          files.erase(files.begin()+j);
          continue;
      }
      TString fullfile = files.at(j);
      TString protocol = "";
      if(fullfile.Contains("pnfs-disk")) protocol = "dcap://dcache-disk.pic.es/";
      //if(fullfile.Contains("pnfs-disk")) protocol = "dcache:";
      fullfile = protocol+fullfile;
      files.at(j) = fullfile;
      TFile *file = TFile::Open(fullfile);
      if(file){
        TTree *tree = (TTree *) file->Get(treename);
        if(tree){
          nentries += tree->GetEntries();
          delete tree;
          file->Close();
        }
        else{ 
          cout << "Fail tree " << fullfile << " " << treename <<  endl; 
          files.erase(files.begin()+j);
          continue;
        }
      }
      else{ 
        cout << "Fail file " << fullfile << endl; 
        files.erase(files.begin()+j);
        continue;
      }
    }

    for(unsigned int j=0;j<files.size();j++){

      chain->Add(files.at(j));
      std::cout << "Add: " << files.at(j) <<std::endl;
      if(sample.Contains("Sherpa2")){
        Sherpa2_xsec s2;
        bool hasXsec = false;
        for(map<TString,int>::iterator it=s2.ntrials_map.begin();it!=s2.ntrials_map.end();it++){
          if(files.at(j).Contains((*it).first)){
            hasXsec = true;
            xsec.push_back(1./((*it).second*files.size()));
            cout << "Setting XSEC for "<<(*it).first<< ": 1./(" << (*it).second << "*" << files.size() << ")" << endl;
            break;
          }
        }
        if(!hasXsec){
          cout << "No XSEC for " << files.at(j) << endl;
          exit(1);
        }
        ktfac2.push_back(1.);
        assert(xsec.size() == (unsigned int) chain->GetNtrees());
      }
      else{
        bool hasXsec = false;
        for(map<TString,float>::iterator it=xsec_map.begin();it!=xsec_map.end();it++){
          if(files.at(j).Contains((*it).first)){
            hasXsec = true;
            xsec.push_back((*it).second*kfac_map[(*it).first]/nentries);
            if(ktfac2_map.find((*it).first)!=ktfac2_map.end())
              ktfac2.push_back(ktfac2_map[(*it).first]);
            else
              ktfac2.push_back(1.);
            cout << "Setting XSEC for "<<(*it).first<< ": " << (*it).second <<"/"<<nentries << endl;
            break;
          }
        }
        if(!hasXsec)
          for(map<TString,float>::iterator it=xsec_map.begin();it!=xsec_map.end();it++){
            if(sample.Contains((*it).first)){
              hasXsec = true;
              xsec.push_back((*it).second*kfac_map[(*it).first]/nentries);
              if(ktfac2_map.find((*it).first)!=ktfac2_map.end())
                ktfac2.push_back(ktfac2_map[(*it).first]);
              else
                ktfac2.push_back(1.);
              cout << "Setting XSEC for "<<(*it).first<< ": " << (*it).second <<"/"<<nentries << endl;
              break;
            }
          }
        if(!hasXsec){
          xsec.push_back(1.);
          ktfac2.push_back(1.);
          cout << "Setting XSEC for "<<sample<< ": " << 1 <<"/"<<nentries << endl;
        }
        assert(xsec.size() == (unsigned int) chain->GetNtrees());
        assert(ktfac2.size() == (unsigned int) chain->GetNtrees());
      }

    }
  }
}

void Dataset::SetBranchAddresses(){

  if(chain->GetBranch("HFtype")){
    hasHFtype = true;
    chain->SetBranchAddress("HFtype",&d_HFtype);
    chain->SetBranchAddress("extHFtype",&d_extHFtype);
    chain->SetBranchAddress("extHFtype_prompt",&d_extHFtype_prompt);
    chain->SetBranchAddress("passHFtype",&d_passHFtype);
    chain->SetBranchAddress("particle_jet_id",&d_particle_jet_id);
    chain->SetBranchAddress("particle_jet_count",&d_particle_jet_count);
    chain->SetBranchAddress("particle_jet_trueflav",&d_particle_jet_trueflav);
  }

  if (chain->GetBranch("actualIntPerXing")) chain->SetBranchAddress("actualIntPerXing",&m_td->actualIntPerXing);
  else if (chain->GetBranch("mu")) chain->SetBranchAddress("mu",&m_td->actualIntPerXing);
  chain->SetBranchAddress("mc_n",&m_td->mc_n);
  chain->SetBranchAddress("mc_pt",&m_td->mc_pt);
  chain->SetBranchAddress("mc_eta",&m_td->mc_eta);
  chain->SetBranchAddress("mc_phi",&m_td->mc_phi);
  if(chain->GetBranch("mc_E")) chain->SetBranchAddress("mc_E",&m_td->mc_E);
  if(chain->GetBranch("mc_m")) chain->SetBranchAddress("mc_m",&m_td->mc_m);
  chain->SetBranchAddress("mc_pdgId",&m_td->mc_pdgId);
  chain->SetBranchAddress("mc_status",&m_td->mc_status);
  chain->SetBranchAddress("mc_parent_index",&m_td->mc_parent_index);
  chain->SetBranchAddress("mc_child_index",&m_td->mc_child_index);
  if(isNtupTruth){
    m_td->top_hfor_type = -1;
    chain->SetBranchAddress("mcevt_weight",&m_td->mcevt_weight);
    chain->SetBranchAddress("mcevt_pdf_id1",&m_td->mcevt_pdf_id1);
    chain->SetBranchAddress("mcevt_pdf_id2",&m_td->mcevt_pdf_id2);
    chain->SetBranchAddress("mc_channel_number",&m_td->mc_channel_number);
    chain->SetBranchAddress("EventNumber",&m_td->EventNumber);
    chain->SetBranchAddress("jet_AntiKt4TruthJets_n",&m_td->jet_AntiKt4Truth_n);
    chain->SetBranchAddress("jet_AntiKt4TruthJets_pt",&m_td->jet_AntiKt4Truth_pt);
    chain->SetBranchAddress("jet_AntiKt4TruthJets_eta",&m_td->jet_AntiKt4Truth_eta);
    chain->SetBranchAddress("jet_AntiKt4TruthJets_phi",&m_td->jet_AntiKt4Truth_phi);
    chain->SetBranchAddress("jet_AntiKt4TruthJets_E",&m_td->jet_AntiKt4Truth_E);
  }else if (is7TeV){
    chain->SetBranchAddress("mc_channel_number",&m_td->mc_channel_number);
    chain->SetBranchAddress("EventNumber",&m_td->EventNumber);
    chain->SetBranchAddress("top_hfor_type",&m_td->top_hfor_type);
    chain->SetBranchAddress("jet_antiKt4Truth_pt",&m_td->jet_AntiKt4Truth_pt);
    chain->SetBranchAddress("jet_antiKt4Truth_eta",&m_td->jet_AntiKt4Truth_eta);
    chain->SetBranchAddress("jet_antiKt4Truth_phi",&m_td->jet_AntiKt4Truth_phi);
    chain->SetBranchAddress("jet_antiKt4Truth_E",&m_td->jet_AntiKt4Truth_E);
  }else if (isNtupTop){
    chain->SetBranchAddress("mcevt_weight",&m_td->mcevt_weight);
    chain->SetBranchAddress("mcevt_pdf_id1",&m_td->mcevt_pdf_id1);
    chain->SetBranchAddress("mcevt_pdf_id2",&m_td->mcevt_pdf_id2);
    m_td->mcevt_pdf_id1 = new vector<int>(1);
    m_td->mcevt_pdf_id2 = new vector<int>(1);
    chain->SetBranchAddress("mc_channel_number",&m_td->mc_channel_number);
    chain->SetBranchAddress("EventNumber",&m_td->EventNumber);
    chain->SetBranchAddress("top_hfor_type",&m_td->top_hfor_type);
    chain->SetBranchAddress("jet_AntiKt4Truth_n",&m_td->jet_AntiKt4Truth_n);
    chain->SetBranchAddress("jet_AntiKt4Truth_pt",&m_td->jet_AntiKt4Truth_pt);
    chain->SetBranchAddress("jet_AntiKt4Truth_eta",&m_td->jet_AntiKt4Truth_eta);
    chain->SetBranchAddress("jet_AntiKt4Truth_phi",&m_td->jet_AntiKt4Truth_phi);
    chain->SetBranchAddress("jet_AntiKt4Truth_E",&m_td->jet_AntiKt4Truth_E);
  }else{
    chain->SetBranchAddress("channelNumber",&d_mc_channel_number);
    chain->SetBranchAddress("eventNumber",&d_eventNumber);
    chain->SetBranchAddress("mcevt_pdf_id1",&d_mcevt_pdf_id1);
    chain->SetBranchAddress("mcevt_pdf_id2",&d_mcevt_pdf_id2);
    m_td->mcevt_pdf_id1 = new vector<int>(1);
    m_td->mcevt_pdf_id2 = new vector<int>(1);
    chain->SetBranchAddress("hfor",&m_td->top_hfor_type);
    chain->SetBranchAddress("jet_AntiKt4Truth_n",&d_jet_AntiKt4Truth_n);
    chain->SetBranchAddress("jet_AntiKt4Truth_pt",&m_td->jet_AntiKt4Truth_pt);
    chain->SetBranchAddress("jet_AntiKt4Truth_eta",&m_td->jet_AntiKt4Truth_eta);
    chain->SetBranchAddress("jet_AntiKt4Truth_phi",&m_td->jet_AntiKt4Truth_phi);
    chain->SetBranchAddress("jet_AntiKt4Truth_E",&m_td->jet_AntiKt4Truth_E);
  }

}

bool Dataset::isLastEntry(int i){
  return( m_td->EventNumber == chain->GetTree()->GetEntries());
}
