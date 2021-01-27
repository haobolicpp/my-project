# http://zhsk.12348.gov.cn/qa.wap/ 采集所有内容，通过谷歌浏览器F12，分析Sources标签和Newwork标签，得出如下结论：
# 要点1：网页http://zhsk.12348.gov.cn/qa.wap/query/lb?cls=2，最后一个数字范围是2~17，对应了各大模块。
# 要点2：通过下面的ajax请求，可以返回当前模块一共有多少个条目，response就是一个数字：
#         url: '/qa.wap/query/GetCount' + "?rid=" + encodeURIComponent('1/6') + "&t=" + Math.random(),
#         type: "Get",
#         【说明】：encodeURIComponent中的6代表了第几个模块，通过该函数后变为了1%F6，可以在python中引用urllib包中parse模块的quote和unquote
# 要点3：模块数据分页查询。加载页面之前会调用ajax请求，点击下一页会重发该请求更新页面数据，num会变化：
#           url: "/qa.wap/query/GetLb",
#           type: "post",
#           data: { rid: '1/17', pagesize: pagesize, pageindex: num },  #这里rid和上面的对应，pagesize表示一个页面几个条目，固定是30，pageindex为第几个页面数据。
#           dataType: "json",
#       response数据为，这里的id是关键：
#           {"total":46,
#               "list":[
#                   {"type":"刑事诉讼法","typeid":"17","q":"司法机关应当如何处置依法不负刑事责任的精神病人？","id":"CA00000197"},
#                   {"type":"刑事诉讼法","typeid":"17","q":"检察机关收缴犯罪赃款赃物的条件和程序是什么？","id":"CA00000204"}。。。
#要点4：通过上一步中的id，可以通过构建url来查询出结果数据，url例如为：http://zhsk.12348.gov.cn/qa.wap/query/detail?id=CA00000197，剩下的就是解析该页面的数据了。

import requests
from bs4 import BeautifulSoup
import urllib.parse
import random

#测试
headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.110 Safari/537.36'}
r = requests.get('http://zhsk.12348.gov.cn/qa.wap/', headers=headers)
print(r.status_code)
#print(r.text) #返回页面代码

#测试2
#获取模块总条目
url = '/qa.wap/query/GetCount' + "?rid=" + urllib.parse.quote('1/6') + "&t=" + str(random.random())
base_url = 'http://zhsk.12348.gov.cn'+url
headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.110 Safari/537.36',
    'X-Requested-With': 'XMLHttpRequest'
}
r = requests.get(base_url, headers=headers)
print(r.status_code)
print(r.json()) 
#获取一个页面的数据
url = '/qa.wap/query/GetLb'
base_url = 'http://zhsk.12348.gov.cn'+url
data = { 'rid': '1/6', 'pagesize': 30, 'pageindex': 1 }
r = requests.post(base_url, data=data, headers=headers)
print(r.status_code)
json = r.json()
print(type(json)) #dict数据
#获取id
id = json['list'][0]['id']
print(id)
#获取一个问题的答案
base_url = 'http://zhsk.12348.gov.cn/qa.wap/query/detail?id='+id
r = requests.get(base_url, headers=headers)
print(r.status_code)
#print(r.text) 
#解析答案
soup = BeautifulSoup(r.text, 'lxml')
q = soup.find_all('h4')
print(q[0].string) #问题标题

p = soup.find_all(attrs={'class': 'clearfix mtb'})
print(p[0].find_all('p')[0].string) #问--该项可能没有

p = soup.find_all(attrs={'class': 'clearfix'})
print(p[0].find_all('p')) #答