#!/usr/bin/env python3

import pandas as pd
import urllib.request
import matplotlib.pyplot as plt

# Helper to read IAEA API CSV
def download_livechart_csv(url):
    req = urllib.request.Request(url, 
           headers={'User-Agent': 'Mozilla/5.0'})
    return pd.read_csv(urllib.request.urlopen(req))

# 1) download level data
url_levels = (
    "https://www-nds.iaea.org/relnsd/v1/data?"
    "fields=levels&nuclides=49K"
)
df_levels = download_livechart_csv(url_levels)

# 2) tidy up: filter, sort by energy
# Expect columns: 'energy', 'spin_parity', etc.
df_levels = df_levels.sort_values(by='energy')

# 3) prepare for plotting
energies = df_levels['energy'].values  # in keV
labels = df_levels.get('spin_parity', ['']*len(energies))

# simple scheme plot
fig, ax = plt.subplots(figsize=(4, len(energies)*0.5))

for i, (E, lbl) in enumerate(zip(energies, labels)):
    # horizontal line at energy
    ax.hlines(E, 0, 1, color='black')
    # annotate with level label
    ax.text(1.02, E, f"{E:.1f} keV {lbl}", va='center')

# finalize axis
ax.set_ylim(-50, energies.max()+100)
ax.set_xlim(0, 2)
ax.set_xticks([])
ax.set_ylabel("Excitation Energy (keV)")
ax.set_title("Level scheme for $^{49}$K")

plt.tight_layout()
plt.show()