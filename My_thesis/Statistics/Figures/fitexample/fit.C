{
  gROOT->ProcessLine(".L FitCrossCheckForLimits.C+");
  gROOT->ProcessLine("LimitCrossCheck::PlotFitCrossChecks(\"toy_combined_fit_model.root\") ");
  gROOT->ProcessLine(".q");
}
