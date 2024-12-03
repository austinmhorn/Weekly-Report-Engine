import csv
import sys

def fix_csv_file(input_file, output_file):
    with open(input_file, 'r') as infile:
        lines = infile.readlines()

    fixed_lines = []
    current_row = []

    for line in lines:
        # Check if the line matches the structure of a valid row
        if line.count(',') >= 21 and not line.strip().startswith("Future Lease Rent"):
            # Save the current row if it exists
            if current_row:
                fixed_lines.append(''.join(current_row).strip() + '\n')  # Join and add newline
                current_row = []
            # Add the valid line as a new row
            fixed_lines.append(line.strip() + '\n')
        else:
            # Handle cases where "Future Lease Rent" is a new line or partial data continues
            current_row.append(line.strip() + ' ')

    # Add the last row if necessary
    if current_row:
        fixed_lines.append(''.join(current_row).strip() + '\n')

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
