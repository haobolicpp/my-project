#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Wed Jun  7 00:31:01 2017

@author: yisi
"""
import numpy as np

def twomass(y, t, params):
    #theta = lambda x : 1 if x>0 else 0
    def theta(x,x0) : 
        if x>0 :
            th = np.tanh(50*(x/x0))
        else :
            th = 0
        return th
    def a(a0,xl,xr,l) : return a0+l*(xl+xr)
    #def P1(a1,Ps,Pe,amin) : return Ps+(Ps-Pe)*(a1/amin)**2
    def P1(a1,Ps,amin,a0) : return Ps*(1-max(amin,0)*(amin/a1)**2)*theta(a1,a0)
    x1l, y1l, x2l, y2l, x1r, y1r, x2r, y2r = y
    [m1l, m2l, m1r, m2r, k1l, k2l, k1r, k2r, 
    r1l, r2l, r1r, r2r, c1l, c2l, c1r, c2r,
    kcl, kcr, a10, a20, d1, d2, l, Ps] = params
    derivs = [y1l,
             -(r1l*y1l+k1l*x1l+theta(-a(a10,x1l,x1r,l),a10)*c1l*a(a10,x1l,x1r,l)/2/l+kcl*(x1l-x2l)-l*d1*P1(a(a10,x1l,x1r,l),Ps,min(a(a10,x1l,x1r,l),a(a20,x2l,x2r,l)),a10))/m1l,
             y2l,
             -(r2l*y2l+k2l*x2l+theta(-a(a20,x2l,x2r,l),a10)*c2l*a(a20,x2l,x2r,l)/2/l+kcl*(x2l-x1l))/m2l,
             y1r,
             -(r1r*y1r+k1r*x1r+theta(-a(a10,x1r,x1l,l),a10)*c1r*a(a10,x1r,x1l,l)/2/l+kcr*(x1r-x2r)-l*d1*P1(a(a10,x1r,x1l,l),Ps,min(a(a10,x1r,x1l,l),a(a20,x2r,x2l,l)),a10))/m1r,
             y2r,
             -(r2r*y2r+k2r*x2r+theta(-a(a20,x2r,x2l,l),a10)*c2r*a(a20,x2r,x2l,l)/2/l+kcr*(x2r-x1r))/m2r,              
             ]
    return derivs