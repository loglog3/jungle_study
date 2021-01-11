import time
from selenium import webdriver
from urllib.request import urlopen, Request
import requests
from bs4 import BeautifulSoup
import webbrowser
import urllib
import os
import bs4
import sys


def checkCommand():

    # 크롬드라이버 경로
    driver = webdriver.Chrome(
        '/Users/loglo/OneDrive/Desktop/chromedriver_win32/chromedriver')
    # 크롤링 원하는 주소
    driver.get(
        "https://academic.microsoft.com/search?q=1&f=&orderBy=0&skip=0&take=10")
    driver.implicitly_wait(15)

    # 이때부터는 DOM 요소 읽어드리면서 긁으시면 됩니다
    inputElement = driver.find_element_by_css_selector("#search-input")
    driver.implicitly_wait(10)

    inputElement.clear()
    inputElement.send_keys("Supply Chain Management")
    driver.implicitly_wait(50)

    search = driver.find_element_by_css_selector("#SearchBoxSubmit")
    search.click()
    search.click()

    for pages in range(10):

        driver.get(driver.current_url)
        driver.implicitly_wait(20)

        html = driver.page_source
        soup = bs4.BeautifulSoup(html, 'html.parser')
        driver.implicitly_wait(300)

        result = soup.select(
            'body > div > div > div > router-view > ma-serp > div > div.results > div > compose > div > div.results div > compose > div > div > a.title.au-target')
        time.sleep(2)
        for value in result:
            print("-" + value.text.strip())

        driver.implicitly_wait(20)
        nextButton = driver.find_element_by_css_selector(
            "body > div > div > div > router-view > ma-serp > div > div.results > div > compose > div > div.results > ma-pager > div > i.icon-up.right.au-target")
        driver.implicitly_wait(20)
        time.sleep(5)
        nextButton.click()


checkCommand()
