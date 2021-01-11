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
    # 크롬 버전에 맞는 드라이버 다운받은 경로
    driver = webdriver.Chrome(
        '/Users/loglo/OneDrive/Desktop/chromedriver_win32/chromedriver')
    # 스크래핑할 사이트
    driver.get("https://shopping.naver.com/")

    # 이후로는 dom요소 건드는거입니다.
    # 아래는 이동하기 위한 코드입니다
    driver.implicitly_wait(30)

    inputElement = driver.find_element_by_css_selector("[title*='검색어 입력']")
    driver.implicitly_wait(30)

    inputElement.send_keys("안다즈 서울 강남")
    driver.implicitly_wait(20)

    search = driver.find_element_by_css_selector(
        "#autocompleteWrapper > a.co_srh_btn._search.N\=a\:SNB\.search")
    search.click()
    driver.implicitly_wait(20)

    sort = driver.find_element_by_xpath(
        '//*[@id="__next"]/div/div[2]/div/div[3]/div[1]/div[1]/div/div[1]/a[2]')
    sort.click()
    driver.implicitly_wait(20)

    # driver.implicitly_wait(10)

    # 실질적으로 긁어오는 코드입니다
    # html알아서 파싱해서 원하는 부분 자바스크립트 선택자 아래에 넣으시면 됩니다
    html = driver.page_source

    soup = bs4.BeautifulSoup(html, 'html.parser')
    result = soup.select(
        'li > div > div.basicList_info_area__17Xyo > div.basicList_price_area__1UXXR > strong > span > span')
    for i in range(3):
        print(result[i].text)

    # 아래와 같이 hotel.txt파일에 입력됩니다
    sys.stdout = open("hotel.txt", 'w')
    print(result)
    sys.stdout.close()


checkCommand()
# time.sleep(20)


# def checkCommand(result):
#     text=result
#     # if text.find("안다") >=0:
#         driver = webdriver.Chrome('/Users/loglo/OneDrive/Desktop/chromedriver_win32/chromedriver')
#         driver.get("https://shopping.naver.com/")
#         driver.implicitly_wait(20)
#
#         inputElement = driver.find_element_by_css_selector("[title*='검색어 입력']")
#         driver.implicitly_wait(20)
#
#         inputElement.send_keys("안다즈 서울 강남")
#         driver.implicitly_wait(10)
#
#         sort = driver.find_element_by_xpath('//button[contains(text(), "낮은 가격순")]')
#         sort.click()
#         driver.implicitly_wait(10)
#
#         # driver.implicitly_wait(10)
#         time.sleep(2)
#
#         html = driver.page_source
#
#         soup = bs4.BeautifulSoup(html, 'html.parser')
#         result = soup.select('')
#         price = result[0].text
#         print(price)
#
#         f = open("hotel.txt", 'w')
#         f.wrtie("안다즈 서울 강남의 최저가는"+price+"입니다")
#         f.close()


#
# def openNaver():
#     driver = webdriver.Chrome('/Users/loglo/OneDrive/Desktop/chromedriver_win32')
#     driver.get('http://www.naver.com')
#     driver.implicitly_wait(10)
#
#     inputElement = driver.find_element_by_name("query")
#
# openNaver()
