import csv

# -------- CONFIG --------
input_csv  = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Co60_0077_v2.csv"
output_csv = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Co60_0077_v_corr.csv"
# ------------------------

# Storage: {DetectorID: {peak: (mean, sigma)}}
data = {}

# -------- READ INPUT --------
with open(input_csv, newline="") as f:
    reader = csv.DictReader(f)

    required_cols = {"DetectorID", "Peak", "Mean", "Sigma"}
    if not required_cols.issubset(reader.fieldnames):
        raise ValueError(f"Input CSV must contain columns: {required_cols}")

    for row in reader:
        det  = int(row["DetectorID"])
        peak = int(row["Peak"])
        mean = float(row["Mean"])
        sigma = float(row["Sigma"])

        if peak not in (1, 2):
            print(f"Detector {det} has extra peak: {peak}")
            continue

        data.setdefault(det, {})[peak] = (mean, sigma)

# -------- WRITE OUTPUT --------
with open(output_csv, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["DetectorID", "Mean1", "Sigma1", "Mean2", "Sigma2"])

    for det in sorted(data):
        peaks = data[det]

        if 1 not in peaks or 2 not in peaks:
            continue

        mean1, sigma1 = peaks[1]
        mean2, sigma2 = peaks[2]

        writer.writerow([det, mean1, sigma1, mean2, sigma2])

print(f"Converted CSV written to: {output_csv}")