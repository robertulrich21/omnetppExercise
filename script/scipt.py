import matplotlib.pyplot
import pandas as pd
from matplotlib import pyplot as plot
from typing import List, Tuple
import os
from collections import defaultdict


results_sca = defaultdict()
results_vec = pd.DataFrame()
parameter_values_str = []
allMetrics = ["totalQueueNonEmptyTime:sum", "totalQueueEmptyTime:sum", "meanQueueFillLevel:mean", "meanQueueWaitingTime:mean", "meanQueueInterArrivalTime:mean", "meanServiceUnitFillLevel:mean", "meanServiceUnitWaitingTime:mean"]
# generates all parametervalues to generate file names
for i in range(1, 11):
    for j in range(0, 10):
            
        # 10 does not have fractions
        if(i == 10 and j != 0):
                continue
            
        n = str(i)
        if j != 0:
            n += "." + str(j)
        parameter_values_str.append(n)


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


def evaluate_scalar_data(n):
    for elem in allMetrics:
        generate_csv(f"simulations/results/General-N={n}-#*.sca", f"csv/{elem}_par:{n}_sca.csv", [("name", elem)])  
        df = pd.read_csv(f"csv/{elem}_par:{n}_sca.csv")
        filter_df = pd.to_numeric( df['value'])
        results_sca[elem] = filter_df.mean()    

def evaluate_vector_data(n):
    generate_csv(f"simulations/results/General-N={n}-#*.vec", f"csv/meanQueueFillLevel{n}_vec.csv", [("name", "meanQueueFillLevel:vector")])  
    df = pd.read_csv(f"csv/meanQueueFillLevel{n}_vec.csv")
    results_vec.insert(0, 'Time',df[df.columns[0]])
    d = pd.DataFrame()
    for i in range(1,len(df.columns),2):
        d = d.assign(i=df[df.columns[i]])
    results_vec.insert(1,'Values', d.mean(axis=1))
        
def show_all_scalar_data():

    x = list(range(0, len(allMetrics)))
    plot.xticks(x, allMetrics)
    for counter, m in zip(x,allMetrics):
        plot.bar(counter,results_sca[m])
    plot.show()
    
def show_vector_data():
    plot.plot(results_vec['Time'], results_vec['Values'], linestyle='-')
    plot.show()


    
def main():
    evaluate_scalar_data("1.2")
    show_all_scalar_data()
    evaluate_vector_data("1.2")
    show_vector_data()
    
if __name__ == "__main__":
    main()