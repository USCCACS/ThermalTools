import matplotlib.pyplot as plt # Plotting package
import numpy as np # For basic math 
import pandas as pd # For reading temperature dump files; faster processing than usual read
import linecache # Traversing to specific linenumbers in the file without opening explicitly; faster
#import os 
#import subprocess
#import shlex
import sys # For command line args

plt.style.use('ggplot') # R style plots

# Class that contains all the data members and member functions required for calculating thermal gradient and Thermal Conductivity
class TC:
    def __init__(self, ip_val = {}, temp_data = pd.DataFrame()):
        self.ip_val = ip_val # Values from the input files stored as a dict
        self.temp_data = temp_data # Values from the Temperature LAMMPS Dump file stored as a pandas df
        self.meanTC = 0.0
        self.stddevTC = 0.0
        self.meaninvTC = 0.0
        self.stddevinvTC = 0.0
        self.boxsize = 0
    
    @classmethod # Used to instantiate the class
    def read_data(cls, inputfile, tempdata):
        input_values = {}
        with open(inputfile, 'r') as fin:
            for line in fin:
                data = line.split()
                try :
                    input_values.update({data[0]:float(data[1])})
                except ValueError:
                    input_values.update({data[0]:data[1]})
        fin.close()
        # Area in Amstrong^2
        input_values.update({'Area': input_values['height']*input_values['width']})
        # how frequently heat is added in ps
        input_values.update({'delT' : input_values['frequency']*input_values['dtstep']})
        # heatflux = heat in eV/(time(ps)*Area)
        input_values.update({'heatflux' : -input_values['energy']/(2.0*input_values['Area']*input_values['delT'])})
        # conversion from eV to Watt
        input_values.update({'evtoWatt' : 1.60217733*1000})
        #return input_values

        c = int(linecache.getline(tempdata, 4).split()[1]) + 1
        hot = ((c-1)/4) + input_values['nbin_skip']
        cold = ((c-1)*3/4) - input_values['nbin_skip']
        df = pd.read_csv(tempdata, delim_whitespace = True, comment = '#', header = None, names = ['Bin', 'Coord', 'Ncount', 'v_atemp'], chunksize = c, dtype = float)
        TC1 = cls(input_values, df)
        slope = []
        df1  = pd.DataFrame()
        for chunk in TC1.temp_data:
            chunk = chunk.dropna()
            chunk = chunk.reset_index(drop = True) 
            TC1.boxsize = round(chunk.at[chunk.shape[0]-1, 'Coord'] - chunk.at[0, 'Coord'] )
            fit_region = [chunk.loc[hot:cold]['Coord'].values, chunk.loc[hot:cold]['v_atemp'].values]
            fit=np.polyfit(fit_region[0],fit_region[1],1)
            fit_fn=np.poly1d(fit)
            slope.append(fit[0])
            df1 = pd.concat((df1, chunk))
        by_row_index = df1.groupby(df1.index)
        df_means = by_row_index.mean()
        K = [TC1.ip_val['evtoWatt']*TC1.ip_val['heatflux']/i for i in slope]    
        Kinv = [1.0/i for i in K]
        print Kinv
        TC1.meanTC = np.mean(K)
        TC1.stddevTC = np.std(K)
        TC1.meaninvTC = np.mean(Kinv)
        TC1.stddevinvTC = np.std(Kinv)
        print 'Mean Thermal Conductivity of ',TC1.meanTC ,'\n', 'Standard Deviation in Thermal Conductivity is ',TC1.stddevTC, '\n'
        print 'Mean Inverse Thermal Conductivity is', TC1.meaninvTC, '\n', 'Standard Deviation in Inverse Thermal Conductivity is', TC1.stddevinvTC, '\n'
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
        #plt.show()
        if(TC1.ip_val['plt_op'] == 'display'):
            plt.show()
        elif(TC1.ip_val['plt_op'] == 'png'):
            plt.savefig('TC' + str(int(TC1.boxsize)) + '.png')
        return ((TC1.meaninvTC, TC1.stddevinvTC),  1/TC1.boxsize), ((TC1.meanTC, TC1.stddevTC), TC1.ip_val['temperature']) 

if(__name__ == '__main__'):
    scaling_param = {}
    with open('scaling_params.txt', 'r') as fin:
        for line in fin:
            data = line.split()
            try :
                scaling_param.update({data[0]:float(data[1])})
            except ValueError:
                scaling_param.update({data[0]:data[1].split(',')})
    print scaling_param
    def plotscaling(X = [], Y = [], yerror = [], title = [], xlabel = [], ylabel = [], flag = 'png', scalingtype = 'length'):
        fig = plt.figure(figsize=(10,8))
        plt.title(title, color = 'r', fontsize=20)
        plt.xlabel(xlabel, fontsize=14)
        plt.ylabel(ylabel, fontsize=14)
        plt.xticks(fontsize = 14)
        plt.yticks(fontsize = 14)
        plt.errorbar(X, Y, yerr = yerror)
        if(flag == 'display'):
            plt.show()
        elif(flag == 'png'):
            plt.savefig(scalingtype + 'Scaling' + '.png')

    if(not(scaling_param['L_scaling']) and not(scaling_param['T_scaling'])):
        plot = TC.read_data(scaling_param['default_ip'][0], scaling_param['default_Tdump'][0])
    lscaling = []

    if(scaling_param['L_scaling']):
        for i in zip(scaling_param['L_scaling_ip'], scaling_param['L_scaling_Tdump']):
            lscaling.append(TC.read_data(i[0],i[1]))
        plotscaling([i[0][1] for i in lscaling], [i[0][0][0] for i in lscaling],[i[0][0][1] for i in lscaling], 'Length Scaling', 'Inverse Length(x10$\ \mu m^{-1}$)', 'Inverse Thermal Conductivity(mK/W)')
    
    tscaling = []
    if(scaling_param['T_scaling']):
        for i in zip(scaling_param['T_scaling_ip'], scaling_param['T_scaling_Tdump']):
            tscaling.append(TC.read_data(i[0],i[1]))
        plotscaling([i[1][1] for i in tscaling], [i[1][0][0] for i in tscaling],[i[1][0][1] for i in tscaling], 'Temperature Scaling', 'Temperature($K$)', 'Thermal Conductivity(W/mK)', scalingtype = 'Temperature')

    
    
    
    
    #yo, yo1 = TC.read_data('input.txt', 'Temperature.txt')
    #print yo 
    #print yo1
