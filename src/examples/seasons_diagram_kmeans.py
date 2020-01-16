import matplotlib.pyplot as plt
import numpy as np

seasons_nn = [0.140365,0.0667735,0.0269824,0.285229]
seasons_pr = [0.118181,0.170835,0.218868,0.0535329]
x = [1,2,3,4]

#Plot the points
plt.plot(x,seasons_nn)
plt.plot(x,seasons_pr)
plt.show() #in Ubuntu Linux