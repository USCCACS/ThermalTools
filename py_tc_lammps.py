import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import linecache
import os
import subprocess
import shlex
import sys

plt.style.use('ggplot')
# readinput() reads the input file containing information on parameters required to
# calculate thermal conductivity

def readinput(file1):
    input_values = {}
    with open(file1, 'r') as fin:
        for line in fin:
            data = line.split()
            input_values.update({data[0]:float(data[1])})
    fin.close()
    # Area in Amstrong^2
    input_values.update({'Area': input_values['height']*input_values['width']})
    # how frequently heat is added in ps
    input_values.update({'delT' : input_values['frequency']*input_values['dtstep']})
    # heatflux = heat in eV/(time(ps)*Area)
    input_values.update({'heatflux' : -input_values['energy']/(2.0*input_values['Area']*input_values['delT'])})
    # conversion from eV to Watt
    input_values.update({'evtoWatt' : 1.60217733*1000})
    return input_values

# cal_tc() calculates thermal conductivity value
# given the temperature profile and the parameters from
# readinput() as arguments

def cal_tc(tempdata, i_p):
    c = int(linecache.getline(tempdata, 4).split()[1]) + 1
    hot = ((c-1)/4) + 3
    cold = ((c-1)*3/4) - 3
    df = pd.read_csv(tempdata, delim_whitespace = True, comment = '#', header = None, names = ['Bin', 'Coord', 'Ncount', 'v_atemp'], chunksize = c, dtype = float)
    slope = []
    df1  = pd.DataFrame()
    for chunk in df:
        chunk = chunk.dropna()
        chunk = chunk.reset_index(drop = True) 
        boxsize = round(chunk.at[chunk.shape[0]-1, 'Coord'] - chunk.at[0, 'Coord'] )
        fit_region = [chunk.loc[hot:cold]['Coord'].values, chunk.loc[hot:cold]['v_atemp'].values]
        fit=np.polyfit(fit_region[0],fit_region[1],1)
        fit_fn=np.poly1d(fit)
        slope.append(fit[0])
        df1 = pd.concat((df1, chunk))
    by_row_index = df1.groupby(df1.index)
    df_means = by_row_index.mean()
    K = [i_p['evtoWatt']*i_p['heatflux']/i for i in slope]    
    Kinv = [1/i for i in K]
    K_mean = np.mean(K)
    K_std = np.std(K)
    Kinv_mean = np.mean(Kinv)
    Kinv_std = np.std(Kinv)
    print('Mean Thermal Conductivity is', K_mean,'\n', 'Standard Deviation in Thermal Conductivity is ',K_std, '\n')
    print('Mean Inverse Thermal Conductivity is', Kinv_mean, '\n', 'Standard Deviation in Inverse Thermal Conductivity is', Kinv_std, '\n')
    fit_region = [df_means.loc[hot:cold]['Coord'].values, df_means.loc[hot:cold]['v_atemp'].values]
    fit=np.polyfit(fit_region[0],fit_region[1],1)
    fit_fn=np.poly1d(fit)
    df_means.plot('Coord', 'v_atemp', linewidth = 2.0, figsize=(10,8))
    plt.plot(fit_region[0],fit_region[1], linewidth = 3.0)
    plt.plot(fit_region[0],fit_region[1],'yo',fit_region[0],fit_fn(fit_region[0]),"--k",linewidth=2.0)
    plt.xticks(fontsize=14)
    plt.yticks(fontsize=14)
    plt.title('Temperature Gradient', color = 'r', fontsize=20)
    plt.ylabel('Temperature(K)', fontsize=14)
    plt.xlabel('Distance ($\AA$)', fontsize=14)
    plt.show()
    return [Kinv_mean, 1/boxsize]
