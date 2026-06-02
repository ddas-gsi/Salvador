#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt

# Your 3 data points

# 6095, 53Ca centered, Au target ======================
# 6095 53Ca centered Au target, TOF.Offset.2 fixed with 52Ca AoQ 2.6
#x = np.array([2.65527, 2.56643, 2.59614])   # AoQs
#y = np.array([315.7, 309.7, 311.7])         # ToF offsets

# 6095 53Ca centered Au target, TOF.Offset.5 fixed with 52Ca AoQ 2.6
#x = np.array([2.54683, 2.62764, 2.60073])   # AoQs
#y = np.array([-122.4, -116.4, -118.4])         # ToF offsets

# 6095 53Ca centered Au target, TOF.Offset.0 fixed with 52Ca AoQ 2.6
#x = np.array([2.67937, 2.59858, 2.77485])   # AoQs
#y = np.array([305.7, 300.7, 311.7])         # ToF offsets

# 6095 53Ca centered Au target, TOF.Offset.1 fixed with 52Ca AoQ 2.6
#x = np.array([2.35829, 2.44236, 2.49797])   # AoQs
#y = np.array([305.7, 311.7, 315.7])         # ToF offsets

# 6095 53Ca centered Au target, TOF.Offset.3 fixed with 53Ca AoQ 2.65
#x = np.array([2.62997, 2.68288, 2.6558])   	# AoQs
#y = np.array([-120.4, -116.4, -118.4529247262164])         	# ToF offsets

# 6095 53Ca centered Au target, TOF.Offset.4 fixed with 53Ca AoQ 2.65
#x = np.array([2.62069, 2.67326, 2.64639])   	# AoQs
#y = np.array([-120.4, -116.4, -118.4529247262164])         	# ToF offsets

# 6095 53Ca centered Au target, TOF.Offset.0 fixed with 53Ca AoQ 2.65.. NEW Round
#x = np.array([2.66049, 2.69312, 2.46396])   # AoQs
#y = np.array([300.7736304110567, 302.8, 288.8])         # ToF offsets

# 6095 53Ca centered Au target, TOF.Offset.1 fixed with 53Ca AoQ 2.65.. NEW Round
#x = np.array([2.64077, 2.66661, 2.61231])   # AoQs
#y = np.array([322.9952939757207, 324.9, 320.9])         # ToF offsets

# 5020, 53Ca centered, Be target ======================
# 5020 53Ca centered Be target, TOF.Offset.0 fixed with 53Ca AoQ 2.65
#x = np.array([2.65337, 2.68461, 2.45546])   # AoQs
#y = np.array([300.1521287915849, 302.1, 288.1])         # ToF offsets

# 5020 53Ca centered Be target, TOF.Offset.1 fixed with 53Ca AoQ 2.65
#x = np.array([2.65192, 2.67796, 2.62372])   # AoQs
#y = np.array([323.67601772280926, 325.6, 321.6])         # ToF offsets

# 5020 53Ca centered Be target, TOF.Offset.2 fixed with 53Ca AoQ 2.65
#x = np.array([2.65336, 2.68177, 2.62280])   # AoQs
#y = np.array([311.96495269975924, 313.9, 309.9])         # ToF offsets

# 5020 53Ca centered Be target, TOF.Offset.3 fixed with 53Ca AoQ 2.65
#x = np.array([2.65223, 2.62637, 2.68035])   # AoQs
#y = np.array([-118.88763254470942,-120.8,-116.8])         # ToF offsets

# 5020 53Ca centered Be target, TOF.Offset.4 fixed with 53Ca AoQ 2.65
#x = np.array([2.65196, 2.62611, 2.67972])   # AoQs
#y = np.array([-118.172627273355,-120.1,-116.1])         # ToF offsets

# 5020 53Ca centered Be target, TOF.Offset.5 fixed with 53Ca AoQ 2.65
#x = np.array([2.65166, 2.62548, 2.67920])   # AoQs
#y = np.array([-118.4529247262164, -120.4, -116.4])         # ToF offsets


# 3003, 50,51Ca centered, Be target ======================
# 3003, 50,51Ca centered, Be target, TOF.Offset.0 fixed with 50Ca AoQ 2.50
#x = np.array([2.50186, 2.53296, 2.31222])   # AoQs
#y = np.array([300.0, 302.0, 288.0])         # ToF offsets

# 3003, 50,51Ca centered, Be target, TOF.Offset.1 fixed with 50Ca AoQ 2.50
#x = np.array([2.44999, 2.47606, 2.42385])   # AoQs
#y = np.array([320.0, 322.0, 318.0])         # ToF offsets

# 3003, 50,51Ca centered, Be target, TOF.Offset.2 fixed with 50Ca AoQ 2.50
#x = np.array([2.50046, 2.52869, 2.47215])   # AoQs
#y = np.array([311.9, 313.9, 309.9])         # ToF offsets

