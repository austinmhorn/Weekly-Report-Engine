import os
import base64
import argparse
import zipfile
import google.auth.exceptions
from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build
from datetime import datetime

# Log file setup
logs_dir = "logs"
os.makedirs(logs_dir, exist_ok=True)
timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
log_file_path = os.path.join(logs_dir, f"parse_inbox_{timestamp}.txt")

# Function to write to log file
def log_message(message):
    with open(log_file_path, "a") as log_file:
        log_file.write(f"{message}\n")
    print(message)  # Also print to console for immediate feedback

def append_current_date(report_name):
    current_date = datetime.now().strftime("%m/%d/%Y")
    return f"{report_name} - {current_date}"

# Function to parse the date from the command line argument
def parse_date(input_date):
    try:
        # Convert input date "YYYY.MM.DD" to "MM/DD/YYYY"
        return datetime.strptime(input_date, "%Y.%m.%d").strftime("%m/%d/%Y")
    except ValueError:
        log_message(f"Invalid date format: {input_date}. Use YYYY.MM.DD.")
        exit(1)

# Define the Gmail API scope
SCOPES = ['https://www.googleapis.com/auth/gmail.readonly']

def authenticate_gmail():
    """Authenticates and returns the Gmail API service."""
    creds = None
    if os.path.exists('token.json'):
        creds = Credentials.from_authorized_user_file('token.json', SCOPES)
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            try:
                creds.refresh(Request())
            except google.auth.exceptions.RefreshError as e:
                log_message("Token has expired or is invalid, re-authenticating.")
                creds = None  # Force re-authentication
        if not creds:
            flow = InstalledAppFlow.from_client_secrets_file('oauth2-credentials.json', SCOPES)
            creds = flow.run_local_server(port=8080)
        with open('token.json', 'w') as token_file:
            token_file.write(creds.to_json())
    return build('gmail', 'v1', credentials=creds)

def download_attachments(service, user_id='me', subject='Specific Subject', output_dir='attachments'):
    """Downloads attachments from emails with a specific subject."""
    os.makedirs(output_dir, exist_ok=True)
    
    # Search for emails with the specified subject
    query = f'subject:"{subject}"'
    results = service.users().messages().list(userId=user_id, q=query).execute()
    messages = results.get('messages', [])
    
    if not messages:
        log_message(f"No emails found with subject '{subject}'")
        return
    
    # Define the zip file path inside the output directory
    zip_filename = os.path.join(output_dir, 'Archive.zip')
    
    # Create a ZIP file to store the CSV files
    with zipfile.ZipFile(zip_filename, 'w', zipfile.ZIP_DEFLATED) as archive:
        for message in messages:
            msg = service.users().messages().get(userId=user_id, id=message['id']).execute()
            for part in msg.get('payload', {}).get('parts', []):
                if part['filename'] and part['filename'].endswith('.csv'):
                    # Attachment part found
                    attachment_id = part['body']['attachmentId']
                    attachment = service.users().messages().attachments().get(
                        userId=user_id, messageId=message['id'], id=attachment_id
                    ).execute()
                    data = base64.urlsafe_b64decode(attachment['data'].encode('UTF-8'))
                    
                    file_path = os.path.join(output_dir, part['filename'])
                    with open(file_path, 'wb') as file:
                        file.write(data)
                    log_message(f"Downloaded: {file_path}")
                    
                    # Add the CSV file to the ZIP archive
                    archive.write(file_path, arcname=part['filename'])
                    os.remove(file_path)  # Optionally remove the file after zipping it

    log_message(f"All CSV files have been compressed into {zip_filename}")

if __name__ == '__main__':
    # Set up command-line argument parsing
    parser = argparse.ArgumentParser(description='Download email attachments based on a specific date in the subject.')
    parser.add_argument('date', help='Date in the format YYYY.MM.DD')
    args = parser.parse_args()
    
    # Parse the input date
    target_date = parse_date(args.date)
    subject = f"Entrata Reports - Weekly Report Packet for ACAM - {target_date}"
    
    # Authenticate Gmail and download attachments
    service = authenticate_gmail()
    download_attachments(service, subject=subject)
