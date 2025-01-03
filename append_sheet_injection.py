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
from logging_config import setup_logging  # Import centralized logging configuration
import logging

# Set up centralized logging
setup_logging()
logger = logging.getLogger(__name__)

# Argument parsing
parser = argparse.ArgumentParser()
parser.add_argument("date", help="The date in YYYY.MM.DD format.")
args = parser.parse_args()

logger.info(f"Starting append_sheet_injection.py for date: {args.date}")

# Extract the provided date or default to today's date
try:
    provided_date = datetime.strptime(args.date, "%Y.%m.%d").strftime("%Y.%m.%d")
except ValueError:
    logger.error("Error: The provided date must be in the format YYYY.MM.DD")
    sys.exit(1)

# Define the bin directory using the date
bin_directory = f"data/{provided_date}/bin/"
if not os.path.exists(bin_directory):
    logger.error(f"Error: The directory '{bin_directory}' does not exist.")
    sys.exit(1)

# Google Sheets API setup
scope = ["https://spreadsheets.google.com/feeds", "https://www.googleapis.com/auth/drive"]
creds = ServiceAccountCredentials.from_json_keyfile_name('service-account-credentials.json', scope)
client = gspread.authorize(creds)
spreadsheet = client.open("Weekly Reporting for Tracker")

def safe_sheet_update(sheet, range_name, data, retries=5):
    for attempt in range(retries):
        try:
            sheet.update(range_name=range_name, values=data)
            return
        except APIError as e:
            if attempt < retries - 1:
                logger.warning(f"Rate limit hit. Retrying... ({attempt + 1}/{retries})")
                time.sleep(2 ** attempt)
            else:
                logger.error(f"Failed to update sheet after {retries} attempts.")
                raise e

# Find all CSV files in the specified bin folder
csv_files = glob.glob(f'{bin_directory}/*.csv')

if not csv_files:
    logger.info(f"No CSV files found in the directory: {bin_directory}")
    sys.exit(1)

# Loop through each CSV file
for csv_file in csv_files:
    sheet_name = os.path.splitext(os.path.basename(csv_file))[0]
    try:
        worksheet = spreadsheet.worksheet(sheet_name)
    except gspread.exceptions.WorksheetNotFound:
        worksheet = spreadsheet.add_worksheet(title=sheet_name, rows="1000", cols="26")
        logger.info(f"Created new sheet: {sheet_name}")

    with open(csv_file, 'r', encoding='utf-8') as csvfile:
        reader = csv.reader(csvfile)
        data = list(reader)[1:]  # Skip the first row (header)

    existing_data = worksheet.get_all_values()
    next_empty_row = len(existing_data) + 1

    required_rows = next_empty_row + len(data) - 1
    current_rows = worksheet.row_count
    if required_rows > current_rows:
        worksheet.add_rows(required_rows - current_rows)
        logger.info(f"Added {required_rows - current_rows} rows to '{sheet_name}'.")

    chunk_size = 500
    for i in range(0, len(data), chunk_size):
        chunk = data[i:i + chunk_size]
        start_chunk_row = next_empty_row + i
        end_chunk_row = start_chunk_row + len(chunk) - 1
        end_chunk_col = chr(64 + max(len(row) for row in chunk))
        start_cell = f'A{start_chunk_row}'
        end_cell = f'{end_chunk_col}{end_chunk_row}'

        try:
            safe_sheet_update(worksheet, range_name=f'{start_cell}:{end_cell}', data=chunk)
            progress_message = (
                f"✅ Chunk {i // chunk_size + 1}: Rows {start_chunk_row} to {end_chunk_row} "
                f"successfully appended to '{sheet_name}'."
            )
            logger.info(progress_message)
            print(progress_message)  # Write progress directly to the terminal
        except Exception as e:
            error_message = (
                f"❌ Error appending chunk {i // chunk_size + 1}: Rows {start_chunk_row} to "
                f"{end_chunk_row} in '{sheet_name}': {e}"
            )
            logger.error(error_message)
            print(error_message)  # Write error directly to the terminal
            raise

logger.info("All CSV files have been processed.")
print("All CSV files have been processed.")  # Notify completion on terminal