# 3003, 50,51Ca centered, Be target, TOF.Offset.3 fixed with 50Ca AoQ 2.50
# x = np.array([2.50436, 2.47885, 2.52986])   # AoQs
# y = np.array([-118.3, -120.3, -116.3])         # ToF offsets

# 3003, 50,51Ca centered, Be target, TOF.Offset.4 fixed with 50Ca AoQ 2.50
#x = np.array([2.49756, 2.47210, 2.52295])   # AoQs
# y = np.array([-118.3, -120.3, -116.3])         # ToF offsets

# 3003, 50,51Ca centered, Be target, TOF.Offset.5 fixed with 50Ca AoQ 2.50
#x = np.array([2.49998, 2.47452, 2.52542])   # AoQs
#y = np.array([-118.3, -120.3, -116.3])         # ToF offsets


# 2010, 50,51Ca centered, Au target ======================
# 2010, 50,51Ca centered, Au target, TOF.Offset.0 fixed with 50Ca AoQ 2.50
#x = np.array([2.50197, 2.31287, 2.5331])   # AoQs
#y = np.array([300.0, 288.0, 302.0])         # ToF offsets

# 2010, 50,51Ca centered, Au target, TOF.Offset.1 fixed with 50Ca AoQ 2.50
#x = np.array([2.44993, 2.42381, 2.47599])   # AoQs
#y = np.array([320.0, 318.0, 322.0])         # ToF offsets

# 2010, 50,51Ca centered, Au target, TOF.Offset.2 fixed with 50Ca AoQ 2.50
#x = np.array([2.50047, 2.47214, 2.52872])   # AoQs
#y = np.array([311.9, 309.9, 313.9])         # ToF offsets

# 2010, 50,51Ca centered, Au target, TOF.Offset.3 fixed with 50Ca AoQ 2.50
#x = np.array([2.50467, 2.52971, 2.47954])   # AoQs
#y = np.array([-118.2, -116.2, -120.2])         # ToF offsets

# 2010, 50,51Ca centered, Au target, TOF.Offset.4 fixed with 50Ca AoQ 2.50
#x = np.array([2.49818, 2.52327, 2.47305])   # AoQs
#y = np.array([-118.2, -116.2, -120.2])         # ToF offsets

# 2010, 50,51Ca centered, Au target, TOF.Offset.5 fixed with 50Ca AoQ 2.50
#x = np.array([2.50049, 2.52555, 2.47538])   # AoQs
#y = np.array([-118.2, -116.2, -120.2])         # ToF offsets


# 1010, 50,51Ca centered, C target ======================
# 1010, 50,51Ca centered, C target, TOF.Offset.0 fixed with 50Ca AoQ 2.50
#x = np.array([2.50187, 2.53304, 2.47072])   # AoQs
#y = np.array([300.0, 302.0, 298.0])         # ToF offsets

# 1010, 50,51Ca centered, C target, TOF.Offset.1 fixed with 50Ca AoQ 2.50
#x = np.array([2.48908, 2.51501, 2.46307])   # AoQs
#y = np.array([323.0, 325.0, 321.0])         # ToF offsets

# 1010, 50,51Ca centered, C target, TOF.Offset.2 fixed with 50Ca AoQ 2.50
#x = np.array([2.49906, 2.52730, 2.47086])   # AoQs
#y = np.array([311.8, 313.8, 309.8])         # ToF offsets

# 1010, 50,51Ca centered, C target, TOF.Offset.3 fixed with 50Ca AoQ 2.50
#x = np.array([2.50873, 2.48298, 2.53442])   # AoQs
#y = np.array([-118.0, -120.0, -116.0])         # ToF offsets

# 1010, 50,51Ca centered, C target, TOF.Offset.4 fixed with 50Ca AoQ 2.50
#x = np.array([2.50216, 2.47648, 2.52783])   # AoQs
#y = np.array([-118.0, -120.0, -116.0])         # ToF offsets

# # 1010, 50,51Ca centered, C target, TOF.Offset.5 fixed with 50Ca AoQ 2.50
# x = np.array([2.50195, 2.47628, 2.52760])   # AoQs
# y = np.array([-118.2, -120.2, -116.2])         # ToF offsets

# 2010, 50,51Ca centered, Au target ======================
# 2010, 50,51Ca centered, Au target, TOF.Offset.0 fixed with 49K AoQ 2.578947
# x = np.array([2.58449, 2.61535, 2.55331])   # AoQs
# y = np.array([299.8868339, 301.8868339, 297.8868339 ])         # ToF offsets ; Res=299.5363557113782

# 2010, 50,51Ca centered, Au target, TOF.Offset.1 fixed with 49K AoQ 2.578947
# x = np.array([2.57335, 2.59931, 2.54735])   # AoQs
# y = np.array([323.8397837, 325.8397837, 321.8397837])         # ToF offsets; Res=324.27170830708513

