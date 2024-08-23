#! /usr/bin/env python3

import random

import matplotlib.pyplot as plt
import numpy as np

asize = 1024
psize = 2 * 1024
uList = [0] * psize

for i in range(300):
    random.seed(i)
    for j in range(psize):
        limit = j
        vaddr = int(asize * random.random())
        if (vaddr < limit):
            uList[j] += 1

fig = plt.figure()
x = np.linspace(1, psize, psize)
plt.plot(x, [u / 300 for u in uList], color='blue')
plt.ylim(0, 1)
plt.margins(0)
plt.xlabel('Value of the Bounds Register')
plt.ylabel('Fraction of Valid Randomly-Generated Virtual Addresses')
plt.savefig('1.5.png', dpi=227)
plt.show()