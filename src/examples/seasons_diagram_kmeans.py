import matplotlib.pyplot as plt
import numpy as np

seasons_nn = [0.220106,0.0427614,0.0576579,0.0925214]
seasons_pr = [0.19907,0.0911062,0.107741,0.137663]
x = [1,2,3,4]

#Plot the points
plt.plot(x,seasons_nn)
plt.plot(x,seasons_pr)
plt.show() #in Ubuntu Linux