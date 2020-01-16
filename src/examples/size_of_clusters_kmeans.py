import matplotlib
import matplotlib.pyplot as plt
import numpy as np


labels = ['Cluster 1', 'Cluster 2', 'Cluster 3', 'Cluster 4']
nn_kmeans = [161, 60, 124, 154]
pr_kmeans = [147, 117, 107, 129]

x = np.arange(len(labels))  # the label locations
width = 0.35  # the width of the bars

fig, ax = plt.subplots()
rects1 = ax.bar(x - width/2, nn_kmeans, width, label='nn_kmeans')
rects2 = ax.bar(x + width/2, pr_kmeans, width, label='pr_kmeans')

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('Scores')
ax.set_title('Scores by file and method')
ax.set_xticks(x)
ax.set_xticklabels(labels)
ax.legend()


def autolabel(rects):
    """Attach a text label above each bar in *rects*, displaying its height."""
    for rect in rects:
        height = rect.get_height()
        ax.annotate('{}'.format(height),
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')


autolabel(rects1)
autolabel(rects2)

fig.tight_layout()

plt.show()