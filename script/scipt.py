import matplotlib.pyplot
import pandas as pd
from matplotlib import pyplot as plot
from typing import List, Tuple
import os
from collections import defaultdict
import os.path
import pathlib


results_sca = defaultdict(defaultdict)
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

def generate_all_csv(path_to_folder, force_recalc = False):
    # check if csv folder exists if not create one
    pathlib.Path(path_to_folder).mkdir(parents=True, exist_ok=True)

    # run oppscavtool for all files that do not exist. If force_recalc = true then recalculate everything!    
    for n in parameter_values_str:
        if not os.path.isfile(f"{path_to_folder}/meanQueueFillLevel{n}_vec.csv") or force_recalc:
            generate_csv(f"simulations/results/General-N={n}-#*.vec", f"{path_to_folder}/meanQueueFillLevel{n}_vec.csv", [("name", "meanQueueFillLevel:vector")])  

        for elem in allMetrics:
            if not os.path.isfile(f"{path_to_folder}/{elem}_par:{n}_sca.csv") or force_recalc:
                generate_csv(f"simulations/results/General-N={n}-#*.sca", f"{path_to_folder}/{elem}_par:{n}_sca.csv", [("name", elem)])  



def evaluate_scalar_data_per_run(path_to_folder):

    # creates a list of metrics for every n
    dataframes = {}
    for n in parameter_values_str:
        total_df = pd.DataFrame()

        for elem in allMetrics:
            df = pd.read_csv(f"{path_to_folder}/{elem}_par:{n}_sca.csv")

            column_name = df['name'].iloc[0]
            # remove all useless colums and rename the value column to the name of the value it stores!
            df2 = df[["repetition", "value"]].rename({'repetition': 'id', 'value': column_name}, axis = 'columns')
            
            if total_df.empty:
                total_df = df2
            else:
                # append column to dataframe
                total_df = pd.merge(total_df, df2, on = 'id', how = 'inner')

        dataframes[n] = total_df
            # filter_df = pd.to_numeric( df['value'])
            # results_sca[elem][n] = filter_df.mean()   
    return dataframes
    

def evaluate_scalar_data_per_metric(path_to_folder):

    # creates a list of metrics for every n
    dataframes = {}
    for elem in allMetrics:
        total_df = pd.DataFrame()

        for n in parameter_values_str:
            df = pd.read_csv(f"{path_to_folder}/{elem}_par:{n}_sca.csv")

            column_name = df['N'].iloc[0]
            # remove all useless colums and rename the value column to the name of the value it stores!
            df2 = df[["repetition", "value"]].rename({'repetition': 'id', 'value': float(column_name)}, axis = 'columns')
            
            if total_df.empty:
                total_df = df2
            else:
                # append column to dataframe
                total_df = pd.merge(total_df, df2, on = 'id', how = 'inner') # really inefficient!!! making a big list would be better!!

        dataframes[elem] = total_df

    return dataframes



def evaluate_vector_data(n):
    df = pd.read_csv(f"csv/meanQueueFillLevel{n}_vec.csv")
    results_vec.insert(0, 'Time',df[df.columns[0]])
    d = pd.DataFrame()
    for i in range(1,len(df.columns),2):
        d = d.assign(i=df[df.columns[i]])
    results_vec.insert(1,'Values', d.mean(axis=1))
        
def show_all_scalar_data(n):   
    x = list(range(0, len(allMetrics)))
    plot.xticks(x, allMetrics)
    for counter, m in zip(x,allMetrics):
        plot.bar(counter,results_sca[m][n])
    plot.show()
    
def show_vector_data():
    #task3.1
    plot.plot(results_vec['Time'], results_vec['Values'], linestyle='-')
    plot.title("Task 3.1 - queue fill level vs. time ; par('interArrivalTime') = 1.2", fontsize=15)
    plot.xlabel("time in seconds", fontsize=15)
    plot.ylabel("queue fill level", fontsize=15)
    
    plot.show()

def show_task_3():
    #task3.2
    x = list()
    y = list()
    y_analytical = list()
    for par in parameter_values_str:
        
        rho = (1.0/float(par))
        if (rho != 1): 
            x.append(results_sca["meanServiceUnitWaitingTime:mean"][par]/(results_sca["meanQueueInterArrivalTime:mean"][par]))
            y.append(results_sca["meanQueueWaitingTime:mean"][par] + results_sca["meanServiceUnitWaitingTime:mean"][par])
            y_analytical.append(1/(1-rho))
        print(rho)
            
    #x,  y, y_analytical = zip(*sorted(zip(x, y, y_analytical)))
        
    plot.plot(x,y,label="Simulation")
    plot.plot(x,y_analytical,label="analytical approach")
    plot.xlabel("ρ = (1/meanQueueWaitingTime) / (1/meanServiceTime)", fontsize=15)
    plot.ylabel("averageDelay = meanQueueTime + meanServiceUnitTime", fontsize=15)
    plot.title("Task 3.2 - average delay vs. ρ", fontsize=15)
    plot.legend()
 
    
    plot.show()
    
    #3.3
    x = list()
    y = list()
    y_analytical = list()
    for par in parameter_values_str:
        x.append(1.0/(results_sca["meanQueueInterArrivalTime:mean"][par])/ (1.0/results_sca["meanServiceUnitWaitingTime:mean"][par]))
        y.append(results_sca["totalQueueNonEmptyTime:sum"][par] / (results_sca["totalQueueNonEmptyTime:sum"][par] + results_sca["totalQueueEmptyTime:sum"][par]))
        rho = (1.0/float(par))
        y_analytical.append(rho)
    
    #x,  y, y_analytical = zip(*sorted(zip(x, y, y_analytical)))

    plot.xlabel("ρ = (1/meanQueueWaitingTime) / (1/meanServiceTime)", fontsize=15)
    plot.ylabel("averageQueueUtilization = sumTotalQueueNonEmptyTime / sumTotalTime", fontsize=15)
    plot.title("Task 3.3 - average queue utilization vs. ρ", fontsize=15)
    plot.plot(x,y, label="Simulation")
    plot.plot(x,y_analytical, label="analytical approach")
    plot.legend()   
    plot.show()
    
    #3.4
    x = list()
    y = list()
    y_analytical = list()
    for par in parameter_values_str:
        x.append(1.0/(results_sca["meanQueueInterArrivalTime:mean"][par])/ (1.0/results_sca["meanServiceUnitWaitingTime:mean"][par]))
        y.append(results_sca["meanServiceUnitFillLevel:mean"][par] + results_sca["meanQueueFillLevel:mean"][par] )
        rho = (1.0/float(par))
        if (rho == 1):
            x = x[:-1]
            y = y[:-1]   
        else:   
            y_analytical.append(rho/(1-rho))
    
    #x,  y, y_analytical = zip(*sorted(zip(x, y, y_analytical)))


    plot.xlabel("ρ = (1/meanQueueWaitingTime) / (1/meanServiceTime)", fontsize=15)
    plot.ylabel("avergaeItemsInSystem=meanServiceFillLevel + meanQueueFillLevel", fontsize=15)
    plot.plot(x,y,label="Simulation")
    plot.plot(x,y_analytical,label="analytical approach")
    plot.title("Task 3.4 - average number of items in the system vs. ρ", fontsize=15)
 
    plot.legend()
    plot.show()


