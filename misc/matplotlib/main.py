#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt

x      = np.array([80*0, 80*1, 80*2, 80*3, 80*4])
y      = np.array([
   3479760896 - 300000*0,
   3479760896 - 300000*1,
   3479760896 - 300000*2,
   3479760896 - 300000*3,
   3479760896 - 300000*4 ])
y_ml   = np.array([3479760896, 3479605248, 3437199360, 3437035520, 3436937216])
y_noml = np.array([3479482368, 3453829120, 3453952000, 3453845504, 3453636608])
plt.plot(x, y, label='yosou')
plt.plot(x, y_ml, label='w/ memory leak')
plt.plot(x, y_noml, label='w/o memory leak')
plt.legend()
plt.show()

