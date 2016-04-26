if [ ! -d /nfs/at3users ]; then
  ~/.mountsshfs.sh
fi
cp -f /nfs/atlas-data06/scratch/jmontejo/RooStat_files/RootStat_VLQ_v4/{other,detector}UNC600_singlet_new.eps /projects/work/Tesis/My_thesis/Analysis/Figures_HtX
cp -f /nfs/atlas-data06/scratch/jmontejo/RooStat_files/RootStat_VLQ_v4/results_AllSyst_Job_600_singlet_new/Reduced.eps /projects/work/Tesis/My_thesis/Analysis/Figures_HtX/CorrMat_HtX.eps

cp -f /nfs/atlas-data06/scratch/jmontejo/RooStat_files/RootStat_PP_v2/{other,detector}UNCljets_tesis.eps /projects/work/Tesis/My_thesis/Analysis/Figures_ttH
cp -f /nfs/atlas-data06/scratch/jmontejo/RooStat_files/RootStat_PP_v2/results_AllSyst_lepjets_8TeV_PP_ttH_Dic09_unblinding_qcdsmooth_TRFscaled/Reduced.eps /projects/work/Tesis/My_thesis/Analysis/Figures_ttH/CorrMat_ttH.eps
cp -f /nfs/atlas-data06/scratch/jmontejo/RooStat_files/RootStat_PP_v2/eps-files/Dic09_AllSyst_lepjets_qcdsmooth_TRFscaled_pulls_125.eps /projects/work/Tesis/My_thesis/Analysis/Figures_ttH
cp -f /nfs/atlas-data06/scratch/jmontejo/RooStat_files/RootStat_PP_v2/{other,detector}UNCthreefit.eps /projects/work/Tesis/My_thesis/Analysis/Figures_ttH
cp -f /nfs/atlas-data06/scratch/jmontejo/RooStat_files/RootStat_Stop2/{other,detector}UNCtesis_stop2.eps /projects/work/Tesis/My_thesis/Analysis/Figures_stop2
cp -f /nfs/atlas-data06/scratch/jmontejo/RooStat_files/RootStat_Stop2/results_AllSyst_Job_Stop2_500_Stop1_300_LSP_120_BrH1_/Reduced.eps /projects/work/Tesis/My_thesis/Analysis/Figures_stop2/CorrMat_stop2.eps
cp -f /nfs/atlas-data06/scratch/jmontejo/RooStat_files/RootStat_PP_v2/{other,detector}UNCthreeanalyses.eps  /projects/work/Tesis/My_thesis/Analysis/Figures_stop2/
