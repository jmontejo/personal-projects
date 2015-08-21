import array
ptbins = array.array("d",[0,50,100,150,200,250,300,400,500,600])

vars = {
        "ttbar_pt"  : ("ttbar_pt/1000.",                    (len(ptbins)-1,ptbins) , "t#bar{t} p_{T} (GeV)"        ),
        "top_pt"    : ("top_pt/1000.",                      (len(ptbins)-1,ptbins) , "top p_{T}"        ),
}

categories = {
        "tt1bq": ("(floor(extHFtype/100) == 10)",2,"tt+b"),
        "tt1gbbq": ("(floor(extHFtype/100) == 1)",2,"tt+B"),
        "tt2bq": ("(floor(extHFtype/100) == 20)",2,"tt+bb"),
}

