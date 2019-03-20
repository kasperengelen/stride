
import pandas as pd
from math import sqrt

def determine_stats():
    filenames= [
        "./data/regionA_stan_r0=11_VC=0.8_SR=0.00000167_cnt=500_days=500.csv",
        "./data/regionB_stan_r0=11_VC=0.8_SR=0.00000167_cnt=500_days=500.csv",
        "./data/regionA_stan_r0=11_VC=0.8_SR=0.00000167_cnt=100_days=500.csv",
        "./data/regionB_stan_r0=11_VC=0.8_SR=0.00000167_cnt=100_days=500.csv"
    ]


    for filename in filenames:

        df = pd.read_csv(filename)

        # retrieve last row
        last_row = df.tail(1)
        last_row_t = last_row.transpose()

        # determine amount > 50000, < 50000
        total = int(last_row_t.count())
        outbreaks = int(last_row_t[last_row_t > 50000].count())
        extinctions = int(last_row_t[last_row_t <= 50000].count())

        # print
        print("Results for {}".format(filename))
        print("Total simulations: {}".format(total))
        print("Outbreaks: {}, frac: {}".format(outbreaks, outbreaks/total))
        print("Extinctions: {}, frac: {}".format(extinctions, extinctions/total))
        print("Interval: {}".format(calc_interval_95(outbreaks/total, total)))
        print("-----------------")
# ENDFUNCTION

def calc_interval_95(avg_pct, count):
    # we take the distance between (1-alpha/2) and avg=0
    # then rescaling using stdev=sqrt((avg_pct * (1-avg_pct)) / count)
    # then shift by avg_pct
    
    lower  = avg_pct - 1.96*sqrt((avg_pct * (1-avg_pct)) / count)
    higher = avg_pct + 1.96*sqrt((avg_pct * (1-avg_pct)) / count)

    return "[{}, {}]".format(lower, higher)
# ENDFUNCTION
