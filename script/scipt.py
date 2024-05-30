import matplotlib.pyplot
import pandas as pd
from matplotlib import pyplot as plot
from typing import List, Tuple
import os
from collections import defaultdict
import argparse

results = defaultdict(defaultdict)

def generate_csv(path: str, csv_filename: str,filter:List[Tuple[str, str]]) -> None:
    
    if len(filter) == 0:
        filter_str = ""
        
    if len(filter) >= 1:
                
        accumulated_filters = f"{filter[0][0]}=~\"{filter[0][1]}\""
        
        for i in range(1, len(filter)):
            
            accumulated_filters += f" AND {filter[i][0]}=~\"{filter[i][1]}\""
        
        filter_str = f"--filter '{accumulated_filters}'"
        
    
    print(f"opp_scavetool export {filter_str} {path} -F CSV-S -o {csv_filename}")
    os.system(f"opp_scavetool export {filter_str} {path} -F CSV-S -o {csv_filename}")

def filter_csv() -> tuple[list, list]:

    df = pd.read_csv("out.csv")
    custWaitingTimePerCashier =  list()
    index = list()
    i  = 0
    while True:
        filter_df = pd.to_numeric( df['value'])
        if filter_df.empty: 
            break
        custWaitingTimePerCashier.append( filter_df.mean())
        index.append(i)
        i+=1


    return index, custWaitingTimePerCashier

    

def iterate_over_n():
    for i in range(1, 11):
        for j in range(0, 10):
            
            # 10 does not have fracrions
            if(i == 10 and j != 0):
                continue
            
            n = str(i)
            if j != 0:
                n += "." + str(j)
            
            evaluate_for_fixed_n(n)
            
            
            

def evaluate_for_fixed_n(n: str):
    allMetrics = ["totalQueueNonEmptyTime:sum", "totalQueueEmptyTime:sum", "meanQueueFillLevel:mean", "meanQueueWaitingTime:mean", "meanQueueInterArrivalTime:mean", "meanServiceUnitFillLevel:mean", "meanServiceUnitWaitingTime:mean"]
    for elem in allMetrics:
        n_float = float(n)
        os.system("pwd")
        generate_csv(f"simulations/results/General-N={n}-#*.sca", f"csv/out{n}.csv", [("name", elem)])  
        df = pd.read_csv(f"csv/out{n}.csv")
        filter_df = pd.to_numeric( df['value'])
        print(filter_df.mean())
        results[elem][n] = filter_df.mean()
    

def main():
    iterate_over_n()
    # evaluate_for_fixed_n("1.1")
    # evaluate_for_fixed_n("9")
    print(results)
    pass

def process_scalar_data():
    allMetrics = ["serviceTimePerCashier:sum", "idleTimePerCashier:sum","custTotalWaitingTime:sum", "custMeanWaitingTime:mean"]
    for index, name in zip(range(len(allMetrics)),allMetrics): 
        plot.figure(index)
        plot.title(name)

        generate_csv("results/*true*.sca", "out.csv", [("name", name)])
        index, data = filter_csv()
        bar_width = 0.25
            
        plot.bar(index,data, label="use random strategy", width=bar_width)
            
        generate_csv("results/*false*.sca", "out.csv", [("name", name)])
        index, data = filter_csv()
        plot.bar([e+bar_width for e in index ] ,data, label="use smallest queue first", width=bar_width)
            
        plot.legend()
    plot.show()

def process_vector_data():
    
    allMetrics = ["queueSizeOverTime:vector", "customerWaitingTime:vector"]
    for index, name in zip(range(len(allMetrics)),allMetrics): 
        plot.figure(index)
        plot.title(name)

        generate_csv("results/*true*.vec", "out.csv", [("name", name)])
        
        generate_csv("results/*false*.vec", "out.csv", [("name", name)])
     
    
    
if __name__ == '__main__':
    main()
    
    
    
    

    
