import csv
import sys

def fix_csv_file(input_file, output_file):
    with open(input_file, 'r') as infile:
        lines = infile.readlines()

    fixed_lines = []
    current_row = []

    for line in lines:
        # If the line has the correct number of commas for the expected columns
        if line.count(',') >= 21:
            # Save the current row if it exists
            if current_row:
                fixed_lines.append(''.join(current_row))
                current_row = []
            # Add the current valid line
            fixed_lines.append(line)
        else:
            # Otherwise, continue the previous row
            current_row.append(line.strip() + ' ')

    # Add the last row if necessary
    if current_row:
        fixed_lines.append(''.join(current_row))

    # Write the fixed lines to the output file
    with open(output_file, 'w', newline='') as outfile:
        outfile.writelines(fixed_lines)

    print(f"Fixed CSV written to: {output_file}")

# Main entry point
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python fix_csv_format.py <input_file> <output_file>")
        sys.exit(1)

    input_csv = sys.argv[1]
    output_csv = sys.argv[2]
    fix_csv_file(input_csv, output_csv)