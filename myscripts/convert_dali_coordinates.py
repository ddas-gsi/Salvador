#!/usr/bin/env python3

import csv

def convert_csv_to_dat(input_csv, output_dat):
    lines_out = []

    with open(input_csv, "r") as f:
        reader = csv.DictReader(f, delimiter=",")  # comma-seperated
        for row in reader:
            # print(row)
            det = int(row["Id"])
            x = float(row["x_pos"])
            y = float(row["y_pos"])
            z = float(row["z_pos"])

            lines_out.append(f"Average.Position.X.{det}:                    {x}")
            lines_out.append(f"Average.Position.Y.{det}:                    {y}")
            lines_out.append(f"Average.Position.Z.{det}:                    {z}")

    with open(output_dat, "w") as out:
        out.write("\n".join(lines_out))

    print(f"Written to {output_dat}")


if __name__ == "__main__":
    #convert_csv_to_dat("detectors.csv", "positions.dat")
    convert_csv_to_dat("DALI.HYPATIA.20251010.csv", "dali_coordinates_20251010.dat")

