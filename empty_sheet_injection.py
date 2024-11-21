import os
import sys
import gspread
from oauth2client.service_account import ServiceAccountCredentials
import csv
import time
from datetime import datetime
from gspread.exceptions import APIError

# Extract the provided date or default to today's date
if len(sys.argv) == 2:
    provided_date = sys.argv[1]
    try:
        # Validate the provided date format
        time.strptime(provided_date, "%Y.%m.%d")
    except ValueError:
        print("Error: The provided date must be in the format YYYY.MM.DD")
        sys.exit(1)
else:
    # Default to today's date if no date is provided
    provided_date = datetime.now().strftime("%Y.%m.%d")
    print(f"No date provided. Defaulting to today's date: {provided_date}")

# Define the bin directory using the date
bin_directory = f"data/{provided_date}/bin/"
if not os.path.exists(bin_directory):
    print(f"Error: The directory '{bin_directory}' does not exist.")
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
                print(f"Rate limit hit. Retrying... ({attempt + 1}/{retries})")
                time.sleep(2 ** attempt)
            else:
                raise e

# Function to upload data to a new or existing sheet
def upload_csv_to_sheet(csv_filename, sheet):
    with open(csv_filename, 'r', encoding='utf-8') as csvfile:
        reader = csv.reader(csvfile)
        data = list(reader)

    # Ensure data has consistent row lengths
    max_cols = max(len(row) for row in data)
    data = [row + [''] * (max_cols - len(row)) for row in data]

    # Clear the sheet before updating
    sheet.clear()

    # Update the sheet with the CSV data
    safe_sheet_update(sheet, range_name='A1', data=data)

# Search for CSV files in the specified directory
csv_files = [f for f in os.listdir(bin_directory) if f.endswith('.csv')]

if not csv_files:
    print(f"No CSV files found in the directory: {bin_directory}")
    sys.exit(1)

for csv_file in csv_files:
    # Extract the name for the new sheet (without '.csv' extension)
    sheet_name = os.path.splitext(csv_file)[0]

    try:
        # Try to open the sheet if it exists
        sheet = spreadsheet.worksheet(sheet_name)
    except gspread.exceptions.WorksheetNotFound:
        # If the sheet doesn't exist, create a new one dynamically
        rows, cols = 1000, 26  # Default fallback
        with open(os.path.join(bin_directory, csv_file), 'r', encoding='utf-8') as f:
            reader = csv.reader(f)
            data = list(reader)
            rows = len(data)
            cols = max(len(row) for row in data)
        sheet = spreadsheet.add_worksheet(title=sheet_name, rows=rows, cols=cols)

    # Full path of the CSV file
    csv_path = os.path.join(bin_directory, csv_file)

    # Upload data from CSV file to the corresponding sheet
    try:
        upload_csv_to_sheet(csv_path, sheet)
        print(f"✅ Data from '{csv_file}' synced successfully.")
    except Exception as e:
        print(f"❌ Error syncing '{csv_file}': {e}")

print("\nAll CSV files have been processed.")
