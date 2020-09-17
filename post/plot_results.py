#!/usr/bin/env python

'''
Simple plotter for mattress-metropolis-app

Usage:
  plot_results.py [-s] [-r [-J <coupling_value>]] <log_file>

Options:
  -s, --save                             Save the figure as plot.py in cwd.
  -r, --plot-reference-result            Plots exact data for reference
  -J, --coupling-value <coupling_value>  Ising coupling value [default: 1.0]

'''

import yaml
import math
from docopt import docopt
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.pyplot as plt
from utility.get_value_if_prefix_matches import get_value_if_prefix_matches

color_specific_heat = 'red'
color_abs_magnetization = 'blue'
color_abs_magnetization_fill = 'lightblue'

po = docopt(__doc__)

log_file_path = po['<log_file>']

# Calculate the reference data:
if po['--plot-reference-result']:
    J = float(po['--coupling-value'])
    ising_tc = J / math.log(1 + math.sqrt(2)) / 2
    t_to_m = lambda t: (1 - np.sinh(J / 2 / t)**(-4))**(1 / 8) / 2
    exact_temperature_midpart = np.linspace(0, ising_tc, num=100, endpoint=False)[1:]
    exact_abs_magnetization_midpart = t_to_m(exact_temperature_midpart)
    exact_temperature = (0.0,) + tuple(exact_temperature_midpart) + (ising_tc,)
    exact_abs_magnetization = (0.5,) + tuple(exact_abs_magnetization_midpart) + (0.0,)

# Parse the log file:
with open(log_file_path) as log_file:
    for line in log_file:
        line = line.strip()
        prefix_temperature = "[RESULTS] T: "
        prefix_specific_heat = "[RESULTS] C: "
        prefix_abs_magnetization = "[RESULTS] |M|: "
        if (_ := get_value_if_prefix_matches(line, prefix_temperature)):
            temperature = yaml.safe_load(_)
        if (_ := get_value_if_prefix_matches(line, prefix_specific_heat)):
            specific_heat = yaml.safe_load(_)
        if (_ := get_value_if_prefix_matches(line, prefix_abs_magnetization)):
            abs_magnetization = yaml.safe_load(_)

#patch the data to include 0 temperature:
temperature = [0.0, *temperature]
specific_heat = [0.0, *specific_heat]
abs_magnetization = [0.5, *abs_magnetization]

# create the plot:
fig = plt.figure()
ax1 = fig.add_subplot(111)
ax2 = ax1.twinx()
ax1.set_xlabel('T')
ax1.set_ylabel('|M|', color=color_abs_magnetization)
ax2.set_ylabel('C', color=color_specific_heat)
ax1.set_xlim(0, max(temperature))
ax1.set_ylim(0, 0.5 * 1.01)
ax2.set_ylim(0, max(specific_heat) * 1.1)
ax1.tick_params(axis='y', labelcolor=color_abs_magnetization)
ax2.tick_params(axis='y', labelcolor=color_specific_heat)
if po['--plot-reference-result']:
    ax1.axvline(x=ising_tc, color='gray', linewidth=3, linestyle=":")
    ax1.plot(exact_temperature, exact_abs_magnetization, color='gray', linewidth=3)        
ax1.plot(temperature, abs_magnetization, color=color_abs_magnetization, linewidth=3)
ax1.fill_between(temperature, abs_magnetization, color=color_abs_magnetization_fill)
ax2.plot(temperature, specific_heat, color=color_specific_heat, linewidth=3)
ax1.grid()
if po['--save']: 
  plt.savefig('plot.png')
plt.show()
