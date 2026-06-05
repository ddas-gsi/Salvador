import csv

def read_csv(file_path):
    with open(file_path, mode='r', encoding='utf-8') as csvfile:
        # DictReader uses the first row as keys for a dictionary
        reader = csv.DictReader(csvfile)
        
        data = {}
        for row in reader:
            # Access data using the header name
            data[row['DetectorID']] = {
                'Mean': row['MPV/Mean'],
                'Sigma': row['Sigma'],
                'TimeOffset': row['TimeOffset']
            }
            
        return data

data_g700 = read_csv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_g700.csv")
data_le500 = read_csv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_le500.csv")


def line_from_two_points(x, y):
    """
    Given two points, return slope (m) and intercept (b)
    for the line y = mx + b.

    x: list or tuple of two x-values
    y: list or tuple of two y-values
    """
    x1, x2 = x
    y1, y2 = y

    if x2 == x1:
        raise ValueError("Vertical line (undefined slope)")

    m = (y2 - y1) / (x2 - x1)
    b = y1 - m * x1

    return m, b

# print(float(data_g700['2']["Mean"]), float(data_le500['2']["Mean"]))
# x = [float(data_g700['2']["Mean"]), float(data_le500['2']["Mean"])]
# y = [0,15]
# m, b = line_from_two_points(x, y)
# print(f"Slope (m): {m}, Intercept (b): {b}")

dali_time_offsets_filename = "/u/ddas/software/Salvador/dali_toffset_LinCalib_49K.dat"

with open(dali_time_offsets_filename, mode='w', encoding='utf-8') as f:

    for det_id in data_g700.keys():
        mean_g700 = float(data_g700[det_id]["Mean"])
        mean_le500 = float(data_le500[det_id]["Mean"])
        y = [0,10]
        x = [mean_g700, mean_le500]
        gain, offset = line_from_two_points(x, y)
        f.write(f"DALI.Toffset.{det_id}:\t{-offset:.2f}\n")
        f.write(f"DALI.TimGain.{det_id}:\t{gain:.5f}\n")
        print(f"Detector {det_id}: Intercept (c) = {-offset:.2f}, Slope (m) = {gain:.5f}")