# 2010, 50,51Ca centered, Au target, TOF.Offset.2 fixed with 49K AoQ 2.578947
# x = np.array([2.57835, 2.60671, 2.55014])   # AoQs
# y = np.array([311.868658, 313.868658, 309.868658])         # ToF offsets; Res=311.90736170431165

# 2010, 50,51Ca centered, Au target, TOF.Offset.3 fixed with 49K AoQ 2.578947
# x = np.array([2.57635, 2.55125, 2.60171])   # AoQs
# y = np.array([-118.5699418, -120.5699418, -116.5699418])         # ToF offsets, Res=-118.37091845196178

# 2010, 50,51Ca centered, Au target, TOF.Offset.4 fixed with 49K AoQ 2.578947
# x = np.array([2.57606, 2.55095, 2.60126])   # AoQs
# y = np.array([-118.0539759, -120.0539759, -116.0539759])         # ToF offsets, Res=-117.82679519055745

# 2010, 50,51Ca centered, Au target, TOF.Offset.5 fixed with 49K AoQ 2.578947
# x = np.array([2.57614, 2.55104, 2.60137])   # AoQs
# y = np.array([-118.2377383, -120.2377383, -116.2377383])         # ToF offsets, Res=-118.01806582307984

# 3000, 50,51Ca centered, Be target ======================
# # 3000, 50,51Ca centered, Be target, TOF.Offset.0 fixed with 49K AoQ 2.578947
# x = np.array([2.58295, 2.61427, 2.5517])   # AoQs
# y = np.array([299.8977963, 301.8977963, 297.8977963])         # ToF offsets ; Res= 299.6404229

# # 3000, 50,51Ca centered, Be target, TOF.Offset.1 fixed with 49K AoQ 2.578947
# x = np.array([2.57342, 2.5994, 2.54735])   # AoQs
# y = np.array([323.8332353, 325.8332353, 321.8332353])         # ToF offsets; Res= 324.260314

# # 3000, 50,51Ca centered, Be target, TOF.Offset.2 fixed with 49K AoQ 2.578947
# x = np.array([2.57824, 2.60655, 2.54982])   # AoQs
# y = np.array([311.8693433, 313.8693433, 309.8693433])         # ToF offsets; Res= 311.9218047

# # 3000, 50,51Ca centered, Be target, TOF.Offset.3 fixed with 49K AoQ 2.578947
# x = np.array([2.57674, 2.5511, 2.60232])   # AoQs
# y = np.array([-118.6416324, -120.6416324, -116.6416324])         # ToF offsets, Res= -118.467687

# # 3000, 50,51Ca centered, Be target, TOF.Offset.4 fixed with 49K AoQ 2.578947
# x = np.array([2.57668, 2.5511, 2.60223])   # AoQs
# y = np.array([-118.1062276, -120.1062276, -116.1062276])         # ToF offsets, Res= -117.9280646

# # 3000, 50,51Ca centered, Be target, TOF.Offset.5 fixed with 49K AoQ 2.578947
# x = np.array([2.57664, 2.55101, 2.60221])   # AoQs
# y = np.array([-118.2979044, -120.2979044, -116.2979044])         # ToF offsets, Res= -118.1160788




# 1000, 50,51Ca centered, C target ======================
# 1000, 50,51Ca centered, C target, TOF.Offset.0 fixed with 49K AoQ 2.578947
# x = np.array([2.58308, 2.61403, 2.55138])   # AoQs
# y = np.array([299.8795464, 301.8795464,	297.8795464])         # ToF offsets ; Res= 299.6316647

# # 1000, 50,51Ca centered, C target, TOF.Offset.1 fixed with 49K AoQ 2.578947
# x = np.array([2.5736, 2.59962, 2.54754])   # AoQs
# y = np.array([323.8430233, 325.8430233, 321.8430233])         # ToF offsets ; Res= 324.25475146

# # 1000, 50,51Ca centered, C target, TOF.Offset.2 fixed with 49K AoQ 2.578947
# x = np.array([2.57826, 2.60658, 2.54979])   # AoQs
# y = np.array([311.8656745, 313.8656745, 309.8656745])         # ToF offsets ; Res= 311.91761087

# # 1000, 50,51Ca centered, C target, TOF.Offset.3 fixed with 49K AoQ 2.578947
# x = np.array([2.57669, 2.55077, 2.60252])   # AoQs
# y = np.array([-118.6772936, -120.6772936, -116.6772936])         # ToF offsets ; Res= -118.50049235

# # 1000, 50,51Ca centered, C target, TOF.Offset.4 fixed with 49K AoQ 2.578947
# x = np.array([2.57664, 2.55081, 2.60239])   # AoQs
# y = np.array([-118.1679974, -120.1679974, -116.1679974])         # ToF offsets ; Res= -117.986994436

