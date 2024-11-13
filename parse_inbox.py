import os
import base64
from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build
from datetime import datetime

def append_current_date(report_name):
    current_date = datetime.now().strftime("%m/%d/%Y")
    return f"{report_name} - {current_date}"

# Example usage
report_name = "Entrata Reports - Birchstone Dashboard v2.3"
updated_report_name = append_current_date(report_name)

# Define the Gmail API scope
SCOPES = ['https://www.googleapis.com/auth/gmail.readonly']

def authenticate_gmail():
    """Authenticates and returns the Gmail API service."""
    creds = None
    if os.path.exists('token.json'):
        creds = Credentials.from_authorized_user_file('token.json', SCOPES)
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
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
        print(f"No emails found with subject '{subject}'")
        return
    
    for message in messages:
        msg = service.users().messages().get(userId=user_id, id=message['id']).execute()
        for part in msg.get('payload', {}).get('parts', []):
            if part['filename']:
                # Attachment part found
                attachment_id = part['body']['attachmentId']
                attachment = service.users().messages().attachments().get(
                    userId=user_id, messageId=message['id'], id=attachment_id
                ).execute()
                data = base64.urlsafe_b64decode(attachment['data'].encode('UTF-8'))
                
                file_path = os.path.join(output_dir, part['filename'])
                with open(file_path, 'wb') as file:
                    file.write(data)
                print(f"Downloaded: {file_path}")

if __name__ == '__main__':
    service = authenticate_gmail()
    download_attachments(service, subject="Entrata Reports")

