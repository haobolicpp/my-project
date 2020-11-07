
# coding: utf-8

import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import odeint
from vocal_fold_model import twomass


# Parameters estimation
# Assuming the two sides are symmetric, m1 is the bottom portion and m2 is the upper portion of the vocal fold.
# m1 = 0.01 g
# m2 = 0.005 g
# The dimensions are
# d1 = 0.1 cm
# d2 = 0.05 cm
# l = 0.3 cm
# 
# k1 = 0.8 g/ms^2
# k2 = 0.08 g/ms^2
# kc = 0.25 g/ms^2
# 
# c1 = 3 * k1
# c2 = 3 * k2
# 
# [1] H. Herzel et al 1995
# [2] I. Steinecke and H. Herzel 1995
# [3] H. Koda et al 2015

# Parameters
po = 1/4;     # which power law
Q = 100;        # scaling factor with respect to mass
dr = 0.00001          # dampling ratio
m1l = m1r = m1 = 0.01 * Q
m2l = m2r = m2 = 0.005 * Q
k1l = k1r = k1 = 5*Q**(po)
k2l = k2r = k2 = 2*Q**(po)
kcl = kcr = kc = 2*Q**(po)
c1l = c1r = c1 = 3*k1
c2l = c2r = c2 = 3*k2
r1l = r1r = r1 = 2*dr*(m1*k1)**0.5
r2l = r2r = r2 = 2*dr*(m2*k2)**0.5
a10 = a20 = 0.05*Q**(2*po)
l = 0.3*Q**(po)
d1 =0.1*Q**(po)
d2 = 0.05*Q**(po)
Ps = 0.05

# Initialization
x1l0 = x1r0 = 0.1 
y1l0 = y1r0 = 0.1
x2l0 = x2r0 = 0.1
y2l0 = y2r0 = 0

# Bundle params
params = [m1l, m2l, m1r, m2r, k1l, k2l, k1r, k2r, 
    r1l, r2l, r1r, r2r, c1l, c2l, c1r, c2r,
    kcl, kcr, a10, a20, d1, d2, l, Ps]
# Bundle initial conditions
y0 = [x1l0,y1l0,x2l0,y2l0,x1r0,y1r0,x2r0,y2r0]

# Set time steps
t0 = 0.0
T = 50.
N = 10000
#t = np.arange(0., T, dt)
t = np.linspace(t0, T, N)


# call the ODE solver




psoln = odeint(twomass, y0, t, args=(params,))

def func_odeint(p_y0, p_params, p_t0, p_T, p_N):
    tt = np.linspace(p_t0, p_T, p_N)
    ndarray =  odeint(twomass, p_y0, tt, args=(p_params,))
    list = ndarray.tolist()
    return list


