#!/usr/bin/env python3
import os
import time
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from heatup import heat_up

# Email account credentials
GMAIL_USER = '465testserver@gmail.com'
GMAIL_PASS = 'deng byvn lnqo qvpd'

# Recipient email (sending to yourself in this case)
recipient_email = '465testserver@gmail.com'


def send_email(subject, body, recipient_email):
# Setting up the message
	msg = MIMEMultipart()
	msg['From'] = GMAIL_USER
	msg['To'] = recipient_email
	msg['Subject'] = subject

# Attach the message body
	msg.attach(MIMEText(body, 'plain'))

# Send the email
	try:
		with smtplib.SMTP('smtp.gmail.com', 587) as smtpserver:
			smtpserver.ehlo()
			smtpserver.starttls()  # Secure the connection
			smtpserver.ehlo()
			smtpserver.login(GMAIL_USER, GMAIL_PASS)  # Log in to your Gmail account
			smtpserver.sendmail(GMAIL_USER, recipient_email, msg.as_string())  # Send the email
			print("Email sent successfully!")
	except Exception as e:
		print(f"Failed to send email: {e}")






max_temp = 45
msg_string = "Warning, current CPU temp is:"

def measure_temp():
	temp = os.popen("vcgencmd measure_temp").readline()
	temp = (temp.replace("temp=","").replace("'C", ""))
	return float(temp)

while True:
	temp = measure_temp();
	if temp > max_temp:
		print(f"Warning, current CPU temp is {temp}")
		send_email("Temperature Warning",f"CPU temperature is {temp} degrees celcius ", GMAIL_USER)
	heat_up()
