#!/usr/bin/env python
import sys
import matplotlib.pyplot as plt
import yaml
import math



color_specific_heat = 'red'
color_abs_magnetization = 'blue'
color_abs_magnetization_fill = 'lightblue'

def get_value_if_prefix_matches(text, prefix):
    if text.startswith(prefix):
        return text[len(prefix):]
    return None

log_file_path = sys.argv[1]
print(f"{sys.argv=}")
print(f"{log_file_path=}")
if len(sys.argv) > 2:
    J = float(sys.argv[2])
    ising_tc = (1 / 2 / math.log(1 + math.sqrt(2))) * J
    print(f"{ising_tc=}")

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
if 'ising_tc' in locals():
    ax1.axvline(x=ising_tc, color='gray', linewidth=3)
ax1.plot(temperature, abs_magnetization, color=color_abs_magnetization, linewidth=3)
ax1.fill_between(temperature, abs_magnetization, color=color_abs_magnetization_fill)
ax2.plot(temperature, specific_heat, color=color_specific_heat, linewidth=3)
ax1.grid()
plt.show()
