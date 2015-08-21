import array
ptbins = array.array("d",[0,50,100,150,200,250,300,400,500,600])

vars = {
        #"q1_pt"     : ("q1_pt/1000.",                       (40,0,600), "Leading b-jet p_{T} (GeV)"      ),
        #"q1_eta"    : ("abs(q1_eta)",                       (26,0,2.6), "Leading b-jet |#eta|"     ),
        #"qq_ht"     : ("qq_ht/1000.",                       (30,0,600), "HT^{bb} (GeV)"       ),
        #"qq_pt"     : ("qq_pt/1000.",                       (30,0,600), "p_{T}^{bb} (GeV)"      ),
        "qq_m"      : ("qq_m/1000.",                        (20,0,600), "M^{bb} (GeV)"      ),
        #"qq_dr"     : ("qq_dr",                             (30,0,6), "#Delta R^{bb}"        ),
        #"ttbar_pt"  : ("ttbar_pt/1000.",                    (len(ptbins)-1,ptbins) , "t#bar{t} p_{T} (GeV)"        ),
        #"top_pt"    : ("top_pt/1000.",                      (len(ptbins)-1,ptbins) , "top p_{T} (GeV)"        ),
}

categories = {
        "tt1bq": ("(floor(extHFtype/100) == 10)",2,"tt+b"),
        "tt1gbbq": ("(floor(extHFtype/100) == 1)",2,"tt+B"),
        "tt2bq": ("(floor(extHFtype/100) == 20)",2,"tt+bb"),
}

