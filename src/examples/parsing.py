# Load the Pandas libraries with alias 'pd'
import pandas as pd
import sys
# Read data from file 'filename.csv'
# (in the same directory that your python process is based)
# Control delimiters, rows, column names with read_csv (see later)
data = pd.read_csv(sys.argv[1],header=None)

r = data.apply(lambda x : sum(x[1:]),axis=1)
print(r)