# # # 1000, 50,51Ca centered, C target, TOF.Offset.5 fixed with 49K AoQ 2.578947
# x = np.array([2.57654, 2.55064, 2.60232])   # AoQs
# y = np.array([-118.3514679, -120.3514679, -116.3514679])         # ToF offsets ; Res= -118.1620434


# 5000, 52,53Ca centered, Be target ======================
# # 5000, 52,53Ca centered, Be target, TOF.Offset.0 fixed with 51K AoQ 2.6842
# x = np.array([2.67462, 2.70764, 2.64185])   # AoQs
# y = np.array([299.9687209, 301.9687209, 297.9687209])         # ToF offsets ; Res= 300.5467508

# # TOF.Offset.1 fixed with 51K AoQ 2.6842
# x = np.array([2.68688, 2.71437, 2.65923])   # AoQs
# y = np.array([323.5368458, 325.5368458, 321.5368458])         # ToF offsets; Res=323.34706466

# # # TOF.Offset.2 fixed with 51K AoQ 2.6842
# x = np.array([2.68129, 2.71131, 2.65094])   # AoQs
# y = np.array([311.7423594, 313.7423594, 309.7423594])         # ToF offsets; Res=311.94315424

# # TOF.Offset.3 fixed with 51K AoQ 2.6842
# x = np.array([2.68126, 2.65357, 2.70887])   # AoQs
# y = np.array([-119.0502864, -121.0502864, -117.0502864])         # ToF offsets; Res=-118.834938

# # TOF.Offset.4 fixed with 51K AoQ 2.6842
# x = np.array([2.68325, 2.65566, 2.71069])   # AoQs
# y = np.array([-118.3179556, -120.3179556, -116.3179556])         # ToF offsets; Res=-118.244503

# # TOF.Offset.5 fixed with 51K AoQ 2.6842
# x = np.array([2.6825, 2.65495, 2.71001])   # AoQs
# y = np.array([-118.5750022, -120.5750022, -116.5750022])         # ToF offsets; Res=-118.44976723

# 6000, 52,53Ca centered, Au target ======================
# # TOF.Offset.0 fixed with 51K AoQ 2.6842
# x = np.array([2.67869, 2.71113, 2.6459])   # AoQs
# y = np.array([300.1521287915849, 302.1521287915849, 298.1521287915849])         # ToF offsets; Res=300.49780647

# # TOF.Offset.1 fixed with 51K AoQ 2.6842
# x = np.array([2.68734, 2.71466, 2.65983])   # AoQs
# y = np.array([323.67601772280926, 325.67601772280926, 321.67601772280926])         # ToF offsets; Res=323.452335207

# # TOF.Offset.2 fixed with 51K AoQ 2.6842
# x = np.array([2.68413, 2.71392, 2.65416])   # AoQs
# y = np.array([311.96495269975924, 313.96495269975924, 309.96495269975924])         # ToF offsets; Res=311.9743587

# # TOF.Offset.3 fixed with 51K AoQ 2.6842
# x = np.array([2.6816, 2.65449, 2.70879])   # AoQs
# y = np.array([-118.88763254470942, -120.88763254470942, -116.88763254470942])         # ToF offsets; Res=-118.697293

# # TOF.Offset.4 fixed with 51K AoQ 2.6842
# x = np.array([2.68318, 2.65604, 2.71011])   # AoQs
# y = np.array([-118.172627273355, -120.172627273355, -116.172627273355])         # ToF offsets; Res=-118.09121275

# # TOF.Offset.5 fixed with 51K AoQ 2.6842
# x = np.array([2.68208, 2.65492, 2.709])   # AoQs
# y = np.array([-118.4529247262164, -120.4529247262164, -116.4529247262164])         # ToF offsets; Res=-118.289425





# ToF_for_AoQ = 49/19.   #49K
ToF_for_AoQ = 51/19.   #51K


# ---- Linear fit ----
coeffs_lin = np.polyfit(x, y, 1)  # degree 1
y_fit_lin = np.polyval(coeffs_lin, x)

# ---- Quadratic fit ----
coeffs_quad = np.polyfit(x, y, 2)  # degree 2
y_fit_quad = np.polyval(coeffs_quad, x)

print("Linear fit coefficients:", coeffs_lin)  # slope, intercept

print(f"ToF offset for AoQ: {ToF_for_AoQ} is: ", np.polyval(coeffs_lin, ToF_for_AoQ))

print("Quadratic fit coefficients:", coeffs_quad)  # ax^2 + bx + c

# Plot
plt.scatter(x, y, color='red', label='Data points')
plt.plot(x, y_fit_lin, label='Linear fit')
# plt.plot(x, y_fit_quad, label='Quadratic fit')
plt.legend()
plt.show()

