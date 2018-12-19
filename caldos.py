import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import linecache
import os
import subprocess
import shlex
import sys
#Variables for plotting
omega = '$\omega(meV)$'
vacf_math = '$\\frac{\mathbb{E}[V_tV_0]}{\mathbb{E}[V_0V_0]}$'
time = 'Time(ps)'
dos_math = '$G(\omega)$'
dos = 'Density of States'
vacf = 'Velocity auto-correlation of atom'
cv = 'Specific Heat($C_v$)'
temp = 'Temperature(K)'

#Filenames stored in a dict for ease of access
p_files = {'VOC_typeA.txt':[time, vacf_math, vacf + ' 1'], 'VOC_typeB.txt':[time, vacf_math, vacf + ' 2'], 'PDOS_typeA.txt':[omega, dos_math, dos + ' Atom' + ' 1'], 'PDOS_typeB.txt':[omega, dos_math, dos + ' Atom' + ' 2'] }
t_files ={'Specific_heat.txt':[temp, cv, 'Specific Heat'], 'DOS_total.txt':[omega, dos_math, dos]}


def get_dos(filename):
    input="./dos"+" "+"dumpdos.nve"+" "+filename
    os.system('rm -f dos')
    os.system('gcc -lm -o dos ./dos.c')
    args=shlex.split(input)
    child_process=subprocess.Popen(args)
    child_process.wait()
    if child_process.returncode > 0 :
        print("-----error-------")
        sys.exit()
    else:
        print("----Successful-----")

def showimage(filename, label1):
    inputdata = open(filename, 'r')
    xval=list();
    yval=list();
    for lines in inputdata:
        lines = lines.replace("\n", "").split()
        xval.append(float(lines[0]))
        yval.append(float(lines[1]))
    plt.plot(xval, yval, linewidth=2.5, label = label1)
    plt.legend(fontsize = 12)
    
def plotsubplots(data, num1, num):
    for (i,j) in zip(data, range(1,num)):
        plt.subplot(num1,2,j)
        showimage(i, data[i][2])
        plt.xlabel(data[i][0], fontsize=15)
        plt.ylabel(data[i][1], fontsize=25)
        plt.xticks(fontsize = 15)
        plt.yticks(fontsize = 15)
        plt.xlim(xmin=0)
    plt.show()

get_dos('input.txt')
plt.figure(figsize=(20,8))
plotsubplots(t_files,1,3)
