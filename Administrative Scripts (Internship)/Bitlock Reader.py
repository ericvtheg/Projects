import win32com.client
import xlsxwriter
import BitlockedPC
import re

pcs = []
invalid_pcs = []

#grabs body of email in NewBitlocker inbox
outlook = win32com.client.Dispatch("Outlook.Application").GetNamespace("MAPI")
inbox = outlook.GetDefaultFolder(6).Folders('NewBitlocker')
messages = inbox.items


# clear FailedToRead.txt
# use this txt file to print unreadable formatted emails to 
with open('FailedToRead.txt', "w") as f:
			f.write('******************************FAILED TO READ EMAILS*********************************************** \n')

# looping through each message
for xx in messages:
	if('RE:' in xx.Subject) or ('Re:' in xx.Subject): # avoids re-reading email threads
		continue
	
	#define necessary variables
	start = -1
	bool = []
	body = xx.Body
	split = body.split("\r\n\r\n")
	num = 0 # number of correctly formatted pc names
	
	try:
		#finding how many hostnames were sent in a single email
		num = body.count('==========================================================') 
		
		i = 0
		while (i < num):
			bool.append(true)
			print('hit')
			i = i + 1
		print(split)
		if( num > 0): # correct format
			num = num - 1
			for xx in split:
				if(bool == True):
					start = start + 1
					if('Host' in xx):
						bool = False
						
			hostname = split[start]
			osname = split[start+1]
			model = split[start+3]
			serial = split[start+5]
			pcTech = split[start+6]
			date = split[start+14]
			encType = split[start+15]
			campus = split[start+16]
			department = split[start+17]
			room = split[start+18]
			user = split[start+19]
			

			hostname = hostname.strip("Host Name: ")
			osname = (osname.split(":",1)[-1]).strip()
			model = (model.strip("System Model: "))
			serial = (serial.strip("Serial Number: "))
			pcTech = (pcTech.split(":",1)[-1]).strip()
			date = (date.strip("Deployment Date: "))
			encType = (encType.split(":",1)[-1]).strip()
			campus = (campus.split(":",1)[-1]).strip()
			department = (department.split(":",1)[-1]).strip()
			room = (room.split(":",1)[-1]).strip()
			user = (user.split(":",1)[-1]).strip()

			parameters = []
			parameters.append(hostname)
			parameters.append(osname)
			parameters.append(model)
			parameters.append(serial)
			parameters.append(date)
			parameters.append(encType)
			parameters.append(campus)
			parameters.append(department)
			parameters.append(room)
			parameters.append(user)
			parameters.append(pcTech)

			PC = BitlockedPC.BitlockedPC(parameters)
			print(PC)
			pcs.append(PC)
			
		elif('==========================================================' not in xx.body): #incorrect format
			if('old pc' in xx.body.lower() or 'old laptop' in xx.body.lower() or ' old ' in xx.body.lower()):
				with open('FailedToRead.txt', "a") as f:
					f.write(xx.body)
					f.write('**************************************************************************************************************************************************\n')
				continue
			
			print('ERROR: invalid format, attempting to recover PC hostnames...')
			
			# finding hostnames by grabbing strings that start with PC and gets the rest of the digits following it 
			temp = re.findall('(?:PC)(\d*)', xx.body)
			if(temp == []):
				print('...no hostnames found')
			else:
				for each in temp:
					invalid_pcs.append(each)
					
				print("Success, recovered: " + str(temp))
		
		else:
			print("INVALID EMAIL FORMAT FROM:")
			print(xx.Sender)
			with open('FailedToRead.txt', "a") as f:
				f.write(xx.body)
				f.write('**************************************************************************************************************************************************\n')
	except:
		print("INVALID EMAIL FORMAT")
		with open('FailedToRead.txt', "a") as f:
			f.write(xx.body)
			f.write('**************************************************************************************************************************************************\n')
			
# excel sheet code here

workbook = xlsxwriter.Workbook('BitlockedPCs.xlsx')
worksheet = workbook.add_worksheet()
bold = workbook.add_format({'bold': True})

worksheet.write('A1', 'Hostname', bold)
worksheet.write('B1', 'osName', bold)
worksheet.write('C1', 'PC Model', bold)
worksheet.write('D1', 'Serial', bold)
worksheet.write('E1', 'Date', bold)
worksheet.write('F1', 'EncType', bold)
worksheet.write('G1', 'Campus', bold)
worksheet.write('H1', 'Department', bold)
worksheet.write('I1', 'Room', bold)
worksheet.write('J1', 'User', bold)
worksheet.write('K1', "PcTech", bold)

worksheet.set_column(1, 1, 10)
worksheet.set_column(2, 2, 20)
worksheet.set_column(3, 3, 18)
worksheet.set_column(4, 4, 12)
worksheet.set_column(5, 5, 15)
worksheet.set_column(6, 6, 10)
worksheet.set_column(7, 7, 8)
worksheet.set_column(8, 8, 18)
worksheet.set_column(9, 9, 12)
worksheet.set_column(10, 10, 14)
worksheet.set_column(11, 11, 15)


# contents of excel sheet
i = 2
for pc in pcs:
	worksheet.write('A'+ str(i), pc.hostname)
	worksheet.write('B'+ str(i), pc.osname)
	worksheet.write('C'+ str(i), pc.model)
	worksheet.write('D'+ str(i), pc.serial)
	worksheet.write('E'+ str(i), pc.date)
	worksheet.write('F'+ str(i), pc.encType)
	worksheet.write('G'+ str(i), pc.campus)
	worksheet.write('H'+ str(i), pc.department)
	worksheet.write('I'+ str(i), pc.room)
	worksheet.write('J'+ str(i), pc.user)
	worksheet.write('K' + str(i), pc.pcTech)
	i = i + 1
	
for xx in invalid_pcs:
	if(xx != '' or xx != ' '):
		worksheet.write('A'+ str(i), 'PC'+str(xx))
		i = i + 1
	
try:
	workbook.close()
except:
	print("*****ERROR: could not save excel document*****")
	print("*****please close excel document and try again*****")