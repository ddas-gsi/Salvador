#!/usr/bin/env python3

# import matplotlib.pyplot as plt
# import numpy as np

# # Potassium isotopes
# x = [39, 41, 43, 45, 47, 49, 51,53]

# # Potassium levels
# levels = {
#     r"$s_{1/2^+}$": [2522, 980, 561, 474, 0.0, 0.0, 737, 837],
#     r"$d_{3/2^+}$": [0.0, 0.0, 0.0, 0.0, 360, 92, 0.0, 0.0],
#     r"$f_{7/2^-}$": [2814,1294,738,1081,2020,2104, np.nan, np.nan]
# }

# plt.figure(figsize=(12,6))

# for label, energies in levels.items():

#     # draw horizontal level bars
#     for i in range(len(x)):
#         if not np.isnan(energies[i]):
#             plt.plot([x[i]-0.3, x[i]+0.3],
#                      [energies[i], energies[i]],
#                      linewidth=3)

#     # draw dashed connections between edges
#     for i in range(len(x)-1):
#         if not np.isnan(energies[i]) and not np.isnan(energies[i+1]):
#             plt.plot([x[i]+0.3, x[i+1]-0.3],
#                      [energies[i], energies[i+1]],
#                      linestyle='--',
#                      alpha=0.6)
            
# for label, energies in levels.items():

#     # find last valid point
#     for i in reversed(range(len(x))):
#         if not np.isnan(energies[i]):
#             x_last = x[i]
#             y_last = energies[i]
#             break

#     # plt.text(x_last + 0.5, y_last, 
#     #          rf"${label}$",
#     #          va='center',
#     #          fontsize=12)
#     plt.text(x_last + 0.5, y_last+20, label, va='center', fontsize=12)

# plt.ylabel("Excitation Energy (keV)", fontsize=14)
# plt.xlabel("Isotope")
# plt.xticks(fontsize=12)
# plt.xticks(x, [
#     r"$^{39}\mathrm{K}_{20}$",
#     r"$^{41}\mathrm{K}_{22}$",
#     r"$^{43}\mathrm{K}_{24}$",
#     r"$^{45}\mathrm{K}_{26}$",
#     r"$^{47}\mathrm{K}_{28}$",
#     r"$^{49}\mathrm{K}_{30}$",
#     r"$^{51}\mathrm{K}_{32}$",
#     r"$^{53}\mathrm{K}_{34}$"
# ])
# plt.show()



import matplotlib.pyplot as plt
import numpy as np

# isotopes
A = [39,41,43,45,47,49,51,53,55]

# levels
# levels = {
#     r"$s_{1/2^+}$": [2522, 980, 561, 474, 0.0, 0.0, 737, 837, 668],
#     r"$d_{3/2^+}$": [0.0, 0.0, 0.0, 0.0, 360, 92, 0.0, 0.0, 0.0],
#     r"$f_{7/2^-}$": [2814,1294,738,1081,2020,2104,np.nan,np.nan, np.nan]
# }

# level_colors = {
#     r"$s_{1/2^+}$": "#1f77b4",   # blue
#     r"$d_{3/2^+}$": "#d62728",   # red
#     r"$f_{7/2^-}$": "#2ca02c"    # green
# }

levels = {
    r"$1/2^+$": [2522, 980, 561, 474, 0.0, 0.0, 737, 837, 668],
    r"$3/2^+$": [0.0, 0.0, 0.0, 0.0, 360, 92, 0.0, 0.0, 0.0],
    r"$7/2^-$": [2814,1294,738,1081,2020,2104,np.nan,np.nan, np.nan]
}

level_colors = {
    r"$1/2^+$": "#1f77b4",   # blue
    r"$3/2^+$": "#d62728",   # red
    r"$7/2^-$": "#2ca02c"    # green
}

fig, ax = plt.subplots(figsize=(10,6))

bar_width = 0.35

for label, energies in levels.items():

    # horizontal level bars
    for i in range(len(A)):
        if not np.isnan(energies[i]):
            ax.plot(
                [A[i]-bar_width, A[i]+bar_width],
                [energies[i], energies[i]],
                linewidth=6,
                color=level_colors[label],
                solid_capstyle='butt'
            )

    # evolution dashed lines
    for i in range(len(A)-1):
        if not np.isnan(energies[i]) and not np.isnan(energies[i+1]):
            ax.plot(
                [A[i]+bar_width, A[i+1]-bar_width],
                [energies[i], energies[i+1]],
                linestyle='-.',
                linewidth=0.5,
                # color=level_colors[label],
                color="black",
                alpha=0.7
            )

    # state label on right
    for i in reversed(range(len(A))):
        if not np.isnan(energies[i]):
            ax.text(
                A[i]+0.8,
                energies[i],
                label,
                va='center',
                fontsize=13
            )
            break


# axis labels
ax.set_ylabel("Excitation Energy (keV)", fontsize=15)

# isotope labels
ax.set_xticks(A)
ax.set_xticklabels(
    [rf"$^{{{a}}}\mathrm{{K}}_{{{a-19}}}$" for a in A],
    fontsize=15
)

# styling like PRL
ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)

ax.tick_params(axis='y', labelsize=12)
ax.tick_params(axis='x', length=6)

ax.set_xlim(min(A)-1,max(A)+3)
ax.set_ylim(-150,3200)

plt.tight_layout()

plt.savefig("K_shell_evolution.pdf")
plt.show()