def add_combined_metrics(dataframes):
    start = time.time()
    for i, df in dataframes.items():
        # add the metrics on every dataset

        #1 roh for every datapoint
        # roh = meanServiceUnitWaitingTime:mean / meanQueueInterArrivalTime:mean
        df['roh'] = df['meanServiceUnitWaitingTime:mean'] / df['meanQueueInterArrivalTime:mean']
        
        #2 mean total waiting time
        # average delay = meanQueueWaitingTime:mean + meanServiceUnitWaitingTime:mean
        df['average_delay'] = df['meanQueueWaitingTime:mean'] + df['meanServiceUnitWaitingTime:mean']

        #3 average queue utilization
        # = 1 / (1 + totalQueueNonEmptyTime:sum / totalQueueNonEmptyTime:sum) 
        df['average_queue_utilization'] = df['totalQueueNonEmptyTime:sum'] / (df['totalQueueEmptyTime:sum'] + df['totalQueueNonEmptyTime:sum'])

        #4 average ammount of items in system
        # meanServiceUnitFillLevel:mean  + meanQueueFillLevel:mean
        df['average_item_in_system'] = df['meanServiceUnitFillLevel:mean'] + df['meanQueueFillLevel:mean']
    print(f"the calc took {time.time() - start}")

    
def main():
    generate_all_csv("csv/")

    dataframes = evaluate_scalar_data_per_run("csv/")
    add_combined_metrics(dataframes)
    #print(dataframes)
    
    data = {}
    data['roh'] = [x * 0.01 for x in range(0, 100)]

    theoretical_values = pd.DataFrame(data)

    theoretical_values['average_delay'] = 1 / (1 - theoretical_values['roh'])
    theoretical_values['average_queue_utilization'] = theoretical_values['roh']
    theoretical_values['average_item_in_system'] = theoretical_values['roh'] / (1 - theoretical_values['roh'])


    # crazy scatterplot for all values
    _, ax = plot.subplots()
    for i, df in dataframes.items():
        df.plot(ax = ax, x='roh', y='average_delay', style='1')

    theoretical_values.plot(ax = ax, x='roh', y='average_delay')
       
    plot.show()

    _, ax = plot.subplots()
    for i, df in dataframes.items():
        df.plot(ax = ax, x='roh', y='average_queue_utilization', style='1')
    theoretical_values.plot(ax = ax, x='roh', y='average_queue_utilization')

    plot.show()

    _, ax = plot.subplots()
    for i, df in dataframes.items():
        df.plot(ax = ax, x='roh', y='average_item_in_system', style='1')
    theoretical_values.plot(ax = ax, x='roh', y='average_item_in_system')

    plot.show()

        
    
if __name__ == "__main__":
    main()



#
#
#sorted_df = df.drop('id', axis = 1)
#        # mieser trick, dass man plots stapeln kann und dass mit roh indiyiert wird
#        sorted_df.columns = sorted_df.columns.map(lambda x: 1 / float(x))
#        tmpdf = sorted_df.mean()
#        
#        _, ax = plot.subplots()
#        # sorted_df.boxplot(ax = ax)
#        tmpdf.plot(ax = ax)
#
#        indecies = sorted_df.columns.values.tolist()
#        df_dict = {}
#        for rho in indecies:
##            match i:
##                case "totalQueueNonEmptyTime:sum":
##
##                case "totalQueueEmptyTime:sum":
##
##                case "meanQueueFillLevel:mean":
##
##                case "meanQueueWaitingTime:mean":
##
##                case "meanQueueInterArrivalTime:mean":
##
##                case "meanServiceUnitFillLevel:mean":
##
##                case "meanServiceUnitWaitingTime:mean":
##
#
#            if rho == 1:
#                continue
#            # calculate theoretical value
#            value = rho / (1 - rho)
#            df_dict[rho] = value
#        df_from_dict = pd.DataFrame(df_dict, index = [0])
#        # print(df_from_dict)
#        df_from_dict.mean().plot(ax = ax)
#        plot.show()
#
#    #show_all_scalar_data("1.2")
#    #evaluate_vector_data("1.2")
#    #show_vector_data()
#    #show_task_3()