import gspread
from oauth2client.service_account import ServiceAccountCredentials
import csv
import glob
import os
import sys
import time
import argparse
from datetime import datetime
from gspread.exceptions import APIError

# Argument parsing
parser = argparse.ArgumentParser()
parser.add_argument("date", help="The date in YYYY.MM.DD format.")
parser.add_argument("--log", help="Path to the log file.")
args = parser.parse_args()

# Log file setup
if args.log:
    log_file_path = args.log
else:
    logs_dir = "logs"
    os.makedirs(logs_dir, exist_ok=True)
    timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    log_file_path = os.path.join(logs_dir, f"append_sheet_injection_{timestamp}.txt")

# Logging function
def log_message(message):
    with open(log_file_path, "a") as log_file:
        log_file.write(f"{message}\n")
    print(message)  # Also print to console for immediate feedback

log_message(f"Starting append_sheet_injection.py for date: {args.date}")

# Extract the provided date or default to today's date
if len(sys.argv) == 2:
    provided_date = sys.argv[1]
    try:
        # Validate the provided date format
        datetime.strptime(provided_date, "%Y.%m.%d")
    except ValueError:
        log_message("Error: The provided date must be in the format YYYY.MM.DD")
        sys.exit(1)
else:
    # Default to today's date if no date is provided
    provided_date = datetime.now().strftime("%Y.%m.%d")
    log_message(f"No date provided. Defaulting to today's date: {provided_date}")

# Define the bin directory using the date
bin_directory = f"data/{provided_date}/bin/"
if not os.path.exists(bin_directory):
    log_message(f"Error: The directory '{bin_directory}' does not exist.")
    sys.exit(1)

# Google Sheets API setup
scope = ["https://spreadsheets.google.com/feeds", "https://www.googleapis.com/auth/drive"]
creds = ServiceAccountCredentials.from_json_keyfile_name('service-account-credentials.json', scope)
client = gspread.authorize(creds)
spreadsheet = client.open("Weekly Reporting for Tracker")

# Function to safely update a sheet with retries
def safe_sheet_update(sheet, range_name, data, retries=5):
    for attempt in range(retries):
        try:
            sheet.update(range_name=range_name, values=data)
            return
        except APIError as e:
            if attempt < retries - 1:
                log_message(f"Rate limit hit. Retrying... ({attempt + 1}/{retries})")
                time.sleep(2 ** attempt)
            else:
                raise e

# Find all CSV files in the specified bin folder
csv_files = glob.glob(f'{bin_directory}/*.csv')

if not csv_files:
    log_message(f"No CSV files found in the directory: {bin_directory}")
    sys.exit(1)

# Loop through each CSV file
for csv_file in csv_files:
    # Get the filename without the path and extension
    sheet_name = os.path.splitext(os.path.basename(csv_file))[0]
    
    # Try to find a worksheet with the same name as the CSV file
    try:
        worksheet = spreadsheet.worksheet(sheet_name)
    except gspread.exceptions.WorksheetNotFound:
        # If not found, create a new worksheet with that name
        worksheet = spreadsheet.add_worksheet(title=sheet_name, rows="1000", cols="26")
        log_message(f"Created new sheet: {sheet_name}")

    # Read the CSV file and skip the header row
    with open(csv_file, 'r', encoding='utf-8') as csvfile:
        reader = csv.reader(csvfile)
        data = list(reader)[1:]  # Skip the first row (header)

    # Find the next empty row in the worksheet
    existing_data = worksheet.get_all_values()
    next_empty_row = len(existing_data) + 1  # The first empty row after existing data

    # Check if the worksheet needs more rows to fit the new data
    required_rows = next_empty_row + len(data) - 1
    current_rows = worksheet.row_count

    # Add extra rows if required
    if required_rows > current_rows:
        worksheet.add_rows(required_rows - current_rows)
        log_message(f"Added {required_rows - current_rows} rows to '{sheet_name}' to accommodate new data.")

    # Determine the maximum number of columns in the data
    max_cols = max(len(row) for row in data)
    end_col = chr(64 + max_cols)  # Convert column number to letter (e.g., 1 -> A, 26 -> Z)

    # Define the range dynamically based on the data
    start_cell = f'A{next_empty_row}'
    end_cell = f'{end_col}{next_empty_row + len(data) - 1}'

    # Update the worksheet in smaller chunks starting at the next empty row
    chunk_size = 500  # Define the chunk size for splitting data
    for i in range(0, len(data), chunk_size):
        chunk = data[i:i + chunk_size]  # Slice the data into chunks
        start_chunk_row = next_empty_row + i  # Calculate the starting row for this chunk
        end_chunk_row = start_chunk_row + len(chunk) - 1  # Calculate the ending row
        end_chunk_col = chr(64 + max(len(row) for row in chunk))  # Determine the end column

        # Define the range for this chunk
        start_cell = f'A{start_chunk_row}'
        end_cell = f'{end_chunk_col}{end_chunk_row}'
        
        try:
            safe_sheet_update(worksheet, range_name=f'{start_cell}:{end_cell}', data=chunk)
            log_message(f"✅ Chunk {i // chunk_size + 1}: Rows {start_chunk_row} to {end_chunk_row} successfully appended to '{sheet_name}'.")
        except Exception as e:
            log_message(f"❌ Error appending chunk {i // chunk_size + 1}: Rows {start_chunk_row} to {end_chunk_row} in '{sheet_name}': {e}")
            raise


log_message("All CSV files have been processed.")
