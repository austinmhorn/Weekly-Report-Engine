import subprocess
from datetime import datetime
import os
import sys

# Set the working directory to the script's directory
script_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_dir)

# Function to build the program (CMake)
def build_program():
    subprocess.run(['cmake', '-S', '.', '-B', 'build'])

# Function to run a program and log its output with a timestamp
def run_and_log_with_timestamp_in_logs(program_path):
    """
    Runs a C++ program and redirects its output to a timestamped file in the 'logs' directory.
    
    :param program_path: Path to the compiled C++ program.
    """
    # Ensure the logs directory exists
    logs_dir = "logs"
    os.makedirs(logs_dir, exist_ok=True)
    
    # Create a timestamped filename
    timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    output_file = os.path.join(logs_dir, f"output_{timestamp}.txt")
    
    try:
        with open(output_file, "w") as file:
            # Run the program and redirect both stdout and stderr to the file
            subprocess.run(
                [program_path], stdout=file, stderr=subprocess.STDOUT, check=True
            )
        print(f"Output has been saved to {output_file}.")
    except subprocess.CalledProcessError as e:
        print(f"Error while running the program: {e}")
    except FileNotFoundError:
        print(f"Program not found: {program_path}")

# Get the date from the command-line arguments or default to today's date
if len(sys.argv) > 1:
    date_provided = sys.argv[1]
else:
    # Default to today's date in 'YYYY.MM.DD' format
    date_provided = datetime.today().strftime('%Y.%m.%d')

print(f"Running with date: {date_provided}")

#build_program()

subprocess.run('rm -R attachments/*', shell=True)

# Log file setup for parse_inbox.py
logs_dir = "logs"
os.makedirs(logs_dir, exist_ok=True)
timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
log_file_path = os.path.join(logs_dir, f"output_{timestamp}.txt")

with open(log_file_path, "a") as log_file:
    try:
        # Run parse_inbox.py and log output
        subprocess.run(['python3', 'parse_inbox.py', date_provided], stdout=log_file, stderr=subprocess.STDOUT, check=True)
        print(f"parse_inbox.py executed successfully. Output logged to {log_file_path}.")
    except subprocess.CalledProcessError as e:
        print(f"Error while executing parse_inbox.py: {e}")
    except FileNotFoundError:
        print("The parse_inbox.py script was not found. Make sure it's in the same directory.")

# Run the C++ program (build and log its output)
run_and_log_with_timestamp_in_logs("./build/Weekly-Report-Engine")

with open(log_file_path, "a") as log_file:
    try:
        # Run append_sheet_injection.py and log output
        subprocess.run(['python3', 'append_sheet_injection.py', date_provided], stdout=log_file, stderr=subprocess.STDOUT, check=True)
        print(f"append_sheet_injection.py executed successfully. Output logged to {log_file_path}.")
    except subprocess.CalledProcessError as e:
        print(f"Error while executing append_sheet_injection.py: {e}")
    except FileNotFoundError:
        print("The append_sheet_injection.py script was not found. Make sure it's in the same directory.")
