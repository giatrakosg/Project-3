import matplotlib.pyplot as plt
import numpy as np

seasons_nn = [0.0501502,0.0485321,0.189111,0.0899072]
seasons_pr = [0.24387,0.121594,0.0686717,0.133835]
x = [1,2,3,4]

#Plot the points
plt.plot(x,seasons_nn)
plt.plot(x,seasons_pr)
plt.show() #in Ubuntu Linux