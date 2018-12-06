#!/usr/bin/python
import urllib2
from bs4 import BeautifulSoup
import requests
from collections import OrderedDict
from linkedin import linkedin
from lxml import html
import re
import time
import getpass
from requests.auth import HTTPBasicAuth
import webbrowser

def scraper(quotepage):

    values = OrderedDict([
        ("Name", ""),
        ("Website",""),    #1
        ("Address",""),    #2
        ("Phone",""),      #3
        ("Industries",""), #4
        ("Employees",""),  #5
        ("Revenue",""),    #6
        ("Ownership",""),  #7
        #("Contacts","")    #8
    ])

    #opening webpage/BeautifulSoup initializer/fetch webpage
    page = urllib2.urlopen(quotepage, headers=headers)
    soup = BeautifulSoup(page, 'html.parser')
    page_response = requests.get(quotepage, timeout=5)
    page_content = BeautifulSoup(page_response.content, "html.parser")

    #going layer by layer into html
    tabs = soup.find('div',attrs={"class":"seo-company-container"})
    infocontainer = tabs.find('div',attrs={"class":"seo-company-info-container"})
    td = infocontainer.findAll('td', attrs={"class":"seo-company-data"})

    # grabbing # Contacts
    first = infocontainer.find('div', attrs={"class": "company-counts company-counts-total-active"})
    contact_text = first.text.strip().splitlines()

    #Linkedin employees  #
    linkedin = 'https://www.google.com/search?ei=oeQ-W5asG4jaswWRiYfoDQ&q=general+wholesale+company+linkedin+employee'
    w = requests.get(linkedin, headers=headers)
    tempsoup = BeautifulSoup(w.text, "html.parser")
    results = tempsoup.find('span', attrs={"class":"st"})
    employees_linkedin = re.findall(r'\d+', results.text)

    #For loop that iterates through each of the 8 values in the table
    i = 0
    for k, v in values.iteritems():
        values[k] = td[i].text.strip().splitlines()
        #print(i)
        i += 1

    #delete unecessary values and update values with best scraped data
    del values["Address"]
    del values["Ownership"]
    values.update({'Contacts':contact_text})
    values.update({'Employees': employees_linkedin})

    return values;

#return array of strings
def inputarray():
    lines = [line.rstrip('\n') for line in open("input.txt")]
    return lines;

def data_grab_result(fullsearch):
    print(fullsearch)
    #page = requests.get(fullsearch)
    #soup = BeautifulSoup(page.text, "html.parser")
    #td = soup.find('div', attrs={"class" : "dc-body general"})
    #content = td.find('div', attrs={"class" : "content"})
    #padding = content.find('div', attrs ={"class": "padding-20px"})
    #table = padding.find('table', attrs = {"class": "result"})
    #searchResult = content.find('div', attrs = {"class": "general-display-none"})

    #print(content)


    #page = requests.get(fullsearch)
    #soup = BeautifulSoup(page.text, "html.parser")
    #print soup.find('a', attrs={"class": "break-word companyName"})


#entry object
class Entry:
    Name = "Unknown"
    Website = "Unknown"    #1
    Address = "Unknown"    #2
    Phone = "Unknown"      #3
    Industries = "Unknown" #4
    Employees = "Unknown"  #5
    Revenue = "Unknown"    #6
    Ownership = "Unknown"  #7
    Contacts = "Unknown"   #8


    # The class "constructor" - It's actually an initializer
    def __init__(self, values):
        self.Name = values["Name"]
        self.Website = values["Website"]   #1
        #self.Address = values["Address"]    #2
        self.Phone = values["Phone"]    #3
        self.Industries = values["Industries"] #4
        self.Employees = values["Employees"]  #5
        self.Revenue = values["Revenue"]    #6
        #self.Ownership = values["Ownership"]  #7
        self.Contacts = values["Contacts"]     #8

def make_entry(values):
    entry = Entry(values)
    return entry

#main
quotepage = 'https://people.ucsc.edu/~eventor/linked.html'
realpage = 'https://connect.data.com/company/view/gbRnaIk6SZoAorybDl9Hzg/linkedin-corporation'
data_blank = 'https://connect.data.com/search#p%3Dsearchresult%3B%3Bss%3Dsingleboxsearch%3B%3Bq%3D%7B%22freeText%22%3A%22'
end_data_blank = '%22%7D'
num = 0;
payload = dict(
    j_username= '',
    j_password= '',
    CSRF_TOKEN= ''
)
print("""\
                       .|
                       | |
                       |'|            ._____
               ___    |  |            |.   |' .---"|
       _    .-'   '-. |  |     .--'|  ||   | _|    |
    .-'|  _.|  |    ||   '-__  |   |  |    ||      |
    |' | |.    |    ||       | |   |  |    ||      |
 ___|  '-'     '    ""       '-'   '-.'    '`      |____
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
""")
print("Welcome to the DataScraper 9000 designed by Eric Ventor.")
print("")
print("Please use Data Connect login to continue")
#x = raw_input('Email:')
#y = getpass.getpass('Password:')

print("Logging in...")
with requests.session() as s:
    headers = {'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36'}
    page = s.get('https://connect.data.com/login', auth=('awesomesimpsons@gmail.com', ''), headers=headers)
    login_html = html.fromstring(page.text)
    hidden_inputs = login_html.xpath(r'//form/input[@type="hidden"]')
    form = {x.attrib["name"]: x.attrib["value"] for x in hidden_inputs}
    print (page.text)
    #print(form['CSRF_TOKEN'])
    #payload['login_btn'] = 'Log In'
    payload['CSRF_TOKEN'] = form['CSRF_TOKEN']

    print(payload)

    #r = s.get('https://connect.data.com/login?id=loginForm', headers=headers)
    response = s.post('https://connect.data.com/login?id=loginForm', headers=headers, data=payload,)
    print(response.status_code)
    print(response.url)


#reading input file
companies_list = []
companies_list = inputarray()
print(companies_list)


#grabs first result of google search
#r = requests.get(fullsearch)
#soup = BeautifulSoup(r.text, "html.parser")
#print soup.find('cite').text

for x in companies_list:
    #time.sleep(60)
    word_string = companies_list[num].replace(' ','%20')
    fullsearch = (data_blank + word_string + end_data_blank)
    data_grab_result(fullsearch)
    #temp = scraper(fullsearch)
    #lawl = make_entry(temp);
    time.sleep(60)




#array of entry objects
entries = []
entries.append(lawl)

print("Name: ",lawl.Name)
print("Website: ",lawl.Website)
print("Phone: ",lawl.Phone)
print("Industries: ",lawl.Industries)
print("Employees: ",lawl.Employees)
print("Revenue: ",lawl.Revenue)
print("Contacts: ",lawl.Contacts)

#currently stuck at login screen on script
