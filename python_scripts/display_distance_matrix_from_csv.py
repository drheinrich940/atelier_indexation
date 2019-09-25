import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# FIXME 'fisrt_image_name,second_image_name,distance' need to be included at the beginning of the csv file
csv_data_name = 'select___from_DISTANCE_TABLE2'
df = pd.read_csv('../' + csv_data_name + '.csv ')

df.columns = df.columns.str.strip()
df2 = df.sort_values(['fisrt_image_name', 'second_image_name'])

# print(df2)
#print(df2['distance'].values.reshape((500,500)).shape)

# Plotting
fig = plt.figure(figsize=(17, 17))
ax = fig.add_subplot(111)
cax = ax.matshow(df2['distance'].values.reshape((500,500)), interpolation='none', vmin=0, vmax=100)
fig.colorbar(cax)
plt.title('Image distances matrix')
plt.setp(ax.get_xticklabels(), rotation=-45, ha="right", rotation_mode="anchor")
plt.show()