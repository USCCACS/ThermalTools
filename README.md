## Outline for the Manual
 
1. [**Background**](#1.-Background) : Introduction to thermal conductivity, <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /></a> of materials
2. [**NEMD simulations**](#2.-NEMD-Simulations) : Calculation of <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /></a> using molecular dynamics simulations
3. [**System size scaling**](#3.-System-size-scaling) : Effect of simulation cell size on calculated <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /></a>
4. [**Temperature scaling**](#4.-Temperature-scaling) : Effect of system temperature on calculated <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /></a>
5. [**Thermal conductivity plugins**](#5.-Thermal-conductivity-plugins) : Calculating <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /></a> of MoS<sub>2</sub>
6. [**Quantum corrections**](#6.-Quantum-corrections) : Beyond classical thermal conductivity simulations
    * Velocity autocorrelation in equilibrium simulations
    * Phonon density of states
    * Specific heat of materials
    * Quantum-corrected thermal conductivity
7. [**Summary and take-away messages**](#7.-Summary-and-take-away-messages) : Things to remember when you do your own thermal conductivity simulations
8. [**Current research applications**](#8.-Current-research-applications) : Engineering thermal conductivity of materials. Brief look at thermal conductivity of fractal and alloyed systems
9. [**Downloads/Documentation**](#9.-Downloads-and-Documentation) : Thermal conductivity plugin downloads, Plugin manuals and contributing to software development
10. [**References**](#10.-References)

# 1. Background 


## Introduction to thermal conductivity


* Thermal conductivity (denoted by the symbol <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /></a>) is a fundamental property of materials that determines their ability to conduct (i.e. transmit) heat (See Refs 1,2). Materials with a higher <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /></a> values conduct heat well and low-<a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /></a> materials are more insulating (Ref 9.).

<div align=center>
<h4>Table 1: Thermal conductivity of common materials</h4>
</div>

| Material       | <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /> (W/m-K) |
|----------------|:-------------------------------------:|
| Diamond        | 1000                                  |
| Silver         | 406                                   |
| Copper         | 401                                   |
| Water          | 0.591                                 |
| Wood           | 0.12                                  |
| Wool           | 0.0464                                |
| Air            | 0.025                                 |
| Silica Aerogel | 0.003                                 |


* High-<a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /></a> materials are commonly used in heat-sink and thermal-dissipation applications and materials with low thermal conductivity are used primarily for insulation. Low <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /></a> insulating materials are also used for thermoelectric energy harvesting applications (See [Section 8](#8.-Current-research-applications)).

* Both high and low <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /></a> materials are extremely useful for engineers. 
# 2. NEMD Simulations

### Theory and Equations

As described previously, thermal conductivity is calculated by measuring the temperature gradient along the material. We establish the thermal gradient by adding and removing a predefined quantity of heat, E<sub>0</sub>, at <a href="https://www.codecogs.com/eqnedit.php?latex=x&space;=&space;\frac{L}{4}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?x&space;=&space;\frac{L}{4}" title="x = \frac{L}{4}" /></a> and <a href="https://www.codecogs.com/eqnedit.php?latex=x&space;=&space;\frac{3L}{4}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?x&space;=&space;\frac{3L}{4}" title="x = \frac{3L}{4}" /></a> respectively. Since we can control the amount, E<sub>0</sub> and frequency of heat input, <a href="https://www.codecogs.com/eqnedit.php?latex=\frac{1}{\Delta&space;t}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\frac{1}{\Delta&space;t}" title="\frac{1}{\Delta t}" /></a> , we effectively control the heat flux in the system. Once the steady-state temperature profile is established in the simulation cell, we combine the temperature profile with the known heatflux to calculate the thermal conductivity.

Specifically,

Heat Flux, <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\displaystyle&space;J_y&space;=&space;\frac{1}{2}&space;\frac{E_0}{\Delta&space;t}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\displaystyle&space;J_y&space;=&space;\frac{1}{2}&space;\frac{E_0}{\Delta&space;t}" title="\large \displaystyle J_y = \frac{1}{2} \frac{E_0}{\Delta t}" /></a>

The factor <a href="https://www.codecogs.com/eqnedit.php?latex=\frac{1}{2}$" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\frac{1}{2}$" title="\frac{1}{2}$" /></a> comes from the fact that heat conduction happens along both the +x and -x directions away from the heat source at <a href="https://www.codecogs.com/eqnedit.php?latex=x&space;=&space;\frac{L}{4}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?x&space;=&space;\frac{L}{4}" title="x = \frac{L}{4}" /></a>.

Also, from Fourier's law of thermal conduction, we have <a href="https://www.codecogs.com/eqnedit.php?latex=\displaystyle&space;J_y&space;=&space;-&space;\kappa&space;\cdot&space;A&space;\cdot&space;\frac{\mathrm{d}T}{\mathrm{d}x}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\displaystyle&space;J_y&space;=&space;-&space;\kappa&space;\cdot&space;A&space;\cdot&space;\frac{\mathrm{d}T}{\mathrm{d}x}" title="\displaystyle J_y = - \kappa \cdot A \cdot \frac{\mathrm{d}T}{\mathrm{d}x}" /></a>, where A is the cross sectional area of heat transfer. (Ref 6)

Putting these equations together, we have <a href="https://www.codecogs.com/eqnedit.php?latex=\displaystyle&space;\kappa&space;=&space;-&space;\frac{1}{2\&space;A}&space;\frac{E_0}{\Delta&space;t}&space;\frac{\mathrm{d}x}{\mathrm{d}T}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\displaystyle&space;\kappa&space;=&space;-&space;\frac{1}{2\&space;A}&space;\frac{E_0}{\Delta&space;t}&space;\frac{\mathrm{d}x}{\mathrm{d}T}" title="\displaystyle \kappa = - \frac{1}{2\ A} \frac{E_0}{\Delta t} \frac{\mathrm{d}x}{\mathrm{d}T}" /></a>

Note here that the thermal conductivity <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /></a>  goes as the _inverse_ of the temperature gradient. More conducting material will have 'flatter' temperature profiles.

<div align=center>
<h4>Image 1: Schematic of NEMD simulations for measuring thermal conductivity of 2D materials</h4>
<img src = "./Media/calculation_schematic.png"  width="500" height="250">
</div>
# 3. System size scaling

## Thermal conductivity is affected by size of the system in NEMD simulations
Length Scaling is a direct consequence because of MD. The thermal transport in NEMD happens over two square regions. Phonons having mean free path largest than the size of the square do not take part in thermal transport in our system setup. As the system increases in size, more and more phonons are being involved in thermal transport. Studies have shown the mean free path for Transition metal Dichalcogenides is of the order of 1000 nm \cite{gandi2016thermal}. It is therefore essential to scale systems to at least one third its value.

These simulations are performed inside the Casimir Limit \cite{casimir1938note}. This results in thermal conductivity values having a strong size dependence. Following the scaling scheme of Schon \cite{oligschleger1999simulation} which uses Matthiessen's Rule  \cite{klemens1958thermal}. To predict the thermal conductivity of the infinite system we make use of a linear function \cite{schelling2002comparison} such as

<a href="https://www.codecogs.com/eqnedit.php?latex=\frac{1}{\kappa}&space;\propto&space;\frac{1}{\kappa_0}&space;\bigg(&space;\frac{1}{L}&space;&plus;&space;\frac{1}{L_0}&space;\bigg)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\frac{1}{\kappa}&space;\propto&space;\frac{1}{\kappa_0}&space;\bigg(&space;\frac{1}{L}&space;&plus;&space;\frac{1}{L_0}&space;\bigg)" title="\frac{1}{\kappa} \propto \frac{1}{\kappa_0} \bigg( \frac{1}{L} + \frac{1}{L_0} \bigg)" /></a>
Here, <a href="https://www.codecogs.com/eqnedit.php?latex=\frac{1}{L}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\frac{1}{L}" title="\frac{1}{L}" /></a> is the inverse scattering length relating to the heat source-sink distance and $\frac{1}{L_0}$ is the material's inverse scattering length because of phonon phonon scattering. The thermal conductivity of the bulk material can be obtained by extrapolating Figure \ref{Figure2} a) and Figure \ref{Figure3} a) to zero. This gives us a value of $277.58 W/mK$ for MoSe$_2$ and $271.095 W/mK$ for WSe$_2$.


It is important to note that length scaling in this work happens in two dimensions. It is important to maintain a 2:1 ratio between length and width while increasing system size. Previous results, in contrast have reported only one dimensional scaling. The major difference is that one dimensional scaling ignores the contribution of phonons with wavelengths larger than the smallest dimension of the solid, whereas two dimensional scaling is inclusive.
