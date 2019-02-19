## Outline for the Manual
 
1. [**Background**](#1.-Background) : Introduction to thermal conductivity, $\kappa$ of materials
2. [**NEMD simulations**](#2.-NEMD-Simulations) : Calculation of $\Large{\kappa}$ using molecular dynamics simulations
3. [**System size scaling**](#3.-System-size-scaling) : Effect of simulation cell size on calculated $\kappa$
4. [**Temperature scaling**](#4.-Temperature-scaling) : Effect of system temperature on calculated $\kappa$
5. [**Thermal conductivity plugins**](#5.-Thermal-conductivity-plugins) : Calculating $\kappa$ of MoS<sub>2</sub>
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


* Thermal conductivity (denoted by the symbol $\kappa$) is a fundamental property of materials that determines their ability to conduct (i.e. transmit) heat (See Refs 1,2). Materials with a higher $\kappa$ values conduct heat well and low-$\kappa$ materials are more insulating (Ref 9.).

<div align=center>
<h4>Table 1: Thermal conductivity of common materials</h4>
</div>

| Material       | $\huge{\kappa}$ (W/m-K) |
|----------------|:-------------------------------------:|
| Diamond        | 1000                                  |
| Silver         | 406                                   |
| Copper         | 401                                   |
| Water          | 0.591                                 |
| Wood           | 0.12                                  |
| Wool           | 0.0464                                |
| Air            | 0.025                                 |
| Silica Aerogel | 0.003                                 |


* High-$\kappa$ materials are commonly used in heat-sink and thermal-dissipation applications and materials with low thermal conductivity are used primarily for insulation. Low $\kappa$ insulating materials are also used for thermoelectric energy harvesting applications (See [Section 8](#8.-Current-research-applications)).

* Both high and low $\kappa$ materials are extremely useful for engineers. Below, we see two demonstrations of the extremely-low $\kappa$ materials used as thermal insulators on the Space Shuttle during re-entry (Refs. 10,11).
