import gspread
from oauth2client.service_account import ServiceAccountCredentials
import csv
import glob
import os

# Define scope and credentials
scope = ["https://spreadsheets.google.com/feeds", "https://www.googleapis.com/auth/drive"]
creds = ServiceAccountCredentials.from_json_keyfile_name('service-account-credentials.json', scope)
client = gspread.authorize(creds)

# Open the Google Sheet
spreadsheet = client.open("Weekly Reporting for Tracker")

# Find all CSV files in the bin folder
csv_files = glob.glob('bin/*.csv')

# Loop through each CSV file
for csv_file in csv_files:
    # Get the filename without the path and extension
    sheet_name = os.path.splitext(os.path.basename(csv_file))[0]
    
    # Try to find a worksheet with the same name as the CSV file
    try:
        worksheet = spreadsheet.worksheet(sheet_name)
    except gspread.exceptions.WorksheetNotFound:
        # If not found, create a new worksheet with that name
        worksheet = spreadsheet.add_worksheet(title=sheet_name, rows="1000", cols="18")
        print(f"Created new sheet: {sheet_name}")

    # Read the CSV file and skip the header row
    with open(csv_file, 'r') as csvfile:
        reader = csv.reader(csvfile)
        data = list(reader)[1:]  # Skip the first row (header)

    # Restrict data to only 18 columns if it has more
    data = [row[:18] for row in data]

    # Find the next empty row in the worksheet
    existing_data = worksheet.get_all_values()
    next_empty_row = len(existing_data) + 1  # The first empty row after existing data

    # Check if the worksheet needs more rows to fit the new data
    required_rows = next_empty_row + len(data) - 1
    current_rows = worksheet.row_count

    # Add extra rows if required
    if required_rows > current_rows:
        worksheet.add_rows(required_rows - current_rows)
        print(f"Added {required_rows - current_rows} rows to '{sheet_name}' to accommodate new data.")
    
    # Define the range to start updating from the next empty row, limited to columns A:R
    start_cell = f'A{next_empty_row}'
    end_cell = f'R{next_empty_row + len(data) - 1}'

    # Update the worksheet in one batch starting at the next empty row
    worksheet.update(range_name=f'{start_cell}:{end_cell}', values=data)
    print(f"Data from {csv_file} appended to sheet '{sheet_name}' successfully.")

print("All CSV files uploaded successfully.")


