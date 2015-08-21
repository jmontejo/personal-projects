import array
ptbins = array.array("d",[0,50,100,150,200,250,300,400,500,600])

vars = {
        "q1_pt"     : ("q1_pt/1000.",                       (40,0,600), "Leading c-jet p_{T}"      ),
        "q1_eta"    : ("abs(q1_eta)",                       (26,0,2.6), "Leading c-jet |#eta|"     ),
        "qq_ht"     : ("qq_ht/1000.",                       (30,0,600), "HT^{cc}"       ),
        "qq_pt"     : ("qq_pt/1000.",                       (30,0,600), "p_{T}^{cc}"      ),
        "qq_m"      : ("qq_m/1000.",                        (20,0,600), "M^{cc}"      ),
        "qq_dr"     : ("qq_dr",                             (30,0,6), "#Delta R^{cc}"        ),
        "ttbar_pt"  : ("ttbar_pt/1000.",                    (len(ptbins)-1,ptbins) , "t#bar{t} p_{T} (GeV)"        ),
        "top_pt"    : ("top_pt/1000.",                      (len(ptbins)-1,ptbins) , "top p_{T}"        ),
}

categories = {
        "tt1cq": ("(extHFtype == 10)",2,"tt+c"),
        "tt1gccq": ("(extHFtype == 1)",2,"tt+C"),
        "tt2cq": ("(extHFtype == 20)",2,"tt+cc"),
}

