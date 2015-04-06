#!/usr/bin/env python

import Herakles

print "Connect"
h=Herakles.Uhal("localhost",50001)
h.SetVerbose(True)

print "Write one value to one register"
h.Write(2,0x44004444)
print "Read one value from one register"
print h.Read(2)

print "Write two values to consecutive registers"
h.Write(3,[8,16]) 
print "Read five values from consecutive registers"
print h.Read(3,5)

print "Write two values to one register (fifo)"
h.Write(3,(22,23),True) 
print "Read two values from one register (fifo)"
print h.Read(3,2,True)
