import os
import gspread
from oauth2client.service_account import ServiceAccountCredentials
import csv
import sys
import time
from gspread.exceptions import APIError

# Define scope and credentials
scope = ["https://spreadsheets.google.com/feeds", "https://www.googleapis.com/auth/drive"]
creds = ServiceAccountCredentials.from_json_keyfile_name('service-account-credentials.json', scope)
client = gspread.authorize(creds)

# Open the Google Sheet
spreadsheet = client.open("Weekly Reporting for Tracker")

# Function to upload data to a new or existing sheet
def upload_csv_to_sheet(csv_filename, sheet):
    with open(csv_filename, 'r') as csvfile:
        reader = csv.reader(csvfile)
        data = list(reader)
    
    # Clear the sheet before updating
    sheet.clear()

    # Update the sheet with the CSV data
    sheet.update(range_name='A1', values=data)

# Search for CSV files in the 'bin' subdirectory
bin_directory = 'bin'
csv_files = [f for f in os.listdir(bin_directory) if f.endswith('.csv')]

for csv_file in csv_files:
    # Extract the name for the new sheet (without '.csv' extension)
    sheet_name = os.path.splitext(csv_file)[0]

    try:
        # Try to open the sheet if it exists
        sheet = spreadsheet.worksheet(sheet_name)
    except gspread.exceptions.WorksheetNotFound:
        # If the sheet doesn't exist, create a new one
        sheet = spreadsheet.add_worksheet(title=sheet_name, rows=1000, cols=26)

    # Full path of the CSV file
    csv_path = os.path.join(bin_directory, csv_file)

    # Upload data from CSV file to the corresponding sheet
    upload_csv_to_sheet(csv_path, sheet)

    print(f"\nData from {csv_file} synced to Google Sheet successfully.")

print("\nAll CSV files have been processed.")