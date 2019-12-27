import matplotlib.pyplot as plt
import numpy as np
x = np.arange(0, 3*np.pi, 0.1)
y = np.sin(x)
y_cos = np.cos(x)
plt.plot(x, y)
plt.plot(x,y_cos)
plt.xlabel('Angle in radians')
plt.ylabel('y axis')
plt.title('Sine and Cosine')
plt.legend(['Sine', 'Cosine'])
plt.show()

x = np.arange(0, 3 * np.pi, 0.1)
y_sin = np.sin(x)
y_cos = np.cos(x)
# Set up a subplot grid that has height 2 and width 1 subplots,
# and set the first such subplot as active. Index starts at 1
#in the upper left corner and increases to the right.
plt.subplot(2, 1, 1)
plt.plot(x, y_sin)
plt.title('Sine')
# Set the second subplot as active
plt.subplot(2, 1, 2)
plt.plot(x, y_cos)
plt.title('Cosine')
plt.show()
