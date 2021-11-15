import pyhf
pyhf.set_backend("numpy")
import math

def significance(sig, bkg, bkgunc):
    model = pyhf.simplemodels.hepdata_like(
        signal_data=sig, bkg_data=bkg, bkg_uncerts=bkgunc
    )
    observations = [s+b for s,b in zip(sig,bkg)]
    data = pyhf.tensorlib.astensor(observations + model.config.auxdata)
    test_mu = 0.0
    init_pars = model.config.suggested_init()
    par_bounds = model.config.suggested_bounds()
    fixed_params = model.config.suggested_fixed()
    q0 = pyhf.infer.test_statistics.q0(test_mu, data, model, init_pars, par_bounds, fixed_params)
    return math.sqrt(q0)
