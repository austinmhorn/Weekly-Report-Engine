import os
import base64
import argparse
import zipfile
import logging
from datetime import datetime
import google.auth.exceptions
from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build
from logging_config import setup_logging  # Import the centralized logging setup

# Set up centralized logging
setup_logging()

logger = logging.getLogger(__name__)

# Set up argument parser
parser = argparse.ArgumentParser(description="Process some inbox data.")
parser.add_argument("date", help="The date in YYYY.MM.DD format")

# Parse the arguments
args = parser.parse_args()

logger.info(f"Starting script for date: {args.date}")

def append_current_date(report_name):
    """Appends current date to the report name."""
    current_date = datetime.now().strftime("%m/%d/%Y")
    return f"{report_name} - {current_date}"

def parse_date(input_date):
    """Parses the input date and converts it to the required format."""
    try:
        return datetime.strptime(input_date, "%Y.%m.%d").strftime("%m/%d/%Y")
    except ValueError:
        logger.error(f"Invalid date format: {input_date}. Use YYYY.MM.DD.")
        exit(1)

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
                logger.error("Token has expired or is invalid, re-authenticating.")
                creds = None
        if not creds:
            flow = InstalledAppFlow.from_client_secrets_file('oauth2-credentials.json', SCOPES)
            creds = flow.run_local_server(port=8080, open_browser=False)
        with open('token.json', 'w') as token_file:
            token_file.write(creds.to_json())
    return build('gmail', 'v1', credentials=creds)

def download_attachments(service, user_id='me', subject='Specific Subject', output_dir='attachments'):
    """Downloads attachments from emails with a specific subject."""
    os.makedirs(output_dir, exist_ok=True)
    
    query = f'subject:"{subject}"'
    results = service.users().messages().list(userId=user_id, q=query).execute()
    messages = results.get('messages', [])
    
    if not messages:
        logger.info(f"No emails found with subject '{subject}'")
        return
    
    zip_filename = os.path.join(output_dir, 'Archive.zip')
    
    with zipfile.ZipFile(zip_filename, 'w', zipfile.ZIP_DEFLATED) as archive:
        for message in messages:
            msg = service.users().messages().get(userId=user_id, id=message['id']).execute()
            for part in msg.get('payload', {}).get('parts', []):
                if part['filename'] and part['filename'].endswith('.csv'):
                    attachment_id = part['body']['attachmentId']
                    attachment = service.users().messages().attachments().get(
                        userId=user_id, messageId=message['id'], id=attachment_id
                    ).execute()
                    data = base64.urlsafe_b64decode(attachment['data'].encode('UTF-8'))
                    
                    file_path = os.path.join(output_dir, part['filename'])
                    with open(file_path, 'wb') as file:
                        file.write(data)
                    logger.info(f"Downloaded: {file_path}")
                    archive.write(file_path, arcname=part['filename'])
                    os.remove(file_path)

    logger.info(f"All CSV files have been compressed into {zip_filename}")

if __name__ == '__main__':
    target_date = parse_date(args.date)
    subject = f"Entrata Reports - Weekly Report Packet for ACAM - {target_date}"
    service = authenticate_gmail()
    download_attachments(service, subject=subject)
