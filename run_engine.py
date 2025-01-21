import os
import subprocess
import sys
from datetime import datetime
from logging_config import setup_logging  # Import centralized logging configuration
import logging
from rename_latest_log import rename_log_file

# Rename the latest log file
rename_log_file()

# Set up centralized logging
setup_logging()
logger = logging.getLogger(__name__)

# Get the date from the command line argument or default to today's date
if len(sys.argv) == 2:
    try:
        provided_date = sys.argv[1]
        datetime.strptime(provided_date, "%Y.%m.%d")  # Validate date format
        logger.info(f"Using provided date: {provided_date}")
    except ValueError:
        logger.error("❌ Error: The provided date must be in the format YYYY.MM.DD")
        sys.exit(1)
else:
    provided_date = datetime.now().strftime("%Y.%m.%d")
    logger.info(f"No date provided. Defaulting to today's date: {provided_date}")

# Function to execute a script and log its output
def execute_script(script, *args):
    command = ["python3", script] + list(args)
    logger.info(f"Executing: {' '.join(command)}")
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    logger.info(stdout.decode())
    logger.error(stderr.decode())
    if process.returncode == 0:
        logger.info(f"✅ {script} completed successfully.")
    else:
        logger.error(f"❌ {script} failed with return code {process.returncode}.")

def print_and_log_message(msg):
    logger.info(msg)
    print(msg)

# Main workflow
if __name__ == "__main__":
    try:
        # Step 1: Clean attachments directory
        logger.info("Cleaning attachments directory...")
        clean_command = "rm -R attachments/*"
        clean_process = subprocess.run(clean_command, shell=True, capture_output=True, text=True)
        if clean_process.returncode == 0:
            logger.info("✅ Attachments directory cleaned successfully.")
        else:
            logger.error(f"❌ Error cleaning attachments directory: {clean_process.stderr}")

        # Print step 1 completion message
        print_and_log_message("Step 1️⃣ of 4️⃣ completed successfully.")

        # Step 2: Parse Gmail inbox
        execute_script("parse_inbox.py", provided_date)

        # Print step 2 completion message
        print_and_log_message("Step 2️⃣ of 4️⃣ completed successfully.")

        # Step 3: Run C++ program (Weekly-Report-Engine)
        logger.info("Executing Weekly-Report-Engine...")
        process = subprocess.Popen(["./build/Weekly-Report-Engine"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        logger.info(stdout.decode())
        logger.error(stderr.decode())
        if process.returncode == 0:
            logger.info("✅ Weekly-Report-Engine completed successfully.")
        else:
            logger.error(f"❌ Weekly-Report-Engine failed with return code {process.returncode}.")

        # Print step 3 completion message
        print_and_log_message("Step 3️⃣ of 4️⃣ completed successfully.")

        # Step 3.5: Fix CSV format
        logger.info("Fixing CSV format...")
        try:
            input_file = f"data/{provided_date}/clean/Availability.csv"
            output_file = f"data/{provided_date}/bin/Availability Report.csv"
            command = ["python3", "fix_csv_format.py", input_file, output_file]
            logger.info(f"Executing: {' '.join(command)}")
            process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            stdout, stderr = process.communicate()
            logger.info(stdout.decode())
            logger.error(stderr.decode())
            if process.returncode == 0:
                logger.info("✅ fix_csv_format.py completed successfully.")
            else:
                logger.error(f"❌ fix_csv_format.py failed with return code {process.returncode}.")
        except Exception as e:
            logger.error(f"❌ An error occurred during Step 3.5: {e}")


        # Step 4: Append data to Google Sheets
        execute_script("append_sheet_injection.py", provided_date)

        # Print step 4 completion message
        print_and_log_message("Step 4️⃣ of 4️⃣ completed successfully.")

        # Final completion message
        print_and_log_message("All steps completed successfully.")
        print_and_log_message("✅ Weekly report generation process finished.")

    except Exception as e:
        logger.error(f"❌ An error occurred: {e}")
