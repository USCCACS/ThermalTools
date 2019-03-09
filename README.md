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
7. [**Summary**](#7.-Summary) : Things to remember when you do your own thermal conductivity simulations
8. [**Current research applications**](#8.-Current-research-applications) : Engineering thermal conductivity of materials. Brief look at thermal conductivity of fractal and alloyed systems
9. [**References**](#9.-References)

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

As described previously, thermal conductivity is calculated by measuring the temperature gradient along the material. We establish the thermal gradient by adding and removing a predefined quantity of heat, E<sub>0</sub>, at <a href="https://www.codecogs.com/eqnedit.php?latex=x&space;=&space;\frac{L}{4}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?x&space;=&space;\frac{L}{4}" title="x = \frac{L}{4}" /></a> and <a href="https://www.codecogs.com/eqnedit.php?latex=x&space;=&space;\frac{3L}{4}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?x&space;=&space;\frac{3L}{4}" title="x = \frac{3L}{4}" /></a> respectively. Since we can control the amount, E<sub>0</sub> and frequency of heat input, <a href="https://www.codecogs.com/eqnedit.php?latex=\frac{1}{\Delta&space;t}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\frac{1}{\Delta&space;t}" title="\frac{1}{\Delta t}" /></a> , we effectively control the heat flux in the system. Once the steady-state temperature profile is established in the simulation cell, we combine the temperature profile with the known heatflux to calculate the thermal conductivity.

Specifically,

Heat Flux, <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\displaystyle&space;J_y&space;=&space;\frac{1}{2}&space;\frac{E_0}{\Delta&space;t}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\displaystyle&space;J_y&space;=&space;\frac{1}{2}&space;\frac{E_0}{\Delta&space;t}" title="\large \displaystyle J_y = \frac{1}{2} \frac{E_0}{\Delta t}" /></a>

The factor <a href="https://www.codecogs.com/eqnedit.php?latex=\frac{1}{2}$" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\frac{1}{2}$" title="\frac{1}{2}$" /></a> comes from the fact that heat conduction happens along both the +x and -x directions away from the heat source at <a href="https://www.codecogs.com/eqnedit.php?latex=x&space;=&space;\frac{L}{4}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?x&space;=&space;\frac{L}{4}" title="x = \frac{L}{4}" /></a>.

Also, from Fourier's law of thermal conduction, we have <a href="https://www.codecogs.com/eqnedit.php?latex=\displaystyle&space;J_y&space;=&space;-&space;\kappa&space;\cdot&space;A&space;\cdot&space;\frac{\mathrm{d}T}{\mathrm{d}x}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\displaystyle&space;J_y&space;=&space;-&space;\kappa&space;\cdot&space;A&space;\cdot&space;\frac{\mathrm{d}T}{\mathrm{d}x}" title="\displaystyle J_y = - \kappa \cdot A \cdot \frac{\mathrm{d}T}{\mathrm{d}x}" /></a>, where A is the cross sectional area of heat transfer. (Ref 6)

Putting these equations together, we have <a href="https://www.codecogs.com/eqnedit.php?latex=\displaystyle&space;\kappa&space;=&space;-&space;\frac{1}{2\&space;A}&space;\frac{E_0}{\Delta&space;t}&space;\frac{\mathrm{d}x}{\mathrm{d}T}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\displaystyle&space;\kappa&space;=&space;-&space;\frac{1}{2\&space;A}&space;\frac{E_0}{\Delta&space;t}&space;\frac{\mathrm{d}x}{\mathrm{d}T}" title="\displaystyle \kappa = - \frac{1}{2\ A} \frac{E_0}{\Delta t} \frac{\mathrm{d}x}{\mathrm{d}T}" /></a>

Note here that the thermal conductivity <a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;\kappa" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large&space;\kappa" title="\large \kappa" /></a>  goes as the _inverse_ of the temperature gradient. More conducting material will have 'flatter' temperature profiles.

<div align=center>
<h4>Image 1: Schematic of NEMD simulations for measuring thermal conductivity of 2D materials</h4>
<img src = "./Media/calculation_schematic.png"  width="500" height="250">
</div>

# 3. System size scaling

## Thermal conductivity is affected by size of the system in NEMD simulations
Length Scaling is a direct consequence because of MD. The thermal transport in NEMD happens over two square regions. Phonons having mean free path largest than the size of the square do not take part in thermal transport in our system setup. As the system increases in size, more and more phonons are being involved in thermal transport. Studies have shown the mean free path for Transition metal Dichalcogenides is of the order of 1000 nm \cite{gandi2016thermal}. It is therefore essential to scale systems to at least one third its value.

These simulations are performed inside the Casimir Limit (See Ref.13). This results in thermal conductivity values having a strong size dependence. Following the scaling scheme of Schon which uses Matthiessen's Rule . To predict the thermal conductivity of the infinite system we make use of a linear function such as

<a href="https://www.codecogs.com/eqnedit.php?latex=\frac{1}{\kappa}&space;\propto&space;\frac{1}{\kappa_0}&space;\bigg(&space;\frac{1}{L}&space;&plus;&space;\frac{1}{L_0}&space;\bigg)" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\frac{1}{\kappa}&space;\propto&space;\frac{1}{\kappa_0}&space;\bigg(&space;\frac{1}{L}&space;&plus;&space;\frac{1}{L_0}&space;\bigg)" title="\frac{1}{\kappa} \propto \frac{1}{\kappa_0} \bigg( \frac{1}{L} + \frac{1}{L_0} \bigg)" /></a>
Here, <a href="https://www.codecogs.com/eqnedit.php?latex=\frac{1}{L}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\frac{1}{L}" title="\frac{1}{L}" /></a> is the inverse scattering length relating to the heat source-sink distance and <a href="https://www.codecogs.com/eqnedit.php?latex=\frac{1}{L_0}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\frac{1}{L_0}" title="\frac{1}{L_0}" /></a> is the material's inverse scattering length because of phonon phonon scattering. 

It is important to note that length scaling happens in two dimensions. It is important to maintain a 2:1 ratio between length and width while increasing system size. The major difference is that one dimensional scaling ignores the contribution of phonons with wavelengths larger than the smallest dimension of the solid, whereas two dimensional scaling is inclusive.

# 4. Temperature scaling


## Thermal conductivity is affected by the temperature of the system


The system temperature reflects the population of phonons in the material. At higher temperatures, there is a greater chance of phonon scattering (by other phonons) leading to reduced thermal conductivity.

# 6. Quantum corrections

## Going beyond classical thermal conductivity simulations

<h4>Image 4: Experimentally-measured thermal conductivity of two bulk semiconductors, Silicon and Germanium. Taken from Glassbrenner, C. J. and G. A. Slack, Phys. Rev. 134, 4A (1964) A1058-A1069</h4>
<div align=center>
<img src = "./Media/expt_thermal_conductivity.png" width="500" height="250">
</div>

The main discrepancy arises because classical molecular dynamics simulations (like ours) do not correctly describe the specific heat of materials (denoted by C<sub>V</sub>) at low temperatures. There is a method to correct for this deficiency by calculating the quantum-mechanically accurate specific heat of materials separately using equilibrium molecular dynamics simulations. This involves computing the velocity autocorrelation functions, the vibrational density of states and the specific heat, in that order, from the MD simulations, as shown below.


<div align=center>
<h4>Image 5: Workflow for calculating specific heat from equilibrium MD simulations</h4>
<img src = "./Media/cv_workflow.png" width="800" height="250">
</div>

### Velocity AutoCorrelation Functions (VACF) and Vibrational Density Of States (VDOS)

The velocity autocorrelation function Z(t) for atom type <a href="https://www.codecogs.com/eqnedit.php?latex=\alpha" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\alpha" title="\alpha" /></a> is defined as <a href="https://www.codecogs.com/eqnedit.php?latex=\displaystyle&space;Z_{\alpha}(t)&space;=&space;\frac{1}{N}\sum_{i=1}^N{\frac{v_i(t)\cdot&space;v_i(0)}{v_i(0)\cdot&space;v_i(0)}}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\displaystyle&space;Z_{\alpha}(t)&space;=&space;\frac{1}{N}\sum_{i=1}^N{\frac{v_i(t)\cdot&space;v_i(0)}{v_i(0)\cdot&space;v_i(0)}}" title="\displaystyle Z_{\alpha}(t) = \frac{1}{N}\sum_{i=1}^N{\frac{v_i(t)\cdot v_i(0)}{v_i(0)\cdot v_i(0)}}" /></a>


The density of states is given by the equation
<br>
<a href="https://www.codecogs.com/eqnedit.php?latex=G(\omega)&space;=&space;\displaystyle&space;\sum_{\alpha}&space;\frac{6N_\alpha}{\pi}&space;\displaystyle\int\limits_{0}^{\infty}&space;Z_{\alpha}(t)cos(\omega&space;t)dt" target="_blank"><img src="https://latex.codecogs.com/svg.latex?G(\omega)&space;=&space;\displaystyle&space;\sum_{\alpha}&space;\frac{6N_\alpha}{\pi}&space;\displaystyle\int\limits_{0}^{\infty}&space;Z_{\alpha}(t)cos(\omega&space;t)dt" title="G(\omega) = \displaystyle \sum_{\alpha} \frac{6N_\alpha}{\pi} \displaystyle\int\limits_{0}^{\infty} Z_{\alpha}(t)cos(\omega t)dt" /></a>
<br>
Here Z<sub><a href="https://www.codecogs.com/eqnedit.php?latex=\alpha" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\alpha" title="\alpha" /></a></sub> is the velocity autocorrelation for each element. G(<a href="https://www.codecogs.com/eqnedit.php?latex=\omega" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\omega" title="\omega" /></a>) is the density of states. If you observe the following equation you can see that the vibrational density of state is simply proportional to the Fourier transform of the velocity autocorrelation. 
<br>


With the density of states available, we can calculate specific heat using the following equation. This simply multiplies the vibrational energy of each state with the temperature derivative of the probability of occupation of each vibrational state.


<a href="https://www.codecogs.com/eqnedit.php?latex=\displaystyle\frac{C_v&space;(T)}{3Nk_B}&space;=&space;\frac{\displaystyle\int\limits_{0}^{\infty}\frac{u^2&space;e^u}{{(e^u&space;-&space;1)}^2}G(\omega)d\omega}{\displaystyle\int\limits_{0}^{\infty}&space;G(\omega)d\omega}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\displaystyle\frac{C_v&space;(T)}{3Nk_B}&space;=&space;\frac{\displaystyle\int\limits_{0}^{\infty}\frac{u^2&space;e^u}{{(e^u&space;-&space;1)}^2}G(\omega)d\omega}{\displaystyle\int\limits_{0}^{\infty}&space;G(\omega)d\omega}" title="\displaystyle\frac{C_v (T)}{3Nk_B} = \frac{\displaystyle\int\limits_{0}^{\infty}\frac{u^2 e^u}{{(e^u - 1)}^2}G(\omega)d\omega}{\displaystyle\int\limits_{0}^{\infty} G(\omega)d\omega}" /></a>
where <a href="https://www.codecogs.com/eqnedit.php?latex=\displaystyle&space;u&space;=&space;\frac{\hbar\omega}{k_B&space;T}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\displaystyle&space;u&space;=&space;\frac{\hbar\omega}{k_B&space;T}" title="\displaystyle u = \frac{\hbar\omega}{k_B T}" /></a>

### Major steps involved in VACF <a href="https://www.codecogs.com/eqnedit.php?latex=\rightarrow" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\rightarrow" title="\rightarrow" /></a> VDOS  <a href="https://www.codecogs.com/eqnedit.php?latex=\rightarrow" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\rightarrow" title="\rightarrow" /></a> C<sub>v</sub> calculations
 
1. **Create a relaxed system at any given temperature**  : We use a LAMMPS Script to create a dump file.
2. **Compute the VACF, VDOS and C<sub>v</sub> from the LAMMPS dump file** : Use the file `caldos.py` which does the calculation for you

The directory also has a file `caldos.py`. This is a python script that calls a C program `dos.c` that performs the VDOS calculations for us and plots the output. We won't actually be modifying that file in today's session. `dos.c` requires an input file `input.txt` that contains values for the various parameters required for the calculation. Let us take a look at that file.

It is essential to understand the variables in this file. We will talk about a few important ones.

1. `dT` refers to the time-step of the equilibrium MD simulations. 
2. `TFREQ` refers to frequency with which snapshots are written out by LAMMPS. I
3. `massMo` and `massS` are the atomic masses of Molybdenum (95.94) and Sulfur (32.065) respectively. We can change them according to the atoms in the system. 
4. `Corrlength` is the length of the trajectory over which VACF is calculated
5. `Ninitial` is the number of initial conditions for calculating VACF and `Ngap` is the time-delay between consecutive initial conditions. These two sampling parameters are described in detail below.

### Important points about N<sub>initial</sub>, N<sub>gap</sub> and Corrlength

One of the most significant improvements enabled by the thermal conductivity plugin tools is the usage of multiple initial conditions for calculation of velocity autocorrelation functions. This relies on the fact that the crystal at equilibrium 'loses memory' of its velocities after a few ps. Therefore, a single long MD trajectory can be broken into multiple overlapping independent sub-trajectories, each `Corrlength` steps long and separated from each successive sub-trajectory by `Ngap` steps. This significantly improves the sample-size for the calculation of atomic velocity correlations, often by over 1 order of magnitude.

* N<sub>Frame</sub> is the total number of frames, which is equal to the number of steps that we run the simulation for.
* We must ensure that <a href="https://www.codecogs.com/eqnedit.php?latex=\large{\left(N_{initial}\times&space;N_{gap}\right)&space;&plus;&space;Corrlength&space;\leq&space;N_{Frame}}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large{\left(N_{initial}\times&space;N_{gap}\right)&space;&plus;&space;Corrlength&space;\leq&space;N_{Frame}}" title="\large{\left(N_{initial}\times N_{gap}\right) + Corrlength \leq N_{Frame}}" /></a>

* If this condition isn't satisfied we will get an error, because the program does not get enough data

## Applying Quantum corrections to our thermal conductivity


The `caldos.py` script also calculates the temperature-dependent specific heat, C<sub>v</sub>, of the material using the density of states. This value can be used to correct our thermal conductivity value that was over-estimated at low temperatures using the equation below.


<div align=center>
<a href="https://www.codecogs.com/eqnedit.php?latex=\Large{\kappa_{Corr}&space;=&space;\displaystyle\left(\frac{C_v(T)}{3Nk_B}\right)&space;\cdot&space;\kappa_{MD}}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\Large{\kappa_{Corr}&space;=&space;\displaystyle\left(\frac{C_v(T)}{3Nk_B}\right)&space;\cdot&space;\kappa_{MD}}" title="\Large{\kappa_{Corr} = \displaystyle\left(\frac{C_v(T)}{3Nk_B}\right) \cdot \kappa_{MD}}" /></a>
    </div>
    
# 7. Summary


* NEMD is a simple method to compute lattice thermal conductivity of nanomaterials
* The thermal conductivity plugin, `calthermal_conductivity.py` helps with size-scaling and temperature-scaling calculations to obtain the material-intrinsic thermal conductivity at different temperatures
* The thermal conductivity plugin, `caldos.py` helps include quantum-mechanical corrections to computed $\kappa$ to obtain experimentally-realistic values at low temperatures

---

* Thermal conductivity simulations must be performed for a sufficiently long time (usually ~10s of ns) to ensure that a good steady state has been reached
* We must ensure good sampling (i.e. number of independent initial conditions) in the VACF and VDOS calculation for smooth curves.

# 8. Current research applications


## Thermal transport in fractal crystals

* Fractals are infinitely complex patterns that are self-similar across different scales. They are created by repeating a simple process over and over in an ongoing feedback loop driven by recursion (See Ref. 12). 


<div align=center>
<h4>Image 7: The Sierpinski carpet is a model fractal system in two-dimensions</h4>
<img src = "./Media/Animated_Sierpinski_carpet.gif" width = "200" height = "200">
</div>

* Here is a picture depicting the structure and heat flux in such a fractal system composed of MoSe<sub>2</sub> and WSe<sub>2</sub> materials.

<div align=center>
<h4>Image 8: Atomic structure and heat-flux through a fractal two-dimensional material</h4>
<img src = "./Media/frac_heatflux.png" width = "800">
</div>

* Local heat flux <a href="https://www.codecogs.com/eqnedit.php?latex=\large{\displaystyle&space;\vec{J_{at}}&space;=&space;\displaystyle&space;\mathbb{E}[\sigma.\vec{v}]_{t}}" target="_blank"><img src="https://latex.codecogs.com/svg.latex?\large{\displaystyle&space;\vec{J_{at}}&space;=&space;\displaystyle&space;\mathbb{E}[\sigma.\vec{v}]_{t}}" title="\large{\displaystyle \vec{J_{at}} = \displaystyle \mathbb{E}[\sigma.\vec{v}]_{t}}" /></a>. It is clear from the above figure that majority of the heat flux moves through the MoSe<sub>2</sub> lattice and the interface acts as a source of phonon scattering.

# 9. References

---

1. Cahill, D.G., et al., [Nanoscale thermal transport](http://aip.scitation.org/doi/10.1063/1.1524305). Journal of Applied Physics, 2003. 93(2): p. 793-818.
2. Cahill, D.G., et al., [Nanoscale thermal transport. II. 2003-2012](http://aip.scitation.org/doi/10.1063/1.4832615). Applied Physics Reviews, 2014. 1(1).
3. Payam, N. and J.S. David, [Thermal conductivity of single-layer WSe 2 by a Stillinger–Weber potential](http://iopscience.iop.org/article/10.1088/1361-6528/aa55e1/meta). Nanotechnology, 2017. 28(7): p. 075708.
4. Sahoo, S., et al., [Temperature-Dependent Raman Studies and Thermal Conductivity of Few-Layer MoS2](https://pubs.acs.org/doi/10.1021/jp402509w). Journal of Physical Chemistry C, 2013. 117(17): p. 9042-9047.
5. Yan, R.S., et al., [Thermal Conductivity of Monolayer Molybdenum Disulfide Obtained from Temperature-Dependent Raman Spectroscopy.](https://pubs.acs.org/doi/10.1021/nn405826k) ACS Nano, 2014. 8(1): p. 986-993.
6. Wu, X.F., et al., [How to characterize thermal transport capability of 2D materials fairly? - Sheet thermal conductance and the choice of thickness.](https://www.sciencedirect.com/science/article/pii/S0009261416310193) Chemical Physics Letters, 2017. 669: p. 233-237.
7. Klemens, P.G., [Thermal Conductivity and Lattice Vibrational Modes.](https://www.sciencedirect.com/science/article/pii/S0081194708605512) Solid State Physics, 1958. 7: p. 1-98.
8. Thermal Conductivity Wiki page: https://en.wikipedia.org/wiki/Thermal_conductivity
9. Fourier's Law:  https://en.wikipedia.org/wiki/Thermal_conduction
10. Thermal Insulation in a Space Shuttle: https://www.nasa.gov/sites/default/files/atoms/files/shuttle_tiles_5_8v2.pdf
11. Space Shuttle thermal protection system: https://en.wikipedia.org/wiki/Space_Shuttle_thermal_protection_system
12. Sierpinski carpet: https://en.wikipedia.org/wiki/Sierpinski_carpet
13. HBG Casimir. [Note on the conduction of heat in crystals](https://www.sciencedirect.com/journal/physica/vol/5/issue/6) Physica, 5(6):495-500, 1938
---

<div align=right><a href="#Outline-for-this-hands-on-session">[^Top]</a></div>
