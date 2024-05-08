import matplotlib.pyplot
import pandas as pd
from matplotlib import pyplot as plot
from typing import List, Tuple
import os
import argparse


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
        filter_df = pd.to_numeric( df[df.module == f'Supermarket.cashiers[{i}]']['value'])
        if filter_df.empty: 
            break
        custWaitingTimePerCashier.append( filter_df.mean())
        index.append(i)
        i+=1


    return index, custWaitingTimePerCashier    

if __name__ == '__main__':
    
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--path", type=str, default="*.sca")
    args = parser.parse_args()
    generate_csv("*true*.sca", "out.csv", [("name", "custMeanWaitingTime:mean")])
    index, data = filter_csv()
    bar_width = 0.25
    
    
    plot.bar(index,data, label="true", width=bar_width)
    
    generate_csv("*false*.sca", "out.csv", [("name", "custMeanWaitingTime:mean")])
    index, data = filter_csv()
    plot.bar([e+bar_width for e in index ] ,data, label="false", width=bar_width)
    plot.show()

    
